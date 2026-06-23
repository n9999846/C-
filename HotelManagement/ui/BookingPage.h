#ifndef BOOKING_PAGE_H
#define BOOKING_PAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include "SystemIntegrator.h"

class BookingPage : public QWidget {
    Q_OBJECT

public:
    explicit BookingPage(SystemIntegrator* sys, QWidget* parent = nullptr);
    void refresh();

private slots:
    void onNewBooking();
    void onCheckIn();
    void onCheckOut();
    void onCancel();
    void onExtend();

private:
    void setupTable();

    SystemIntegrator* sys;
    QTableWidget* table;
};

#endif
