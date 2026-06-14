#ifndef SYSTEM_INTEGRATOR_H
#define SYSTEM_INTEGRATOR_H

#include <string>
#include <map>
#include <vector>
#include <functional>
#include "RoomManager.h"
#include "BookingManager.h"
#include "BillingManager.h"
#include "Logger.h"
#include "Date.h"

class SystemIntegrator {
private:
    RoomManager* roomMgr;
    BookingManager* bookingMgr;
    BillingManager* billingMgr;
    Logger* logger;
    bool initialized;
    string dataPath;

public:
    SystemIntegrator();
    ~SystemIntegrator();

    bool initialize();
    void shutdown();
    bool isInitialized() const;

    bool addStandardRoom(const string& number, int floor, double price,
                         int beds, bool window, bool wifi);
    bool addDeluxeRoom(const string& number, int floor, double price,
                       bool balcony, bool bathtub, bool breakfast);
    bool addSuiteRoom(const string& number, int floor, double price,
                      bool balcony, bool bathtub, bool breakfast,
                      double livingArea, bool meetingRoom, bool kitchen);
    bool deleteRoom(const string& roomNumber);
    vector<Room*> getAvailableRooms() const;
    vector<Room*> getAllRooms() const;

    bool registerCustomer(const string& id, const string& name,
                          const string& phone, const string& idCard);
    bool registerVIPCustomer(const string& id, const string& name,
                             const string& phone, const string& idCard,
                             int level, const string& memberSince);
    bool upgradeToVIP(const string& customerId, int level);
    vector<Customer*> getAllCustomers() const;

    string createBooking(const string& customerId, const string& roomNumber,
                         const Date& checkIn, const Date& checkOut);
    bool cancelBooking(const string& bookingId);
    bool checkIn(const string& bookingId, const Date& today);
    Bill checkOut(const string& bookingId, const Date& today);
    bool extendStay(const string& bookingId, const Date& newCheckOut);

    bool payBill(const string& billId);
    vector<Bill> getUnpaidBills() const;

    string getSystemStatusReport() const;
    map<string, double> getRevenueStats() const;
    vector<pair<string, double>> getTopCustomers(int n) const;

    string executeSafe(function<string()> action);
};

#endif
