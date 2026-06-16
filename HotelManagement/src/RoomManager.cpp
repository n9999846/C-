#include "RoomManager.h"
#include "StandardRoom.h"
#include "DeluxeRoom.h"
#include "SuiteRoom.h"
#include <algorithm>
#include <fstream>
#include <sstream>

RoomManager::RoomManager() {}

RoomManager::~RoomManager() {
    for (Room* room : rooms) {
        delete room;
    }
    rooms.clear();
}

void RoomManager::addRoom(Room* room) {
    rooms.push_back(room);
}

bool RoomManager::removeRoom(const string& roomNumber) {
    auto it = find_if(rooms.begin(), rooms.end(), [&](Room* r) {
        return r->getRoomNumber() == roomNumber;
    });
    if (it != rooms.end()) {
        delete* it;
        rooms.erase(it);
        return true;
    }
    return false;
}

Room* RoomManager::findRoom(const string& roomNumber) const {
    auto it = find_if(rooms.begin(), rooms.end(), [&](Room* r) {
        return r->getRoomNumber() == roomNumber;
    });
    return (it != rooms.end()) ? *it : nullptr;
}

vector<Room*> RoomManager::getAllRooms() const {
    return rooms;
}

vector<Room*> RoomManager::getAvailableRooms() const {
    vector<Room*> result;
    for (Room* r : rooms) {
        if (r->isAvailable()) {
            result.push_back(r);
        }
    }
    return result;
}

vector<Room*> RoomManager::getRoomsByType(const string& type) const {
    vector<Room*> result;
    for (Room* r : rooms) {
        if (r->getRoomType() == type) {
            result.push_back(r);
        }
    }
    return result;
}

vector<Room*> RoomManager::getRoomsByFloor(int floor) const {
    vector<Room*> result;
    for (Room* r : rooms) {
        if (r->getFloor() == floor) {
            result.push_back(r);
        }
    }
    return result;
}

vector<Room*> RoomManager::getRoomsByStatus(Room::Status status) const {
    vector<Room*> result;
    for (Room* r : rooms) {
        if (r->getStatus() == status) {
            result.push_back(r);
        }
    }
    return result;
}

vector<Room*> RoomManager::getRoomsByPriceRange(double min, double max) const {
    vector<Room*> result;
    for (Room* r : rooms) {
        double price = r->getActualPrice();
        if (price >= min && price <= max) {
            result.push_back(r);
        }
    }
    return result;
}

Room* RoomManager::findAvailableRoomByType(const string& type) const {
    for (Room* r : rooms) {
        if (r->getRoomType() == type && r->isAvailable()) {
            return r;
        }
    }
    return nullptr;
}

void RoomManager::sortByPrice() {
    sort(rooms.begin(), rooms.end(), [](Room* a, Room* b) {
        return a->getActualPrice() < b->getActualPrice();
    });
}

void RoomManager::sortByRoomNumber() {
    sort(rooms.begin(), rooms.end(), [](Room* a, Room* b) {
        return a->getRoomNumber() < b->getRoomNumber();
    });
}

void RoomManager::sortByFloor() {
    sort(rooms.begin(), rooms.end(), [](Room* a, Room* b) {
        return a->getFloor() < b->getFloor();
    });
}

int RoomManager::getTotalRoomCount() const {
    return rooms.size();
}

int RoomManager::getAvailableRoomCount() const {
    return getAvailableRooms().size();
}

double RoomManager::getOccupancyRate() const {
    if (rooms.empty()) return 0.0;
    int occupied = 0;
    for (Room* r : rooms) {
        if (r->getStatus() == Room::Status::OCCUPIED) {
            occupied++;
        }
    }
    return (static_cast<double>(occupied) / rooms.size()) * 100.0;
}

map<string, int> RoomManager::getRoomTypeDistribution() const {
    map<string, int> dist;
    for (Room* r : rooms) {
        dist[r->getRoomType()]++;
    }
    return dist;
}

map<string, double> RoomManager::getAveragePriceByType() const {
    map<string, pair<double, int>> temp;
    for (Room* r : rooms) {
        string type = r->getRoomType();
        temp[type].first += r->getActualPrice();
        temp[type].second++;
    }
    map<string, double> result;
    for (auto& kv : temp) {
        result[kv.first] = kv.second.first / kv.second.second;
    }
    return result;
}

