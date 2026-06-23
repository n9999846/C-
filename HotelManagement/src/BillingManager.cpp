#include "BillingManager.h"
#include "Booking.h"
#include "Room.h"
#include "Customer.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>

BillingManager::BillingManager() : nextBillId(1) {}

Bill BillingManager::generateBill(const Booking& booking, const Room& room, const Customer& customer) {
    string billId = "BL" + to_string(nextBillId++);
    Bill bill(billId, booking.getBookingId(), customer.getId(), room.getRoomNumber());
    bill.generate(booking, room, customer);
    bills.push_back(bill);
    return bill;
}

bool BillingManager::processPayment(const string& billId) {
    Bill* bill = findBill(billId);
    if (bill && !bill->isPaid()) {
        bill->pay();
        return true;
    }
    return false;
}

bool BillingManager::deleteBill(const string& billId) {
    auto it = find_if(bills.begin(), bills.end(), [&](const Bill& b) {
        return b.getBillId() == billId;
    });
    if (it != bills.end()) {
        bills.erase(it);
        return true;
    }
    return false;
}

Bill* BillingManager::findBill(const string& billId) {
    auto it = find_if(bills.begin(), bills.end(), [&](const Bill& b) {
        return b.getBillId() == billId;
    });
    return (it != bills.end()) ? &(*it) : nullptr;
}

vector<Bill> BillingManager::getAllBills() const {
    return bills;
}

vector<Bill> BillingManager::getUnpaidBills() const {
    vector<Bill> result;
    copy_if(bills.begin(), bills.end(), back_inserter(result), [](const Bill& b) {
        return !b.isPaid();
    });
    return result;
}

vector<Bill> BillingManager::getPaidBills() const {
    vector<Bill> result;
    copy_if(bills.begin(), bills.end(), back_inserter(result), [](const Bill& b) {
        return b.isPaid();
    });
    return result;
}

vector<Bill> BillingManager::getCustomerBills(const string& customerId) const {
    vector<Bill> result;
    copy_if(bills.begin(), bills.end(), back_inserter(result), [&](const Bill& b) {
        return b.getCustomerId() == customerId;
    });
    return result;
}

vector<Bill> BillingManager::getDateBills(const Date& date) const {
    vector<Bill> result;
    copy_if(bills.begin(), bills.end(), back_inserter(result), [&](const Bill& b) {
        return b.getBillDate() == date;
    });
    return result;
}

double BillingManager::getDailyRevenue(const Date& date) const {
    return accumulate(bills.begin(), bills.end(), 0.0, [&](double sum, const Bill& b) {
        return sum + (b.getBillDate() == date ? static_cast<double>(b) : 0.0);
    });
}

double BillingManager::getMonthlyRevenue(int year, int month) const {
    return accumulate(bills.begin(), bills.end(), 0.0, [&](double sum, const Bill& b) {
        Date d = b.getBillDate();
        return sum + (d.getYear() == year && d.getMonth() == month ? static_cast<double>(b) : 0.0);
    });
}

double BillingManager::getTotalRevenue() const {
    return accumulate(bills.begin(), bills.end(), 0.0, [](double sum, const Bill& b) {
        return sum + static_cast<double>(b);
    });
}

map<string, double> BillingManager::getRevenueByRoomType() const {
    map<string, double> revenue;
    for (const auto& bill : bills) {
        revenue[bill.getRoomNumber()] += static_cast<double>(bill);
    }
    return revenue;
}

vector<pair<string, double>> BillingManager::getTopCustomers(int n) const {
    map<string, double> customerTotal;
    for (const auto& bill : bills) {
        customerTotal[bill.getCustomerId()] += static_cast<double>(bill);
    }

    vector<pair<string, double>> sorted(customerTotal.begin(), customerTotal.end());
    sort(sorted.begin(), sorted.end(), [](const pair<string, double>& a, const pair<string, double>& b) {
        return a.second > b.second;
    });

    if (n < static_cast<int>(sorted.size())) {
        sorted.resize(n);
    }
    return sorted;
}

map<string, double> BillingManager::getMonthlyRevenueTrend(int months) const {
    map<string, double> trend;

    Date today = Date::today();
    int year = today.getYear();
    int month = today.getMonth();

    for (int i = 0; i < months; ++i) {
        double revenue = getMonthlyRevenue(year, month);
        string key = to_string(year) + "-" + (month < 10 ? "0" : "") + to_string(month);
        trend[key] = revenue;

        month--;
        if (month < 1) {
            month = 12;
            year--;
        }
    }

    return trend;
}

double BillingManager::getAverageRoomCharge() const {
    if (bills.empty()) return 0.0;
    double total = accumulate(bills.begin(), bills.end(), 0.0, [](double sum, const Bill& b) {
        return sum + b.getRoomCharge();
    });
    return total / bills.size();
}

int BillingManager::getPaidBillCount() const {
    return count_if(bills.begin(), bills.end(), [](const Bill& b) {
        return b.isPaid();
    });
}

int BillingManager::getUnpaidBillCount() const {
    return count_if(bills.begin(), bills.end(), [](const Bill& b) {
        return !b.isPaid();
    });
}

void BillingManager::saveBills(const string& filename) const {
    ofstream file(filename);
    if (!file) return;

    for (const auto& bill : bills) {
        file << bill.getBillId() << "|"
             << bill.getBookingId() << "|"
             << bill.getCustomerId() << "|"
             << bill.getRoomNumber() << "|"
             << bill.getNights() << "|"
             << bill.getRoomCharge() << "|"
             << bill.getServiceCharge() << "|"
             << bill.getDiscountRate() << "|"
             << bill.getTotalAmount() << "|"
             << (bill.isPaid() ? "true" : "false") << "|"
             << bill.getBillDate().toString() << endl;
    }
}

void BillingManager::loadBills(const string& filename) {
    ifstream file(filename);
    if (!file) return;

    bills.clear();

    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        istringstream iss(line);
        string billId, bookingId, customerId, roomNumber, nightsStr;
        string roomChargeStr, serviceChargeStr, discountStr, totalStr, paidStr, dateStr;

        getline(iss, billId, '|');
        getline(iss, bookingId, '|');
        getline(iss, customerId, '|');
        getline(iss, roomNumber, '|');
        getline(iss, nightsStr, '|');
        getline(iss, roomChargeStr, '|');
        getline(iss, serviceChargeStr, '|');
        getline(iss, discountStr, '|');
        getline(iss, totalStr, '|');
        getline(iss, paidStr, '|');
        getline(iss, dateStr, '|');

        // 利用 friend class BillingManager 直接设置私有成员
        Bill bill;
        bill.billId = billId;
        bill.bookingId = bookingId;
        bill.customerId = customerId;
        bill.roomNumber = roomNumber;
        bill.nights = stoi(nightsStr);
        bill.roomCharge = stod(roomChargeStr);
        bill.serviceCharge = stod(serviceChargeStr);
        bill.discountRate = stod(discountStr);
        bill.totalAmount = stod(totalStr);
        bill.paid = (paidStr == "true");
        bill.billDate = Date(dateStr);

        bills.push_back(bill);

        // 更新 nextBillId
        if (billId.size() > 2 && billId.substr(0, 2) == "BL") {
            try {
                int num = stoi(billId.substr(2));
                if (num >= nextBillId) {
                    nextBillId = num + 1;
                }
            } catch (...) {}
        }
    }
}
