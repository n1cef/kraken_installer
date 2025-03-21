#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonDocument>

namespace Ui {
class Keyboard;
}

class Keyboard : public QWidget
{
    Q_OBJECT

public:
    explicit Keyboard(QWidget *parent = nullptr);
    ~Keyboard();

    void saveSettings(); //  save the setting in the json file

signals:
    void nextButtonClicked();
private slots:
    void on_btnNext_clicked();

private:
    Ui::Keyboard *ui;
};

#endif // KEYBOARD_H
