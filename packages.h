#ifndef PACKAGES_H
#define PACKAGES_H

#include <QWidget>

namespace Ui {
class Packages;
}

class Packages : public QWidget
{
    Q_OBJECT

public:
    explicit Packages(QWidget *parent = nullptr);
    ~Packages();
signals:
    void nextButtonClicked();

private slots:
    void saveSettings();
    void onNextClicked();
private:
    Ui::Packages *ui;
     QMap<QString, QStringList> createPackageMap();
};

#endif // PACKAGES_H
