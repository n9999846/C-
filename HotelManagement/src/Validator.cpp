#include "Validator.h"
#include <cctype>

bool Validator::isValidRoomNumber(const string& num) {
    if (num.empty() || num.size() > 4) return false;
    for (size_t i = 0; i < num.size(); i++) {
        if (!isdigit(num[i])) return false;
    }
    return true;
}

bool Validator::isValidPhone(const string& phone) {
    if (phone.size() != 11) return false;
    if (phone[0] != '1') return false;
    for (size_t i = 0; i < phone.size(); i++) {
        if (!isdigit(phone[i])) return false;
    }
    return true;
}

bool Validator::isValidIdCard(const string& id) {
    if (id.size() != 18) return false;
    for (int i = 0; i < 17; i++) {
        if (!isdigit(id[i])) return false;
    }
    // 最后一位可以是数字或X/x
    if (!isdigit(id[17]) && id[17] != 'X' && id[17] != 'x') return false;
    return true;
}

bool Validator::isValidDate(const string& date) {
    // 格式: YYYY-MM-DD
    if (date.size() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return false;
    }
    // 基本范围校验
    int year  = (date[0]-'0')*1000 + (date[1]-'0')*100 + (date[2]-'0')*10 + (date[3]-'0');
    int month = (date[5]-'0')*10 + (date[6]-'0');
    int day   = (date[8]-'0')*10 + (date[9]-'0');
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    return true;
}

bool Validator::isValidName(const string& name) {
    if (name.size() < 2 || name.size() > 20) return false;
    if (name.empty()) return false;
    return true;
}

bool Validator::isValidPrice(double price) {
    return price > 0.0;
}

bool Validator::isPositiveInt(int n) {
    return n >= 0;
}

bool Validator::isNonEmptyString(const string& s) {
    return !s.empty();
}
