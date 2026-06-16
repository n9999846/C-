#include "Date.h"
#include <sstream>
#include <iomanip>

Date::Date() : year(1900), month(1), day(1) {}

Date::Date(int y, int m, int d) : year(y), month(m), day(d) {}

Date::Date(const string& dateStr) {
    istringstream iss(dateStr);
    char dash;
    iss >> year >> dash >> month >> dash >> day;
}

int Date::getYear() const { return year; }
int Date::getMonth() const { return month; }
int Date::getDay() const { return day; }

string Date::toString() const {
    ostringstream oss;
    oss << setfill('0') << setw(4) << year << "-"
        << setw(2) << month << "-"
        << setw(2) << day;
    return oss.str();
}

bool Date::isLeapYear() const {
    if (year % 400 == 0) return true;
    if (year % 100 == 0) return false;
    if (year % 4 == 0) return true;
    return false;
}

int Date::getDaysInMonth() const {
    int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear()) return 29;
    return days[month];
}

bool Date::isValid() const {
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > getDaysInMonth()) return false;
    return true;
}

Date Date::today() {
    return Date(2026, 6, 16);
}

Date Date::operator+(int days) const {
    Date result = *this;
    result.day += days;
    while (result.day > result.getDaysInMonth()) {
        result.day -= result.getDaysInMonth();
        result.month++;
        if (result.month > 12) {
            result.month = 1;
            result.year++;
        }
    }
    return result;
}

Date Date::operator-(int days) const {
    Date result = *this;
    result.day -= days;
    while (result.day < 1) {
        result.month--;
        if (result.month < 1) {
            result.month = 12;
            result.year--;
        }
        result.day += result.getDaysInMonth();
    }
    return result;
}

int Date::operator-(const Date& other) const {
    Date d1 = *this;
    Date d2 = other;
    int days = 0;
    bool negative = false;
    
    if (d1 < d2) {
        swap(d1, d2);
        negative = true;
    }
    
    while (d1 != d2) {
        d2++;
        days++;
    }
    
    return negative ? -days : days;
}

Date& Date::operator++() {
    *this = *this + 1;
    return *this;
}

Date Date::operator++(int) {
    Date temp = *this;
    *this = *this + 1;
    return temp;
}

Date& Date::operator--() {
    *this = *this - 1;
    return *this;
}

Date Date::operator--(int) {
    Date temp = *this;
    *this = *this - 1;
    return temp;
}

bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

bool Date::operator>(const Date& other) const {
    return other < *this;
}

bool Date::operator==(const Date& other) const {
    return year == other.year && month == other.month && day == other.day;
}

bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}

bool Date::operator<=(const Date& other) const {
    return *this < other || *this == other;
}

bool Date::operator>=(const Date& other) const {
    return *this > other || *this == other;
}

ostream& operator<<(ostream& os, const Date& d) {
    os << d.toString();
    return os;
}

istream& operator>>(istream& is, Date& d) {
    string dateStr;
    is >> dateStr;
    Date temp(dateStr);
    d = temp;
    return is;
}

Date operator+(int days, const Date& d) {
    return d + days;
}