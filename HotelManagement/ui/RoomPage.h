#ifndef ROOM_PAGE_H
#define ROOM_PAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include "SystemIntegrator.h"

class RoomPage : public QWidget {
    Q_OBJECT

public:
    explicit RoomPage(SystemIntegrator* sys, QWidget* parent = nullptr);
    void refresh();

private slots:
    void onAddStandard();
    void onAddDeluxe();
    void onAddSuite();
    void onDeleteRoom();

private:
    void setupTable();

    SystemIntegrator* sys;
    QTableWidget* table;
};

#endif
