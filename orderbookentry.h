#pragma once
#include <string>

using namespace std;

enum class OrderBookType
{
    bid,
    ask,
    unknown,
    sell
};

class OrderBookEntry
{
    public:

        OrderBookEntry(std::string _timeStamp, std::string _product, OrderBookType _orderType, double _price, double _amount);

        static OrderBookType stringToOBT(string s);
        static bool compareByTimestamp(OrderBookEntry& e1, OrderBookEntry& e2);
        static bool compareByPriceAsc(OrderBookEntry& e1, OrderBookEntry& e2);
        static bool compareByPriceDsc(OrderBookEntry& e1, OrderBookEntry& e2);
        std::string timeStamp;
        std::string product;
        OrderBookType orderType;
        double price;
        double amount;
};