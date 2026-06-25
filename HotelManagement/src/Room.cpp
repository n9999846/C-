#include "Room.h"

Room::Room() : roomNumber(""), floor(0), pricePerNight(0.0), status(Status::AVAILABLE) {}

Room::Room(const string& number, int floor, double price)
    : roomNumber(number), floor(floor), pricePerNight(price), status(Status::AVAILABLE) {}

Room::~Room() {}

double Room::getActualPrice() const {
    return pricePerNight;
}

string Room::getRoomNumber() const {
    return roomNumber;
}

int Room::getFloor() const {
    return floor;
}

double Room::getPricePerNight() const {
    return pricePerNight;
}

Room::Status Room::getStatus() const {
    return status;
}

string Room::getStatusString() const {
    switch (status) {
        case Status::AVAILABLE: return "空闲";
        case Status::BOOKED: return "已预订";
        case Status::OCCUPIED: return "已入住";
        case Status::MAINTENANCE: return "维修中";
        default: return "未知";
    }
}

bool Room::isAvailable() const {
    return status == Status::AVAILABLE;
}

void Room::setStatus(Status s) {
    status = s;
}

void Room::setPrice(double price) {
    pricePerNight = price;
}

void Room::book() {
    if (status == Status::AVAILABLE) {
        status = Status::BOOKED;
    }
}

void Room::checkIn() {
    if (status == Status::BOOKED) {
        status = Status::OCCUPIED;
    }
}

void Room::checkOut() {
    if (status == Status::OCCUPIED) {
        status = Status::AVAILABLE;
    }
}

void Room::setFloor(int f) {
    floor = f;
}

void Room::setMaintenance() {
    if (status != Status::BOOKED && status != Status::OCCUPIED) {
        status = Status::MAINTENANCE;
    }
}

bool Room::operator<(const Room& other) const {
    return pricePerNight < other.pricePerNight;
}

bool Room::operator==(const Room& other) const {
    return roomNumber == other.roomNumber;
}

ostream& operator<<(ostream& os, const Room& room) {
    os << "房间号: " << room.roomNumber
       << " | 楼层: " << room.floor
       << " | 价格: " << room.pricePerNight
       << " | 状态: " << room.getStatusString();
    return os;
}