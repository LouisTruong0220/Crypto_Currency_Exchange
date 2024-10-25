#include "menu.h"
#include "orderbookentry.h"
#include "csvreader.h"
#include <iostream>
#include <vector>

using namespace std;

MenuApp::MenuApp(){

}

void MenuApp::init(){
    int input;
    currenttime = ordbook.getEarlytime();
    Wallet.insertCurrency("BTC", 20);
    while(true){
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}

void MenuApp::printMenu()
{
    // 1 print help
    std::cout << "1: Print help " << std::endl;
    // 2 print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;
    // 3 make an offer
    std::cout << "3: Make an ask " << std::endl;
    // 4 make a bid 
    std::cout << "4: Make a bid " << std::endl;
    // 5 print wallet
    std::cout << "5: Print wallet " << std::endl;
    // 6 continue   
    std::cout << "6: Continue " << std::endl;

    std::cout << "============== " << std::endl;
    cout << "Current time is: " << currenttime << endl;
}

void MenuApp::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void  MenuApp::printMarketStats()
{
    for (string const& product : ordbook.getKnownProducts()) {
        cout << "Product: " << product << endl;
        vector<OrderBookEntry> entries1 = ordbook.getOrders(OrderBookType::ask, product, currenttime);
        cout << "Asks seen: " << entries1.size() << endl;
        double max1 = ordbook.getHighPrice(entries1);
        cout << "Max ask: " << max1 << endl;
        double min1 = ordbook.getLowPrice(entries1);
        cout << "Min ask: " << min1 << endl;
        vector<OrderBookEntry> entries2 = ordbook.getOrders(OrderBookType::bid, product, currenttime);
        cout << "Bids seen: " << entries2.size() << endl;
        double max2 = ordbook.getHighPrice(entries2);
        cout << "Max bid: " << max2 << endl;
        double min2 = ordbook.getLowPrice(entries2);
        cout << "Min bid: " << min2 << endl;
        vector<OrderBookEntry> asks = ordbook.getOrders(OrderBookType::ask, product, currenttime);
        vector<OrderBookEntry> bids = ordbook.getOrders(OrderBookType::bid, product, currenttime);

        double vwapAsk = ordbook.calculateVWAP(asks);
        double vwapBid = ordbook.calculateVWAP(bids);

        cout << "VWAP Ask: " << vwapAsk << endl;
        cout << "VWAP Bid: " << vwapBid << endl;
    }

    // double max = ordbook.getHighPrice();
    // cout << max;
    // std::cout << "Orderbook contains " << ordbook << " entries." << std::endl;

    // unsigned int bids = 0;
    // unsigned int asks = 0;
    // for(OrderBookEntry& e : ordbook){
    //     if(e.orderType == OrderBookType::bid){
    //         ++bids;
    //     }
    //     else if(e.orderType == OrderBookType::ask){
    //         ++asks;
    //     }
    // }
    // cout << "Bids: " << bids << endl;
    // cout << "Asks: " << asks << endl;
}

void MenuApp::enterAsk()
{
    std::cout << "Mark and ask - enter the amount: product, price, amount. " << std::endl;
    string input;
    getline(cin, input);

    vector<string> tokens = csvreader::tokenise(input, ',');
    if(tokens.size() != 3){
        cout << "Bad input";
    }   
    else{
        try{
            OrderBookEntry obe = csvreader::stringtoOBE(currenttime, tokens[0], OrderBookType::ask, tokens[1], tokens[2]);
            if(Wallet.canFulfillOrder(obe)){
                cout << "Asking finished" << endl;
                ordbook.insertOrder(obe);
                vector<string> prd = csvreader::tokenise(obe.product, '/');
                Wallet.removeCurrency(prd[0], obe.amount);
                Wallet.insertCurrency(prd[1], obe.amount * obe.price);
            }
            else{
                cout << "Insufficient funds" << endl;
            }
        }catch(const exception& e){
            cout << "Bad input" << endl;
        }
    }
    cout << "You entered: " << input << endl;
}

void MenuApp::enterBid()
{
    std::cout << "Mark and bid - enter the amount: product, price, amount. " << std::endl;
    string input;
    getline(cin, input);

    vector<string> tokens = csvreader::tokenise(input, ',');
    if(tokens.size() != 3){
        cout << "Bad input";
    }
    else{
        try{
            OrderBookEntry obe = csvreader::stringtoOBE(currenttime, tokens[0], OrderBookType::bid, tokens[1], tokens[2]);
            if(Wallet.canFulfillOrder(obe)){
                cout << "Bidding finished" << endl;
                ordbook.insertOrder(obe);
                vector<string> prd = csvreader::tokenise(obe.product, '/');
                Wallet.insertCurrency(prd[0], obe.amount);
                Wallet.removeCurrency(prd[1], obe.amount * obe.price);
            }
            else{
                cout << "Insufficient funds" << endl;
            }
        }catch(const exception& e){
            cout << "Bad input" << endl;
        }
    }
    cout << "You entered: " << input << endl;
}

void MenuApp::printWallet()
{
    std::cout << "Your wallet contains: " << std::endl;
    cout << Wallet.toString() << endl;
}
        
void MenuApp::gotoNextTimeframe()
{
    std::cout << "Going to next time frame. " << std::endl;
    vector<OrderBookEntry> sale = ordbook.MatchAsktoBid("ETH/BTC", currenttime);
    cout << "SALES: " << sale.size() << endl;
    for(OrderBookEntry& e : sale){
        cout << "Sale price: " << e.price << " Sale amount: " << e.amount << endl;
    }
    currenttime = ordbook.getNextTime(currenttime);
}
 
int MenuApp::getUserOption()
{
    int userOption;
    string line;
    std::cout << "Type in 1-6" << std::endl;
    getline(cin, line);
    try{
        userOption = stoi(line);
    }catch(const exception& e){
        cout << "Bad number input" << endl;
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void MenuApp::processUserOption(int userOption)
{
    if (userOption == 0) // bad input
    {
        std::cout << "Invalid choice. Choose 1-6" << std::endl;
    }
    if (userOption == 1) // bad input
    {
        printHelp();
    }
    if (userOption == 2) // bad input
    {
        printMarketStats();
    }
    if (userOption == 3) // bad input
    {
        enterAsk();
    }
    if (userOption == 4) // bad input
    {
        enterBid();
    }
    if (userOption == 5) // bad input
    {
        printWallet();
    }
    if (userOption == 6) // bad input
    {
        gotoNextTimeframe();
    }       
}
