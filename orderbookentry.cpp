#include "orderbookentry.h"

using namespace std;

OrderBookEntry::OrderBookEntry(std::string _timeStamp, std::string _product, OrderBookType _orderType, double _price, double _amount) : timeStamp(_timeStamp), product(_product), orderType(_orderType), price(_price), amount(_amount){

}

OrderBookType OrderBookEntry::stringToOBT(string s){
    if(s == "ask")
        return OrderBookType::ask;
    if(s == "bid")
        return OrderBookType::bid;
    return OrderBookType::unknown;
}

bool OrderBookEntry::compareByTimestamp(OrderBookEntry& e1, OrderBookEntry& e2){
    return e1.timeStamp < e2.timeStamp;
}

bool OrderBookEntry::compareByPriceAsc(OrderBookEntry& e1, OrderBookEntry& e2){
    return e1.price < e2.price;
}


bool OrderBookEntry::compareByPriceDsc(OrderBookEntry& e1, OrderBookEntry& e2){
    return e1.price > e2.price;
}