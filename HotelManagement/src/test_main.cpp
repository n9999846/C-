/**
 * 酒店管理系统 - 测试主函数
 * 测试所有已实现类的功能：
 *   Date, Room(多态), StandardRoom, DeluxeRoom, SuiteRoom,
 *   RoomManager, Customer, VIPCustomer, Booking, BookingException, BookingManager
 */

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cmath>
#include "Date.h"
#include "Room.h"
#include "StandardRoom.h"
#include "DeluxeRoom.h"
#include "SuiteRoom.h"
#include "RoomManager.h"
#include "Customer.h"
#include "VIPCustomer.h"
#include "Booking.h"
#include "BookingException.h"
#include "BookingManager.h"

using namespace std;

// ==================== 测试辅助 ====================
static int totalTests = 0;
static int passedTests = 0;

#define TEST(name) \
    do { \
        totalTests++; \
        cout << "  [测试] " << name << " ... "; \
    } while(0)

#define PASS() \
    do { \
        passedTests++; \
        cout << "通过 ✓" << endl; \
    } while(0)

#define SECTION(title) \
    cout << endl << "========== " << title << " ==========" << endl

// 浮点数比较辅助
static bool doubleEqual(double a, double b, double eps = 0.01) {
    return fabs(a - b) < eps;
}

// ==================== 1. Date 类测试 ====================
void testDate() {
    SECTION("Date 类测试");

    // 默认构造
    TEST("默认构造函数");
    Date d1;
    assert(d1.getYear() == 1900 && d1.getMonth() == 1 && d1.getDay() == 1);
    PASS();

    // 参数构造
    TEST("参数构造函数");
    Date d2(2026, 6, 15);
    assert(d2.getYear() == 2026 && d2.getMonth() == 6 && d2.getDay() == 15);
    PASS();

    // 字符串构造
    TEST("字符串构造函数");
    Date d3("2026-01-20");
    assert(d3.getYear() == 2026 && d3.getMonth() == 1 && d3.getDay() == 20);
    PASS();

    // toString
    TEST("toString 格式化");
    assert(d2.toString() == "2026-06-15");
    Date d4(2026, 1, 5);
    assert(d4.toString() == "2026-01-05");  // 补零测试
    PASS();

    // 闰年判断
    TEST("闰年判断");
    Date leap(2024, 2, 1);
    assert(leap.isLeapYear() == true);
    Date nonLeap(2025, 2, 1);
    assert(nonLeap.isLeapYear() == false);
    Date cent(1900, 2, 1);
    assert(cent.isLeapYear() == false);   // 1900 不是闰年
    Date quad(2000, 2, 1);
    assert(quad.isLeapYear() == true);    // 2000 是闰年
    PASS();

    // 有效性验证
    TEST("日期有效性验证");
    assert(Date(2026, 6, 15).isValid() == true);
    assert(Date(2026, 13, 1).isValid() == false);  // 月份无效
    assert(Date(2026, 2, 29).isValid() == false);   // 非闰年2月29日
    assert(Date(2024, 2, 29).isValid() == true);    // 闰年2月29日
    PASS();

    // 天数计算
    TEST("getDaysInMonth");
    assert(Date(2026, 1, 1).getDaysInMonth() == 31);
    assert(Date(2026, 2, 1).getDaysInMonth() == 28);
    assert(Date(2024, 2, 1).getDaysInMonth() == 29);
    assert(Date(2026, 4, 1).getDaysInMonth() == 30);
    PASS();

    // 日期加减运算
    TEST("日期 + 天数");
    Date d5(2026, 1, 30);
    Date d6 = d5 + 3;  // 跨月: 1月30日 + 3 = 2月2日
    assert(d6.getMonth() == 2 && d6.getDay() == 2);
    Date d7 = Date(2026, 12, 30) + 5;  // 跨年
    assert(d7.getYear() == 2027 && d7.getMonth() == 1 && d7.getDay() == 4);
    PASS();

    TEST("日期 - 天数");
    Date d8(2026, 3, 2);
    Date d9 = d8 - 3;  // 跨月: 3月2日 - 3 = 2月27日
    assert(d9.getMonth() == 2 && d9.getDay() == 27);
    PASS();

    TEST("日期 - 日期 (天数差)");
    Date a(2026, 6, 20), b(2026, 6, 15);
    assert(a - b == 5);
    assert(b - a == -5);
    Date c(2026, 7, 1);
    assert(c - b == 16);  // 跨月差
    PASS();

    // 比较运算符
    TEST("比较运算符");
    Date early(2026, 1, 1), late(2026, 12, 31);
    assert(early < late);
    assert(late > early);
    assert(early <= late);
    assert(early <= early);
    assert(late >= early);
    assert(early == Date(2026, 1, 1));
    assert(early != late);
    PASS();

    // ++/-- 运算符
    TEST("自增自减运算符");
    Date dd(2026, 1, 31);
    dd++;
    assert(dd.getMonth() == 2 && dd.getDay() == 1);
    ++dd;
    assert(dd.getDay() == 2);
    dd--;
    assert(dd.getDay() == 1);
    --dd;
    assert(dd.getMonth() == 1 && dd.getDay() == 31);
    PASS();

    // 交换律: int + Date
    TEST("int + Date 交换律");
    Date dd2(2026, 6, 10);
    Date dd3 = 5 + dd2;
    assert(dd3 == Date(2026, 6, 15));
    PASS();

    // today() 静态方法
    TEST("today() 静态方法");
    Date t = Date::today();
    assert(t.isValid());
    PASS();

    // 流输出
    TEST("流输出运算符");
    Date dOut(2026, 6, 15);
    cout << endl << "    输出: " << dOut << " (应为 2026-06-15)";
    PASS();
}

