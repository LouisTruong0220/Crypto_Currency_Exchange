#pragma once
#include "orderbookentry.h"
#include <vector>

class OrderBook{
    public:
        OrderBook(string filename);
        vector<string> getKnownProducts();
        vector<OrderBookEntry> getOrders(OrderBookType type, string product, string timestamp);

        static double getHighPrice(vector<OrderBookEntry>& orr);
        static double getLowPrice(vector<OrderBookEntry>& orr);

        string getEarlytime();
        string getNextTime(string timestamp);

        void insertOrder(OrderBookEntry& book);
        vector<OrderBookEntry> MatchAsktoBid(string product, string timestamp);

        double calculateVWAP(vector<OrderBookEntry>& entries);
    private:
        vector<OrderBookEntry> orders_entry;
};