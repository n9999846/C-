#include "DeluxeRoom.h"

const double BREAKFAST_FEE = 50.0;

DeluxeRoom::DeluxeRoom() : Room(), hasBalcony(false), hasBathtub(false), hasBreakfast(false) {}

DeluxeRoom::DeluxeRoom(const string& number, int floor, double price,
                       bool balcony, bool bathtub, bool breakfast)
    : Room(number, floor, price), hasBalcony(balcony), hasBathtub(bathtub), hasBreakfast(breakfast) {}

string DeluxeRoom::getRoomType() const {
    return "豪华间";
}

int DeluxeRoom::getMaxGuests() const {
    return 3;
}

string DeluxeRoom::getDescription() const {
    string desc = "豪华间";
    if (hasBalcony) desc += "，有阳台";
    if (hasBathtub) desc += "，有浴缸";
    if (hasBreakfast) desc += "，含早餐";
    return desc;
}

double DeluxeRoom::getActualPrice() const {
    double price = pricePerNight;
    if (hasBreakfast) {
        price += BREAKFAST_FEE;
    }
    return price;
}

bool DeluxeRoom::getHasBalcony() const {
    return hasBalcony;
}

bool DeluxeRoom::getHasBathtub() const {
    return hasBathtub;
}

bool DeluxeRoom::getHasBreakfast() const {
    return hasBreakfast;
}

ostream& operator<<(ostream& os, const DeluxeRoom& room) {
    os << static_cast<const Room&>(room)
       << " | 阳台: " << (room.hasBalcony ? "有" : "无")
       << " | 浴缸: " << (room.hasBathtub ? "有" : "无")
       << " | 早餐: " << (room.hasBreakfast ? "含" : "不含");
    return os;
}