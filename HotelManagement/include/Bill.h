#ifndef BILL_H
#define BILL_H

#include <iostream>
#include <string>
#include "Date.h"

class Booking;
class Room;
class Customer;

using namespace std;

class Bill {
private:
    string billId;
    string bookingId;
    string customerId;
    string roomNumber;
    int nights;
    double roomCharge;
    double serviceCharge;
    double discountRate;
    double totalAmount;
    bool paid;
    Date billDate;

public:
    Bill();
    Bill(const string& billId, const string& bookingId,
         const string& customerId, const string& roomNumber);

    void generate(const Booking& booking, const Room& room,
                  const Customer& customer, double serviceRate = 0.1);
    void applyDiscount(double rate);
    void pay();

    string getBillId() const;
    string getBookingId() const;
    string getCustomerId() const;
    string getRoomNumber() const;
    int getNights() const;
    double getRoomCharge() const;
    double getServiceCharge() const;
    double getDiscountRate() const;
    double getTotalAmount() const;
    bool isPaid() const;
    Date getBillDate() const;

    Bill operator+(const Bill& other) const;
    friend ostream& operator<<(ostream& os, const Bill& bill);
    operator double() const;
};

#endif
