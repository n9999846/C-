#include "StandardRoom.h"

StandardRoom::StandardRoom() : Room(), bedCount(1), hasWindow(false), hasWifi(false) {}

StandardRoom::StandardRoom(const string& number, int floor, double price,
                           int beds, bool window, bool wifi)
    : Room(number, floor, price), bedCount(beds), hasWindow(window), hasWifi(wifi) {}

string StandardRoom::getRoomType() const {
    return "标准间";
}

int StandardRoom::getMaxGuests() const {
    return bedCount + 1;
}

string StandardRoom::getDescription() const {
    string desc = "标准间，" + to_string(bedCount) + "张床";
    if (hasWindow) desc += "，有窗";
    if (hasWifi) desc += "，含WiFi";
    return desc;
}

int StandardRoom::getBedCount() const {
    return bedCount;
}

bool StandardRoom::getHasWindow() const {
    return hasWindow;
}

bool StandardRoom::getHasWifi() const {
    return hasWifi;
}

ostream& operator<<(ostream& os, const StandardRoom& room) {
    os << static_cast<const Room&>(room)
       << " | 床位数: " << room.bedCount
       << " | 有窗: " << (room.hasWindow ? "是" : "否")
       << " | WiFi: " << (room.hasWifi ? "有" : "无");
    return os;
}