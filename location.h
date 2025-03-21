#ifndef LOCATION_H
#define LOCATION_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonDocument>

namespace Ui {
class Location;
}

class Location : public QWidget
{
    Q_OBJECT

public:
    explicit Location(QWidget *parent = nullptr);
    ~Location();

    void saveSettings(); // Save location settings to JSON

signals:
    void nextButtonClicked();
    void backButtonClicked();

private slots:
    void on_nextButton_clicked();
    void on_backButton_clicked();

private:
    Ui::Location *ui;
    void loadCountries();
    void loadTimezones();
};

#endif // LOCATION_H
