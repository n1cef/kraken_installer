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

}
Keyboard::~Keyboard()
{
    delete ui;
}


void Keyboard::saveSettings() {
    // Create a JSON object
    QJsonObject settings;
    settings["language"] = ui->comboBoxLanguage->currentText();
    settings["keyboard"] = ui->comboBoxKeyboard->currentText();

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
