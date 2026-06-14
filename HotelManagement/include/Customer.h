#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <iostream>
#include <string>

using namespace std;

class Customer {
protected:
    string id;
    string name;
    string phone;
    string idCard;
    int totalSpent;

public:
    Customer();
    Customer(const string& id, const string& name,
             const string& phone, const string& idCard);
    virtual ~Customer();

    virtual string getCustomerType() const;
    virtual double getDiscountRate() const;
    virtual int getMaxActiveBookings() const;

    string getId() const;
    string getName() const;
    string getPhone() const;
    string getIdCard() const;
    int getTotalSpent() const;

    void setName(const string& newName);
    void setPhone(const string& newPhone);
    void addSpent(int amount);

    friend ostream& operator<<(ostream& os, const Customer& c);
    bool operator==(const Customer& other) const;
};

#endif
