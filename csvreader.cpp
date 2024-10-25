#include "csvreader.h"
#include "orderbookentry.h"
#include <iostream>
#include <fstream>

using namespace std;

csvreader::csvreader(){
    // cout << "DONE";
}

vector<OrderBookEntry> csvreader::Read(string file){
    vector<OrderBookEntry> entries;

    ifstream csvfile{file};
    string line;
    if(csvfile.is_open()){
        while(getline(csvfile, line)){
            try{
                OrderBookEntry obe = stringtoOBE(tokenise(line, ','));
                entries.push_back(obe);
            }catch(const exception& e){
                // cout << "CSVreader::readCSV bad data" << endl;
            }
        }
    }

    cout << "csvreader::Read read " << entries.size() << " entries" << endl;
    return entries;
}

vector<string> csvreader::tokenise(string line, char separator){
    vector<string> tokens;
    signed int start, end;
    start = line.find_first_not_of(separator, 0);
    do {
        string tok;
        end = line.find_first_of(separator, start);
        if (start == line.length() || start == end)
            break;
        if (end >= 0)
            tok = line.substr(start, end - start);
        else {
            tok = line.substr(start, line.length() - start);
        }
        tokens.push_back(tok);
        start = end + 1;
    } while (end > 0);
    return tokens;
}

OrderBookEntry csvreader::stringtoOBE(vector<string> tokens){

    double price, amount;
    if(tokens.size() != 5){
        // cout << "Bad line" << endl;
        throw exception{};
    }
    try{  
        price = stod(tokens[3]);
        amount = stod(tokens[4]); 

    }catch(const exception& e){
        throw;
    }


    OrderBookEntry obe{tokens[0], tokens[1], OrderBookEntry::stringToOBT(tokens[2]), price, amount};
    return obe;
}

OrderBookEntry csvreader::stringtoOBE(string time, string prod, OrderBookType ordtype, string pr, string amo){
    double price, amount;
    try
    {
        price = stod(pr);
        amount = stod(amo);
    }
    catch(const std::exception& e)
    {
        throw;
    }
    OrderBookEntry obe{time, prod, ordtype, price, amount};
    return obe;
}