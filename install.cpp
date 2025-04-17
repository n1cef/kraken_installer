#include "install.h"
#include "ui_install.h"
#include <QFile>
#include <QMessageBox>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QDir>
#include <QDebug>
#include <QTextCursor>
#include <QProcessEnvironment>
#include <QJsonArray>

Install::Install(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Install)

{
    ui->setupUi(this);
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
    ui->statusLabel->setText("Waiting to start...");
    process = new QProcess(this);

    connect(process, &QProcess::readyReadStandardOutput,
            this, &Install::handleStandardOutput);
    connect(process, &QProcess::readyReadStandardError,
            this, &Install::handleStandardError);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Install::handleScriptOutput);

    appendToConsole("Ready to execute scripts...", Qt::gray);
}


QJsonObject Install::loadSettings()
{
    QFile file("/home/settings.json");
    if (!file.open(QIODevice::ReadOnly)) {
        appendToConsole("Error: Could not open settings file!", Qt::red);
        return QJsonObject();
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        appendToConsole("JSON Parse Error: " + parseError.errorString(), Qt::red);
        return QJsonObject();
    }

    return doc.object();
}


bool Install::validateSettings(const QJsonObject& settings)
{
    QStringList requiredFields = {
        "partition/disk_path", "partition/enable_swap", "partition/separate_home",
        "user/username", "user/password", "language", "keyboard", "user/hostname", "timezone"
    };

    foreach(const QString& field, requiredFields) {
        QStringList parts = field.split('/');
        QJsonValue val = settings;
        foreach(const QString& part, parts) {
            val = val[part];
            if(val.isUndefined()) {
                appendToConsole("Missing field: " + field, Qt::red);
                return false;
            }
        }
    }
    return true;
}






void Install::updateProgress(int value)
{
    ui->progressBar->setValue(value);
    ui->statusLabel->setText(QString("Progress: %1%").arg(value));
}

void Install::on_install_btn_clicked()
{
    ui->outputConsole->clear();
    appendToConsole("Loading settings...", Qt::blue);

    QJsonObject settings = loadSettings();
    if(settings.isEmpty()) return;

    if(!validateSettings(settings)) {
        appendToConsole("Invalid settings configuration!", Qt::red);
        return;
    }

    // Extract parameters
    QJsonObject partition = settings["partition"].toObject();
    QJsonObject user = settings["user"].toObject();
    QJsonArray packagesArray = settings["packages"].toArray();


    QString packagesParam;
    foreach(const QJsonValue &pkg, packagesArray) {
        packagesParam += pkg.toString() + ",";
    }
    if(!packagesParam.isEmpty()) packagesParam.chop(1);


    QStringList params = {
        partition["disk_path"].toString(),
        partition["separate_home"].toString(),
        partition["enable_swap"].toString(),
        user["username"].toString(),
        user["password"].toString(),
        settings["language"].toString(),
        settings["keyboard"].toString(),
        user["hostname"].toString(),
        settings["timezone"].toString(),
         packagesParam
    };

    appendToConsole("Starting installation...", Qt::blue);

    QString scriptPath = "/home/kraken.sh";
    process->start("bash", QStringList() << scriptPath << params);
}






void Install::handleStandardOutput()
{
    QByteArray output = process->readAllStandardOutput();
    QString message = QString::fromUtf8(output);

    // Handle progress updates
    if(message.startsWith("PROGRESS:")) {
        QStringList parts = message.split(":");
        if(parts.count() >= 3) {
            int progress = parts[1].toInt();
            QString status = parts[2].trimmed();

            ui->progressBar->setValue(progress);
            ui->statusLabel->setText(status);
        }
    }
    else {
        appendToConsole(message, Qt::black);
    }
}

void Install::handleStandardError()
{
    QByteArray error = process->readAllStandardError();
    if(error.contains("ERROR:")) {
        QStringList parts = QString::fromUtf8(error).split(":");
        ui->statusLabel->setText(parts.last().trimmed());
    }
    appendToConsole(QString::fromUtf8(error), Qt::red);
}


void Install::appendToConsole(const QString &text, const QColor &color)
{
    QTextCharFormat format;
    format.setForeground(color);

    QTextCursor cursor(ui->outputConsole->document());
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(text, format);

    // Auto-scroll to bottom
    ui->outputConsole->verticalScrollBar()->setValue(
        ui->outputConsole->verticalScrollBar()->maximum()
        );
}




void Install::handleScriptOutput(int exitCode, QProcess::ExitStatus exitStatus)
{
    if(exitStatus == QProcess::NormalExit && exitCode == 0) {
        appendToConsole("\nScript completed successfully!\n", Qt::darkGreen);
        emit installationComplete();  // Add this signal
    } else {
        appendToConsole(QString("\nScript failed with code %1\n").arg(exitCode), Qt::red);
    }
}





Install::~Install() {

    delete ui;
}
