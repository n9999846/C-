#ifndef DELUXE_ROOM_H
#define DELUXE_ROOM_H

#include "Room.h"

class DeluxeRoom : public Room {
protected:
    bool hasBalcony;
    bool hasBathtub;
    bool hasBreakfast;

public:
    DeluxeRoom();
    DeluxeRoom(const string& number, int floor, double price,
               bool balcony, bool bathtub, bool breakfast);

    string getRoomType() const override;
    int getMaxGuests() const override;
    string getDescription() const override;
    double getActualPrice() const override;

    bool getHasBalcony() const;
    bool getHasBathtub() const;
    bool getHasBreakfast() const;

    friend ostream& operator<<(ostream& os, const DeluxeRoom& room);
};

#endif
