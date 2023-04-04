#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "stockProfit.h"
using namespace std;

//----------HELPER FUNCTIONS-------------//
void readFile1(string fileName, int& m, int& n, vector<vector<int> >& A){
    ifstream inFile(fileName);
    if(inFile.is_open()){
        string lineFromFile;
        getline(inFile, lineFromFile);

        istringstream stream(lineFromFile);
        //Variables
        string tempM, tempN;
        getline(stream, tempM, ' ');
        getline(stream, tempN, ' ');
        m = stoi(tempM);
        n = stoi(tempN);

        //Parse m lines:
        while(getline(inFile, lineFromFile)){
            vector<int> currentStock;
            istringstream stream2(lineFromFile);
            for(int i = 0; i < n; i++){
                string tempPrice;
                getline(stream2, tempPrice, ' ');
                int dayPrice = stoi(tempPrice);
                currentStock.push_back(dayPrice);
            }
            A.push_back(currentStock);
        }

    }
}

//----------ALGORITHM FUNCTIONS-------------//

void ALG1(int &m, int &n, vector<vector<int> >& A) {
    stockProfit currentMax(1, 1, 2, A);
    // i loop iterates through all stocks:
    for (int i = 0; i < m; i++) {
        //j loop iterates through all dates:
        //j loop stores first date:
        for (int j = 0; j < n; j++) {
            //k loop iterates through all dates again:
            //k loop looks at dates after j loop:
            for (int k = j + 1; k < n; k++) {
                stockProfit currentProfit(i, j, k, A);
                if(currentMax.profit < currentProfit.profit) {
                    currentMax.setIndex(i);
                    currentMax.setBuyDate(j);
                    currentMax.setSellDate(k);
                    currentMax.setProfit(currentProfit.profit);
                }
            }
        }
    }
    // Output (StockIndex BuyDate SellDate)
    cout<< currentMax.getIndex() + 1 << " " << currentMax.getBuyDate() + 1 << " " << currentMax.getSellDate() + 1 <<endl;
}

void ALG2(int &m, int &n, vector<vector<int> > &A){
    int max_profit = A[0][1] - A[0][0];
    int buyDate = 1;
    int sellDate = -1;
    int stockIndex = 1;
    stockProfit currentMax(1, 1, 2, A);

    //i loop iterates through stocks
    for(int i = 0; i < m; i++){
        int lowestPrice = A[i][0];
        int tempMax = max_profit;
        for(int j = 0; j < n; j++){
            if(A[i][j] - lowestPrice > tempMax){
                tempMax = A[i][j] - lowestPrice;
                sellDate = j;
                stockIndex = i;
            }
            if(A[i][j] < lowestPrice){
                lowestPrice = A[i][j];
                buyDate = j;
            }
        }
        if(tempMax > max_profit){
            max_profit = tempMax;
            currentMax.setIndex(stockIndex);
            currentMax.setBuyDate(buyDate);
            currentMax.setSellDate(sellDate);
            currentMax.setProfit(tempMax);
        }
    }
    // Output
    cout<< currentMax.getIndex() + 1 << " " << currentMax.getBuyDate() + 1 << " " << currentMax.getSellDate() + 1 <<endl;
}

int main() {

    //Problem 1:
    int m, n;
    string problemNum;
    vector<vector<int> > A;
    readFile1("cop4533/testCases.txt", m, n, A);

    //Algorithm 1
    ALG1(m, n, A);

    //Algorithm 2
    ALG2(m,n,A);

    return 0;
}
