#ifndef VIP_CUSTOMER_H
#define VIP_CUSTOMER_H

#include "Customer.h"

class VIPCustomer : public Customer {
private:
    int vipLevel;
    string memberSince;

public:
    VIPCustomer();
    VIPCustomer(const string& id, const string& name,
                const string& phone, const string& idCard,
                int level, const string& memberSince);

    string getCustomerType() const override;
    double getDiscountRate() const override;
    int getMaxActiveBookings() const override;

    int getVipLevel() const;
    string getMemberSince() const;
    void upgradeLevel();

    friend ostream& operator<<(ostream& os, const VIPCustomer& vip);
};

#endif
