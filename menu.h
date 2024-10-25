#pragma once

#include "wallet.h"
#include "OrderBook.h"

class MenuApp{
    public:
        MenuApp();
        void init();
    private:
        void printMenu();
        void printHelp();
        void printMarketStats();
        void enterAsk();
        void enterBid();
        void printWallet();
        void gotoNextTimeframe();
        int getUserOption();
        void processUserOption(int userOption);

        string currenttime;
        OrderBook ordbook{"csvfile.csv"};
        wallet Wallet;
};