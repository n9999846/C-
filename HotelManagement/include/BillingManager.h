#ifndef BILLING_MANAGER_H
#define BILLING_MANAGER_H

#include <vector>
#include <map>
#include <string>
#include "Bill.h"
#include "Date.h"

class BillingManager {
private:
    vector<Bill> bills;
    int nextBillId;

public:
    BillingManager();

    Bill generateBill(const Booking& booking, const Room& room, const Customer& customer);
    bool processPayment(const string& billId);
    bool deleteBill(const string& billId);
    Bill* findBill(const string& billId);

    vector<Bill> getAllBills() const;
    vector<Bill> getUnpaidBills() const;
    vector<Bill> getPaidBills() const;
    vector<Bill> getCustomerBills(const string& customerId) const;
    vector<Bill> getDateBills(const Date& date) const;

    double getDailyRevenue(const Date& date) const;
    double getMonthlyRevenue(int year, int month) const;
    double getTotalRevenue() const;

    map<string, double> getRevenueByRoomType() const;
    vector<pair<string, double>> getTopCustomers(int n) const;
    map<string, double> getMonthlyRevenueTrend(int months) const;

    double getAverageRoomCharge() const;
    int getPaidBillCount() const;
    int getUnpaidBillCount() const;

    void saveBills(const string& filename) const;
    void loadBills(const string& filename);
};

#endif
