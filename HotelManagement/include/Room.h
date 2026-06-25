#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <string>

using namespace std;

class Room {
protected:
    string roomNumber;
    int floor;
    double pricePerNight;

public:
    enum class Status { AVAILABLE, BOOKED, OCCUPIED, MAINTENANCE };

protected:
    Status status;

public:
    Room();
    Room(const string& number, int floor, double price);
    virtual ~Room();

    virtual string getRoomType() const = 0;
    virtual int getMaxGuests() const = 0;
    virtual string getDescription() const = 0;
    virtual double getActualPrice() const;

    string getRoomNumber() const;
    int getFloor() const;
    double getPricePerNight() const;
    Status getStatus() const;
    string getStatusString() const;
    bool isAvailable() const;

    void setStatus(Status s);
    void setPrice(double price);
    void setFloor(int f);

    void book();
    void checkIn();
    void checkOut();
    void setMaintenance();

    bool operator<(const Room& other) const;
    bool operator==(const Room& other) const;
    friend ostream& operator<<(ostream& os, const Room& room);
};

#endif
