#include "user.h"
#include "ui_user.h"
#include <QMessageBox>
#include <QFile>
#include <QRegularExpression>
User::User(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::User)
{
    ui->setupUi(this);
    connect(ui->user_btn, &QPushButton::clicked,
            this, &User::onNextClicked);
}



bool User::saveUserSettings() {
    QFile file("/home/settings.json");
    if(file.open(QIODevice::ReadWrite)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject root = doc.object();

        QJsonObject userSettings;
        userSettings["hostname"] = ui->hostnameEdit->text();
        userSettings["username"] = ui->usernameEdit->text();
        userSettings["password"] = ui->passwordEdit->text(); // Note: Store hashed password in real applications

        root["user"] = userSettings;

        file.resize(0);
        file.write(QJsonDocument(root).toJson());
        file.close();
        return true;
    }
    QMessageBox::warning(this, "Error", "Failed to save user settings!");
    return false;
}


void User::onNextClicked() {
    // Check all fields are filled
    if(ui->hostnameEdit->text().isEmpty() ||
        ui->usernameEdit->text().isEmpty() ||
        ui->passwordEdit->text().isEmpty() ||
        ui->confirmPassEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Incomplete Form", "Please fill all required fields!");
        return;
    }

    // Check password match
    if(ui->passwordEdit->text() != ui->confirmPassEdit->text()) {
        QMessageBox::warning(this, "Password Mismatch", "Password and confirmation do not match!");
        return;
    }

    // Validate hostname format (optional)
    QRegularExpression hostRegex("^[a-zA-Z0-9-]{1,63}$");
    if(!hostRegex.match(ui->hostnameEdit->text()).hasMatch()) {
        QMessageBox::warning(this, "Invalid Hostname",
                             "Hostname must be 1-63 characters, only a-z, A-Z, 0-9, and hyphens");
        return;
    }

    // Validate username format (optional)
    QRegularExpression userRegex("^[a-z_][a-z0-9_-]*$");
    if(!userRegex.match(ui->usernameEdit->text()).hasMatch()) {
        QMessageBox::warning(this, "Invalid Username",
                             "Username must:\n- Start with lowercase letter or underscore\n"
                             "- Contain only lowercase letters, numbers, hyphens, and underscores");
        return;
    }

    // Save and proceed if all validations pass
    if(saveUserSettings()) {
        emit nextButtonClicked();
    }
}



User::~User()
{
    delete ui;
}
