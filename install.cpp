#include "install.h"
#include "ui_install.h"
#include <QFile>
#include <QMessageBox>
#include <QJsonDocument>
#include <QDir>
#include <QDebug>
#include <QTextCursor>
#include <QProcessEnvironment>

Install::Install(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Install)

{
    ui->setupUi(this);
    process = new QProcess(this);

    connect(process, &QProcess::readyReadStandardOutput,
            this, &Install::handleStandardOutput);
    connect(process, &QProcess::readyReadStandardError,
            this, &Install::handleStandardError);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Install::handleScriptOutput);

    appendToConsole("Ready to execute scripts...", Qt::gray);
}



void Install::updateProgress(int value)
{
    ui->progressBar->setValue(value);
    ui->statusLabel->setText(QString("Progress: %1%").arg(value));
}

void Install::on_install_btn_clicked()
{
    // Clear previous output
    ui->outputConsole->clear();
    appendToConsole("Starting script execution...", Qt::blue);

    QString scriptPath = "/home/kraken.sh";

    if(!QFile::exists(scriptPath)) {
        appendToConsole("Error: Script not found!", Qt::red);
        return;
    }

    process->start("bash", QStringList() << scriptPath);
    connect(process, &QProcess::errorOccurred, [this](QProcess::ProcessError error) {
        appendToConsole("Error: " + process->errorString(), Qt::red);
    });
}


void Install::handleStandardOutput()
{
    QByteArray output = process->readAllStandardOutput();
    appendToConsole(QString::fromUtf8(output), Qt::black);

    // Simple progress detection (customize for your script)
    if(output.contains("Progress:")) {
        int progress = output.split(':').last().trimmed().toInt();
        updateProgress(progress);
    }
}

void Install::handleStandardError()
{
    QByteArray error = process->readAllStandardError();
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
