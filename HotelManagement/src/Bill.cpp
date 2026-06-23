#include "Bill.h"
#include "Booking.h"
#include "Room.h"
#include "Customer.h"

Bill::Bill() : billId(""), bookingId(""), customerId(""), roomNumber(""),
               nights(0), roomCharge(0.0), serviceCharge(0.0),
               discountRate(1.0), totalAmount(0.0), paid(false), billDate() {}

Bill::Bill(const string& billId, const string& bookingId,
           const string& customerId, const string& roomNumber)
    : billId(billId), bookingId(bookingId), customerId(customerId),
      roomNumber(roomNumber), nights(0), roomCharge(0.0), serviceCharge(0.0),
      discountRate(1.0), totalAmount(0.0), paid(false), billDate() {}

void Bill::generate(const Booking& booking, const Room& room,
                    const Customer& customer, double serviceRate) {
    Date actualCheckIn = booking.getActualCheckIn();
    Date actualCheckOut = booking.getActualCheckOut();

    if (actualCheckIn.isValid() && actualCheckIn.getYear() != 1900 &&
        actualCheckOut.isValid() && actualCheckOut.getYear() != 1900) {
        nights = actualCheckOut - actualCheckIn;
    } else {
        nights = booking.calculateNights();
    }

    if (nights < 0) nights = 0;

    roomCharge = nights * room.getActualPrice();
    serviceCharge = roomCharge * serviceRate;
    discountRate = customer.getDiscountRate();
    totalAmount = (roomCharge + serviceCharge) * discountRate;
    billDate = Date::today();
}

void Bill::applyDiscount(double rate) {
    if (rate > 0.0 && rate <= 1.0) {
        discountRate *= rate;
        totalAmount = (roomCharge + serviceCharge) * discountRate;
    }
}

void Bill::pay() {
    paid = true;
}

string Bill::getBillId() const { return billId; }
string Bill::getBookingId() const { return bookingId; }
string Bill::getCustomerId() const { return customerId; }
string Bill::getRoomNumber() const { return roomNumber; }
int Bill::getNights() const { return nights; }
double Bill::getRoomCharge() const { return roomCharge; }
double Bill::getServiceCharge() const { return serviceCharge; }
double Bill::getDiscountRate() const { return discountRate; }
double Bill::getTotalAmount() const { return totalAmount; }
bool Bill::isPaid() const { return paid; }
Date Bill::getBillDate() const { return billDate; }

Bill Bill::operator+(const Bill& other) const {
    Bill result;
    result.totalAmount = this->totalAmount + other.totalAmount;
    result.roomCharge = this->roomCharge + other.roomCharge;
    result.serviceCharge = this->serviceCharge + other.serviceCharge;
    result.nights = this->nights + other.nights;
    return result;
}

ostream& operator<<(ostream& os, const Bill& bill) {
    os << "BillID: " << bill.billId
       << ", BookingID: " << bill.bookingId
       << ", CustomerID: " << bill.customerId
       << ", Room: " << bill.roomNumber
       << ", Nights: " << bill.nights
       << ", RoomCharge: " << bill.roomCharge
       << ", Service: " << bill.serviceCharge
       << ", Discount: " << bill.discountRate
       << ", Total: " << bill.totalAmount
       << ", Paid: " << (bill.paid ? "Yes" : "No")
       << ", Date: " << bill.billDate.toString();
    return os;
}

Bill::operator double() const {
    return totalAmount;
}
