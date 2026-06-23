#include "LoginDialog.h"
#include <QMessageBox>
#include <QFont>
#include <QFrame>

LoginDialog::LoginDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("酒店客房管理系统 — 登录");
    setFixedSize(400, 300);
    setStyleSheet("QDialog { background-color: #f5f5f5; }");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(50, 30, 50, 30);

    // 标题
    QLabel* titleLabel = new QLabel("酒店客房管理系统");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #2c3e50;");
    mainLayout->addWidget(titleLabel);

    // 表单区域
    QFrame* formFrame = new QFrame();
    formFrame->setStyleSheet("QFrame { background-color: white; border-radius: 8px; padding: 15px; }");
    QVBoxLayout* formLayout = new QVBoxLayout(formFrame);
    formLayout->setSpacing(12);

    QLabel* userLabel = new QLabel("用户名:");
    userLabel->setStyleSheet("font-size: 13px; color: #555;");
    usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText("请输入用户名");
    usernameEdit->setMinimumHeight(35);
    usernameEdit->setStyleSheet("QLineEdit { border: 1px solid #ddd; border-radius: 4px; padding: 5px 10px; font-size: 13px; }"
                                "QLineEdit:focus { border-color: #3498db; }");

    QLabel* passLabel = new QLabel("密  码:");
    passLabel->setStyleSheet("font-size: 13px; color: #555;");
    passwordEdit = new QLineEdit();
    passwordEdit->setPlaceholderText("请输入密码");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setMinimumHeight(35);
    passwordEdit->setStyleSheet("QLineEdit { border: 1px solid #ddd; border-radius: 4px; padding: 5px 10px; font-size: 13px; }"
                                "QLineEdit:focus { border-color: #3498db; }");

    formLayout->addWidget(userLabel);
    formLayout->addWidget(usernameEdit);
    formLayout->addWidget(passLabel);
    formLayout->addWidget(passwordEdit);
    mainLayout->addWidget(formFrame);

    // 登录按钮
    loginButton = new QPushButton("登  录");
    loginButton->setMinimumHeight(40);
    loginButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; border: none; border-radius: 6px; font-size: 15px; font-weight: bold; }"
                               "QPushButton:hover { background-color: #2980b9; }"
                               "QPushButton:pressed { background-color: #21618c; }");
    mainLayout->addWidget(loginButton);

    mainLayout->addStretch();

    // 信号连接
    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(passwordEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);
    connect(usernameEdit, &QLineEdit::returnPressed, [this]() { passwordEdit->setFocus(); });
}

void LoginDialog::onLoginClicked() {
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (username == "admin" && password == "admin") {
        accept();
    } else {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误！\n\n提示：默认账号 admin / admin");
        passwordEdit->clear();
        passwordEdit->setFocus();
    }
}
