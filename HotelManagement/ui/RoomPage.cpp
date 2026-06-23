#include "RoomPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QLabel>

RoomPage::RoomPage(SystemIntegrator* sys, QWidget* parent)
    : QWidget(parent), sys(sys) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // 标题 + 按钮行
    QHBoxLayout* topLayout = new QHBoxLayout();
    QLabel* title = new QLabel("房间管理");
    title->setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50;");
    topLayout->addWidget(title);
    topLayout->addStretch();

    QPushButton* addStdBtn = new QPushButton("+ 标准间");
    QPushButton* addDlxBtn = new QPushButton("+ 豪华间");
    QPushButton* addSteBtn = new QPushButton("+ 套房");
    QPushButton* delBtn = new QPushButton("删除房间");
    QPushButton* refBtn = new QPushButton("刷新");

    QString btnStyle = "QPushButton { background-color: #3498db; color: white; border: none; border-radius: 4px; padding: 6px 14px; font-size: 12px; }"
                       "QPushButton:hover { background-color: #2980b9; }";
    QString delStyle = "QPushButton { background-color: #e74c3c; color: white; border: none; border-radius: 4px; padding: 6px 14px; font-size: 12px; }"
                       "QPushButton:hover { background-color: #c0392b; }";

    addStdBtn->setStyleSheet(btnStyle);
    addDlxBtn->setStyleSheet(btnStyle);
    addSteBtn->setStyleSheet(btnStyle);
    refBtn->setStyleSheet(btnStyle);
    delBtn->setStyleSheet(delStyle);

    topLayout->addWidget(addStdBtn);
    topLayout->addWidget(addDlxBtn);
    topLayout->addWidget(addSteBtn);
    topLayout->addWidget(delBtn);
    topLayout->addWidget(refBtn);
    mainLayout->addLayout(topLayout);

    // 表格
    table = new QTableWidget();
    setupTable();
    mainLayout->addWidget(table);

    connect(addStdBtn, &QPushButton::clicked, this, &RoomPage::onAddStandard);
    connect(addDlxBtn, &QPushButton::clicked, this, &RoomPage::onAddDeluxe);
    connect(addSteBtn, &QPushButton::clicked, this, &RoomPage::onAddSuite);
    connect(delBtn, &QPushButton::clicked, this, &RoomPage::onDeleteRoom);
    connect(refBtn, &QPushButton::clicked, this, &RoomPage::refresh);

    refresh();
}

void RoomPage::setupTable() {
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"房间号", "楼层", "价格/晚", "状态", "类型", "描述"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setAlternatingRowColors(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setStyleSheet("QTableWidget { background-color: white; border: 1px solid #ddd; }"
                         "QHeaderView::section { background-color: #34495e; color: white; padding: 6px; border: none; font-weight: bold; }"
                         "QTableWidget::item { padding: 5px; }");
}

void RoomPage::refresh() {
    if (!sys || !sys->isInitialized()) return;

    auto rooms = sys->getAllRooms();
    table->setRowCount(static_cast<int>(rooms.size()));

    for (int i = 0; i < static_cast<int>(rooms.size()); i++) {
        Room* r = rooms[i];
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(r->getRoomNumber())));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(r->getFloor())));
        table->setItem(i, 2, new QTableWidgetItem(QString("¥%1").arg(r->getActualPrice(), 0, 'f', 2)));
        table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(r->getStatusString())));
        table->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(r->getRoomType())));
        table->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(r->getDescription())));
    }
}

void RoomPage::onAddStandard() {
    QDialog dialog(this);
    dialog.setWindowTitle("添加标准间");
    dialog.setMinimumWidth(350);
    QFormLayout form(&dialog);

    QLineEdit numEdit; numEdit.setPlaceholderText("如 101");
    QSpinBox floorSpin; floorSpin.setRange(1, 99);
    QDoubleSpinBox priceSpin; priceSpin.setRange(0, 99999); priceSpin.setDecimals(2); priceSpin.setValue(299);
    QSpinBox bedSpin; bedSpin.setRange(1, 4); bedSpin.setValue(2);
    QCheckBox winCheck("有窗"); winCheck.setChecked(true);
    QCheckBox wifiCheck("含WiFi"); wifiCheck.setChecked(true);

    form.addRow("房间号:", &numEdit);
    form.addRow("楼  层:", &floorSpin);
    form.addRow("价  格:", &priceSpin);
    form.addRow("床位数:", &bedSpin);
    form.addRow("", &winCheck);
    form.addRow("", &wifiCheck);

    QPushButton* okBtn = new QPushButton("确定");
    QPushButton* cancelBtn = new QPushButton("取消");
    QHBoxLayout btnLayout;
    btnLayout.addWidget(okBtn);
    btnLayout.addWidget(cancelBtn);
    form.addRow("", &btnLayout);

    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        string num = numEdit.text().trimmed().toStdString();
        if (num.empty()) {
            QMessageBox::warning(this, "错误", "房间号不能为空");
            return;
        }
        bool ok = sys->addStandardRoom(num, floorSpin.value(), priceSpin.value(),
                                       bedSpin.value(), winCheck.isChecked(), wifiCheck.isChecked());
        if (ok) {
            QMessageBox::information(this, "成功", "标准间添加成功！");
            refresh();
        } else {
            QMessageBox::warning(this, "失败", "添加失败，房间号可能已存在或格式无效");
        }
    }
}

