#include "OrderBook.h"
#include "csvreader.h"
#include "orderbookentry.h"
#include <map>
#include <algorithm>

OrderBook::OrderBook(string filename){
    orders_entry = csvreader::Read(filename);
}

vector<string> OrderBook::getKnownProducts(){
    vector<string> products;
    
    map<string, bool> prodMap;

    for(OrderBookEntry& e : orders_entry){
        prodMap[e.product] = true;
    }

    for(auto const& e : prodMap){
        products.push_back(e.first);
    }

    return products;
}

vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, string product, string timestamp){
    vector<OrderBookEntry> gOrd;
    for(OrderBookEntry& e : orders_entry){
        if(e.orderType == type && e.product == product && e.timeStamp == timestamp){
            gOrd.push_back(e);
        }
    }

    return gOrd;
}

double OrderBook::getHighPrice(vector<OrderBookEntry>& orr){
    double max = orr[0].price;
    for(OrderBookEntry& e : orr){
        if(e.price > max)
            max = e.price;
    }
    return max;
}

double OrderBook::getLowPrice(vector<OrderBookEntry>& orr){
    double min = orr[0].price;
    for(OrderBookEntry& e : orr){
        if(e.price < min)
            min = e.price;
    }
    return min;
}

string OrderBook::getEarlytime(){
    return orders_entry[0].timeStamp;
}

string OrderBook::getNextTime(string timestamp){
    string next_timestamp = "";
    for(OrderBookEntry& e : orders_entry){
        if(e.timeStamp > timestamp){
            next_timestamp = e.timeStamp;
            break;
        }
    }
    if(next_timestamp == "")
        next_timestamp = orders_entry[0].timeStamp;
    
    return next_timestamp;
}

/**
 * Calculates the Volume Weighted Average Price (VWAP) for a given set of orders.
 * VWAP is a trading benchmark used to give a weighted average price based on both price and volume.
 * 
 * @param entries: A vector of OrderBookEntry objects, where each entry contains price and amount (volume).
 * @return The VWAP, calculated as the total price * volume divided by total volume.
 *         If there are no valid entries (i.e., total volume is 0), returns 0.0.
 */
double OrderBook::calculateVWAP(vector<OrderBookEntry>& entries) {
    double totalValue = 0.0;  // The cumulative value of all trades (price * volume)
    double totalVolume = 0.0; // The cumulative volume of all trades

    // Loop through each order entry and calculate the total value and total volume
    for (OrderBookEntry& entry : entries) {
        totalValue += entry.price * entry.amount;
        totalVolume += entry.amount;
    }

    // If no trades have volume, return 0 to avoid division by zero
    if (totalVolume == 0.0) {
        return 0.0;
    }

    // Return VWAP as total value divided by total volume
    return totalValue / totalVolume;
}

void OrderBook::insertOrder(OrderBookEntry& book){
    orders_entry.push_back(book);
    sort(orders_entry.begin(), orders_entry.end(), OrderBookEntry::compareByTimestamp);
}

vector<OrderBookEntry> OrderBook::MatchAsktoBid(string product, string timestamp){
    vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, product, timestamp);
    vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, product, timestamp);

    vector<OrderBookEntry> sales;

    sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDsc);

    for(OrderBookEntry& ask : asks){
        for(OrderBookEntry& bid : bids){
            if(bid.price >= ask.price && bid.amount != 0){
                OrderBookEntry s{timestamp, product, OrderBookType::sell, ask.price, 0};

                if(bid.amount == ask.amount){
                    s.amount = ask.amount;
                    sales.push_back(s);
                    bid.amount = 0;
                    break;
                }

                if(bid.amount > ask.amount){
                    s.amount = ask.amount;
                    sales.push_back(s);
                    bid.amount = bid.amount - ask.amount;
                    break;
                }

                if(bid.amount < ask.amount){
                    s.amount = bid.amount;
                    sales.push_back(s);
                    ask.amount = ask.amount - bid.amount;
                    bid.amount = 0;
                    continue;
                }
            }
        }
    }

    return sales;
}