#include "welcome.h"
#include "ui_welcome.h"

Welcome::Welcome(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Welcome)
{
    ui->setupUi(this);

    connect(ui->welcome_btn, &QPushButton::clicked,
            this, &Welcome::nextButtonClicked);
}

Welcome::~Welcome()
{
    delete ui;
}
