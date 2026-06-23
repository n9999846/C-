#include <iostream>
#include <iomanip>
#include "SystemIntegrator.h"

using namespace std;

int main() {
    SystemIntegrator sys;

    cout << "========================================" << endl;
    cout << "       酒店客房管理系统 v1.0" << endl;
    cout << "========================================" << endl;

    // ========== 1. 系统初始化 ==========
    cout << "\n[1] 系统初始化..." << endl;
    if (!sys.initialize()) {
        cout << "初始化失败！" << endl;
        return 1;
    }
    cout << "  系统初始化成功。" << endl;

    // ========== 2. 添加房间 ==========
    cout << "\n[2] 添加房间..." << endl;
    sys.addStandardRoom("101", 1, 299.0, 2, true, true);
    sys.addStandardRoom("102", 1, 299.0, 1, true, true);
    sys.addDeluxeRoom("201", 2, 499.0, true, true, true);
    sys.addDeluxeRoom("202", 2, 499.0, true, false, true);
    sys.addSuiteRoom("301", 3, 899.0, true, true, true, 45.0, true, true);
    cout << "  共添加 " << sys.getAllRooms().size() << " 间房间。" << endl;

    // 显示空闲房间
    cout << "  空闲房间:" << endl;
    for (Room* r : sys.getAvailableRooms()) {
        cout << "    " << r->getRoomNumber() << " " << r->getRoomType()
             << " ¥" << r->getActualPrice() << "/晚" << endl;
    }

    // ========== 3. 注册客户 ==========
    cout << "\n[3] 注册客户..." << endl;
    sys.registerCustomer("C001", "张三", "13800001111", "110101200001011234");
    sys.registerCustomer("C002", "李四", "13900002222", "110101200002021234");
    sys.registerVIPCustomer("C003", "王五", "13700003333", "110101200003031234", 2, "2024-01-15");
    cout << "  共注册 " << sys.getAllCustomers().size() << " 位客户。" << endl;

    // ========== 4. 创建预订 ==========
    cout << "\n[4] 创建预订..." << endl;
    string b1 = sys.createBooking("C001", "101", Date(2026, 6, 10), Date(2026, 6, 15));
    string b2 = sys.createBooking("C002", "201", Date(2026, 6, 12), Date(2026, 6, 16));
    string b3 = sys.createBooking("C003", "301", Date(2026, 6, 15), Date(2026, 6, 20));
    cout << "  预订创建: " << b1 << ", " << b2 << ", " << b3 << endl;

    // 测试异常：重复预订
    string fail = sys.createBooking("C001", "101", Date(2026, 6, 10), Date(2026, 6, 15));
    cout << "  重复预订测试: " << (fail.empty() ? "已正确拒绝" : fail) << endl;

    // ========== 5. 入住 ==========
    cout << "\n[5] 办理入住..." << endl;
    sys.checkIn(b1, Date(2026, 6, 10));
    sys.checkIn(b2, Date(2026, 6, 12));
    sys.checkIn(b3, Date(2026, 6, 15));
    cout << "  入住办理完成。" << endl;

    // ========== 6. 退房 & 生成账单 ==========
    cout << "\n[6] 办理退房..." << endl;
    Bill bill1 = sys.checkOut(b1, Date(2026, 6, 15));
    cout << "  账单1: " << bill1 << endl;

    Bill bill2 = sys.checkOut(b2, Date(2026, 6, 16));
    cout << "  账单2: " << bill2 << endl;

    Bill bill3 = sys.checkOut(b3, Date(2026, 6, 20));
    cout << "  账单3: " << bill3 << endl;

    // ========== 7. 支付 ==========
    cout << "\n[7] 支付账单..." << endl;
    sys.payBill(bill1.getBillId());
    sys.payBill(bill3.getBillId());
    cout << "  已支付账单: " << bill1.getBillId() << ", " << bill3.getBillId() << endl;
    cout << "  未付账单数: " << sys.getUnpaidBills().size() << endl;

    // ========== 8. 系统状态报告 ==========
    cout << "\n[8] 系统状态报告:" << endl;
    cout << sys.getSystemStatusReport() << endl;

    // ========== 9. 测试 executeSafe ==========
    cout << "\n[9] 异常包装测试..." << endl;
    string result = sys.executeSafe([]() -> string {
        throw BookingException("测试异常", BookingException::ROOM_NOT_FOUND);
    });
    cout << "  executeSafe 捕获: " << result << endl;

    // ========== 10. 关闭系统 ==========
    cout << "\n[10] 关闭系统..." << endl;
    sys.shutdown();
    cout << "  系统已安全关闭。" << endl;

    return 0;
}
