#include "settingsbar.h"
#include "ui_settingsbar.h"
#include "QDebug"
SettingsBar::SettingsBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsBar)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
}

SettingsBar::~SettingsBar()
{
    delete ui;
}
