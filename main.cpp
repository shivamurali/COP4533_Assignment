#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include "stockProfit.h"
#include <chrono>
#include <random>
#include <unordered_map>
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

int find_max_profit(int i, int j, const vector<vector<int> >& A, unordered_map<string, int>& memo) {
    // Base case: If we reach the last day or there's only one stock, return 0
    if (j == A[0].size() || i == A.size() - 1) {
        return 0;
    }

    // Check if we already computed the result for this subproblem
    string key = to_string(i) + "," + to_string(j);
    if (memo.count(key)) {
        return memo[key];
    }

    // Recursive case: Find the maximum profit by either buying today or skipping today
    int max_profit = 0;
    for (int k = j + 1; k < A[0].size(); k++) {
        int profit_today = A[i+1][k] - A[i][j];
        if (profit_today > 0) {
            int profit_tomorrow = find_max_profit(i+1, k+1, A, memo);
            max_profit = max(max_profit, profit_today + profit_tomorrow);
        }
    }

    // Memoize the result and return it
    memo[key] = max_profit;
    return max_profit;
}

//----------------ALGORITHM FUNCTIONS------------------//

void ALG1(int &m, int &n, vector<vector<int> >& A) {
    auto startTimer = chrono::high_resolution_clock::now();

    stockProfit currentMax(1, 1, 2, A);
    // i loop iterates through all stocks:
    for (int i = 0; i < m; i++) {
        // j loop iterates through all dates:
        // j loop stores first date:
        for (int j = 0; j < n; j++) {
            // k loop iterates through all dates again:
            // k loop looks at dates after j loop:
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

    // Timer
    auto stopTimer = chrono::high_resolution_clock::now();
    auto durationTimer = (stopTimer - startTimer);
    auto totalTime = chrono::duration_cast<chrono::milliseconds>(durationTimer);

    // Output indices
    cout<< currentMax.getIndex() + 1 << " " << currentMax.getBuyDate() + 1 << " " << currentMax.getSellDate() + 1 <<endl;

    // Output time the algorithm took
    cout << "Algorithm 1 took: " << totalTime.count() << " milliseconds!" << endl << endl;
}

void ALG2(int &m, int &n, vector<vector<int> > &A){
    auto startTimer = chrono::high_resolution_clock::now();

    //Variables and intialization
    int max_profit = A[0][1] - A[0][0];
    int buyDate = 0;
    int sellDate = 1;
    int stockIndex = 1;
    int lowestPriceDay = buyDate;
    stockProfit currentMax(1, 1, 2, A);

    // i loop iterates through stocks
    for(int i = 0; i < m; i++)
    {
        lowestPriceDay = 0;
        int lowestPrice = A[i][0];
        int tempMax = max_profit;

        // j loop iterates through the days of the stock
        for(int j = 0; j < n; j++)
        {
            int tempBuyDate = 0;

            // Compare if the current price for that day is less than the lowestPrice
            if(A[i][j] < lowestPrice)
            {
                lowestPrice = A[i][j];
                tempBuyDate = j;
            }

            // Compare if the current price - lowest price is greater than tempMax
            if(A[i][j] - lowestPrice > tempMax)
            {
                //Store these values
                tempMax = A[i][j] - lowestPrice;
                sellDate = j;
                stockIndex = i;
                buyDate = max(tempBuyDate,lowestPriceDay);
            }
            lowestPriceDay = max(lowestPriceDay,tempBuyDate);
        }

        // Set the object's traits
        if(tempMax > max_profit)
        {
            max_profit = tempMax;
            currentMax.setIndex(stockIndex);
            currentMax.setBuyDate(buyDate);
            currentMax.setSellDate(sellDate);
            currentMax.setProfit(tempMax);
        }
    }

    // Timer
    auto stopTimer = chrono::high_resolution_clock::now();
    auto durationTimer = (stopTimer - startTimer);
    auto totalTime = chrono::duration_cast<chrono::milliseconds>(durationTimer);

    // Output indices
    cout<< currentMax.getIndex() + 1 << " " << currentMax.getBuyDate() + 1 << " " << currentMax.getSellDate() + 1 << endl;

    // Output time the algorithm took
    cout << "Algorithm 2 took: " << totalTime.count() << " milliseconds!" << endl << endl;
}

void ALG3A(int& m, int& n, vector<vector<int> > &A){
    auto startTimer = chrono::high_resolution_clock::now();

    int max_profit = 0;
    int tempMax = 0;
    int stockIndex = 0;
    int buyDate = 0;
    int sellDate = 0;
    stockProfit currentMax(1, 1, 2, A);

    // i loop iterates through stocks:
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

    // Timer
    auto stopTimer = chrono::high_resolution_clock::now();
    auto durationTimer = (stopTimer - startTimer);
    auto totalTime = chrono::duration_cast<chrono::milliseconds>(durationTimer);

    // Output indices
    cout << stockIndex + 1 << " " << buyDate+1 << " " << sellDate + 1 << endl;

    // Output time the algorithm took
    cout << "Algorithm 3A took: " << totalTime.count() << " milliseconds!" << endl << endl;
}

void ALG3B(int& m, int& n,vector<vector<int> >& A) {
    auto startTimer = chrono::high_resolution_clock::now();

    //Variables
    vector<int> profits(m, 0);
    int buyDate = 0;
    int sellDate = 0;

    for (int i = 0; i < m; i++) {
        int minPrice = A[i][0];
        for (int j = 1; j < n; j++) {
            if (A[i][j] - minPrice > profits[i]) {
                profits[i] = A[i][j] - minPrice;
            }
            if (A[i][j] < minPrice) {
                minPrice = A[i][j];
            }
        }
    }

    int maxProfit = 0, stockIndex = 0;
    for (int i = 0; i < m; i++) {
        if (profits[i] > maxProfit) {
            maxProfit = profits[i];
            stockIndex = i;
        }
    }

    int minPrice = A[stockIndex][0];

    for (int j = 0; j < n; j++) {
        if (A[stockIndex][j] - minPrice == maxProfit) {
            sellDate = j;
            break;
        }
        if (A[stockIndex][j] < minPrice) {
            minPrice = A[stockIndex][j];
            buyDate = j;
        }
    }

    // Timer
    auto stopTimer = chrono::high_resolution_clock::now();
    auto durationTimer = (stopTimer - startTimer);
    auto totalTime = chrono::duration_cast<chrono::milliseconds>(durationTimer);

    // Output indices
    cout << stockIndex + 1 << " " << buyDate + 1 << " " << sellDate + 1 << endl;

    // Output time the algorithm took
    cout << "Algorithm 3B took: " << totalTime.count() << " milliseconds!" << endl << endl;
}

void ALG4(int &m, int &n, int &k, vector<vector<int> >& A)
{
    auto startTimer = chrono::high_resolution_clock::now();
    vector<stockProfit> allProfits;
    vector<stockProfit> currentCombination;

    //Store all stock profit combinations
    for(int i = 0; i < m; i++){
        for(int j = 0;  j < n; j++){
            for(int p = j+1; p < n; p++){
                stockProfit currentProfit(i,j,p, A);
                allProfits.push_back(currentProfit);
            }
        }
    }

    //Store all Cumulative Profit Combinations:
    int tempTotalMaxProfit;
    int numTransactions;

    while(numTransactions < k){
        for(int i = 0; i < allProfits.size(); i++){
            if (allProfits[i].getProfit() > tempTotalMaxProfit){
                currentCombination.push_back(allProfits[i]);
                numTransactions++;
            }
        }
    }
}

void ALG5(int &m, int &n, int &k, vector<vector<int> >& A)
{
    auto startTimer = chrono::high_resolution_clock::now();


}

void ALG6(int &m, int &n, int &k, vector<vector<int> >& A)
{
    auto startTimer = chrono::high_resolution_clock::now();


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
    readFile1("cop4533/p1_1k.txt", m, n, A);
    //readFile2("cop4533/testCases.txt", m, n, k, A);
    //Algorithm 1
    //if(problemNumber == "1")
        ALG1(m, n, A);

    //Algorithm 2
    //else if(problemNumber == "2")
        ALG2(m, n, A);

    //Algorithm 3a
    //else if(problemNumber == "3a")
        ALG3A(m, n, A);
        //ALG3Aalt(A);

    //Algorithm 3b
    //else if(problemNumber == "3b")
        ALG3B(m, n, A);

        ALG4(m, n, k, A);

        ALG5(m, n, k, A);

        ALG6(m, n, k , A);
    //else
    //    cout<<"Invalid input"<<endl;

    return 0;
}
