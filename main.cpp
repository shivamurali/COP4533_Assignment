#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include "stockProfit.h"
using namespace std;

//----------HELPER FUNCTIONS-------------//

//PROBLEM 1
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

int findMin(vector<int> A, int min, int max, stack<int>& minIndexes){
    int minimum = A[min];
    minIndexes.push(min);
    for(int i = min; i <= max; i++){
        if(A[i] < minimum) {
            minimum = A[i];
            minIndexes.push(i);
        }
    }
    return minimum;
}

int findMax(vector<int> A, int min, int max, stack<int>& maxIndexes){
    int maximum = A[min];
    maxIndexes.push(min);
    for(int i = min; i <= max; i++){
        if(A[i] > maximum){
            maximum = A[i];
            maxIndexes.push(i);
        }
    }
    return maximum;
}

int findMaxProfit(vector<int> A, int& min, int& maxi, stack<int> &minIndexes, stack<int> &maxIndexes){
    if(min >= maxi)
        return 0;
    int middle = (min+maxi)/2;
    int midPlus = middle + 1;

    int leftHalf = findMaxProfit(A, min, middle, minIndexes, maxIndexes);
    int rightHalf = findMaxProfit(A, midPlus, maxi, minIndexes, maxIndexes);

    int leftMin = findMin(A, min, middle, minIndexes);
    int rightMax = findMax(A, middle + 1, maxi, maxIndexes);

    return max(max(leftHalf, rightHalf), (rightMax - leftMin));
}
//PROBLEM 2
void readFile2(string fileName, int& m, int& n, int& k, vector<vector<int> >& A){
    ifstream inFile(fileName);
    if(inFile.is_open()){
        //Variables
        string lineFromFile;
        string tempK, tempM, tempN;

        //Read in first line
        getline(inFile, tempK);

        //Read in second line
        getline(inFile,lineFromFile);
        istringstream stream(lineFromFile);

        getline(stream, tempM, ' ');
        getline(stream, tempN, ' ');
        k = stoi(tempK);
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

//----------------ALGORITHM FUNCTIONS------------------//

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
    int buyDate = 0;
    int sellDate = 1;
    int stockIndex = 1;
    int lowestPriceDay = buyDate;
    stockProfit currentMax(1, 1, 2, A);

    //i loop iterates through stocks
    for(int i = 0; i < m; i++)
    {
        lowestPriceDay = 0;
        int lowestPrice = A[i][0];
        int tempMax = max_profit;

        for(int j = 0; j < n; j++)
        {
            int tempBuyDate = 0;

            if(A[i][j] < lowestPrice)
            {
                lowestPrice = A[i][j];
                tempBuyDate = j;
            }

            if(A[i][j] - lowestPrice > tempMax)
            {
                tempMax = A[i][j] - lowestPrice;
                sellDate = j;
                stockIndex = i;
                buyDate = max(tempBuyDate,lowestPriceDay);
            }
            lowestPriceDay = max(lowestPriceDay,tempBuyDate);
        }
        if(tempMax > max_profit)
        {
            max_profit = tempMax;
            currentMax.setIndex(stockIndex);
            currentMax.setBuyDate(buyDate);
            currentMax.setSellDate(sellDate);
            currentMax.setProfit(tempMax);
        }
    }
    // Output
    cout<< currentMax.getIndex() + 1 << " " << currentMax.getBuyDate() + 1 << " " << currentMax.getSellDate() + 1 << endl;
}

void ALG3A(int& m, int& n, vector<vector<int> > &A){
    int max_profit = 0;
    int tempMax = 0;
    int stockIndex = 0;
    int buyDate = 0;
    int sellDate = 0;
    stockProfit currentMax(1, 1, 2, A);

    //i loop iterates through stocks:
    int min = 0;
    int max = A[2].size();
    stack<int> minIndexes;
    stack<int> maxIndexes;
    for(int i = 0; i < m; i++){
        tempMax = findMaxProfit(A[i], min, max, minIndexes, maxIndexes);
        if(tempMax > max_profit){
            max_profit = tempMax;
            stockIndex = i;
            buyDate = minIndexes.top();
            sellDate = maxIndexes.top();
        }
    }
    cout<<stockIndex+1<<" "<<buyDate+1<<" "<<sellDate+1<<endl;
   // cout<<findMaxProfit(A[2], min, max, minIndexes, maxIndexes)<<endl;
   // cout<<minIndexes.top()<<" "<<maxIndexes.top()<<endl;
}

void ALG3B(int& m, int& n, vector<vector<int> > &A){
    //Return indexes for currentMax
    //ALG3
    //variables
    int max_profit = A[0][1] - A[0][0];
    int buyDate = 0;
    int sellDate = 1;
    int stockIndex = 0;
    stockProfit currentMax(1, 1, 2, A);

    //i loop iterates through stocks
    for(int i = 0; i < m; i++){
        //
        int tempProfit = max_profit;
        int sum = tempProfit;
        int largestSum = tempProfit;
        //J loop iterates through stock prices, stopping before last date:
        for(int j = 0; j < n - 1; j++){
            tempProfit = A[i][j+1] - A[i][j];
            //If sum is positive, keep track temporarily:
            if(sum > 0){
                sum += tempProfit;
            }
            //If sum is negative, use tempProfit instead:
            else{
                sum = tempProfit;
                buyDate = j;
                //stockIndex = i;
            }

            //Change sum to new greatest profit value for current stock:
            if(sum > largestSum){
                largestSum = sum;
                sellDate = j + 1;
                stockIndex = i;
            }
        }
        //If profit for this stock is greater than previous stocks, change stock and set values:
        if(largestSum > max_profit){
            max_profit = largestSum;
            currentMax.setIndex(stockIndex);
            currentMax.setBuyDate(buyDate);
            currentMax.setSellDate(sellDate);
            currentMax.setProfit(max_profit);
        }
    }
    //return indices for currentMax
    cout<< currentMax.getIndex() + 1 << " " << currentMax.getBuyDate() + 1 << " " << currentMax.getSellDate() + 1 <<endl;
}

int main() {

    //Problem 1:
    //Sample File for testing:
    int m, n, k;
    vector<vector<int> > A;
    string problemNumber, currentLine;

    /*
    cout<<"Enter problem number: ";
    cin >>problemNumber;
    cout<<"Enter m and n separated by a space: ";
    cin>>m>>n;
    cout<<"m = "<<m<<" and n = "<<n<<endl;
    cout<<"Enter stock prices: "<<endl;
    cin.ignore();
    for(int i = 0; i < m; i++){
        vector<int> currentStock;
        getline(cin, currentLine);
        istringstream stream2(currentLine);
        for(int j = 0; j < n; j++){
            string currentPrice;
            getline(stream2, currentPrice, ' ');
            int dayPrice = stoi(currentPrice);
            currentStock.push_back(dayPrice);
        }
        A.push_back(currentStock);
    }
    */
    //readFile1("cop4533/testCases.txt", m, n, A);
    readFile2("cop4533/testCases.txt", m, n, k, A);
    //Algorithm 1
    //if(problemNumber == "1")
        //ALG1(m, n, A);

    //Algorithm 2
    //else if(problemNumber == "2")
        //ALG2(m, n, A);

    //Algorithm 3a
    //else if(problemNumber == "3a")
        //ALG3A(m, n, A);

    //Algorithm 3b
    //else if(problemNumber == "3b")
        //ALG3B(m, n, A);

    //else
    //    cout<<"Invalid input"<<endl;

    return 0;
}
