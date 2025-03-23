#ifndef INSTALL_H
#define INSTALL_H

#include <QWidget>
#include <QProcess>
#include <QJsonObject>
#include <QScrollBar>
#include <QTextCharFormat>
#include <QMainWindow>
namespace Ui {
class Install;
}

class Install : public QWidget
{
    Q_OBJECT

public:
    explicit Install(QWidget *parent = nullptr);
    ~Install();
//signals:
   // void nextButtonClicked();

signals:  // Uncomment this section
    void installationComplete();
//public slots:
    //void startInstallation();

private slots:
    void updateProgress(int value);
    void handleStandardOutput();
    void handleStandardError();
    void on_install_btn_clicked();  // Auto-connected via naming convention
    void handleScriptOutput(int exitCode, QProcess::ExitStatus exitStatus);
private:
    Ui::Install *ui;
    QJsonObject loadSettings();
    bool validateSettings(const QJsonObject& settings);
    void appendToConsole(const QString &text, const QColor &color = Qt::black);
    QProcess *process;


};


#endif // INSTALL_H
