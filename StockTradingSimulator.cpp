#include<bits/stdc++.h>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
using namespace std;

double LogNormdist(double mean, double stddev) {
    static default_random_engine gen(time(0));
    lognormal_distribution<double> distribution(mean, stddev);
    return distribution(gen);
}
double Normdist(double mean, double stddev) {
    static default_random_engine gen(time(0));
    normal_distribution<double> distribution(mean, stddev);
    return distribution(gen);
}

class Stock {
public:
    //Stock(string name, double price) : name(name), price(price) {}
    Stock(string name,double price){
        this->name=name;
        this->price=price;
    }
    string getName() const { return name; }
    double getPrice() const { return price; }
    void updatePrice(double change) {
        price += change;
        if (price < 0) price = 0;
    }

private:
    string name;
    double price;
};

class Market {
public:
    Market() {
        stocks.push_back(Stock("TechCorp", Normdist(100,3)));
        stocks.push_back(Stock("HealthInc", Normdist(150,3)));
        stocks.push_back(Stock("FinanceCo",Normdist(50,3)));
    }
    void pricechange() {
        for (auto &stock : stocks) {
            double change = Normdist(0, 1);
            stock.updatePrice(change);
        }
        triggerRandomEvent();
    }
    void displayPrices() const {
        for (const auto &stock : stocks) cout << stock.getName() << ": $" << stock.getPrice() << endl;
    }
    Stock& getStock(int index) {
        return stocks[index];
    }

private:
    vector<Stock> stocks;
    void triggerRandomEvent() {
        static default_random_engine gen(time(0));
        uniform_int_distribution<int> distribution(0, 100);
        int probofevent = distribution(gen);
        if (probofevent < 50) {            
            int stocknum = distribution(gen) % stocks.size();
            double eventImpact = Normdist(0, 15);
            cout << "Random Event: " << stocks[stocknum].getName() << " is affected by " << eventImpact << endl;
            stocks[stocknum].updatePrice(eventImpact);
        }
    }
};

class Player {
public:
    
    Player(){
        balance = 1000.0;
    }
    int getday(Stock &stock){return day[&stock]; }
    
    void buystock(Stock &stock, int quantity) {
        double cost = stock.getPrice() * quantity;
        if (balance >= cost) {
            balance -= cost;
            portfolio[&stock] += quantity;
            cout << "Bought " << quantity << " shares of " << stock.getName() << endl;
        } else {
            cout << "Not enough balance to buy " << quantity << " shares of " << stock.getName() << endl;
        }
    }
    void closeshort(Stock &stock){
        if(day[&stock]==1){
            double cost = stock.getPrice() *(- portfolio[&stock] );
            balance -= cost;
            cout << "Bought " << -portfolio[&stock] << " shares of " << stock.getName() << endl;
            portfolio[&stock] =0;
            day[&stock]=0;
            
        }

    }
    void sellstock(Stock &stock, int quantity) {
        if (portfolio[&stock] >= quantity) {
            double revenue = stock.getPrice() * quantity;
            balance += revenue;
            portfolio[&stock] -= quantity;
            cout << "Sold " << quantity << " shares of " << stock.getName() << endl;
        } 
        else {
            double revenue = stock.getPrice() * quantity;
            balance += revenue;
            portfolio[&stock] -= quantity;
            cout << "Sold " << quantity << " shares of " << stock.getName() << endl;
            day[&stock]+=1;
            
        }
    }

    void displayPortfolio() const {
        cout << "Balance: $" << balance << endl;
        for (const auto &entry : portfolio) {
            cout << entry.first->getName() << ": " << entry.second << " shares" << endl;
        }
    }

private:
    double balance;
    unordered_map<Stock*, int> portfolio;
    unordered_map<Stock*, int> day;
};

int main() {
    Market market;
    Player player;
    char choice=-1;
    while(choice!=4) {
        cout << "Current Stock Prices:"<<endl;
        market.displayPrices();

        cout << "Your Portfolio:"<<endl;
        player.displayPortfolio();

        cout << "Choose an action:"<<endl;
        cout << "1. Buy Stock"<<endl;
        cout << "2. Sell Stock"<<endl;
        cout << "3. Next Day"<<endl;
        cout << "4. Exit"<<endl;
        cin >> choice;
        
        
        if (choice == '1') {
            int stocknum, quantity;
            cout << "Enter stock index to buy (0: TechCorp, 1: HealthInc, 2: FinanceCo): ";
            cin >> stocknum;
            if(stocknum<0 || stocknum>=3){cout << "Invalid stock index"<<endl;continue;}
            cout << "Enter quantity: ";
            cin >> quantity;

            if (stocknum >= 0 && stocknum < 3) {
                player.buystock(market.getStock(stocknum), quantity);
            } 
            
            
        } 
        else if (choice == '2') {
            int stocknum, quantity;
            cout << "Enter stock index to sell (0: TechCorp, 1: HealthInc, 2: FinanceCo): ";
            cin >> stocknum;
            if(stocknum<0 || stocknum>=3){cout << "Invalid stock index"<<endl;continue;}
            cout << "Enter quantity: ";
            cin >> quantity;

            if (stocknum >= 0 && stocknum < 3) {
                player.sellstock(market.getStock(stocknum), quantity);
            }
        } 
        else if (choice == '3') {
            market.pricechange();
            for(int i=0;i<3;i++){
                player.closeshort(market.getStock(i));
            }
        }
        
        
    } 

    return 0;
}
