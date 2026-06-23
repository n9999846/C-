#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent = nullptr);

private slots:
    void onLoginClicked();

private:
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;
    QPushButton* loginButton;
};

#endif
