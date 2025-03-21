#ifndef PARTITION_H
#define PARTITION_H

#include <QWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>

namespace Ui {
class Partition;
}

class Partition : public QWidget
{
    Q_OBJECT

public:
    explicit Partition(QWidget *parent = nullptr);
    ~Partition();
    void populateDisks();
    void checkDiskSize();
  double parseSizeToGB(const QString &sizeStr) const;

signals:
    void nextButtonClicked();

private slots:
    void savePartitionSettings();
    void onNextClicked();
private:
    Ui::Partition *ui;

};

#endif // PARTITION_H
