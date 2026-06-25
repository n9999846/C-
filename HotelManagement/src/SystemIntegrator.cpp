#include "SystemIntegrator.h"
#include "StandardRoom.h"
#include "DeluxeRoom.h"
#include "SuiteRoom.h"
#include "VIPCustomer.h"
#include "BookingException.h"
#include "Validator.h"
#include "FileHelper.h"
#include <sstream>
#include <iomanip>

// ========== 构造与析构 ==========

SystemIntegrator::SystemIntegrator()
    : roomMgr(nullptr), bookingMgr(nullptr), billingMgr(nullptr),
      logger(nullptr), initialized(false), dataPath("data/") {}

SystemIntegrator::~SystemIntegrator() {
    if (initialized) {
        shutdown();
    }
}

// ========== 初始化与关闭 ==========

bool SystemIntegrator::initialize() {
    if (initialized) return true;

    // 获取 Logger 单例
    logger = &Logger::getInstance();

    // 创建各 Manager
    roomMgr = new RoomManager();
    bookingMgr = new BookingManager();
    billingMgr = new BillingManager();

    // 关联 BookingManager 与 RoomManager
    bookingMgr->setRoomManager(roomMgr);

    // 从文件加载数据
    roomMgr->loadRooms(dataPath + "rooms.txt");
    bookingMgr->loadCustomers(dataPath + "customers.txt");
    bookingMgr->loadBookings(dataPath + "bookings.txt");
    billingMgr->loadBills(dataPath + "bills.txt");

    initialized = true;
    logger->info("系统初始化完成");
    return true;
}

void SystemIntegrator::shutdown() {
    if (!initialized) return;

    if (logger) logger->info("系统正在关闭，保存数据...");

    // 保存所有数据
    if (roomMgr)     roomMgr->saveRooms(dataPath + "rooms.txt");
    if (bookingMgr)  bookingMgr->saveCustomers(dataPath + "customers.txt");
    if (bookingMgr)  bookingMgr->saveBookings(dataPath + "bookings.txt");
    if (billingMgr)  billingMgr->saveBills(dataPath + "bills.txt");

    // 释放资源（RoomManager/BookingManager 的析构函数会 delete 内部指针）
    delete billingMgr;  billingMgr = nullptr;
    delete bookingMgr;  bookingMgr = nullptr;
    delete roomMgr;     roomMgr = nullptr;

    if (logger) {
        logger->info("系统已关闭");
        Logger::destroy();
        logger = nullptr;
    }

    initialized = false;
}

bool SystemIntegrator::isInitialized() const {
    return initialized;
}

// ========== 数据持久化 ==========

void SystemIntegrator::saveAllData() {
    if (!initialized) return;
    if (roomMgr)    roomMgr->saveRooms(dataPath + "rooms.txt");
    if (bookingMgr) bookingMgr->saveCustomers(dataPath + "customers.txt");
    if (bookingMgr) bookingMgr->saveBookings(dataPath + "bookings.txt");
    if (billingMgr) billingMgr->saveBills(dataPath + "bills.txt");
}

// ========== 房间管理接口 ==========

bool SystemIntegrator::addStandardRoom(const string& number, int floor, double price,
                                       int beds, bool window, bool wifi) {
    if (!initialized) return false;
    if (!Validator::isValidRoomNumber(number)) {
        logger->error("无效的房间号: " + number);
        return false;
    }
    if (roomMgr->findRoom(number)) {
        logger->error("房间已存在: " + number);
        return false;
    }
    roomMgr->addRoom(new StandardRoom(number, floor, price, beds, window, wifi));
    logger->info("添加标准间: " + number);
    saveAllData();
    return true;
}

