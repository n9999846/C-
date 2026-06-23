#include "DashboardPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QFont>

DashboardPage::DashboardPage(SystemIntegrator* sys, QWidget* parent)
    : QWidget(parent), sys(sys) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // 标题
    QLabel* title = new QLabel("系统仪表盘");
    QFont titleFont = title->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setStyleSheet("color: #2c3e50;");
    mainLayout->addWidget(title);

    // 统计卡片网格
    QGridLayout* cardGrid = new QGridLayout();
    cardGrid->setSpacing(15);

    auto createCard = [&](const QString& titleText, const QString& color) -> QLabel* {
        QFrame* card = new QFrame();
        card->setMinimumSize(150, 100);
        card->setStyleSheet(QString("QFrame { background-color: %1; border-radius: 10px; padding: 15px; }").arg(color));
        QVBoxLayout* cardLayout = new QVBoxLayout(card);

        QLabel* titleLabel = new QLabel(titleText);
        titleLabel->setStyleSheet("color: white; font-size: 12px;");
        cardLayout->addWidget(titleLabel);

        QLabel* valueLabel = new QLabel("0");
        valueLabel->setStyleSheet("color: white; font-size: 28px; font-weight: bold;");
        valueLabel->setAlignment(Qt::AlignCenter);
        cardLayout->addWidget(valueLabel);

        return valueLabel;
    };

    totalRoomsLabel = createCard("总房间数", "#3498db");
    availableRoomsLabel = createCard("空闲房间", "#2ecc71");
    totalCustomersLabel = createCard("客户总数", "#9b59b6");
    unpaidBillsLabel = createCard("未付账单", "#e74c3c");

    QFrame* card1 = new QFrame(); card1->setMinimumSize(150,100);
    card1->setStyleSheet("QFrame { background-color: #3498db; border-radius: 10px; padding: 15px; }");
    QVBoxLayout* l1 = new QVBoxLayout(card1);
    QLabel* t1 = new QLabel("总房间数"); t1->setStyleSheet("color:white; font-size:12px;"); l1->addWidget(t1);
    totalRoomsLabel->setAlignment(Qt::AlignCenter); l1->addWidget(totalRoomsLabel);
    cardGrid->addWidget(card1, 0, 0);

    QFrame* card2 = new QFrame(); card2->setMinimumSize(150,100);
    card2->setStyleSheet("QFrame { background-color: #2ecc71; border-radius: 10px; padding: 15px; }");
    QVBoxLayout* l2 = new QVBoxLayout(card2);
    QLabel* t2 = new QLabel("空闲房间"); t2->setStyleSheet("color:white; font-size:12px;"); l2->addWidget(t2);
    availableRoomsLabel->setAlignment(Qt::AlignCenter); l2->addWidget(availableRoomsLabel);
    cardGrid->addWidget(card2, 0, 1);

    QFrame* card3 = new QFrame(); card3->setMinimumSize(150,100);
    card3->setStyleSheet("QFrame { background-color: #9b59b6; border-radius: 10px; padding: 15px; }");
    QVBoxLayout* l3 = new QVBoxLayout(card3);
    QLabel* t3 = new QLabel("客户总数"); t3->setStyleSheet("color:white; font-size:12px;"); l3->addWidget(t3);
    totalCustomersLabel->setAlignment(Qt::AlignCenter); l3->addWidget(totalCustomersLabel);
    cardGrid->addWidget(card3, 0, 2);

    QFrame* card4 = new QFrame(); card4->setMinimumSize(150,100);
    card4->setStyleSheet("QFrame { background-color: #e74c3c; border-radius: 10px; padding: 15px; }");
    QVBoxLayout* l4 = new QVBoxLayout(card4);
    QLabel* t4 = new QLabel("未付账单"); t4->setStyleSheet("color:white; font-size:12px;"); l4->addWidget(t4);
    unpaidBillsLabel->setAlignment(Qt::AlignCenter); l4->addWidget(unpaidBillsLabel);
    cardGrid->addWidget(card4, 0, 3);

    mainLayout->addLayout(cardGrid);

    // 系统状态报告
    QLabel* reportTitle = new QLabel("系统状态报告");
    reportTitle->setStyleSheet("font-size: 14px; font-weight: bold; color: #2c3e50; margin-top: 10px;");
    mainLayout->addWidget(reportTitle);

    reportText = new QTextEdit();
    reportText->setReadOnly(true);
    reportText->setMinimumHeight(250);
    reportText->setStyleSheet("QTextEdit { background-color: white; border: 1px solid #ddd; border-radius: 6px; padding: 10px; font-family: monospace; font-size: 13px; }");
    mainLayout->addWidget(reportText);

    // 刷新按钮
    QPushButton* refreshBtn = new QPushButton("刷新");
    refreshBtn->setStyleSheet("QPushButton { background-color: #3498db; color: white; border: none; border-radius: 5px; padding: 8px 20px; font-size: 13px; }"
                              "QPushButton:hover { background-color: #2980b9; }");
    connect(refreshBtn, &QPushButton::clicked, this, &DashboardPage::refresh);
    mainLayout->addWidget(refreshBtn, 0, Qt::AlignRight);

    refresh();
}

void DashboardPage::refresh() {
    if (!sys || !sys->isInitialized()) return;

    totalRoomsLabel->setText(QString::number(sys->getAllRooms().size()));
    availableRoomsLabel->setText(QString::number(sys->getAvailableRooms().size()));
    totalCustomersLabel->setText(QString::number(sys->getAllCustomers().size()));
    unpaidBillsLabel->setText(QString::number(sys->getUnpaidBills().size()));

    reportText->setText(QString::fromStdString(sys->getSystemStatusReport()));
}
