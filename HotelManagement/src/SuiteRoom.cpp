#include "SuiteRoom.h"

const double BREAKFAST_FEE = 50.0;  // 与 DeluxeRoom 中一致
const double SUITE_PREMIUM_RATE = 1.8;

SuiteRoom::SuiteRoom() : DeluxeRoom(), livingRoomArea(0.0), hasMeetingRoom(false), hasKitchen(false) {}

SuiteRoom::SuiteRoom(const string& number, int floor, double price,
                     bool balcony, bool bathtub, bool breakfast,
                     double livingArea, bool meetingRoom, bool kitchen)
    : DeluxeRoom(number, floor, price, balcony, bathtub, breakfast),
      livingRoomArea(livingArea), hasMeetingRoom(meetingRoom), hasKitchen(kitchen) {}

string SuiteRoom::getRoomType() const {
    return "套房";
}

int SuiteRoom::getMaxGuests() const {
    return 4;
}

string SuiteRoom::getDescription() const {
    string desc = DeluxeRoom::getDescription();
    desc += "，套房专属：客厅" + to_string(livingRoomArea) + "平米";
    if (hasMeetingRoom) desc += "，带会议室";
    if (hasKitchen) desc += "，带厨房";
    return desc;
}

double SuiteRoom::getActualPrice() const {
    double price = pricePerNight;
    if (hasBreakfast) {
        price += BREAKFAST_FEE;
    }
    price *= SUITE_PREMIUM_RATE;
    return price;
}

double SuiteRoom::getLivingRoomArea() const {
    return livingRoomArea;
}

bool SuiteRoom::getHasMeetingRoom() const {
    return hasMeetingRoom;
}

bool SuiteRoom::getHasKitchen() const {
    return hasKitchen;
}

ostream& operator<<(ostream& os, const SuiteRoom& room) {
    os << static_cast<const DeluxeRoom&>(room)
       << " | 客厅面积: " << room.livingRoomArea
       << " | 会议室: " << (room.hasMeetingRoom ? "有" : "无")
       << " | 厨房: " << (room.hasKitchen ? "有" : "无");
    return os;
}