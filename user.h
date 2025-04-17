#ifndef USER_H
#define USER_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonDocument>

namespace Ui {
class User;
}

class User : public QWidget
{
    Q_OBJECT

public:
    explicit User(QWidget *parent = nullptr);
    ~User();
    bool saveUserSettings();

signals:
    void nextButtonClicked();

private slots:
    void onNextClicked();
private:
    Ui::User *ui;
};

#endif // USER_H
