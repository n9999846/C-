#include "CustomerPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>

CustomerPage::CustomerPage(SystemIntegrator* sys, QWidget* parent)
    : QWidget(parent), sys(sys) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    QHBoxLayout* topLayout = new QHBoxLayout();
    QLabel* title = new QLabel("客户管理");
    title->setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50;");
    topLayout->addWidget(title);
    topLayout->addStretch();

    QPushButton* addBtn = new QPushButton("+ 注册客户");
    QPushButton* addVipBtn = new QPushButton("+ 注册VIP");
    QPushButton* upBtn = new QPushButton("升级VIP");
    QPushButton* refBtn = new QPushButton("刷新");

    QString btnStyle = "QPushButton { background-color: #3498db; color: white; border: none; border-radius: 4px; padding: 6px 14px; font-size: 12px; }"
                       "QPushButton:hover { background-color: #2980b9; }";
    QString vipStyle = "QPushButton { background-color: #f39c12; color: white; border: none; border-radius: 4px; padding: 6px 14px; font-size: 12px; }"
                       "QPushButton:hover { background-color: #e67e22; }";

    addBtn->setStyleSheet(btnStyle);
    addVipBtn->setStyleSheet(vipStyle);
    upBtn->setStyleSheet(vipStyle);
    refBtn->setStyleSheet(btnStyle);

    topLayout->addWidget(addBtn);
    topLayout->addWidget(addVipBtn);
    topLayout->addWidget(upBtn);
    topLayout->addWidget(refBtn);
    mainLayout->addLayout(topLayout);

    table = new QTableWidget();
    setupTable();
    mainLayout->addWidget(table);

    connect(addBtn, &QPushButton::clicked, this, &CustomerPage::onAddCustomer);
    connect(addVipBtn, &QPushButton::clicked, this, &CustomerPage::onAddVIP);
    connect(upBtn, &QPushButton::clicked, this, &CustomerPage::onUpgradeVIP);
    connect(refBtn, &QPushButton::clicked, this, &CustomerPage::refresh);

    refresh();
}

void CustomerPage::setupTable() {
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"客户ID", "姓名", "电话", "类型", "累计消费", "折扣率"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setAlternatingRowColors(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setStyleSheet("QTableWidget { background-color: white; border: 1px solid #ddd; }"
                         "QHeaderView::section { background-color: #34495e; color: white; padding: 6px; border: none; font-weight: bold; }"
                         "QTableWidget::item { padding: 5px; }");
}

void CustomerPage::refresh() {
    if (!sys || !sys->isInitialized()) return;

    auto customers = sys->getAllCustomers();
    table->setRowCount(static_cast<int>(customers.size()));

    for (int i = 0; i < static_cast<int>(customers.size()); i++) {
        Customer* c = customers[i];
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(c->getId())));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(c->getName())));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(c->getPhone())));
        table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(c->getCustomerType())));
        table->setItem(i, 4, new QTableWidgetItem(QString::number(c->getTotalSpent())));
        table->setItem(i, 5, new QTableWidgetItem(QString("%1").arg(c->getDiscountRate(), 0, 'f', 2)));
    }
}

void CustomerPage::onAddCustomer() {
    QDialog dialog(this);
    dialog.setWindowTitle("注册客户");
    dialog.setMinimumWidth(350);
    QFormLayout form(&dialog);

    QLineEdit idEdit; idEdit.setPlaceholderText("如 C001");
    QLineEdit nameEdit; nameEdit.setPlaceholderText("姓名");
    QLineEdit phoneEdit; phoneEdit.setPlaceholderText("11位手机号");
    QLineEdit idCardEdit; idCardEdit.setPlaceholderText("18位身份证号");

    form.addRow("客户ID:", &idEdit);
    form.addRow("姓  名:", &nameEdit);
    form.addRow("电  话:", &phoneEdit);
    form.addRow("身份证:", &idCardEdit);

    QPushButton* okBtn = new QPushButton("确定");
    QPushButton* cancelBtn = new QPushButton("取消");
    QHBoxLayout btnLayout;
    btnLayout.addWidget(okBtn);
    btnLayout.addWidget(cancelBtn);
    form.addRow("", &btnLayout);

    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        bool ok = sys->registerCustomer(idEdit.text().trimmed().toStdString(),
                                        nameEdit.text().trimmed().toStdString(),
                                        phoneEdit.text().trimmed().toStdString(),
                                        idCardEdit.text().trimmed().toStdString());
        if (ok) { QMessageBox::information(this, "成功", "客户注册成功！"); refresh(); }
        else { QMessageBox::warning(this, "失败", "注册失败，请检查输入信息"); }
    }
}

void CustomerPage::onAddVIP() {
    QDialog dialog(this);
    dialog.setWindowTitle("注册VIP客户");
    dialog.setMinimumWidth(380);
    QFormLayout form(&dialog);

    QLineEdit idEdit; idEdit.setPlaceholderText("如 C004");
    QLineEdit nameEdit;
    QLineEdit phoneEdit;
    QLineEdit idCardEdit;
    QSpinBox levelSpin; levelSpin.setRange(1, 3); levelSpin.setValue(1);

    form.addRow("客户ID:", &idEdit);
    form.addRow("姓  名:", &nameEdit);
    form.addRow("电  话:", &phoneEdit);
    form.addRow("身份证:", &idCardEdit);
    form.addRow("VIP等级:", &levelSpin);

    QPushButton* okBtn = new QPushButton("确定");
    QPushButton* cancelBtn = new QPushButton("取消");
    QHBoxLayout btnLayout;
    btnLayout.addWidget(okBtn);
    btnLayout.addWidget(cancelBtn);
    form.addRow("", &btnLayout);

    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        string today = Date::today().toString();
        bool ok = sys->registerVIPCustomer(idEdit.text().trimmed().toStdString(),
                                           nameEdit.text().trimmed().toStdString(),
                                           phoneEdit.text().trimmed().toStdString(),
                                           idCardEdit.text().trimmed().toStdString(),
                                           levelSpin.value(), today);
        if (ok) { QMessageBox::information(this, "成功", "VIP客户注册成功！"); refresh(); }
        else { QMessageBox::warning(this, "失败", "注册失败"); }
    }
}

void CustomerPage::onUpgradeVIP() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "提示", "请先选择要升级的客户");
        return;
    }
    string custId = table->item(row, 0)->text().toStdString();
    string custType = table->item(row, 3)->text().toStdString();

    if (custType == "VIP客户") {
        QMessageBox::information(this, "提示", "该客户已经是VIP");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("升级为VIP");
    dialog.setMinimumWidth(300);
    QFormLayout form(&dialog);

    QLabel idLabel(QString::fromStdString(custId));
    QSpinBox levelSpin; levelSpin.setRange(1, 3); levelSpin.setValue(1);

    form.addRow("客户ID:", &idLabel);
    form.addRow("VIP等级:", &levelSpin);

    QPushButton* okBtn = new QPushButton("确定升级");
    QPushButton* cancelBtn = new QPushButton("取消");
    QHBoxLayout btnLayout;
    btnLayout.addWidget(okBtn);
    btnLayout.addWidget(cancelBtn);
    form.addRow("", &btnLayout);

    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        bool ok = sys->upgradeToVIP(custId, levelSpin.value());
        if (ok) { QMessageBox::information(this, "成功", "升级成功！"); refresh(); }
        else { QMessageBox::warning(this, "失败", "升级失败"); }
    }
}
