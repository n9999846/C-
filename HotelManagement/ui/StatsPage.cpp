#include "StatsPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QHeaderView>
#include <QMessageBox>
#include <QLabel>

StatsPage::StatsPage(SystemIntegrator* sys, QWidget* parent)
    : QWidget(parent), sys(sys) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // 标题 + 按钮
    QHBoxLayout* topLayout = new QHBoxLayout();
    QLabel* title = new QLabel("账单与统计");
    title->setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50;");
    topLayout->addWidget(title);
    topLayout->addStretch();

    QPushButton* payBtn = new QPushButton("支付选中账单");
    QPushButton* refBtn = new QPushButton("刷新");

    QString payStyle = "QPushButton { background-color: #27ae60; color: white; border: none; border-radius: 4px; padding: 6px 14px; font-size: 12px; }"
                       "QPushButton:hover { background-color: #229954; }";
    QString refStyle = "QPushButton { background-color: #3498db; color: white; border: none; border-radius: 4px; padding: 6px 14px; font-size: 12px; }"
                       "QPushButton:hover { background-color: #2980b9; }";

    payBtn->setStyleSheet(payStyle);
    refBtn->setStyleSheet(refStyle);

    topLayout->addWidget(payBtn);
    topLayout->addWidget(refBtn);
    mainLayout->addLayout(topLayout);

    // 使用 Splitter 分割上下区域
    QSplitter* splitter = new QSplitter(Qt::Vertical);

    // 上半部分：账单表格
    QWidget* billWidget = new QWidget();
    QVBoxLayout* billLayout = new QVBoxLayout(billWidget);
    billLayout->setContentsMargins(0, 0, 0, 0);
    QLabel* billTitle = new QLabel("账单列表");
    billTitle->setStyleSheet("font-size: 13px; font-weight: bold; color: #555; margin-bottom: 5px;");
    billLayout->addWidget(billTitle);

    table = new QTableWidget();
    table->setColumnCount(7);
    table->setHorizontalHeaderLabels({"账单ID", "预订ID", "客户ID", "房间号", "晚数", "总金额", "状态"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setAlternatingRowColors(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setStyleSheet("QTableWidget { background-color: white; border: 1px solid #ddd; }"
                         "QHeaderView::section { background-color: #34495e; color: white; padding: 6px; border: none; font-weight: bold; }"
                         "QTableWidget::item { padding: 5px; }");
    billLayout->addWidget(table);
    splitter->addWidget(billWidget);

    // 下半部分：统计报表 + Top客户
    QWidget* statsWidget = new QWidget();
    QHBoxLayout* statsLayout = new QHBoxLayout(statsWidget);
    statsLayout->setContentsMargins(0, 0, 0, 0);

    // 左侧：系统报表
    QWidget* reportWidget = new QWidget();
    QVBoxLayout* reportLayout = new QVBoxLayout(reportWidget);
    reportLayout->setContentsMargins(0, 0, 5, 0);
    QLabel* reportTitleLabel = new QLabel("统计报表");
    reportTitleLabel->setStyleSheet("font-size: 13px; font-weight: bold; color: #555; margin-bottom: 5px;");
    reportLayout->addWidget(reportTitleLabel);

    reportText = new QTextEdit();
    reportText->setReadOnly(true);
    reportText->setStyleSheet("QTextEdit { background-color: #f8f9fa; border: 1px solid #ddd; border-radius: 6px; padding: 10px; font-family: monospace; font-size: 12px; }");
    reportLayout->addWidget(reportText);
    statsLayout->addWidget(reportWidget, 1);

    // 右侧：Top客户
    QWidget* topWidget = new QWidget();
    QVBoxLayout* topLayout2 = new QVBoxLayout(topWidget);
    topLayout2->setContentsMargins(5, 0, 0, 0);
    QLabel* topTitleLabel = new QLabel("消费排行 Top 5");
    topTitleLabel->setStyleSheet("font-size: 13px; font-weight: bold; color: #555; margin-bottom: 5px;");
    topLayout2->addWidget(topTitleLabel);

    topCustomerTable = new QTableWidget();
    topCustomerTable->setColumnCount(2);
    topCustomerTable->setHorizontalHeaderLabels({"客户ID", "消费总额"});
    topCustomerTable->horizontalHeader()->setStretchLastSection(true);
    topCustomerTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    topCustomerTable->setAlternatingRowColors(true);
    topCustomerTable->setStyleSheet("QTableWidget { background-color: white; border: 1px solid #ddd; }"
                                    "QHeaderView::section { background-color: #8e44ad; color: white; padding: 6px; border: none; font-weight: bold; }");
    topLayout2->addWidget(topCustomerTable);
    statsLayout->addWidget(topWidget, 1);

    splitter->addWidget(statsWidget);
    mainLayout->addWidget(splitter);

    connect(payBtn, &QPushButton::clicked, this, &StatsPage::onPayBill);
    connect(refBtn, &QPushButton::clicked, this, &StatsPage::refresh);

    refresh();
}

void StatsPage::refresh() {
    if (!sys || !sys->isInitialized()) return;

    // 刷新账单表格
    auto allBills = sys->getAllBills();
    table->setRowCount(static_cast<int>(allBills.size()));
    for (int i = 0; i < static_cast<int>(allBills.size()); i++) {
        const Bill& b = allBills[i];
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(b.getBillId())));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(b.getBookingId())));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(b.getCustomerId())));
        table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(b.getRoomNumber())));
        table->setItem(i, 4, new QTableWidgetItem(QString::number(b.getNights())));
        table->setItem(i, 5, new QTableWidgetItem(QString("¥%1").arg(b.getTotalAmount(), 0, 'f', 2)));
        QTableWidgetItem* statusItem = new QTableWidgetItem(b.isPaid() ? "已支付" : "未支付");
        statusItem->setForeground(b.isPaid() ? QColor("#27ae60") : QColor("#e74c3c"));
        table->setItem(i, 6, statusItem);
    }

    // 刷新统计报表
    reportText->setText(QString::fromStdString(sys->getSystemStatusReport()));

    // 刷新 Top 客户
    auto topCustomers = sys->getTopCustomers(5);
    topCustomerTable->setRowCount(static_cast<int>(topCustomers.size()));
    for (int i = 0; i < static_cast<int>(topCustomers.size()); i++) {
        topCustomerTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(topCustomers[i].first)));
        topCustomerTable->setItem(i, 1, new QTableWidgetItem(QString("¥%1").arg(topCustomers[i].second, 0, 'f', 2)));
    }
}

void StatsPage::onPayBill() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "提示", "请先选择要支付的账单");
        return;
    }
    string billId = table->item(row, 0)->text().toStdString();
    string status = table->item(row, 6)->text().toStdString();

    if (status == "已支付") {
        QMessageBox::information(this, "提示", "该账单已支付");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认支付", QString("确定要支付账单 %1 吗？").arg(QString::fromStdString(billId)),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        bool ok = sys->payBill(billId);
        if (ok) { QMessageBox::information(this, "成功", "支付成功！"); refresh(); }
        else { QMessageBox::warning(this, "失败", "支付失败"); }
    }
}