bool SystemIntegrator::addDeluxeRoom(const string& number, int floor, double price,
                                     bool balcony, bool bathtub, bool breakfast) {
    if (!initialized) return false;
    if (!Validator::isValidRoomNumber(number)) {
        logger->error("无效的房间号: " + number);
        return false;
    }
    if (roomMgr->findRoom(number)) {
        logger->error("房间已存在: " + number);
        return false;
    }
    roomMgr->addRoom(new DeluxeRoom(number, floor, price, balcony, bathtub, breakfast));
    logger->info("添加豪华间: " + number);
    saveAllData();
    return true;
}

bool SystemIntegrator::addSuiteRoom(const string& number, int floor, double price,
                                    bool balcony, bool bathtub, bool breakfast,
                                    double livingArea, bool meetingRoom, bool kitchen) {
    if (!initialized) return false;
    if (!Validator::isValidRoomNumber(number)) {
        logger->error("无效的房间号: " + number);
        return false;
    }
    if (roomMgr->findRoom(number)) {
        logger->error("房间已存在: " + number);
        return false;
    }
    roomMgr->addRoom(new SuiteRoom(number, floor, price, balcony, bathtub, breakfast,
                                   livingArea, meetingRoom, kitchen));
    logger->info("添加套房: " + number);
    saveAllData();
    return true;
}

bool SystemIntegrator::deleteRoom(const string& roomNumber) {
    if (!initialized) return false;
    bool ok = roomMgr->removeRoom(roomNumber);
    if (ok) {
        logger->info("删除房间: " + roomNumber);
        saveAllData();
    } else {
        logger->error("删除房间失败，房间不存在: " + roomNumber);
    }
    return ok;
}

bool SystemIntegrator::updateRoom(const string& roomNumber, int floor, double price) {
    if (!initialized) return false;
    Room* room = roomMgr->findRoom(roomNumber);
    if (!room) {
        logger->error("修改房间失败，房间不存在: " + roomNumber);
        return false;
    }
    room->setFloor(floor);
    room->setPrice(price);
    logger->info("修改房间信息: " + roomNumber);
    saveAllData();
    return true;
}

vector<Room*> SystemIntegrator::getAvailableRooms() const {
    if (!initialized) return {};
    return roomMgr->getAvailableRooms();
}

vector<Room*> SystemIntegrator::getAllRooms() const {
    if (!initialized) return {};
    return roomMgr->getAllRooms();
}

// ========== 客户管理接口 ==========

bool SystemIntegrator::registerCustomer(const string& id, const string& name,
                                        const string& phone, const string& idCard) {
    if (!initialized) return false;
    if (!Validator::isNonEmptyString(id)) { logger->error("客户ID不能为空"); return false; }
    if (!Validator::isValidName(name)) { logger->error("无效的客户姓名: " + name); return false; }
    if (!Validator::isValidPhone(phone)) { logger->error("无效的手机号: " + phone); return false; }
    if (bookingMgr->findCustomer(id)) {
        logger->error("客户已存在: " + id);
        return false;
    }
    bookingMgr->addCustomer(new Customer(id, name, phone, idCard));
    logger->info("注册客户: " + name + " (" + id + ")");
    saveAllData();
    return true;
}

bool SystemIntegrator::registerVIPCustomer(const string& id, const string& name,
                                           const string& phone, const string& idCard,
                                           int level, const string& memberSince) {
    if (!initialized) return false;
    if (!Validator::isNonEmptyString(id)) { logger->error("客户ID不能为空"); return false; }
    if (!Validator::isValidName(name)) { logger->error("无效的客户姓名: " + name); return false; }
    if (!Validator::isValidPhone(phone)) { logger->error("无效的手机号: " + phone); return false; }
    if (bookingMgr->findCustomer(id)) {
        logger->error("客户已存在: " + id);
        return false;
    }
    bookingMgr->addCustomer(new VIPCustomer(id, name, phone, idCard, level, memberSince));
    logger->info("注册VIP客户: " + name + " (" + id + ")");
    saveAllData();
    return true;
}

