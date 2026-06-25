#include <QApplication>
#include <QDir>
#include "ui/LoginDialog.h"
#include "ui/MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // 设置工作目录为项目根目录（data/ 所在位置）
    QDir::setCurrent(QCoreApplication::applicationDirPath() + "/..");

    // 设置应用程序全局样式
    app.setStyleSheet("QToolTip { color: #333; background-color: #fff; border: 1px solid #ccc; padding: 4px; }");

    // 登录对话框
    LoginDialog loginDialog;
    if (loginDialog.exec() != QDialog::Accepted) {
        return 0;  // 用户取消登录或关闭窗口
    }

    // 登录成功，显示主窗口
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
