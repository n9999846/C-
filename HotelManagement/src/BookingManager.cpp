#include "BookingManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>

BookingManager::BookingManager() : nextBookingId(1), roomMgr(nullptr) {}

BookingManager::~BookingManager() {
    for (auto customer : customers) {
        delete customer;
    }
}

void BookingManager::setRoomManager(RoomManager* rm) {
    roomMgr = rm;
}

void BookingManager::addCustomer(Customer* customer) {
    customers.push_back(customer);
}

bool BookingManager::removeCustomer(const string& id) {
    auto it = find_if(customers.begin(), customers.end(),
                      [&id](Customer* c) { return c->getId() == id; });
    if (it != customers.end()) {
        delete *it;
        customers.erase(it);
        return true;
    }
    return false;
}

Customer* BookingManager::findCustomer(const string& id) const {
    for (auto customer : customers) {
        if (customer->getId() == id) {
            return customer;
        }
    }
    return nullptr;
}

vector<Customer*> BookingManager::getAllCustomers() const {
    return customers;
}

vector<Customer*> BookingManager::getVIPCustomers() const {
    vector<Customer*> vips;
    for (auto customer : customers) {
        if (dynamic_cast<VIPCustomer*>(customer) != nullptr) {
            vips.push_back(customer);
        }
    }
    return vips;
}

bool BookingManager::upgradeCustomerToVIP(const string& id, int level, const string& memberSince) {
    auto it = find_if(customers.begin(), customers.end(),
                      [&id](Customer* c) { return c->getId() == id; });
    if (it != customers.end()) {
        Customer* oldCustomer = *it;
        VIPCustomer* vip = new VIPCustomer(oldCustomer->getId(), oldCustomer->getName(),
                                           oldCustomer->getPhone(), oldCustomer->getIdCard(),
                                           level, memberSince);
        vip->addSpent(oldCustomer->getTotalSpent());
        delete oldCustomer;
        *it = vip;
        return true;
    }
    return false;
}

Booking BookingManager::createBooking(const string& customerId, const string& roomNumber,
                                      const Date& checkIn, const Date& checkOut) {
    Customer* customer = findCustomer(customerId);
    if (!customer) {
        throw BookingException("客户不存在", BookingException::CUSTOMER_NOT_FOUND);
    }

    if (!roomMgr) {
        throw BookingException("房间管理器未初始化", BookingException::ROOM_NOT_FOUND);
    }
    Room* room = roomMgr->findRoom(roomNumber);
    if (!room) {
        throw BookingException("房间不存在", BookingException::ROOM_NOT_FOUND);
    }

    if (checkIn >= checkOut) {
        throw BookingException("入住日期必须早于退房日期", BookingException::DATE_INVALID);
    }

    if (!isRoomAvailable(roomNumber, checkIn, checkOut)) {
        throw BookingException("房间在指定日期不可用", BookingException::ROOM_UNAVAILABLE);
    }

    int activeBookings = countActiveBookings(customerId);
    if (activeBookings >= customer->getMaxActiveBookings()) {
        throw BookingException("超过最大预订数量", BookingException::MAX_BOOKINGS_EXCEEDED);
    }

    string bookingId = "B" + to_string(nextBookingId++);
    Booking booking(bookingId, customerId, roomNumber, checkIn, checkOut);
    bookings.push_back(booking);
    roomMgr->bookRoom(roomNumber);
    return booking;
}

bool BookingManager::cancelBooking(const string& bookingId) {
    for (auto& booking : bookings) {
        if (booking.getBookingId() == bookingId) {
            if (booking.getStatus() == Booking::Status::PENDING) {
                booking.cancel();
                if (roomMgr) {
                    Room* room = roomMgr->findRoom(booking.getRoomNumber());
                    if (room && room->getStatus() == Room::Status::BOOKED) {
                        room->setStatus(Room::Status::AVAILABLE);
                    }
                }
                return true;
            } else if (booking.getStatus() == Booking::Status::CHECKED_IN) {
                throw BookingException("已入住的预订无法取消", BookingException::ALREADY_CHECKED_IN);
            } else if (booking.getStatus() == Booking::Status::CHECKED_OUT) {
                throw BookingException("已退房的预订无法取消", BookingException::ALREADY_CHECKED_OUT);
            } else {
                throw BookingException("预订已取消", BookingException::BOOKING_CANCELLED);
            }
        }
    }
    throw BookingException("预订不存在", BookingException::BOOKING_NOT_FOUND);
}