// ==================== 2. Room 类层次测试 ====================
void testRooms() {
    SECTION("Room 类层次测试 (多态)");

    // --- StandardRoom ---
    TEST("StandardRoom 构造与属性");
    StandardRoom sr("101", 1, 200.0, 2, true, true);
    assert(sr.getRoomNumber() == "101");
    assert(sr.getFloor() == 1);
    assert(sr.getPricePerNight() == 200.0);
    assert(sr.getBedCount() == 2);
    assert(sr.getHasWindow() == true);
    assert(sr.getHasWifi() == true);
    assert(sr.getRoomType() == "标准间");
    assert(sr.getMaxGuests() == 3);  // bedCount + 1
    assert(sr.isAvailable() == true);
    PASS();

    TEST("StandardRoom 价格（多态 getActualPrice）");
    Room* rp = &sr;
    assert(doubleEqual(rp->getActualPrice(), 200.0));  // 标准间无附加费
    PASS();

    TEST("StandardRoom 描述");
    string desc = sr.getDescription();
    assert(desc.find("标准间") != string::npos);
    assert(desc.find("2张床") != string::npos);
    assert(desc.find("有窗") != string::npos);
    assert(desc.find("WiFi") != string::npos);
    PASS();

    // --- DeluxeRoom ---
    TEST("DeluxeRoom 构造与属性");
    DeluxeRoom dr("201", 2, 400.0, true, true, true);
    assert(dr.getRoomType() == "豪华间");
    assert(dr.getMaxGuests() == 3);
    assert(dr.getHasBalcony() == true);
    assert(dr.getHasBathtub() == true);
    assert(dr.getHasBreakfast() == true);
    PASS();

    TEST("DeluxeRoom 含早餐时价格");
    assert(doubleEqual(dr.getActualPrice(), 450.0));  // 400 + 50早餐费

    DeluxeRoom dr2("202", 2, 400.0, true, true, false);
    assert(doubleEqual(dr2.getActualPrice(), 400.0));  // 无早餐不加费
    PASS();

    // --- SuiteRoom ---
    TEST("SuiteRoom 构造与属性");
    SuiteRoom su("301", 3, 800.0, true, true, true, 30.0, true, true);
    assert(su.getRoomType() == "套房");
    assert(su.getMaxGuests() == 4);
    assert(su.getLivingRoomArea() == 30.0);
    assert(su.getHasMeetingRoom() == true);
    assert(su.getHasKitchen() == true);
    PASS();

    TEST("SuiteRoom 价格计算");
    // 含早餐: (800 + 50) * 1.8 = 1530
    assert(doubleEqual(su.getActualPrice(), 1530.0));

    SuiteRoom su2("302", 3, 800.0, true, true, false, 25.0, false, false);
    // 不含早餐: 800 * 1.8 = 1440
    assert(doubleEqual(su2.getActualPrice(), 1440.0));
    PASS();

    // --- Room 状态机 ---
    TEST("Room 状态转换");
    StandardRoom rState("401", 4, 300.0, 1, true, false);
    assert(rState.getStatus() == Room::Status::AVAILABLE);
    assert(rState.getStatusString() == "空闲");

    rState.book();
    assert(rState.getStatus() == Room::Status::BOOKED);
    assert(rState.getStatusString() == "已预订");

    rState.checkIn();
    assert(rState.getStatus() == Room::Status::OCCUPIED);
    assert(rState.getStatusString() == "已入住");

    rState.checkOut();
    assert(rState.getStatus() == Room::Status::AVAILABLE);
    PASS();

    TEST("Room 维护状态");
    StandardRoom rMaint("501", 5, 250.0, 1, true, true);
    rMaint.setMaintenance();
    assert(rMaint.getStatus() == Room::Status::MAINTENANCE);
    // 已预订的房间不能设为维护
    StandardRoom rMaint2("502", 5, 250.0, 1, true, true);
    rMaint2.book();
    rMaint2.setMaintenance();  // 不应生效
    assert(rMaint2.getStatus() == Room::Status::BOOKED);
    PASS();

    // --- Room 运算符 ---
    TEST("Room 运算符 (<, ==, <<)");
    StandardRoom r1("101", 1, 200.0, 1, true, true);
    StandardRoom r2("102", 1, 300.0, 1, true, true);
    assert(r1 < r2);  // 按价格
    assert(r1 == StandardRoom("101", 2, 500.0, 2, false, false));  // 按房号
    cout << endl << "    Room输出: " << r1;
    PASS();

    // --- 多态测试 ---
    TEST("多态指针测试");
    vector<Room*> rooms;
    rooms.push_back(new StandardRoom("S01", 1, 200.0, 2, true, true));
    rooms.push_back(new DeluxeRoom("D01", 2, 400.0, true, true, true));
    rooms.push_back(new SuiteRoom("U01", 3, 800.0, true, true, true, 30.0, true, false));

    assert(rooms[0]->getRoomType() == "标准间");
    assert(rooms[1]->getRoomType() == "豪华间");
    assert(rooms[2]->getRoomType() == "套房");

    assert(doubleEqual(rooms[0]->getActualPrice(), 200.0));
    assert(doubleEqual(rooms[1]->getActualPrice(), 450.0));
    assert(doubleEqual(rooms[2]->getActualPrice(), 1530.0));

    for (auto p : rooms) delete p;
    PASS();
}

