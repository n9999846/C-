#include "BookingPage.h"
#include "Booking.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QLabel>

BookingPage::BookingPage(SystemIntegrator* sys, QWidget* parent)
    : QWidget(parent), sys(sys) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    QHBoxLayout* topLayout = new QHBoxLayout();
    QLabel* title = new QLabel("预订管理");
    title->setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50;");
    topLayout->addWidget(title);
    topLayout->addStretch();

    QPushButton* newBtn = new QPushButton("+ 新建预订");
    QPushButton* ciBtn = new QPushButton("办理入住");
    QPushButton* coBtn = new QPushButton("办理退房");
    QPushButton* cancelBtn = new QPushButton("取消预订");
    QPushButton* extBtn = new QPushButton("续住");
    QPushButton* refBtn = new QPushButton("刷新");

    QString btnStyle = "QPushButton { background-color: #3498db; color: white; border: none; border-radius: 4px; padding: 6px 14px; font-size: 12px; }"
                       "QPushButton:hover { background-color: #2980b9; }";
    QString greenStyle = "QPushButton { background-color: #27ae60; color: white; border: none; border-radius: 4px; padding: 6px 14px; font-size: 12px; }"
                         "QPushButton:hover { background-color: #229954; }";
    QString orangeStyle = "QPushButton { background-color: #f39c12; color: white; border: none; border-radius: 4px; padding: 6px 14px; font-size: 12px; }"
                          "QPushButton:hover { background-color: #e67e22; }";
    QString redStyle = "QPushButton { background-color: #e74c3c; color: white; border: none; border-radius: 4px; padding: 6px 14px; font-size: 12px; }"
                       "QPushButton:hover { background-color: #c0392b; }";

    newBtn->setStyleSheet(btnStyle);
    ciBtn->setStyleSheet(greenStyle);
    coBtn->setStyleSheet(orangeStyle);
    cancelBtn->setStyleSheet(redStyle);
    extBtn->setStyleSheet(btnStyle);
    refBtn->setStyleSheet(btnStyle);

    topLayout->addWidget(newBtn);
    topLayout->addWidget(ciBtn);
    topLayout->addWidget(coBtn);
    topLayout->addWidget(cancelBtn);
    topLayout->addWidget(extBtn);
    topLayout->addWidget(refBtn);
    mainLayout->addLayout(topLayout);

    table = new QTableWidget();
    setupTable();
    mainLayout->addWidget(table);

    connect(newBtn, &QPushButton::clicked, this, &BookingPage::onNewBooking);
    connect(ciBtn, &QPushButton::clicked, this, &BookingPage::onCheckIn);
    connect(coBtn, &QPushButton::clicked, this, &BookingPage::onCheckOut);
    connect(cancelBtn, &QPushButton::clicked, this, &BookingPage::onCancel);
    connect(extBtn, &QPushButton::clicked, this, &BookingPage::onExtend);
    connect(refBtn, &QPushButton::clicked, this, &BookingPage::refresh);

    refresh();
}

void BookingPage::setupTable() {
    table->setColumnCount(7);
    table->setHorizontalHeaderLabels({"预订ID", "客户ID", "房间号", "入住日期", "退房日期", "状态", "晚数"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setAlternatingRowColors(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setStyleSheet("QTableWidget { background-color: white; border: 1px solid #ddd; }"
                         "QHeaderView::section { background-color: #34495e; color: white; padding: 6px; border: none; font-weight: bold; }"
                         "QTableWidget::item { padding: 5px; }");
}

void BookingPage::refresh() {
    if (!sys || !sys->isInitialized()) return;

    auto bookings = sys->getAllBookings();
    table->setRowCount(static_cast<int>(bookings.size()));

    for (int i = 0; i < static_cast<int>(bookings.size()); i++) {
        const Booking& b = bookings[i];
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(b.getBookingId())));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(b.getCustomerId())));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(b.getRoomNumber())));
        table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(b.getCheckInDate().toString())));
        table->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(b.getCheckOutDate().toString())));

        QTableWidgetItem* statusItem = new QTableWidgetItem(QString::fromStdString(b.getStatusString()));
        if (b.getStatus() == Booking::Status::PENDING) statusItem->setForeground(QColor("#f39c12"));
        else if (b.getStatus() == Booking::Status::CHECKED_IN) statusItem->setForeground(QColor("#27ae60"));
        else if (b.getStatus() == Booking::Status::CHECKED_OUT) statusItem->setForeground(QColor("#3498db"));
        else if (b.getStatus() == Booking::Status::CANCELLED) statusItem->setForeground(QColor("#95a5a6"));
        table->setItem(i, 5, statusItem);

        table->setItem(i, 6, new QTableWidgetItem(QString::number(b.calculateNights())));
    }
}

