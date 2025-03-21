#ifndef INSTALL_H
#define INSTALL_H

#include <QWidget>
#include <QProcess>
namespace Ui {
class Install;
}

class Install : public QWidget
{
    Q_OBJECT

public:
    explicit Install(QWidget *parent = nullptr);
    ~Install();
signals:
    void nextButtonClicked();

public slots:
    void startInstallation();

private slots:
    void onReadyRead();
    void onProcessFinished(int exitCode, QProcess::ExitStatus status);
private:
    Ui::Install *ui;
    QProcess *installProcess;
    QString generatedScriptPath;
    void generateInstallScript();
};


#endif // INSTALL_H
