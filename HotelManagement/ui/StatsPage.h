#ifndef STATS_PAGE_H
#define STATS_PAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QTextEdit>
#include "SystemIntegrator.h"

class StatsPage : public QWidget {
    Q_OBJECT

public:
    explicit StatsPage(SystemIntegrator* sys, QWidget* parent = nullptr);
    void refresh();

private slots:
    void onPayBill();

private:
    void setupTable();

    SystemIntegrator* sys;
    QTableWidget* table;
    QTextEdit* reportText;
    QTableWidget* topCustomerTable;
};

#endif
