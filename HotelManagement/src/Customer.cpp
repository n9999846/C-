#include "Customer.h"

Customer::Customer() : id(""), name(""), phone(""), idCard(""), totalSpent(0) {}

Customer::Customer(const string& id, const string& name,
                   const string& phone, const string& idCard)
    : id(id), name(name), phone(phone), idCard(idCard), totalSpent(0) {}

Customer::~Customer() {}

string Customer::getCustomerType() const {
    return "普通客户";
}

double Customer::getDiscountRate() const {
    return 1.0;
}

int Customer::getMaxActiveBookings() const {
    return 3;
}

string Customer::getId() const {
    return id;
}

string Customer::getName() const {
    return name;
}

string Customer::getPhone() const {
    return phone;
}

string Customer::getIdCard() const {
    return idCard;
}

int Customer::getTotalSpent() const {
    return totalSpent;
}

void Customer::setName(const string& newName) {
    name = newName;
}

void Customer::setPhone(const string& newPhone) {
    phone = newPhone;
}

void Customer::setIdCard(const string& newIdCard) {
    idCard = newIdCard;
}

void Customer::addSpent(int amount) {
    totalSpent += amount;
}

ostream& operator<<(ostream& os, const Customer& c) {
    os << "客户ID: " << c.id
       << ", 姓名: " << c.name
       << ", 电话: " << c.phone
       << ", 身份证: " << c.idCard
       << ", 累计消费: " << c.totalSpent
       << ", 类型: " << c.getCustomerType();
    return os;
}

bool Customer::operator==(const Customer& other) const {
    return id == other.id;
}