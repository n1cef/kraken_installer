#include "user.h"
#include "ui_user.h"
#include <QMessageBox>
#include <QFile>
User::User(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::User)
{
    ui->setupUi(this);
    connect(ui->user_btn, &QPushButton::clicked,
            this, &User::onNextClicked);
}



void User::saveUserSettings() {
    if(ui->passwordEdit->text() != ui->confirmPassEdit->text()) {
        QMessageBox::warning(this, "Error", "Passwords do not match!");
        return;
    }

    QJsonObject userSettings;
    userSettings["hostname"] = ui->hostnameEdit->text();
    userSettings["username"] = ui->usernameEdit->text();
    userSettings["password"] = ui->passwordEdit->text(); // In real apps, encrypt this!

    // Merge with existing settings
    QFile file("/home/settings.json");
    if(file.open(QIODevice::ReadWrite)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject root = doc.object();

        root["user"] = userSettings;  // Nest under "user" key

        file.resize(0);
        file.write(QJsonDocument(root).toJson());
        file.close();
    }
}


void User::onNextClicked() {
    if(ui->hostnameEdit->text().isEmpty() ||
        ui->usernameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill all fields!");
        return;
    }

    saveUserSettings();
    emit nextButtonClicked();
}
User::~User()
{
    delete ui;
}
