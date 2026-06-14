# 酒店客房管理系统

## 项目结构

```
HotelManagement/
├── include/          # 头文件目录
│   ├── Room.h              # 成员A
│   ├── StandardRoom.h      # 成员A
│   ├── DeluxeRoom.h        # 成员A
│   ├── SuiteRoom.h         # 成员A
│   ├── RoomManager.h       # 成员A
│   ├── Customer.h          # 成员B
│   ├── VIPCustomer.h       # 成员B
│   ├── Booking.h           # 成员B
│   ├── BookingException.h  # 成员B
│   ├── BookingManager.h    # 成员B
│   ├── Date.h              # 成员C
│   ├── Bill.h              # 成员C
│   ├── BillingManager.h    # 成员C
│   ├── Logger.h            # 成员D
│   ├── Validator.h         # 成员D
│   ├── FileHelper.h        # 成员D
│   └── SystemIntegrator.h  # 成员D
├── src/              # 源文件目录
├── data/             # 数据文件目录
│   ├── rooms.txt
│   ├── customers.txt
│   ├── bookings.txt
│   ├── bills.txt
│   └── system.log
├── CMakeLists.txt    # CMake 构建配置
└── README.md         # 本文件
```

## 成员分工

| 成员 | 负责文件 | 主要OOP知识点 | 难度 |
|------|---------|--------------|------|
| A | Room*.h/cpp, RoomManager.* | 抽象类、二次继承、多态容器、虚析构 | ★★★ |
| B | Customer*.h/cpp, Booking*.h/cpp, BookingManager.* | 继承多态、二次继承、自定义异常、业务流程 | ★★★☆ |
| C | Date.*, Bill.*, BillingManager.* | 运算符重载、STL算法、lambda | ★★★★ |
| D | Logger.*, Validator.*, FileHelper.*, SystemIntegrator.*, main.cpp | 单例模式、静态工具类、门面模式 | ★★☆ |

## 编译说明

```bash
mkdir build
cd build
cmake ..
make
```

## 数据文件格式

详见 `酒店管理系统详细设计.md` 第8章。

## Qt GUI

Qt界面由AI辅助生成，最后两周集成到项目中。
