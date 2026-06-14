#ifndef SUITE_ROOM_H
#define SUITE_ROOM_H

#include "DeluxeRoom.h"

class SuiteRoom : public DeluxeRoom {
private:
    double livingRoomArea;
    bool hasMeetingRoom;
    bool hasKitchen;

public:
    SuiteRoom();
    SuiteRoom(const string& number, int floor, double price,
              bool balcony, bool bathtub, bool breakfast,
              double livingArea, bool meetingRoom, bool kitchen);

    string getRoomType() const override;
    int getMaxGuests() const override;
    string getDescription() const override;
    double getActualPrice() const override;

    double getLivingRoomArea() const;
    bool getHasMeetingRoom() const;
    bool getHasKitchen() const;

    friend ostream& operator<<(ostream& os, const SuiteRoom& room);
};

#endif
