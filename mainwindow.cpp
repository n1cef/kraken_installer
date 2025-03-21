#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./ui_welcome.h"
#include "./ui_keyboard.h"
#include "./ui_location.h"
#include "./ui_user.h"
#include "./ui_install.h"
#include "./ui_finish.h"
#include <QJsonObject>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget* oldPage = ui->stackedWidget->widget(1); // Get page_2
    ui->stackedWidget->removeWidget(oldPage); // Remove it
    delete oldPage;



    Welcome* welcomePage = new Welcome();
    ui->stackedWidget->addWidget(welcomePage);
    connect(welcomePage, &Welcome::nextButtonClicked,
            this, &MainWindow::switchToKeyboardPage);





    Keyboard* keyboardPage = new Keyboard();
    ui->stackedWidget->addWidget(keyboardPage);
    connect(keyboardPage, &Keyboard::nextButtonClicked,  // FIXED: No parentheses!
            this, &MainWindow::switchToLocationPage);




    Location* locationPage= new Location();
    ui->stackedWidget->addWidget(locationPage);
    connect(locationPage, &Location::nextButtonClicked,  // FIXED: No parentheses!
            this, &MainWindow::switchToUserPage);




    User* userPage= new User();
    ui->stackedWidget->addWidget(userPage);
    connect(userPage, &User::nextButtonClicked,  // FIXED: No parentheses!
            this, &MainWindow::switchToPartitionPage);




    Partition* partitionPage= new Partition();
    ui->stackedWidget->addWidget(partitionPage);

    connect(partitionPage, &Partition::nextButtonClicked,  // FIXED: No parentheses!
            this, &MainWindow::switchToInstallPage);




    Install* installPage= new Install();
    ui->stackedWidget->addWidget(installPage);
    connect(installPage, &Install::nextButtonClicked,  // FIXED: No parentheses!
            this, &MainWindow::switchToFinishPage);





    Finish* finishPage= new Finish();
    ui->stackedWidget->addWidget(finishPage);






    //switchToInstallPage



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_bnt1_clicked()
{
     ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::switchToKeyboardPage()
{
    // Switch to Keyboard page (index 2)
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::switchToLocationPage()
{
    ui->stackedWidget->setCurrentIndex(3);


}

void MainWindow::switchToUserPage(){
    ui->stackedWidget->setCurrentIndex(4);


}


void MainWindow::switchToPartitionPage(){
    ui->stackedWidget->setCurrentIndex(5);

}

void MainWindow::switchToInstallPage()
{
    ui->stackedWidget->setCurrentIndex(6);
}



void MainWindow::switchToFinishPage()
{
    ui->stackedWidget->setCurrentIndex(7);
}


