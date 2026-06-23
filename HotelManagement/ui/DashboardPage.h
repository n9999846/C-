#ifndef DASHBOARD_PAGE_H
#define DASHBOARD_PAGE_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include "SystemIntegrator.h"

class DashboardPage : public QWidget {
    Q_OBJECT

public:
    explicit DashboardPage(SystemIntegrator* sys, QWidget* parent = nullptr);
    void refresh();

private:
    SystemIntegrator* sys;

    QLabel* totalRoomsLabel;
    QLabel* availableRoomsLabel;
    QLabel* totalCustomersLabel;
    QLabel* unpaidBillsLabel;
    QTextEdit* reportText;
};

#endif
