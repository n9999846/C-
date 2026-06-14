#ifndef BOOKING_H
#define BOOKING_H

#include <iostream>
#include <string>
#include "Date.h"

using namespace std;

class Booking {
public:
    enum class Status { PENDING, CHECKED_IN, CHECKED_OUT, CANCELLED };

private:
    string bookingId;
    string customerId;
    string roomNumber;
    Date checkInDate;
    Date checkOutDate;
    Date actualCheckIn;
    Date actualCheckOut;
    Status status;

public:
    Booking();
    Booking(const string& bookingId, const string& customerId,
            const string& roomNumber, const Date& checkIn,
            const Date& checkOut);

    int calculateNights() const;
    bool isDateConflict(const Date& newCheckIn, const Date& newCheckOut) const;
    bool containsDate(const Date& date) const;

    void checkIn(const Date& today);
    void checkOut(const Date& today);
    void cancel();
    void extendStay(const Date& newCheckOut);

    string getBookingId() const;
    string getCustomerId() const;
    string getRoomNumber() const;
    Date getCheckInDate() const;
    Date getCheckOutDate() const;
    Date getActualCheckIn() const;
    Date getActualCheckOut() const;
    Status getStatus() const;
    string getStatusString() const;

    void setStatus(Status s);

    bool operator==(const Booking& other) const;
    bool operator<(const Booking& other) const;
    friend ostream& operator<<(ostream& os, const Booking& b);
};

#endif
