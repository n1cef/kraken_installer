#include "finish.h"
#include "ui_finish.h"
#include <QMessageBox>
#include <QProcess>
Finish::Finish(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Finish)
{
    ui->setupUi(this);
    connect(ui->reboot_btn, &QPushButton::clicked, this, &Finish::onRebootClicked);
}

Finish::~Finish()
{
    delete ui;
}
//void Finish::onRebootClicked()
//{
    // Confirmation dialog
  //  QMessageBox::StandardButton reply;
    //reply = QMessageBox::question(
      //  this,
        //"Confirm Reboot",
        //" On your feet soldier , We are leaving , reboot now ? ",
        //QMessageBox::Yes | QMessageBox::No
        //);

    //if (reply == QMessageBox::Yes) {
        // Execute reboot command (non-systemd)
      //  QProcess::startDetached("reboot");


    //}
//}



void Finish::onRebootClicked()
{
    // Create a custom QMessageBox (non-static)
    QMessageBox msgBox;
    msgBox.setWindowTitle("Confirm Reboot");

    // Set the message with HTML formatting for styling
    msgBox.setText("<span style='font-size: 14pt; font-weight: bold;'>"
                   "On your feet soldier!<br/>We are leaving.<br/>Reboot now?"
                   "</span>");

    // Add buttons
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    // Optional: Style the entire QMessageBox (buttons included)
    msgBox.setStyleSheet(
        "QMessageBox {  }"
        "QPushButton { padding: 5px; min-width: 80px; }"
        );

    // Execute the dialog
    int reply = msgBox.exec();

    if (reply == QMessageBox::Yes) {
        QProcess::startDetached("reboot");
    }
}
