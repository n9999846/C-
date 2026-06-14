#ifndef BOOKING_EXCEPTION_H
#define BOOKING_EXCEPTION_H

#include <exception>
#include <string>

using namespace std;

class BookingException : public exception {
public:
    enum Code {
        ROOM_NOT_FOUND = 1001,
        ROOM_UNAVAILABLE = 1002,
        CUSTOMER_NOT_FOUND = 1003,
        DATE_INVALID = 1004,
        DATE_CONFLICT = 1005,
        BOOKING_NOT_FOUND = 1006,
        ALREADY_CHECKED_IN = 1007,
        ALREADY_CHECKED_OUT = 1008,
        MAX_BOOKINGS_EXCEEDED = 1009,
        BOOKING_CANCELLED = 1010
    };

private:
    string message;
    Code errorCode;

public:
    BookingException(const string& msg, Code code);
    const char* what() const noexcept override;
    int getErrorCode() const;
};

#endif
