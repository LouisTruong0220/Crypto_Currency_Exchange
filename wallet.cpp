#include "wallet.h"
#include "csvreader.h"
#include <vector>
#include <iostream>

wallet::wallet(){

}

void wallet::insertCurrency(string type, double amount){
    double balance;
    if (amount < 0){
        cout << "wallet::insertCurrency Invalid input" << endl;
        throw exception();
        return;
    }
    if(currencies.count(type) == 0){
        balance = 0;
    }
    else{
        balance = currencies[type];
    }
    balance += amount;
    currencies[type] = balance;
}

bool wallet::containsCurrency(string type, double amount){
    if(currencies.count(type) == 0)
        return false;
    else 
        return currencies[type] >= amount;
}

string wallet::toString(){
    string s;
    for(pair<string, double> pair : currencies){
        string currency = pair.first;
        double amount = pair.second;
        s += currency + " : " + to_string(amount) + "\n";
    }
    return s;
}

bool wallet::removeCurrency(string type, double amount){
    if (amount < 0){ 
        cout << "wallet::removeCurrency Invalid input" << endl;
        return false;
    }
    if(currencies.count(type) == 0){
        return false;
    }
    else{
        if(containsCurrency(type, amount)){
            currencies[type] -= amount;
            return true;
        }
        else
            return false;
    }
}

bool wallet::canFulfillOrder(OrderBookEntry order){
    vector<string> currs = csvreader::tokenise(order.product, '/');

    //ask
    if(order.orderType == OrderBookType::ask){
        double amount = order.amount;
        string currency = currs[0];
        cout << "wallet::canFulfillOrder " << currency << " : " << amount << endl;
        return containsCurrency(currency, amount);
    }

    //bid
    if(order.orderType == OrderBookType::bid){
        //i want to buy eth/btc : 200, 0.5 -> buy 200 eth with 0.5 btc/each
        //i have 200 * 0.5 = 100 btc
        double amount = order.amount * order.price;
        string  currency = currs[1];
        cout << "wallet::canFulfillOrder " << currency << " : " << amount << endl;
        return containsCurrency(currency, amount);
    }
    return false;
}