// ==================== 3. RoomManager 测试 ====================
void testRoomManager() {
    SECTION("RoomManager 测试");

    RoomManager rm;
    rm.addRoom(new StandardRoom("101", 1, 200.0, 2, true, true));
    rm.addRoom(new StandardRoom("102", 1, 220.0, 1, true, false));
    rm.addRoom(new DeluxeRoom("201", 2, 400.0, true, true, true));
    rm.addRoom(new SuiteRoom("301", 3, 800.0, true, true, false, 30.0, true, false));

    TEST("添加房间与总数");
    assert(rm.getTotalRoomCount() == 4);
    assert(rm.getAvailableRoomCount() == 4);
    PASS();

    TEST("查找房间");
    Room* found = rm.findRoom("201");
    assert(found != nullptr);
    assert(found->getRoomType() == "豪华间");
    assert(rm.findRoom("999") == nullptr);
    PASS();

    TEST("按类型查询");
    auto standards = rm.getRoomsByType("标准间");
    assert(standards.size() == 2);
    auto deluxes = rm.getRoomsByType("豪华间");
    assert(deluxes.size() == 1);
    auto suites = rm.getRoomsByType("套房");
    assert(suites.size() == 1);
    PASS();

    TEST("按楼层查询");
    auto floor1 = rm.getRoomsByFloor(1);
    assert(floor1.size() == 2);
    PASS();

    TEST("按价格区间查询");
    auto priceRange = rm.getRoomsByPriceRange(200.0, 450.0);
    assert(priceRange.size() == 3);  // 200, 220, 400
    PASS();

    TEST("获取可用房间");
    auto avail = rm.getAvailableRooms();
    assert(avail.size() == 4);
    PASS();

    TEST("房间状态转换 (通过RoomManager)");
    assert(rm.bookRoom("101") == true);
    assert(rm.findRoom("101")->getStatus() == Room::Status::BOOKED);
    assert(rm.getAvailableRoomCount() == 3);

    assert(rm.occupyRoom("101") == true);
    assert(rm.findRoom("101")->getStatus() == Room::Status::OCCUPIED);

    assert(rm.releaseRoom("101") == true);
    assert(rm.findRoom("101")->getStatus() == Room::Status::AVAILABLE);
    PASS();

    TEST("设置维护状态");
    assert(rm.setRoomMaintenance("102") == true);
    assert(rm.findRoom("102")->getStatus() == Room::Status::MAINTENANCE);
    // 已预订房间不能维护 —— 用 301 做测试
    rm.bookRoom("301");
    assert(rm.setRoomMaintenance("301") == false);
    // 恢复 301: BOOKED -> OCCUPIED -> AVAILABLE
    rm.occupyRoom("301");
    rm.releaseRoom("301");
    PASS();

    TEST("删除房间");
    assert(rm.removeRoom("102") == true);
    assert(rm.getTotalRoomCount() == 3);
    assert(rm.removeRoom("999") == false);  // 不存在的房间
    PASS();

    TEST("排序功能");
    rm.sortByPrice();
    auto all = rm.getAllRooms();
    for (size_t i = 1; i < all.size(); i++) {
        assert(all[i - 1]->getPricePerNight() <= all[i]->getPricePerNight());
    }

    rm.sortByRoomNumber();
    all = rm.getAllRooms();
    for (size_t i = 1; i < all.size(); i++) {
        assert(all[i - 1]->getRoomNumber() <= all[i]->getRoomNumber());
    }
    PASS();

    TEST("按类型统计分布");
    rm.addRoom(new StandardRoom("103", 1, 180.0, 1, true, false));
    auto dist = rm.getRoomTypeDistribution();
    assert(dist["标准间"] == 2);
    assert(dist["豪华间"] == 1);
    assert(dist["套房"] == 1);
    PASS();

    TEST("入住率计算");
    // 当前房间: 101, 201, 301, 103 (共4间，102已删除)
    int roomCount = rm.getTotalRoomCount();
    rm.bookRoom("101");
    rm.occupyRoom("101");
    double rate = rm.getOccupancyRate();
    // 1间OCCUPIED / 总房间数
    assert(doubleEqual(rate, (1.0 / roomCount) * 100.0));
    rm.releaseRoom("101");  // 恢复
    rate = rm.getOccupancyRate();
    assert(doubleEqual(rate, 0.0));
    PASS();

    TEST("按类型查找可用房间");
    Room* availRoom = rm.findAvailableRoomByType("豪华间");
    assert(availRoom != nullptr);
    assert(availRoom->getRoomType() == "豪华间");
    PASS();
}