bool SystemIntegrator::upgradeToVIP(const string& customerId, int level) {
    if (!initialized) return false;
    string today = Date::today().toString();
    bool ok = bookingMgr->upgradeCustomerToVIP(customerId, level, today);
    if (ok) {
        logger->info("客户升级为VIP: " + customerId + " 等级=" + to_string(level));
        saveAllData();
    } else {
        logger->error("升级VIP失败，客户不存在: " + customerId);
    }
    return ok;
}

vector<Customer*> SystemIntegrator::getAllCustomers() const {
    if (!initialized) return {};
    return bookingMgr->getAllCustomers();
}

bool SystemIntegrator::updateCustomer(const string& customerId, const string& name,
                                      const string& phone, const string& idCard) {
    if (!initialized) return false;
    Customer* customer = bookingMgr->findCustomer(customerId);
    if (!customer) {
        logger->error("修改客户失败，客户不存在: " + customerId);
        return false;
    }
    if (!Validator::isValidName(name)) { logger->error("无效的客户姓名: " + name); return false; }
    if (!Validator::isValidPhone(phone)) { logger->error("无效的手机号: " + phone); return false; }
    customer->setName(name);
    customer->setPhone(phone);
    customer->setIdCard(idCard);
    logger->info("修改客户信息: " + customerId);
    saveAllData();
    return true;
}

bool SystemIntegrator::deleteCustomer(const string& customerId) {
    if (!initialized) return false;
    Customer* customer = bookingMgr->findCustomer(customerId);
    if (!customer) {
        logger->error("删除客户失败，客户不存在: " + customerId);
        return false;
    }
    // 检查是否有活跃预订
    int active = bookingMgr->countActiveBookings(customerId);
    if (active > 0) {
        logger->error("删除客户失败，该客户有 " + to_string(active) + " 个活跃预订");
        return false;
    }
    bookingMgr->removeCustomer(customerId);
    logger->info("删除客户: " + customerId);
    saveAllData();
    return true;
}

// ========== 预订/入住/退房接口 ==========

string SystemIntegrator::createBooking(const string& customerId, const string& roomNumber,
                                       const Date& checkIn, const Date& checkOut) {
    if (!initialized) return "";
    try {
        Booking booking = bookingMgr->createBooking(customerId, roomNumber, checkIn, checkOut);
        logger->info("创建预订: " + booking.getBookingId() + " 客户=" + customerId + " 房间=" + roomNumber);
        saveAllData();
        return booking.getBookingId();
    } catch (const BookingException& e) {
        logger->error("创建预订失败: " + string(e.what()));
        return "";
    }
}

bool SystemIntegrator::cancelBooking(const string& bookingId) {
    if (!initialized) return false;
    try {
        bool ok = bookingMgr->cancelBooking(bookingId);
        if (ok) logger->info("取消预订: " + bookingId);
        if (ok) saveAllData();
        return ok;
    } catch (const BookingException& e) {
        logger->error("取消预订失败: " + string(e.what()));
        return false;
    }
}

bool SystemIntegrator::checkIn(const string& bookingId, const Date& today) {
    if (!initialized) return false;
    try {
        Booking b = bookingMgr->checkIn(bookingId, today);
        logger->info("入住成功: " + bookingId + " 房间=" + b.getRoomNumber());
        saveAllData();
        return true;
    } catch (const BookingException& e) {
        logger->error("入住失败: " + string(e.what()));
        return false;
    }
}

Bill SystemIntegrator::checkOut(const string& bookingId, const Date& today) {
    if (!initialized) {
        logger->error("系统未初始化");
        return Bill();
    }
    try {
        // 1. 退房，释放房间
        Booking booking = bookingMgr->checkOut(bookingId, today);

        // 2. 查找房间和客户，生成账单
        Room* room = roomMgr->findRoom(booking.getRoomNumber());
        Customer* customer = bookingMgr->findCustomer(booking.getCustomerId());

        if (!room || !customer) {
            logger->error("退房后查找房间或客户失败");
            return Bill();
        }

        Bill bill = billingMgr->generateBill(booking, *room, *customer);
        logger->info("退房成功: " + bookingId + " 生成账单=" + bill.getBillId()
                     + " 总额=" + to_string(bill.getTotalAmount()));
        saveAllData();
        return bill;
    } catch (const BookingException& e) {
        logger->error("退房失败: " + string(e.what()));
        return Bill();
    }
}