Booking BookingManager::checkIn(const string& bookingId, const Date& today) {
    for (auto& booking : bookings) {
        if (booking.getBookingId() == bookingId) {
            if (booking.getStatus() == Booking::Status::PENDING) {
                booking.checkIn(today);
                if (roomMgr) {
                    roomMgr->occupyRoom(booking.getRoomNumber());
                }
                return booking;
            } else if (booking.getStatus() == Booking::Status::CHECKED_IN) {
                throw BookingException("预订已入住", BookingException::ALREADY_CHECKED_IN);
            } else if (booking.getStatus() == Booking::Status::CHECKED_OUT) {
                throw BookingException("预订已退房", BookingException::ALREADY_CHECKED_OUT);
            } else {
                throw BookingException("预订已取消", BookingException::BOOKING_CANCELLED);
            }
        }
    }
    throw BookingException("预订不存在", BookingException::BOOKING_NOT_FOUND);
}

Booking BookingManager::checkOut(const string& bookingId, const Date& today) {
    for (auto& booking : bookings) {
        if (booking.getBookingId() == bookingId) {
            if (booking.getStatus() == Booking::Status::CHECKED_IN) {
                booking.checkOut(today);
                if (roomMgr) {
                    roomMgr->releaseRoom(booking.getRoomNumber());
                }
                return booking;
            } else if (booking.getStatus() == Booking::Status::PENDING) {
                throw BookingException("预订尚未入住", BookingException::DATE_INVALID);
            } else if (booking.getStatus() == Booking::Status::CHECKED_OUT) {
                throw BookingException("预订已退房", BookingException::ALREADY_CHECKED_OUT);
            } else {
                throw BookingException("预订已取消", BookingException::BOOKING_CANCELLED);
            }
        }
    }
    throw BookingException("预订不存在", BookingException::BOOKING_NOT_FOUND);
}

Booking BookingManager::extendStay(const string& bookingId, const Date& newCheckOut) {
    for (auto& booking : bookings) {
        if (booking.getBookingId() == bookingId) {
            if (booking.getStatus() != Booking::Status::CHECKED_IN) {
                throw BookingException("只有已入住的预订才能续住", BookingException::DATE_INVALID);
            }
            if (newCheckOut <= booking.getCheckInDate()) {
                throw BookingException("新退房日期必须晚于入住日期", BookingException::DATE_INVALID);
            }
            if (!isRoomAvailable(booking.getRoomNumber(), booking.getCheckOutDate(), newCheckOut, bookingId)) {
                throw BookingException("新日期与其他预订冲突", BookingException::DATE_CONFLICT);
            }
            booking.extendStay(newCheckOut);
            return booking;
        }
    }
    throw BookingException("预订不存在", BookingException::BOOKING_NOT_FOUND);
}

Booking* BookingManager::findBooking(const string& bookingId) {
    for (auto& booking : bookings) {
        if (booking.getBookingId() == bookingId) {
            return &booking;
        }
    }
    return nullptr;
}

vector<Booking> BookingManager::getCustomerBookings(const string& customerId) const {
    vector<Booking> result;
    for (const auto& booking : bookings) {
        if (booking.getCustomerId() == customerId) {
            result.push_back(booking);
        }
    }
    return result;
}

vector<Booking> BookingManager::getRoomBookings(const string& roomNumber) const {
    vector<Booking> result;
    for (const auto& booking : bookings) {
        if (booking.getRoomNumber() == roomNumber) {
            result.push_back(booking);
        }
    }
    return result;
}

vector<Booking> BookingManager::getActiveBookings() const {
    vector<Booking> result;
    for (const auto& booking : bookings) {
        Booking::Status status = booking.getStatus();
        if (status == Booking::Status::PENDING || status == Booking::Status::CHECKED_IN) {
            result.push_back(booking);
        }
    }
    return result;
}

vector<Booking> BookingManager::getPendingCheckIn(const Date& today) const {
    vector<Booking> result;
    for (const auto& booking : bookings) {
        if (booking.getStatus() == Booking::Status::PENDING && booking.getCheckInDate() == today) {
            result.push_back(booking);
        }
    }
    return result;
}

vector<Booking> BookingManager::getOverdueCheckOut(const Date& today) const {
    vector<Booking> result;
    for (const auto& booking : bookings) {
        if (booking.getStatus() == Booking::Status::CHECKED_IN && booking.getCheckOutDate() < today) {
            result.push_back(booking);
        }
    }
    return result;
}

bool BookingManager::isRoomAvailable(const string& roomNumber, const Date& checkIn,
                                     const Date& checkOut, const string& excludeBookingId) const {
    for (const auto& booking : bookings) {
        if (booking.getRoomNumber() == roomNumber && booking.getBookingId() != excludeBookingId) {
            Booking::Status status = booking.getStatus();
            if (status == Booking::Status::CANCELLED || status == Booking::Status::CHECKED_OUT) {
                continue;
            }
            if (booking.isDateConflict(checkIn, checkOut)) {
                return false;
            }
        }
    }
    return true;
}

int BookingManager::countActiveBookings(const string& customerId) const {
    int count = 0;
    for (const auto& booking : bookings) {
        if (booking.getCustomerId() == customerId) {
            Booking::Status status = booking.getStatus();
            if (status == Booking::Status::PENDING || status == Booking::Status::CHECKED_IN) {
                count++;
            }
        }
    }
    return count;
}