void BookingPage::onNewBooking() {
    QDialog dialog(this);
    dialog.setWindowTitle("新建预订");
    dialog.setMinimumWidth(400);
    QFormLayout form(&dialog);

    QLineEdit custEdit; custEdit.setPlaceholderText("客户ID，如 C001");
    QLineEdit roomEdit; roomEdit.setPlaceholderText("房间号，如 101");
    QDateEdit checkInEdit; checkInEdit.setDate(QDate(2026, 6, 20));
    QDateEdit checkOutEdit; checkOutEdit.setDate(QDate(2026, 6, 25));

    form.addRow("客户ID:", &custEdit);
    form.addRow("房间号:", &roomEdit);
    form.addRow("入住日期:", &checkInEdit);
    form.addRow("退房日期:", &checkOutEdit);

    QPushButton* okBtn = new QPushButton("确定预订");
    QPushButton* cancelBtn = new QPushButton("取消");
    QHBoxLayout btnLayout;
    btnLayout.addWidget(okBtn);
    btnLayout.addWidget(cancelBtn);
    form.addRow("", &btnLayout);

    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QDate ci = checkInEdit.date();
        QDate co = checkOutEdit.date();
        Date checkIn(ci.year(), ci.month(), ci.day());
        Date checkOut(co.year(), co.month(), co.day());

        string result = sys->createBooking(custEdit.text().trimmed().toStdString(),
                                           roomEdit.text().trimmed().toStdString(),
                                           checkIn, checkOut);
        if (!result.empty()) {
            QMessageBox::information(this, "成功", QString("预订创建成功！\n预订ID: %1").arg(QString::fromStdString(result)));
            refresh();
        } else {
            QMessageBox::warning(this, "失败", "创建预订失败，请检查客户ID、房间号和日期");
        }
    }
}

void BookingPage::onCheckIn() {
    int row = table->currentRow();
    if (row < 0) { QMessageBox::information(this, "提示", "请先选择预订"); return; }
    string bookingId = table->item(row, 0)->text().toStdString();
    string status = table->item(row, 5)->text().toStdString();
    if (status != "待入住") { QMessageBox::information(this, "提示", "只有待入住状态的预订才能办理入住"); return; }

    bool ok = sys->checkIn(bookingId, Date::today());
    if (ok) { QMessageBox::information(this, "成功", "入住办理成功！"); refresh(); }
    else { QMessageBox::warning(this, "失败", "入住办理失败"); }
}

void BookingPage::onCheckOut() {
    int row = table->currentRow();
    if (row < 0) { QMessageBox::information(this, "提示", "请先选择预订"); return; }
    string bookingId = table->item(row, 0)->text().toStdString();
    string status = table->item(row, 5)->text().toStdString();
    if (status != "已入住") { QMessageBox::information(this, "提示", "只有已入住状态的预订才能办理退房"); return; }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认退房", QString("确定要办理预订 %1 的退房吗？\n系统将自动生成账单。").arg(QString::fromStdString(bookingId)),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Bill bill = sys->checkOut(bookingId, Date::today());
        if (!bill.getBillId().empty()) {
            QMessageBox::information(this, "退房成功",
                QString("账单ID: %1\n房间: %2\n晚数: %3\n总金额: ¥%4\n\n%5")
                    .arg(QString::fromStdString(bill.getBillId()))
                    .arg(QString::fromStdString(bill.getRoomNumber()))
                    .arg(bill.getNights())
                    .arg(bill.getTotalAmount(), 0, 'f', 2)
                    .arg(bill.isPaid() ? "已支付" : "未支付"));
            refresh();
        } else {
            QMessageBox::warning(this, "失败", "退房失败");
        }
    }
}

void BookingPage::onCancel() {
    int row = table->currentRow();
    if (row < 0) { QMessageBox::information(this, "提示", "请先选择预订"); return; }
    string bookingId = table->item(row, 0)->text().toStdString();
    string status = table->item(row, 5)->text().toStdString();
    if (status != "待入住") { QMessageBox::information(this, "提示", "只有待入住状态的预订才能取消"); return; }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认取消", QString("确定要取消预订 %1 吗？").arg(QString::fromStdString(bookingId)),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        bool ok = sys->cancelBooking(bookingId);
        if (ok) { QMessageBox::information(this, "成功", "预订已取消"); refresh(); }
        else { QMessageBox::warning(this, "失败", "取消失败"); }
    }
}

void BookingPage::onExtend() {
    int row = table->currentRow();
    if (row < 0) { QMessageBox::information(this, "提示", "请先选择预订"); return; }
    string bookingId = table->item(row, 0)->text().toStdString();
    string status = table->item(row, 5)->text().toStdString();
    if (status != "已入住") { QMessageBox::information(this, "提示", "只有已入住状态的预订才能续住"); return; }

    QDialog dialog(this);
    dialog.setWindowTitle("续住");
    dialog.setMinimumWidth(300);
    QFormLayout form(&dialog);

    QDateEdit newCheckOutEdit;
    newCheckOutEdit.setDate(QDate::currentDate().addDays(3));

    form.addRow("新退房日期:", &newCheckOutEdit);

    QPushButton* okBtn = new QPushButton("确定");
    QPushButton* cancelBtn = new QPushButton("取消");
    QHBoxLayout btnLayout;
    btnLayout.addWidget(okBtn);
    btnLayout.addWidget(cancelBtn);
    form.addRow("", &btnLayout);

    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QDate d = newCheckOutEdit.date();
        Date newCheckOut(d.year(), d.month(), d.day());
        bool ok = sys->extendStay(bookingId, newCheckOut);
        if (ok) { QMessageBox::information(this, "成功", "续住成功！"); refresh(); }
        else { QMessageBox::warning(this, "失败", "续住失败"); }
    }
}
