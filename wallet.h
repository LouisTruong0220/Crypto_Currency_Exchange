#include <string>
#include <map>
#include "orderbookentry.h"

using namespace std;

class wallet{
    public:
        wallet();
        // insert currency to the wallet
        void insertCurrency(string type, double amount);
        // remove currency from the wallet
        bool removeCurrency(string type, double amount);
        // check if the wallet contains this much currency or more
        bool containsCurrency(string type, double amount);
        // generate a string representation of the wallet
        string toString();
        // check if the wallet can cope with this ask or bid
        bool canFulfillOrder(OrderBookEntry order);

    private:
        map<string, double> currencies;
};