void BookingManager::saveCustomers(const string& filename) const {
    ofstream file(filename);
    if (!file) return;

    for (const auto& customer : customers) {
        VIPCustomer* vip = dynamic_cast<VIPCustomer*>(customer);
        if (vip) {
            file << customer->getId() << "|"
                 << customer->getName() << "|"
                 << customer->getPhone() << "|"
                 << customer->getIdCard() << "|"
                 << "VIP|"
                 << customer->getTotalSpent() << "|"
                 << vip->getVipLevel() << "|"
                 << vip->getMemberSince() << endl;
        } else {
            file << customer->getId() << "|"
                 << customer->getName() << "|"
                 << customer->getPhone() << "|"
                 << customer->getIdCard() << "|"
                 << "CUSTOMER|"
                 << customer->getTotalSpent() << endl;
        }
    }
}

void BookingManager::loadCustomers(const string& filename) {
    ifstream file(filename);
    if (!file) return;

    for (auto customer : customers) {
        delete customer;
    }
    customers.clear();

    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        istringstream iss(line);
        string id, name, phone, idCard, type, totalSpentStr;
        getline(iss, id, '|');
        getline(iss, name, '|');
        getline(iss, phone, '|');
        getline(iss, idCard, '|');
        getline(iss, type, '|');
        getline(iss, totalSpentStr, '|');

        int totalSpent = stoi(totalSpentStr);

        if (type == "VIP") {
            string levelStr, memberSince;
            getline(iss, levelStr, '|');
            getline(iss, memberSince, '|');
            VIPCustomer* vip = new VIPCustomer(id, name, phone, idCard,
                                               stoi(levelStr), memberSince);
            vip->addSpent(totalSpent);
            customers.push_back(vip);
        } else {
            Customer* customer = new Customer(id, name, phone, idCard);
            customer->addSpent(totalSpent);
            customers.push_back(customer);
        }
    }
}

void BookingManager::saveBookings(const string& filename) const {
    ofstream file(filename);
    if (!file) return;

    for (const auto& booking : bookings) {
        file << booking.getBookingId() << "|"
             << booking.getCustomerId() << "|"
             << booking.getRoomNumber() << "|"
             << booking.getCheckInDate().toString() << "|"
             << booking.getCheckOutDate().toString() << "|";

        Date actualIn = booking.getActualCheckIn();
        if (actualIn.isValid() && actualIn.getYear() != 1900) {
            file << actualIn.toString();
        }
        file << "|";

        Date actualOut = booking.getActualCheckOut();
        if (actualOut.isValid() && actualOut.getYear() != 1900) {
            file << actualOut.toString();
        }
        file << "|";

        string statusStr;
        switch (booking.getStatus()) {
            case Booking::Status::PENDING: statusStr = "PENDING"; break;
            case Booking::Status::CHECKED_IN: statusStr = "CHECKED_IN"; break;
            case Booking::Status::CHECKED_OUT: statusStr = "CHECKED_OUT"; break;
            case Booking::Status::CANCELLED: statusStr = "CANCELLED"; break;
        }
        file << statusStr << endl;
    }
}

void BookingManager::loadBookings(const string& filename) {
    ifstream file(filename);
    if (!file) return;

    bookings.clear();

    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        istringstream iss(line);
        string bookingId, customerId, roomNumber, checkInStr, checkOutStr, actualInStr, actualOutStr, statusStr;
        getline(iss, bookingId, '|');
        getline(iss, customerId, '|');
        getline(iss, roomNumber, '|');
        getline(iss, checkInStr, '|');
        getline(iss, checkOutStr, '|');
        getline(iss, actualInStr, '|');
        getline(iss, actualOutStr, '|');
        getline(iss, statusStr, '|');

        Booking booking(bookingId, customerId, roomNumber,
                        Date(checkInStr), Date(checkOutStr));

        if (!actualInStr.empty()) {
            booking.checkIn(Date(actualInStr));
        }
        if (!actualOutStr.empty()) {
            booking.checkOut(Date(actualOutStr));
        }

        if (statusStr == "CHECKED_IN") {
            booking.setStatus(Booking::Status::CHECKED_IN);
            if (!actualInStr.empty()) {
                booking.checkIn(Date(actualInStr));
            }
        } else if (statusStr == "CHECKED_OUT") {
            booking.setStatus(Booking::Status::CHECKED_OUT);
        } else if (statusStr == "CANCELLED") {
            booking.setStatus(Booking::Status::CANCELLED);
        }

        bookings.push_back(booking);
    }

    int maxId = 1;
    for (const auto& booking : bookings) {
        string id = booking.getBookingId();
        if (id.size() > 1 && id[0] == 'B') {
            try {
                int num = stoi(id.substr(1));
                if (num >= maxId) {
                    maxId = num + 1;
                }
            } catch (...) {}
        }
    }
    nextBookingId = maxId;
}