bool RoomManager::bookRoom(const string& roomNumber) {
    Room* r = findRoom(roomNumber);
    if (r && r->isAvailable()) {
        r->book();
        return true;
    }
    return false;
}

bool RoomManager::occupyRoom(const string& roomNumber) {
    Room* r = findRoom(roomNumber);
    if (r && r->getStatus() == Room::Status::BOOKED) {
        r->checkIn();
        return true;
    }
    return false;
}

bool RoomManager::releaseRoom(const string& roomNumber) {
    Room* r = findRoom(roomNumber);
    if (r && r->getStatus() == Room::Status::OCCUPIED) {
        r->checkOut();
        return true;
    }
    return false;
}

bool RoomManager::setRoomMaintenance(const string& roomNumber) {
    Room* r = findRoom(roomNumber);
    if (r && r->getStatus() != Room::Status::BOOKED && r->getStatus() != Room::Status::OCCUPIED) {
        r->setMaintenance();
        return true;
    }
    return false;
}

void RoomManager::saveRooms(const string& filename) const {
    ofstream ofs(filename);
    if (!ofs.is_open()) return;

    for (Room* r : rooms) {
        ofs << r->getRoomType() << "|"
            << r->getRoomNumber() << "|"
            << r->getFloor() << "|"
            << r->getPricePerNight() << "|"
            << static_cast<int>(r->getStatus());
        
        if (StandardRoom* sr = dynamic_cast<StandardRoom*>(r)) {
            ofs << "|" << sr->getBedCount()
                << "|" << (sr->getHasWindow() ? 1 : 0)
                << "|" << (sr->getHasWifi() ? 1 : 0);
        } else if (DeluxeRoom* dr = dynamic_cast<DeluxeRoom*>(r)) {
            if (SuiteRoom* sr = dynamic_cast<SuiteRoom*>(r)) {
                ofs << "|" << (dr->getHasBalcony() ? 1 : 0)
                    << "|" << (dr->getHasBathtub() ? 1 : 0)
                    << "|" << (dr->getHasBreakfast() ? 1 : 0)
                    << "|" << sr->getLivingRoomArea()
                    << "|" << (sr->getHasMeetingRoom() ? 1 : 0)
                    << "|" << (sr->getHasKitchen() ? 1 : 0);
            } else {
                ofs << "|" << (dr->getHasBalcony() ? 1 : 0)
                    << "|" << (dr->getHasBathtub() ? 1 : 0)
                    << "|" << (dr->getHasBreakfast() ? 1 : 0);
            }
        }
        ofs << endl;
    }
    ofs.close();
}

void RoomManager::loadRooms(const string& filename) {
    ifstream ifs(filename);
    if (!ifs.is_open()) return;

    for (Room* r : rooms) {
        delete r;
    }
    rooms.clear();

    string line;
    while (getline(ifs, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string token;
        vector<string> tokens;
        while (getline(ss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 5) continue;
        
        string type = tokens[0];
        string number = tokens[1];
        int floor = stoi(tokens[2]);
        double price = stod(tokens[3]);
        Room::Status status = static_cast<Room::Status>(stoi(tokens[4]));

        if (type == "标准间" && tokens.size() >= 8) {
            int beds = stoi(tokens[5]);
            bool window = (tokens[6] == "1");
            bool wifi = (tokens[7] == "1");
            StandardRoom* r = new StandardRoom(number, floor, price, beds, window, wifi);
            r->setStatus(status);
            rooms.push_back(r);
        } else if (type == "豪华间" && tokens.size() >= 8) {
            bool balcony = (tokens[5] == "1");
            bool bathtub = (tokens[6] == "1");
            bool breakfast = (tokens[7] == "1");
            DeluxeRoom* r = new DeluxeRoom(number, floor, price, balcony, bathtub, breakfast);
            r->setStatus(status);
            rooms.push_back(r);
        } else if (type == "套房" && tokens.size() >= 11) {
            bool balcony = (tokens[5] == "1");
            bool bathtub = (tokens[6] == "1");
            bool breakfast = (tokens[7] == "1");
            double area = stod(tokens[8]);
            bool meetingRoom = (tokens[9] == "1");
            bool kitchen = (tokens[10] == "1");
            SuiteRoom* r = new SuiteRoom(number, floor, price, balcony, bathtub, breakfast, area, meetingRoom, kitchen);
            r->setStatus(status);
            rooms.push_back(r);
        }
    }
    ifs.close();
}