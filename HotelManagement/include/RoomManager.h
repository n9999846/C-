#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H

#include <vector>
#include <map>
#include <string>
#include "Room.h"

class RoomManager {
private:
    vector<Room*> rooms;

public:
    RoomManager();
    ~RoomManager();

    void addRoom(Room* room);
    bool removeRoom(const string& roomNumber);
    Room* findRoom(const string& roomNumber) const;

    vector<Room*> getAllRooms() const;
    vector<Room*> getAvailableRooms() const;
    vector<Room*> getRoomsByType(const string& type) const;
    vector<Room*> getRoomsByFloor(int floor) const;
    vector<Room*> getRoomsByStatus(Room::Status status) const;
    vector<Room*> getRoomsByPriceRange(double min, double max) const;
    Room* findAvailableRoomByType(const string& type) const;

    void sortByPrice();
    void sortByRoomNumber();
    void sortByFloor();

    int getTotalRoomCount() const;
    int getAvailableRoomCount() const;
    double getOccupancyRate() const;
    map<string, int> getRoomTypeDistribution() const;
    map<string, double> getAveragePriceByType() const;

    bool bookRoom(const string& roomNumber);
    bool occupyRoom(const string& roomNumber);
    bool releaseRoom(const string& roomNumber);
    bool setRoomMaintenance(const string& roomNumber);

    void saveRooms(const string& filename) const;
    void loadRooms(const string& filename);
};

#endif
