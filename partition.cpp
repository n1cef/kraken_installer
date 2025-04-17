#include "partition.h"
#include "ui_partition.h"
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QProcess>

Partition::Partition(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Partition)
{
    ui->setupUi(this);
    connect(ui->diskTreeWidget, &QTreeWidget::itemSelectionChanged,
            this, &Partition::checkDiskSize);

    connect(ui->confirmCheckBox, &QCheckBox::checkStateChanged,
            this, &Partition::checkDiskSize);

    connect(ui->refreshButton, &QPushButton::clicked,
            this, &Partition::populateDisks);

    connect(ui->partition_btn, &QPushButton::clicked,
            this, &Partition::onNextClicked);

}

Partition::~Partition()
{
    delete ui;
}

void Partition::populateDisks() {
    QProcess lsblk;
    lsblk.start("lsblk", {"-J", "-o", "NAME,SIZE,TYPE,MOUNTPOINT"});
    lsblk.waitForFinished();

    QJsonDocument doc = QJsonDocument::fromJson(lsblk.readAll());
    QJsonObject root = doc.object();
    QJsonArray devices = root["blockdevices"].toArray();

    ui->diskTreeWidget->clear();
    foreach(const QJsonValue &device, devices) {
        QJsonObject disk = device.toObject();
        if(disk["type"].toString() == "disk") {
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->diskTreeWidget);
            item->setText(0, "/dev/" + disk["name"].toString());
            item->setText(1, disk["size"].toString());
            // Store raw size in GB as custom data
            item->setData(1, Qt::UserRole,
                          this->parseSizeToGB(disk["size"].toString()));
        }
    }
}



void Partition::checkDiskSize() {
    QTreeWidgetItem *selected = ui->diskTreeWidget->currentItem();
    bool valid = false;

    if(selected) {
        double sizeGB = selected->data(1, Qt::UserRole).toDouble();
        valid = (sizeGB >= 50);
        ui->diskWarningLabel->setVisible(!valid);
    }

    ui->partition_btn->setEnabled(valid &&
                               ui->confirmCheckBox->isChecked());
}

double Partition::parseSizeToGB(const QString &sizeStr) const {
    // Extract numeric value and unit
    QRegularExpression re("([\\d\\.]+)([A-Za-z]+)");
    QRegularExpressionMatch match = re.match(sizeStr);

    if(!match.hasMatch()) {
        qWarning() << "Invalid size format:" << sizeStr;
        return 0.0;
    }

    double value = match.captured(1).toDouble();

    if(value <= 0) {
        qWarning() << "Invalid size value:" << sizeStr;
        return 0.0;
    }
    QString unit = match.captured(2).toUpper();

    // Convert to gigabytes
    if(unit == "B") return value / (1024 * 1024 * 1024);
    if(unit == "K") return value / (1024 * 1024);
    if(unit == "M") return value / 1024;
    if(unit == "G") return value;
    if(unit == "T") return value * 1024;

    qWarning() << "Unknown unit:" << unit;
    return value;  // Assume gigabytes if unknown
}



void Partition::savePartitionSettings() {
    // Validate disk selection
    QTreeWidgetItem *selectedDisk = ui->diskTreeWidget->currentItem();
    if(!selectedDisk) {
        QMessageBox::warning(this, "Error", "Please select a disk!");
        return;
    }

    // Validate disk size
    double diskSizeGB = selectedDisk->data(1, Qt::UserRole).toDouble();
    if(diskSizeGB < 50) {
        QMessageBox::warning(this, "Error",
                             "Selected disk must be at least 50GB!");
        return;
    }

    // Validate confirmation
    if(!ui->confirmCheckBox->isChecked()) {
        QMessageBox::warning(this, "Error",
                             "You must confirm you want to erase the disk!");
        return;
    }

    // Create partition settings object
    QJsonObject partitionSettings;
    partitionSettings["disk_path"] = selectedDisk->text(0);
    partitionSettings["separate_home"] = ui->homeCheckBox->isChecked() ? "yes" : "no";
    partitionSettings["enable_swap"] = ui->swapCheckBox->isChecked() ? "yes" : "no";

    // Merge with existing settings
    QFile file("/home/settings.json");
    if(file.open(QIODevice::ReadWrite)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject root = doc.object();

        root["partition"] = partitionSettings;

        file.resize(0);
        file.write(QJsonDocument(root).toJson());
        file.close();
    } else {
        QMessageBox::warning(this, "Error",
                             "Failed to save partition settings!");
    }
}










void Partition::onNextClicked() {


    savePartitionSettings();
    emit nextButtonClicked();

}
