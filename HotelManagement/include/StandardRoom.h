#ifndef STANDARD_ROOM_H
#define STANDARD_ROOM_H

#include "Room.h"

class StandardRoom : public Room {
private:
    int bedCount;
    bool hasWindow;
    bool hasWifi;

public:
    StandardRoom();
    StandardRoom(const string& number, int floor, double price,
                 int beds, bool window, bool wifi);

    string getRoomType() const override;
    int getMaxGuests() const override;
    string getDescription() const override;

    int getBedCount() const;
    bool getHasWindow() const;
    bool getHasWifi() const;

    friend ostream& operator<<(ostream& os, const StandardRoom& room);
};

#endif