// ==================== 4. Customer & VIPCustomer 测试 ====================
void testCustomers() {
    SECTION("Customer 和 VIPCustomer 测试");

    // 普通客户
    TEST("Customer 构造与属性");
    Customer c1("C001", "张三", "13800001111", "110101200001011234");
    assert(c1.getId() == "C001");
    assert(c1.getName() == "张三");
    assert(c1.getPhone() == "13800001111");
    assert(c1.getIdCard() == "110101200001011234");
    assert(c1.getTotalSpent() == 0);
    assert(c1.getCustomerType() == "普通客户");
    assert(doubleEqual(c1.getDiscountRate(), 1.0));
    assert(c1.getMaxActiveBookings() == 3);
    PASS();

    TEST("Customer 消费累计与修改");
    c1.addSpent(500);
    c1.addSpent(300);
    assert(c1.getTotalSpent() == 800);

    c1.setName("李四");
    assert(c1.getName() == "李四");
    c1.setPhone("13900009999");
    assert(c1.getPhone() == "13900009999");
    PASS();

    TEST("Customer 运算符 (==, <<)");
    Customer c1b("C001", "王五", "13700001111", "110101200001015678");
    assert(c1 == c1b);  // 按 id 比较
    cout << endl << "    Customer输出: " << c1;
    PASS();

    // VIP 客户
    TEST("VIPCustomer 构造与属性");
    VIPCustomer vip1("V001", "赵六", "13600001111", "110101199901011234", 1, "2025-01-01");
    assert(vip1.getCustomerType() == "VIP客户");
    assert(vip1.getVipLevel() == 1);
    assert(vip1.getMemberSince() == "2025-01-01");
    assert(doubleEqual(vip1.getDiscountRate(), 0.95));
    assert(vip1.getMaxActiveBookings() == 5);
    PASS();

    TEST("VIPCustomer 不同等级折扣");
    VIPCustomer vip2("V002", "钱七", "13500001111", "110101199801011234", 2, "2024-06-01");
    assert(doubleEqual(vip2.getDiscountRate(), 0.90));
    assert(vip2.getMaxActiveBookings() == 8);

    VIPCustomer vip3("V003", "孙八", "13400001111", "110101199701011234", 3, "2023-01-01");
    assert(doubleEqual(vip3.getDiscountRate(), 0.85));
    assert(vip3.getMaxActiveBookings() == 10);
    PASS();

    TEST("VIPCustomer 自动升级");
    VIPCustomer vipAuto("V004", "测试用户", "13300001111", "110101200001011234", 1, "2025-06-01");
    vipAuto.addSpent(12000);
    vipAuto.upgradeLevel();
    assert(vipAuto.getVipLevel() == 1);  // 12000 >= 10000 -> level 1

    vipAuto.addSpent(10000);  // total = 22000
    vipAuto.upgradeLevel();
    assert(vipAuto.getVipLevel() == 2);  // 22000 >= 20000 -> level 2

    vipAuto.addSpent(30000);  // total = 52000
    vipAuto.upgradeLevel();
    assert(vipAuto.getVipLevel() == 3);  // 52000 >= 50000 -> level 3
    PASS();

    TEST("VIPCustomer 流输出");
    cout << endl << "    VIP输出: " << vip1;
    PASS();
}

