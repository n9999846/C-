#ifndef BOOKING_MANAGER_H
#define BOOKING_MANAGER_H

#include <vector>
#include <string>
#include "Customer.h"
#include "VIPCustomer.h"
#include "Booking.h"
#include "BookingException.h"
#include "RoomManager.h"
#include "Date.h"

class BookingManager {
private:
    vector<Customer*> customers;
    vector<Booking> bookings;
    int nextBookingId;
    RoomManager* roomMgr;

public:
    BookingManager();
    ~BookingManager();

    void setRoomManager(RoomManager* rm);

    void addCustomer(Customer* customer);
    bool removeCustomer(const string& id);
    Customer* findCustomer(const string& id) const;
    vector<Customer*> getAllCustomers() const;
    vector<Customer*> getVIPCustomers() const;
    bool upgradeCustomerToVIP(const string& id, int level, const string& memberSince);

    Booking createBooking(const string& customerId, const string& roomNumber,
                          const Date& checkIn, const Date& checkOut);
    bool cancelBooking(const string& bookingId);
    Booking checkIn(const string& bookingId, const Date& today);
    Booking checkOut(const string& bookingId, const Date& today);
    Booking extendStay(const string& bookingId, const Date& newCheckOut);

    Booking* findBooking(const string& bookingId);
    vector<Booking> getCustomerBookings(const string& customerId) const;
    vector<Booking> getRoomBookings(const string& roomNumber) const;
    vector<Booking> getActiveBookings() const;
    vector<Booking> getPendingCheckIn(const Date& today) const;
    vector<Booking> getOverdueCheckOut(const Date& today) const;

    bool isRoomAvailable(const string& roomNumber, const Date& checkIn,
                         const Date& checkOut,
                         const string& excludeBookingId = "") const;
    int countActiveBookings(const string& customerId) const;

    void saveCustomers(const string& filename) const;
    void loadCustomers(const string& filename);
    void saveBookings(const string& filename) const;
    void loadBookings(const string& filename);
};

#endif
