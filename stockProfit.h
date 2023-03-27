#ifndef COP4533_STOCKPROFIT_H
#define COP4533_STOCKPROFIT_H

#include <vector>
using namespace std;

class stockProfit
    {
    public:
        int stockIndex;
        int buyDate;
        int sellDate;
        int profit;

        //----------------CONSTRUCTOR--------------------//
        stockProfit stock(int newStockIndex, int newBuyDate, int newSellDate, vector<vector<int>> A)
        {
            stockIndex = newStockIndex;
            buyDate = newBuyDate;
            sellDate = newSellDate;
            profit = A[stockIndex][sellDate] - A[stockIndex][buyDate];
        }

        //-------------------SETTERS------------------//
        void setIndex(int newIndex){
            stockIndex = newIndex;
        }

        void setBuyDate(int newBuyDate){
            buyDate = newBuyDate;
        }

        void setSellDate(int newSellDate){
            sellDate = newSellDate;
        }

        void setProfit(int newProfit){
            profit = newProfit;
        }
    };

#endif //COP4533_STOCKPROFIT_H