bool SystemIntegrator::extendStay(const string& bookingId, const Date& newCheckOut) {
    if (!initialized) return false;
    try {
        bookingMgr->extendStay(bookingId, newCheckOut);
        logger->info("续住成功: " + bookingId);
        saveAllData();
        return true;
    } catch (const BookingException& e) {
        logger->error("续住失败: " + string(e.what()));
        return false;
    }
}

// ========== 账单接口 ==========

bool SystemIntegrator::payBill(const string& billId) {
    if (!initialized) return false;
    bool ok = billingMgr->processPayment(billId);
    if (ok) {
        logger->info("支付成功: " + billId);
        saveAllData();
    } else {
        logger->error("支付失败，账单不存在或已支付: " + billId);
    }
    return ok;
}

vector<Bill> SystemIntegrator::getUnpaidBills() const {
    if (!initialized) return {};
    return billingMgr->getUnpaidBills();
}

vector<Bill> SystemIntegrator::getAllBills() const {
    if (!initialized) return {};
    return billingMgr->getAllBills();
}

vector<Booking> SystemIntegrator::getAllBookings() const {
    if (!initialized) return {};
    return bookingMgr->getActiveBookings();
}

// ========== 统计报表 ==========

string SystemIntegrator::getSystemStatusReport() const {
    if (!initialized) return "系统未初始化";

    ostringstream oss;
    oss << "========== 系统状态报告 ==========" << endl;

    // 房间统计
    oss << "【房间信息】" << endl;
    oss << "  总房间数: " << roomMgr->getTotalRoomCount() << endl;
    oss << "  空闲房间: " << roomMgr->getAvailableRoomCount() << endl;
    oss << "  入住率: " << fixed << setprecision(1) << roomMgr->getOccupancyRate() << "%" << endl;

    // 房型分布
    map<string, int> dist = roomMgr->getRoomTypeDistribution();
    for (auto& kv : dist) {
        oss << "  " << kv.first << ": " << kv.second << "间" << endl;
    }

    // 客户统计
    oss << "【客户信息】" << endl;
    oss << "  总客户数: " << bookingMgr->getAllCustomers().size() << endl;
    oss << "  VIP客户数: " << bookingMgr->getVIPCustomers().size() << endl;

    // 账单统计
    oss << "【账单信息】" << endl;
    oss << "  已付账单: " << billingMgr->getPaidBillCount() << endl;
    oss << "  未付账单: " << billingMgr->getUnpaidBillCount() << endl;
    oss << "  总营收: ¥" << fixed << setprecision(2) << billingMgr->getTotalRevenue() << endl;

    oss << "==================================";
    return oss.str();
}

map<string, double> SystemIntegrator::getRevenueStats() const {
    if (!initialized) return {};
    return billingMgr->getRevenueByRoomType();
}

vector<pair<string, double>> SystemIntegrator::getTopCustomers(int n) const {
    if (!initialized) return {};
    return billingMgr->getTopCustomers(n);
}

// ========== 异常包装 ==========

string SystemIntegrator::executeSafe(function<string()> action) {
    try {
        return action();
    } catch (const BookingException& e) {
        if (logger) logger->error("业务异常: " + string(e.what()));
        return "错误: " + string(e.what());
    } catch (const exception& e) {
        if (logger) logger->error("系统异常: " + string(e.what()));
        return "系统错误: " + string(e.what());
    } catch (...) {
        if (logger) logger->error("未知异常");
        return "未知错误";
    }
}
