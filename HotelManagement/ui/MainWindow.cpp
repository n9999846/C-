#include "MainWindow.h"
#include "DashboardPage.h"
#include "RoomPage.h"
#include "CustomerPage.h"
#include "BookingPage.h"
#include "StatsPage.h"

#include <QApplication>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDateTime>
#include <QTimer>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), sys(nullptr) {
    setWindowTitle("酒店客房管理系统 v1.0");
    setMinimumSize(1000, 650);

    // 初始化系统
    sys = new SystemIntegrator();
    sys->initialize();

    setupMenuBar();
    setupStatusBar();
    setupTabs();

    // 定时刷新状态栏
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this]() {
        statusLabel->setText(QString("  当前用户: admin  |  %1")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
    });
    timer->start(1000);

    refreshAll();
}

MainWindow::~MainWindow() {
    if (sys) {
        if (sys->isInitialized()) sys->shutdown();
        delete sys;
    }
}

void MainWindow::closeEvent(QCloseEvent* event) {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "退出确认", "确定要退出系统吗？\n所有数据将自动保存。",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::setupMenuBar() {
    QMenuBar* menuBar = this->menuBar();

    // 系统菜单
    QMenu* sysMenu = menuBar->addMenu("系统(&S)");
    QAction* exitAction = sysMenu->addAction("退出(&Q)");
    exitAction->setShortcut(QKeySequence("Ctrl+Q"));
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // 帮助菜单
    QMenu* helpMenu = menuBar->addMenu("帮助(&H)");
    QAction* aboutAction = helpMenu->addAction("关于(&A)");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);

    menuBar->setStyleSheet("QMenuBar { background-color: #2c3e50; color: white; }"
                           "QMenuBar::item:selected { background-color: #34495e; }"
                           "QMenu { background-color: #2c3e50; color: white; }"
                           "QMenu::item:selected { background-color: #3498db; }");
}

void MainWindow::setupStatusBar() {
    statusLabel = new QLabel("  当前用户: admin");
    statusBar()->addWidget(statusLabel);
    statusBar()->setStyleSheet("QStatusBar { background-color: #ecf0f1; border-top: 1px solid #bdc3c7; }"
                               "QLabel { color: #555; font-size: 12px; }");
}

void MainWindow::setupTabs() {
    tabWidget = new QTabWidget();
    tabWidget->setStyleSheet(
        "QTabWidget::pane { border: 1px solid #bdc3c7; background-color: white; }"
        "QTabBar::tab { background-color: #ecf0f1; padding: 10px 20px; margin-right: 2px; border: 1px solid #bdc3c7; border-bottom: none; border-top-left-radius: 4px; border-top-right-radius: 4px; font-size: 13px; }"
        "QTabBar::tab:selected { background-color: white; font-weight: bold; color: #2c3e50; }"
        "QTabBar::tab:hover { background-color: #d5dbdb; }");

    dashboardPage = new DashboardPage(sys);
    roomPage = new RoomPage(sys);
    customerPage = new CustomerPage(sys);
    bookingPage = new BookingPage(sys);
    statsPage = new StatsPage(sys);

    tabWidget->addTab(dashboardPage, "  仪表盘  ");
    tabWidget->addTab(roomPage, "  房间管理  ");
    tabWidget->addTab(customerPage, "  客户管理  ");
    tabWidget->addTab(bookingPage, "  预订管理  ");
    tabWidget->addTab(statsPage, "  账单统计  ");

    // 切换 Tab 时刷新对应页面
    connect(tabWidget, &QTabWidget::currentChanged, [this](int index) {
        switch (index) {
            case 0: dashboardPage->refresh(); break;
            case 1: roomPage->refresh(); break;
            case 2: customerPage->refresh(); break;
            case 3: bookingPage->refresh(); break;
            case 4: statsPage->refresh(); break;
        }
    });

    setCentralWidget(tabWidget);
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "关于",
        "<h3>酒店客房管理系统 v1.0</h3>"
        "<p>基于 C++ 面向对象程序设计的酒店客房管理系统</p>"
        "<p>功能模块：房间管理、客户管理、预订管理、账单计费、营收统计</p>"
        "<hr>"
        "<p>OOP知识点：继承、多态、运算符重载、异常处理、STL、单例模式、门面模式</p>"
        "<p>UI 框架：Qt</p>");
}

void MainWindow::refreshAll() {
    dashboardPage->refresh();
    roomPage->refresh();
    customerPage->refresh();
    bookingPage->refresh();
    statsPage->refresh();
}
