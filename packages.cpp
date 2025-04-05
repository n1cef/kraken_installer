#include "packages.h"
#include "ui_packages.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>
#include <QObject>
#include <QJsonObject>

Packages::Packages(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Packages)
{
    ui->setupUi(this);

    connect(ui->package_btn, &QPushButton::clicked,
            this, &Packages::onNextClicked);
}

Packages::~Packages()
{
    delete ui;
}



void Packages::saveSettings() {
    QJsonArray packagesArray;
    QMap<QString, QStringList> packageMap = createPackageMap();

    // Check all checkboxes in all tabs
    foreach(QCheckBox *checkbox, findChildren<QCheckBox*>()) {
        if(checkbox->isChecked()) {
            QStringList packageNames = packageMap.value(checkbox->objectName());
            if(!packageNames.isEmpty()) {
                for(const QString &pkg : packageNames) {
                    packagesArray.append(pkg);
                }
            }
        }
    }

    // Merge with existing settings
    QFile file("/home/settings.json");
    if(file.open(QIODevice::ReadWrite)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject root = doc.object();

        root["packages"] = packagesArray;

        file.resize(0);
        file.write(QJsonDocument(root).toJson());
        file.close();
    } else {
        QMessageBox::warning(this, "Error", "Failed to save package selections!");
    }
}




QMap<QString, QStringList> Packages::createPackageMap() {
    return {
        // Computer Science Tab
        //---------------general develeppent --------
        //compilers
        {"chkGCC", {"gcc"}},
        {"chkClang", {"clang"}},
        {"chkRust", {"rustc"}},
        {"chkllvm", {"llvm"}},
        {"chkgc", {"gc"}},

        //IDEs
        {"chkVSCode", {"vscode"}},
        {"chkVim", {"vim"}},
        {"chkEmacs", {"emacs"}},
        {"chkJetBrains", {"ideaic"}},

        // build tools
        {"chkCmake", {"cmake"}},
        {"chkMake", {"make"}},
        {"chkNinja", {"ninja"}},
        {"chkMeson", {"meson"}},
        //version controle
        {"chkgit", {"git"}},
        {"chkGithubCLI", {"cli"}},
        {"chkGitlabCLI", {"gitlabcli"}},
        //debugging
        {"chkgdb", {"gdb"}},
        {"chkValgrind", {"valgrind"}},
        {"chkStrace", {"strace"}},


        //---------------web dev toools -----------------
        //language

        {"chkJava", {"java"}},
        {"chkPHP", {"php"}},
        {"chkPython3", {"python3"}},
        {"chkGo", {"go"}},
        {"chkRustWeb", {"rust"}},


         // package manager
        {"chkNpm", {"npm"}},
        {"chkPip", {"pip"}},
        {"chkPHPComposer", {"composer"}},
        {"chkMaven", {"maven"}},
        {"chkGin", {"gin"}},
        {"chkCargo", {"cargo"}},
        {"chkRestfulAPI", {"restapi"}},


        //testing
        // todo: add this later we see you dummy   {"chkK6", {"k6"}},
        {"chkSelenium", {"selenium"}},
        {"chkJest", {"jest"}},
        {"chkCypress", {"cypress"}},


        //database

        {"chkMySQL", {"mysql"}},
        {"chkMariaDB", {"mariadb"}},
        {"chkMongoDB", {"mongodb"}},
        {"chkSQLite", {"sqlite3"}},

        //------------devops and cloud tools

        //caintainer
           {"chkDocker", {"docker"}},
           {"chkPodman", {"podman"}},
           {"chkKubernetes", {"kubectl"}},

        //infrastucture as code IAC

        {"chkTerraform", {"terraform"}},
        {"chkAnsible", {"ansible"}},


        //cloud tools

        {"chkAWSCLI", {"awscli"}},
        {"chkAzureCLI", {"azurecli"}},
        {"chkGoogleCloudSDK", {"googlecloudsdk"}},


        //monitoring

        {"chkGrafana", {"grafana"}},
        {"chkNagios", {"nagios"}},
        {"chkPrometheus", {"prometheus"}},
        {"chkDocker", {"docker"}},

        //android dev tools
        {"chkAndroidStudio", {"androidstudio"}},
        {"chkKotlin", {"kotlin"}},
        {"chkReactNative", {"react-native"}},


        //-------------cyber security----------


        //network analysis


        {"chkWireshark", {"wireshark"}},
        {"chkNmap", {"nmap"}},
        {"chkOpenVPN", {"openvpn"}},
        {"chkNetcat", {"netcat"}},
        {"chkCurl", {"curl"}},
        {"chkWget", {"wget"}},
        {"chkWireguard", {"wireguard"}},


        //pentesting

        {"chkMetasploit", {"metasploit"}},
        {"chkBurpsuite", {"burpsuite"}},
        {"chkJohnTheRipper", {"jhontheripper"}},


        //security tools
        {"chkAircrack", {"aircrack"}},
        {"chkHashcat", {"hashcat"}},


        //------------ai ml data science

        //framework



        {"chkScikitLearn", {"scikitlearn"}},
        {"chkTensorFlow", {"tensorflow"}},
        {"chkPyTorch", {"pytorch"}},
        {"chkPandas", {"panda"}},
        {"chkNumPy", {"numpy"}},


        //data vidualiztion

        {"chkMatplotlib", {"matplotlib"}},
        {"chkSeaborn", {"seaborn"}},
        {"chkPlotly", {"plotly"}},


        //big data and r tools

         {"chkSpark", {"spark"}},
         {"chkHadoop", {"hadoop"}},
         {"chkRStudio", {"rstudio"}},
         {"chkCaret", {"caret"}},



        // Physics Tab
        //computation physics
        {"chkROOT", {"root-framework"}},
        {"chkGeant4", {"geant4"}},
        {"chkOpenFOAM", {"openfoam"}},
        {"chkLAMMPS", {"lammps"}},
        {"chkQuantumEspresso", {"quanrumespresso"}},


        //simulation and modeling

        {"chkGromacs", {"gromacs"}},
        {"chkParaView", {"paraview"}},
        {"chkBlender", {"blender"}},
        {"chkSimulation", {"simulation"}},


        //astronomy and astrophy

        {"chkStellarium", {"stellarium"}},
        {"chkAstroPy", {"astropy"}},
        {"chkSAOImageDS9", {"saoimageds9"}},
        {"chkCelestia", {"celestia"}},






        // Mathematics Tab

        //symbolic and numerical
        {"chkSageMath", {"sagemath"}},
        {"chkMaxima", {"maxima"}},
        {"chkSympy", {"sympy"}},

        {"chkOctave", {"octave"}},

        //statistical analysis


        {"chkR", {"R"}},
        {"chkJupyterNotebook", {"jupyter-notebook"}},
        {"chkPSPP", {"pspp"}},
        {"chkGretl", {"gretl"}},


        //virtualisation

        {"chkGnuplot", {"gnuplot"}},
        //{"chkMatplotlib", {"matplotlib"}}, matplotlib is redandant


        //typesetting and publishing

        {"chkTeXmaker", {"texmaker"}},
        {"chkLyX", {"lyx"}},
        {"chkTeXstudio", {"texstudio"}},
        {"chkZotero", {"zotero"}},








    };
}



void Packages::onNextClicked() {


    saveSettings();
    emit nextButtonClicked();

}
