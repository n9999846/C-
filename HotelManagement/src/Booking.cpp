#include "Booking.h"

Booking::Booking() : bookingId(""), customerId(""), roomNumber(""),
                     checkInDate(Date(1900, 1, 1)), checkOutDate(Date(1900, 1, 1)),
                     actualCheckIn(Date(1900, 1, 1)), actualCheckOut(Date(1900, 1, 1)),
                     status(Status::PENDING) {}

Booking::Booking(const string& bookingId, const string& customerId,
                 const string& roomNumber, const Date& checkIn,
                 const Date& checkOut)
    : bookingId(bookingId), customerId(customerId), roomNumber(roomNumber),
      checkInDate(checkIn), checkOutDate(checkOut),
      actualCheckIn(Date(1900, 1, 1)), actualCheckOut(Date(1900, 1, 1)),
      status(Status::PENDING) {}

int Booking::calculateNights() const {
    return checkOutDate - checkInDate;
}

bool Booking::isDateConflict(const Date& newCheckIn, const Date& newCheckOut) const {
    return !(newCheckOut <= checkInDate || newCheckIn >= checkOutDate);
}

bool Booking::containsDate(const Date& date) const {
    return date >= checkInDate && date < checkOutDate;
}

void Booking::checkIn(const Date& today) {
    if (status == Status::PENDING) {
        actualCheckIn = today;
        status = Status::CHECKED_IN;
    }
}

void Booking::checkOut(const Date& today) {
    if (status == Status::CHECKED_IN) {
        actualCheckOut = today;
        status = Status::CHECKED_OUT;
    }
}

void Booking::cancel() {
    if (status == Status::PENDING) {
        status = Status::CANCELLED;
    }
}

void Booking::extendStay(const Date& newCheckOut) {
    if (status == Status::CHECKED_IN) {
        checkOutDate = newCheckOut;
    }
}

string Booking::getBookingId() const {
    return bookingId;
}

string Booking::getCustomerId() const {
    return customerId;
}

string Booking::getRoomNumber() const {
    return roomNumber;
}

Date Booking::getCheckInDate() const {
    return checkInDate;
}

Date Booking::getCheckOutDate() const {
    return checkOutDate;
}

Date Booking::getActualCheckIn() const {
    return actualCheckIn;
}

Date Booking::getActualCheckOut() const {
    return actualCheckOut;
}

Booking::Status Booking::getStatus() const {
    return status;
}

string Booking::getStatusString() const {
    switch (status) {
        case Status::PENDING: return "待入住";
        case Status::CHECKED_IN: return "已入住";
        case Status::CHECKED_OUT: return "已退房";
        case Status::CANCELLED: return "已取消";
        default: return "未知";
    }
}

void Booking::setStatus(Status s) {
    status = s;
}

bool Booking::operator==(const Booking& other) const {
    return bookingId == other.bookingId;
}

bool Booking::operator<(const Booking& other) const {
    return checkInDate < other.checkInDate;
}

ostream& operator<<(ostream& os, const Booking& b) {
    os << "预订ID: " << b.bookingId
       << ", 客户ID: " << b.customerId
       << ", 房间号: " << b.roomNumber
       << ", 预计入住: " << b.checkInDate.toString()
       << ", 预计退房: " << b.checkOutDate.toString();
    if (b.actualCheckIn.isValid() && b.actualCheckIn.getYear() != 1900) {
        os << ", 实际入住: " << b.actualCheckIn.toString();
    }
    if (b.actualCheckOut.isValid() && b.actualCheckOut.getYear() != 1900) {
        os << ", 实际退房: " << b.actualCheckOut.toString();
    }
    os << ", 状态: " << b.getStatusString();
    return os;
}