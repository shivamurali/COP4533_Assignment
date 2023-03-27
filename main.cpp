#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "stockProfit.h"
using namespace std;

//----------HELPER FUNCTIONS-------------//
void readFile(string fileName, int& m, int& n, vector<vector<int> >& A){
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

int main() {

    //Problem 1:

    int m, n;
    vector<vector<int> > A;
    readFile("cop4533/testCases.txt", m, n, A);
    cout<<m<<endl;
    cout<<n<<endl;
    return 0;
}
