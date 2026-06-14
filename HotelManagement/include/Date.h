#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <string>

using namespace std;

class Date {
private:
    int year;
    int month;
    int day;

public:
    Date();
    Date(int y, int m, int d);
    Date(const string& dateStr);

    int getYear() const;
    int getMonth() const;
    int getDay() const;

    string toString() const;
    bool isValid() const;
    bool isLeapYear() const;
    int getDaysInMonth() const;
    static Date today();

    Date operator+(int days) const;
    Date operator-(int days) const;
    int operator-(const Date& other) const;

    Date& operator++();
    Date operator++(int);
    Date& operator--();
    Date operator--(int);

    bool operator<(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>=(const Date& other) const;

    friend ostream& operator<<(ostream& os, const Date& d);
    friend istream& operator>>(istream& is, Date& d);
    friend Date operator+(int days, const Date& d);
};

#endif