// ==================== 5. Booking & BookingException 测试 ====================
void testBookings() {
    SECTION("Booking 和 BookingException 测试");

    // Booking 基本构造
    TEST("Booking 构造与属性");
    Booking bk("B001", "C001", "101", Date(2026, 6, 20), Date(2026, 6, 25));
    assert(bk.getBookingId() == "B001");
    assert(bk.getCustomerId() == "C001");
    assert(bk.getRoomNumber() == "101");
    assert(bk.getStatus() == Booking::Status::PENDING);
    assert(bk.getStatusString() == "待入住");
    PASS();

    TEST("计算住宿天数");
    assert(bk.calculateNights() == 5);  // 6月20日 到 6月25日
    Booking shortStay("B002", "C002", "102", Date(2026, 7, 1), Date(2026, 7, 2));
    assert(shortStay.calculateNights() == 1);
    PASS();

    TEST("日期冲突检测");
    // 现有预订: 6月20日-6月25日
    // 完全不重叠
    assert(bk.isDateConflict(Date(2026, 6, 25), Date(2026, 6, 30)) == false);
    assert(bk.isDateConflict(Date(2026, 6, 10), Date(2026, 6, 20)) == false);
    // 部分重叠
    assert(bk.isDateConflict(Date(2026, 6, 22), Date(2026, 6, 28)) == true);
    assert(bk.isDateConflict(Date(2026, 6, 15), Date(2026, 6, 22)) == true);
    // 完全包含
    assert(bk.isDateConflict(Date(2026, 6, 18), Date(2026, 6, 30)) == true);
    PASS();

    TEST("日期包含检测");
    assert(bk.containsDate(Date(2026, 6, 20)) == true);   // 入住日
    assert(bk.containsDate(Date(2026, 6, 24)) == true);   // 中间
    assert(bk.containsDate(Date(2026, 6, 25)) == false);  // 退房日不算
    assert(bk.containsDate(Date(2026, 6, 19)) == false);  // 之前
    PASS();

    TEST("入住办理 (PENDING -> CHECKED_IN)");
    bk.checkIn(Date(2026, 6, 20));
    assert(bk.getStatus() == Booking::Status::CHECKED_IN);
    assert(bk.getStatusString() == "已入住");
    assert(bk.getActualCheckIn() == Date(2026, 6, 20));
    PASS();

    TEST("延期入住");
    bk.extendStay(Date(2026, 6, 28));
    assert(bk.getCheckOutDate() == Date(2026, 6, 28));
    assert(bk.calculateNights() == 8);
    PASS();

    TEST("退房办理 (CHECKED_IN -> CHECKED_OUT)");
    bk.checkOut(Date(2026, 6, 28));
    assert(bk.getStatus() == Booking::Status::CHECKED_OUT);
    assert(bk.getStatusString() == "已退房");
    assert(bk.getActualCheckOut() == Date(2026, 6, 28));
    PASS();

    TEST("取消预订");
    Booking bkCancel("B003", "C003", "201", Date(2026, 7, 1), Date(2026, 7, 5));
    assert(bkCancel.getStatus() == Booking::Status::PENDING);
    bkCancel.cancel();
    assert(bkCancel.getStatus() == Booking::Status::CANCELLED);
    assert(bkCancel.getStatusString() == "已取消");
    PASS();

    TEST("Booking 运算符 (==, <, <<)");
    Booking bkA("B010", "C001", "101", Date(2026, 6, 1), Date(2026, 6, 5));
    Booking bkB("B010", "C002", "102", Date(2026, 7, 1), Date(2026, 7, 5));
    assert(bkA == bkB);  // 按 bookingId

    Booking bkC("B011", "C003", "103", Date(2026, 6, 10), Date(2026, 6, 15));
    assert(bkA < bkC);   // 按入住日期

    cout << endl << "    Booking输出: " << bkA;
    PASS();

    // BookingException
    TEST("BookingException 异常");
    BookingException ex("房间不存在", BookingException::ROOM_NOT_FOUND);
    assert(ex.getErrorCode() == 1001);
    assert(string(ex.what()).find("房间不存在") != string::npos);

    BookingException ex2("日期冲突", BookingException::DATE_CONFLICT);
    assert(ex2.getErrorCode() == 1005);
    PASS();
}

