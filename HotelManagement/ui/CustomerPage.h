#ifndef CUSTOMER_PAGE_H
#define CUSTOMER_PAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include "SystemIntegrator.h"

class CustomerPage : public QWidget {
    Q_OBJECT

public:
    explicit CustomerPage(SystemIntegrator* sys, QWidget* parent = nullptr);
    void refresh();

private slots:
    void onAddCustomer();
    void onAddVIP();
    void onUpgradeVIP();
    void onEditCustomer();
    void onDeleteCustomer();

private:
    void setupTable();

    SystemIntegrator* sys;
    QTableWidget* table;
};

#endif
