#include "keyboard.h"
#include "ui_keyboard.h"
#include <QFile>
#include <QMessageBox>
Keyboard::Keyboard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Keyboard)
{
    ui->setupUi(this);
    connect(ui->keyboard_btn, &QPushButton::clicked,
           this, &Keyboard::on_btnNext_clicked);
     ui->comboBoxLanguage->removeItem(3);
     ui->comboBoxKeyboard->removeItem(2);

}
Keyboard::~Keyboard()
{
    delete ui;
}


void Keyboard::saveSettings() {
    // Map language to locale code
    QString languageCode;
    if (ui->comboBoxLanguage->currentText() == "French") {
        languageCode = "fr_FR.UTF-8";
    } else if (ui->comboBoxLanguage->currentText() == "English") {
        languageCode = "en_US.UTF-8";
    }
    else if (ui->comboBoxLanguage->currentText() == "Arabic") {
        languageCode = "ar_SA.UTF-8";
    }

    QString KeyboardCode;
    if (ui->comboBoxKeyboard->currentText() == "Azerty") {
        KeyboardCode = "fr";
    } else if (ui->comboBoxKeyboard->currentText() == "Qwerty") {
        KeyboardCode = "en";
    }

    // Create JSON object
    QJsonObject settings;
    settings["language"] = languageCode;
    settings["keyboard"] = KeyboardCode;

    // Save to file
    QFile file("/home/settings.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(settings);
        file.write(doc.toJson());
        file.close();
    } else {
        QMessageBox::warning(this, "Error", "Failed to save settings!");
    }
}



void Keyboard::on_btnNext_clicked() {

    saveSettings(); // Save before navigating
    emit nextButtonClicked(); // Emit signal to switch pages
}
