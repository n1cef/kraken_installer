#include "install.h"
#include "ui_install.h"
#include <QFile>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDir>
Install::Install(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Install)
{
    ui->setupUi(this);
    connect(ui->install_btn, &QPushButton::clicked,
            this, &Install::nextButtonClicked);
}





void Install::startInstallation() {
    // Generate script
    generateInstallScript();

    // Execute with pkexec (GUI sudo)
    installProcess = new QProcess(this);
    connect(installProcess, &QProcess::readyReadStandardOutput, this, &Install::onReadyRead);
    connect(installProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Install::onProcessFinished);

    installProcess->start("pkexec", {"bash", generatedScriptPath});
}

void Install::onReadyRead() {
    QString output = installProcess->readAllStandardOutput();
    ui->outputConsole->appendPlainText(output);

    // Update progress (example heuristic)
    if(output.contains("INSTALLING")) ui->progressBar->setValue(40);
    if(output.contains("CONFIGURING")) ui->progressBar->setValue(75);
}

void Install::generateInstallScript() {
    // Read settings.json
    QFile configFile("/home/settings.json");
    configFile.open(QIODevice::ReadOnly);
    QJsonDocument configDoc = QJsonDocument::fromJson(configFile.readAll());

    // Generate script using template
    generatedScriptPath = QDir::tempPath() + "/home/kraken_install.sh";
    QFile scriptFile(generatedScriptPath);
    scriptFile.open(QIODevice::WriteOnly);

    QString scriptContent = QString(R"(
        #!/bin/bash
        echo "Starting installation..."
        DISK=%1
        USERNAME=%2
        # ... more variables ...
    )").arg(configDoc["partition"]["disk_path"].toString(),
                                     configDoc["user"]["username"].toString());

    scriptFile.write(scriptContent.toUtf8());
    scriptFile.setPermissions(QFile::ExeOwner | QFile::ReadOwner);
}

void Install::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    Q_UNUSED(exitStatus)  // Optional: if not using this parameter

    if(exitCode == 0) {
        ui->outputConsole->appendPlainText("\nInstallation completed successfully!");
        ui->progressBar->setValue(100);
    } else {
        ui->outputConsole->appendPlainText("\nInstallation failed!");
        QMessageBox::critical(this, "Error", "Installation process exited with code: "
                                                 + QString::number(exitCode));
    }
}



Install::~Install()
{
    delete ui;
}
