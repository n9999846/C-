#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>

using namespace std;

class Validator {
public:
    static bool isValidRoomNumber(const string& num);
    static bool isValidPhone(const string& phone);
    static bool isValidIdCard(const string& id);
    static bool isValidDate(const string& date);
    static bool isValidName(const string& name);
    static bool isValidPrice(double price);
    static bool isPositiveInt(int n);
    static bool isNonEmptyString(const string& s);
};

#endif
