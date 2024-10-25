#pragma once

#include "orderbookentry.h"
#include <vector>

class csvreader{
    public:
        csvreader();
        static vector<OrderBookEntry> Read(string file);
        static vector<string> tokenise(string csvline, char separator);
        static OrderBookEntry stringtoOBE(vector<string> strings);
        static OrderBookEntry stringtoOBE(string time, string prod, OrderBookType ordtype, string pr, string amo);

};