#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "welcome.h"
#include "keyboard.h"
#include "location.h"
#include "user.h"
#include "partition.h"
#include "user.h"
#include "install.h"
#include "finish.h"
#include <QJsonObject>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_bnt1_clicked();
    void switchToKeyboardPage();
    void switchToLocationPage();
    void switchToUserPage();
    void switchToPartitionPage();
    void switchToInstallPage();
    void switchToFinishPage();

private:
    Ui::MainWindow *ui;
     //Location *location;

};
#endif // MAINWINDOW_H