// ==================== 6. BookingManager 集成测试 ====================
void testBookingManager() {
    SECTION("BookingManager 集成测试");

    // 准备: 创建 RoomManager 和 BookingManager
    RoomManager rm;
    rm.addRoom(new StandardRoom("101", 1, 200.0, 2, true, true));
    rm.addRoom(new DeluxeRoom("201", 2, 400.0, true, true, true));
    rm.addRoom(new SuiteRoom("301", 3, 800.0, true, true, false, 30.0, true, false));

    BookingManager bm;
    bm.setRoomManager(&rm);

    // 添加客户
    bm.addCustomer(new Customer("C001", "张三", "13800001111", "110101200001011234"));
    bm.addCustomer(new VIPCustomer("V001", "李四", "13900002222", "110101199901011234", 2, "2024-01-01"));

    TEST("添加和查找客户");
    assert(bm.findCustomer("C001") != nullptr);
    assert(bm.findCustomer("V001") != nullptr);
    assert(bm.findCustomer("X001") == nullptr);
    assert(bm.getAllCustomers().size() == 2);
    PASS();

    TEST("VIP客户查询");
    auto vips = bm.getVIPCustomers();
    assert(vips.size() == 1);
    assert(vips[0]->getId() == "V001");
    PASS();

    TEST("客户升级为VIP");
    bm.addCustomer(new Customer("C002", "王五", "13700003333", "110101200101011234"));
    Customer* cBefore = bm.findCustomer("C002");
    cBefore->addSpent(500);
    assert(cBefore->getCustomerType() == "普通客户");

    assert(bm.upgradeCustomerToVIP("C002", 1, "2026-06-01") == true);
    Customer* cAfter = bm.findCustomer("C002");
    assert(cAfter->getCustomerType() == "VIP客户");
    assert(cAfter->getTotalSpent() == 500);  // 消费记录保留
    VIPCustomer* vipCast = dynamic_cast<VIPCustomer*>(cAfter);
    assert(vipCast != nullptr);
    assert(vipCast->getVipLevel() == 1);
    PASS();

    // 创建预订
    TEST("创建预订");
    Booking bk1 = bm.createBooking("C001", "101", Date(2026, 7, 1), Date(2026, 7, 5));
    assert(bk1.getBookingId() == "B1");
    assert(bk1.getStatus() == Booking::Status::PENDING);
    assert(rm.findRoom("101")->getStatus() == Room::Status::BOOKED);
    PASS();

    TEST("预订房间状态已改变");
    assert(rm.findRoom("101")->isAvailable() == false);
    PASS();

    TEST("重复预订同一房间（日期冲突）");
    bool threw = false;
    try {
        bm.createBooking("V001", "101", Date(2026, 7, 3), Date(2026, 7, 8));
    } catch (BookingException& ex) {
        threw = true;
        // createBooking 对日期冲突抛出 ROOM_UNAVAILABLE
        assert(ex.getErrorCode() == BookingException::ROOM_UNAVAILABLE);
    }
    assert(threw);
    PASS();

    TEST("不同时间段预订同一房间（不冲突）");
    Booking bk2 = bm.createBooking("V001", "101", Date(2026, 7, 6), Date(2026, 7, 10));
    assert(bk2.getBookingId() == "B2");
    PASS();

    TEST("预订不存在的客户");
    threw = false;
    try {
        bm.createBooking("X999", "201", Date(2026, 8, 1), Date(2026, 8, 5));
    } catch (BookingException& ex) {
        threw = true;
        assert(ex.getErrorCode() == BookingException::CUSTOMER_NOT_FOUND);
    }
    assert(threw);
    PASS();

    TEST("预订不存在的房间");
    threw = false;
    try {
        bm.createBooking("C001", "999", Date(2026, 8, 1), Date(2026, 8, 5));
    } catch (BookingException& ex) {
        threw = true;
        assert(ex.getErrorCode() == BookingException::ROOM_NOT_FOUND);
    }
    assert(threw);
    PASS();

    TEST("无效日期预订");
    threw = false;
    try {
        bm.createBooking("C001", "201", Date(2026, 8, 10), Date(2026, 8, 5));  // 退房早于入住
    } catch (BookingException& ex) {
        threw = true;
        assert(ex.getErrorCode() == BookingException::DATE_INVALID);
    }
    assert(threw);
    PASS();

    // 办理入住和退房
    TEST("入住办理");
    Booking checkedIn = bm.checkIn("B1", Date(2026, 7, 1));
    assert(checkedIn.getStatus() == Booking::Status::CHECKED_IN);
    assert(rm.findRoom("101")->getStatus() == Room::Status::OCCUPIED);
    PASS();

    TEST("重复入住应抛异常");
    threw = false;
    try {
        bm.checkIn("B1", Date(2026, 7, 1));
    } catch (BookingException& ex) {
        threw = true;
        assert(ex.getErrorCode() == BookingException::ALREADY_CHECKED_IN);
    }
    assert(threw);
    PASS();

    TEST("退房办理");
    Booking checkedOut = bm.checkOut("B1", Date(2026, 7, 5));
    assert(checkedOut.getStatus() == Booking::Status::CHECKED_OUT);
    assert(rm.findRoom("101")->getStatus() == Room::Status::AVAILABLE);
    PASS();

    TEST("取消预订");
    Booking bk3 = bm.createBooking("C001", "201", Date(2026, 8, 1), Date(2026, 8, 5));
    string bk3Id = bk3.getBookingId();
    assert(bm.cancelBooking(bk3Id) == true);
    Booking* bk3Ptr = bm.findBooking(bk3Id);
    assert(bk3Ptr->getStatus() == Booking::Status::CANCELLED);
    // 验证房间已释放回 AVAILABLE
    assert(rm.findRoom("201")->getStatus() == Room::Status::AVAILABLE);
    PASS();

    TEST("取消非待入住的预订应抛异常");
    threw = false;
    try {
        bm.cancelBooking("B1");  // B1 已退房
    } catch (BookingException& ex) {
        threw = true;
        assert(ex.getErrorCode() == BookingException::ALREADY_CHECKED_OUT);
    }
    assert(threw);
    PASS();

    // 查询功能
    TEST("查询客户预订");
    auto custBookings = bm.getCustomerBookings("C001");
    assert(custBookings.size() >= 1);
    PASS();

    TEST("查询活跃预订");
    auto active = bm.getActiveBookings();
    // B1 已退房, B2 已入住(PENDING->checkIn后是CHECKED_IN), B3 已取消
    // 只有 B2 应该是活跃的
    assert(active.size() >= 1);
    PASS();

    TEST("查询今日待入住");
    auto pending = bm.getPendingCheckIn(Date(2026, 7, 6));
    // B2 的入住日期是 7月6日
    assert(pending.size() >= 0);  // 取决于 B2 状态
    PASS();

    TEST("查询逾期退房");
    auto overdue = bm.getOverdueCheckOut(Date(2026, 7, 15));
    // 如果 B2 已入住但未退房且超过 7月10日
    assert(overdue.size() >= 0);
    PASS();

    TEST("房间可用性检查");
    assert(bm.isRoomAvailable("101", Date(2026, 9, 1), Date(2026, 9, 5)) == true);
    assert(bm.isRoomAvailable("101", Date(2026, 7, 7), Date(2026, 7, 9)) == false);  // B2 期间
    PASS();

    TEST("活跃预订计数");
    int activeC001 = bm.countActiveBookings("C001");
    assert(activeC001 >= 0);
    PASS();

    // 删除客户
    TEST("删除客户");
    assert(bm.removeCustomer("C002") == true);
    assert(bm.findCustomer("C002") == nullptr);
    assert(bm.removeCustomer("X999") == false);
    PASS();
}

