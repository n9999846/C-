#include "BookingException.h"

BookingException::BookingException(const string& msg, Code code)
    : message(msg), errorCode(code) {}

const char* BookingException::what() const noexcept {
    return message.c_str();
}

int BookingException::getErrorCode() const {
    return static_cast<int>(errorCode);
}