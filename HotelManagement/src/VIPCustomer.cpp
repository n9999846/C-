#include "VIPCustomer.h"

VIPCustomer::VIPCustomer() : Customer(), vipLevel(1), memberSince("") {}

VIPCustomer::VIPCustomer(const string& id, const string& name,
                         const string& phone, const string& idCard,
                         int level, const string& memberSince)
    : Customer(id, name, phone, idCard), vipLevel(level), memberSince(memberSince) {}

string VIPCustomer::getCustomerType() const {
    return "VIP客户";
}

double VIPCustomer::getDiscountRate() const {
    if (vipLevel == 1) return 0.95;
    if (vipLevel == 2) return 0.90;
    return 0.85;
}

int VIPCustomer::getMaxActiveBookings() const {
    if (vipLevel == 1) return 5;
    if (vipLevel == 2) return 8;
    return 10;
}

int VIPCustomer::getVipLevel() const {
    return vipLevel;
}

string VIPCustomer::getMemberSince() const {
    return memberSince;
}

void VIPCustomer::upgradeLevel() {
    if (totalSpent >= 50000 && vipLevel < 3) {
        vipLevel = 3;
    } else if (totalSpent >= 20000 && vipLevel < 2) {
        vipLevel = 2;
    } else if (totalSpent >= 10000 && vipLevel < 1) {
        vipLevel = 1;
    }
}

ostream& operator<<(ostream& os, const VIPCustomer& vip) {
    os << "VIP客户ID: " << vip.id
       << ", 姓名: " << vip.name
       << ", 电话: " << vip.phone
       << ", 身份证: " << vip.idCard
       << ", 累计消费: " << vip.totalSpent
       << ", VIP等级: " << vip.vipLevel
       << ", 入会日期: " << vip.memberSince
       << ", 折扣率: " << vip.getDiscountRate();
    return os;
}