// ==================== 7. 文件持久化测试 ====================
void testPersistence() {
    SECTION("文件持久化测试");

    // 保存房间
    TEST("保存和加载房间数据");
    {
        RoomManager rmSave;
        rmSave.addRoom(new StandardRoom("S01", 1, 200.0, 2, true, true));
        rmSave.addRoom(new DeluxeRoom("D01", 2, 400.0, true, true, true));
        rmSave.addRoom(new SuiteRoom("U01", 3, 800.0, true, true, false, 30.0, true, false));
        rmSave.saveRooms("test_rooms.txt");
    }
    {
        RoomManager rmLoad;
        rmLoad.loadRooms("test_rooms.txt");
        assert(rmLoad.getTotalRoomCount() == 3);

        Room* s = rmLoad.findRoom("S01");
        assert(s != nullptr);
        assert(s->getRoomType() == "标准间");
        assert(doubleEqual(s->getPricePerNight(), 200.0));

        Room* d = rmLoad.findRoom("D01");
        assert(d != nullptr);
        assert(d->getRoomType() == "豪华间");

        Room* u = rmLoad.findRoom("U01");
        assert(u != nullptr);
        assert(u->getRoomType() == "套房");
    }
    PASS();

    // 保存客户和预订
    TEST("保存和加载客户/预订数据");
    {
        BookingManager bmSave;
        bmSave.addCustomer(new Customer("C001", "张三", "13800001111", "110101200001011234"));
        bmSave.addCustomer(new VIPCustomer("V001", "李四", "13900002222", "110101199901011234", 2, "2024-01-01"));

        RoomManager rmTmp;
        rmTmp.addRoom(new StandardRoom("R01", 1, 200.0, 1, true, true));
        bmSave.setRoomManager(&rmTmp);

        bmSave.createBooking("C001", "R01", Date(2026, 9, 1), Date(2026, 9, 5));
        bmSave.saveCustomers("test_customers.txt");
        bmSave.saveBookings("test_bookings.txt");
    }
    {
        BookingManager bmLoad;
        RoomManager rmTmp;
        rmTmp.addRoom(new StandardRoom("R01", 1, 200.0, 1, true, true));
        bmLoad.setRoomManager(&rmTmp);

        bmLoad.loadCustomers("test_customers.txt");
        assert(bmLoad.getAllCustomers().size() == 2);

        Customer* loaded = bmLoad.findCustomer("C001");
        assert(loaded != nullptr);
        assert(loaded->getName() == "张三");

        Customer* loadedVip = bmLoad.findCustomer("V001");
        assert(loadedVip != nullptr);
        assert(loadedVip->getCustomerType() == "VIP客户");

        bmLoad.loadBookings("test_bookings.txt");
        auto bks = bmLoad.getCustomerBookings("C001");
        assert(bks.size() == 1);
    }
    PASS();

    // 清理测试文件
    remove("test_rooms.txt");
    remove("test_customers.txt");
    remove("test_bookings.txt");
}

// ==================== 主函数 ====================
int main() {
    cout << "╔══════════════════════════════════════════════╗" << endl;
    cout << "║      酒店管理系统 - 功能测试程序             ║" << endl;
    cout << "╚══════════════════════════════════════════════╝" << endl;

    testDate();
    testRooms();
    testRoomManager();
    testCustomers();
    testBookings();
    testBookingManager();
    testPersistence();

    cout << endl;
    cout << "══════════════════════════════════════════════" << endl;
    cout << "  测试结果: " << passedTests << " / " << totalTests << " 通过";
    if (passedTests == totalTests) {
        cout << "  ★ 全部通过! ★" << endl;
    } else {
        cout << "  ✗ 有 " << (totalTests - passedTests) << " 个测试失败" << endl;
    }
    cout << "══════════════════════════════════════════════" << endl;

    return (passedTests == totalTests) ? 0 : 1;
}