void RoomPage::onAddDeluxe() {
    QDialog dialog(this);
    dialog.setWindowTitle("添加豪华间");
    dialog.setMinimumWidth(350);
    QFormLayout form(&dialog);

    QLineEdit numEdit; numEdit.setPlaceholderText("如 201");
    QSpinBox floorSpin; floorSpin.setRange(1, 99); floorSpin.setValue(2);
    QDoubleSpinBox priceSpin; priceSpin.setRange(0, 99999); priceSpin.setDecimals(2); priceSpin.setValue(499);
    QCheckBox balCheck("有阳台"); balCheck.setChecked(true);
    QCheckBox bathCheck("有浴缸"); bathCheck.setChecked(true);
    QCheckBox bfCheck("含早餐"); bfCheck.setChecked(true);

    form.addRow("房间号:", &numEdit);
    form.addRow("楼  层:", &floorSpin);
    form.addRow("价  格:", &priceSpin);
    form.addRow("", &balCheck);
    form.addRow("", &bathCheck);
    form.addRow("", &bfCheck);

    QPushButton* okBtn = new QPushButton("确定");
    QPushButton* cancelBtn = new QPushButton("取消");
    QHBoxLayout btnLayout;
    btnLayout.addWidget(okBtn);
    btnLayout.addWidget(cancelBtn);
    form.addRow("", &btnLayout);

    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        string num = numEdit.text().trimmed().toStdString();
        if (num.empty()) { QMessageBox::warning(this, "错误", "房间号不能为空"); return; }
        bool ok = sys->addDeluxeRoom(num, floorSpin.value(), priceSpin.value(),
                                     balCheck.isChecked(), bathCheck.isChecked(), bfCheck.isChecked());
        if (ok) { QMessageBox::information(this, "成功", "豪华间添加成功！"); refresh(); }
        else { QMessageBox::warning(this, "失败", "添加失败"); }
    }
}

void RoomPage::onAddSuite() {
    QDialog dialog(this);
    dialog.setWindowTitle("添加套房");
    dialog.setMinimumWidth(400);
    QFormLayout form(&dialog);

    QLineEdit numEdit; numEdit.setPlaceholderText("如 301");
    QSpinBox floorSpin; floorSpin.setRange(1, 99); floorSpin.setValue(3);
    QDoubleSpinBox priceSpin; priceSpin.setRange(0, 99999); priceSpin.setDecimals(2); priceSpin.setValue(899);
    QCheckBox balCheck("有阳台"); balCheck.setChecked(true);
    QCheckBox bathCheck("有浴缸"); bathCheck.setChecked(true);
    QCheckBox bfCheck("含早餐"); bfCheck.setChecked(true);
    QDoubleSpinBox areaSpin; areaSpin.setRange(0, 999); areaSpin.setDecimals(1); areaSpin.setValue(45);
    QCheckBox meetCheck("带会议室"); meetCheck.setChecked(true);
    QCheckBox kitCheck("带厨房"); kitCheck.setChecked(true);

    form.addRow("房间号:", &numEdit);
    form.addRow("楼  层:", &floorSpin);
    form.addRow("价  格:", &priceSpin);
    form.addRow("", &balCheck);
    form.addRow("", &bathCheck);
    form.addRow("", &bfCheck);
    form.addRow("客厅面积:", &areaSpin);
    form.addRow("", &meetCheck);
    form.addRow("", &kitCheck);

    QPushButton* okBtn = new QPushButton("确定");
    QPushButton* cancelBtn = new QPushButton("取消");
    QHBoxLayout btnLayout;
    btnLayout.addWidget(okBtn);
    btnLayout.addWidget(cancelBtn);
    form.addRow("", &btnLayout);

    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        string num = numEdit.text().trimmed().toStdString();
        if (num.empty()) { QMessageBox::warning(this, "错误", "房间号不能为空"); return; }
        bool ok = sys->addSuiteRoom(num, floorSpin.value(), priceSpin.value(),
                                    balCheck.isChecked(), bathCheck.isChecked(), bfCheck.isChecked(),
                                    areaSpin.value(), meetCheck.isChecked(), kitCheck.isChecked());
        if (ok) { QMessageBox::information(this, "成功", "套房添加成功！"); refresh(); }
        else { QMessageBox::warning(this, "失败", "添加失败"); }
    }
}

void RoomPage::onDeleteRoom() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "提示", "请先选择要删除的房间");
        return;
    }
    string roomNum = table->item(row, 0)->text().toStdString();
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认删除", QString("确定要删除房间 %1 吗？").arg(QString::fromStdString(roomNum)),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        bool ok = sys->deleteRoom(roomNum);
        if (ok) { QMessageBox::information(this, "成功", "房间已删除"); refresh(); }
        else { QMessageBox::warning(this, "失败", "删除失败"); }
    }
}
