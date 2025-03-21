#include "location.h"
#include "ui_location.h"
#include <QFile>
#include <QMessageBox>
#include <QTimeZone>

Location::Location(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Location)
{
    ui->setupUi(this);

    // Connect buttons
    connect(ui->location_btn, &QPushButton::clicked, this, &Location::on_nextButton_clicked);
    connect(ui->location_btn, &QPushButton::clicked, this, &Location::on_backButton_clicked);

    // Load initial data
    loadCountries();
    loadTimezones();
}

Location::~Location()
{
    delete ui;
}

void Location::saveSettings()
{
    // Create JSON object
    QJsonObject settings;

    if(ui->countryList->currentItem()) {
        settings["country"] = ui->countryList->currentItem()->text();
    }
    settings["timezone"] = ui->timezoneCombo->currentText();

    // Save to file (append to existing settings)
    QFile file("/home/settings.json");
    if(file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QJsonDocument existingDoc = QJsonDocument::fromJson(file.readAll());
        QJsonObject rootObj = existingDoc.object();

        // Merge settings
        for(auto it = settings.constBegin(); it != settings.constEnd(); ++it) {
            rootObj[it.key()] = it.value();
        }

        file.resize(0); // Clear existing content
        file.write(QJsonDocument(rootObj).toJson());
        file.close();
    } else {
        QMessageBox::warning(this, "Error", "Failed to save location settings!");
    }
}

void Location::loadCountries()
{
    // Simple example - add more countries as needed
    QStringList countries = {
        "Tunisia" ,
        "United States", "France", "Germany",
        "United Kingdom", "Japan", "China"
    };

    ui->countryList->addItems(countries);
}

void Location::loadTimezones()
{
    QList<QByteArray> zones = QTimeZone::availableTimeZoneIds();
    foreach(const QByteArray &zone, zones) {
        ui->timezoneCombo->addItem(zone);
    }
}

void Location::on_nextButton_clicked()
{
    if(ui->countryList->currentItem()) {
        saveSettings();
        emit nextButtonClicked();
    } else {
        QMessageBox::warning(this, "Selection Required", "Please select a country!");
    }
    emit nextButtonClicked();

}

void Location::on_backButton_clicked()
{
    emit backButtonClicked();
}
