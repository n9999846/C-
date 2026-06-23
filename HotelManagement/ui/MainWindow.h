#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QStatusBar>
#include <QMenuBar>
#include <QLabel>
#include "SystemIntegrator.h"

class DashboardPage;
class RoomPage;
class CustomerPage;
class BookingPage;
class StatsPage;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onAbout();
    void refreshAll();

private:
    void setupMenuBar();
    void setupStatusBar();
    void setupTabs();

    SystemIntegrator* sys;

    QTabWidget* tabWidget;
    QLabel* statusLabel;

    DashboardPage* dashboardPage;
    RoomPage* roomPage;
    CustomerPage* customerPage;
    BookingPage* bookingPage;
    StatsPage* statsPage;
};

#endif
