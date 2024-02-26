#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QColorDialog>
#include <QDebug>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | /*Qt::WindowSystemMenuHint |*/ Qt::WindowCloseButtonHint);
    ui->setupUi(this);

    if (engine.isColorPrevalenceEnabled())
    {
        ui->chkEnabled->setCheckState(Qt::Checked);
    }
    else
    {
        ui->chkEnabled->setCheckState(Qt::Unchecked);
    }
    ui->btnActive->setEnabled(ui->chkEnabled->isChecked());
    ui->btnInactive->setEnabled(ui->chkEnabled->isChecked());

    if (engine.appsUseLightTheme())
    {
        ui->rbAppLight->setChecked(true);
    }
    else
    {
        ui->rbAppDark->setChecked(true);
    }
    if (engine.systemUsesLightTheme())
    {
        ui->rbSysLight->setChecked(true);
    }
    else
    {
        ui->rbSysDark->setChecked(true);
    }

    ui->chkTransparency->setChecked(engine.isTransparencyEnabled());

    connect(ui->rbAppLight, &QRadioButton::toggled, this, &MainWidget::handleAppTheme);
    connect(ui->rbAppDark, &QRadioButton::toggled, this, &MainWidget::handleAppTheme);
    connect(ui->rbSysLight, &QRadioButton::toggled, this, &MainWidget::handleSystemTheme);
    connect(ui->rbSysDark, &QRadioButton::toggled, this, &MainWidget::handleSystemTheme);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::on_chkEnabled_stateChanged(int value)
{
    //qDebug() << "State changed, value=" << value;
    ui->btnActive->setEnabled(value == 0);
    ui->btnInactive->setEnabled(value == 0);
}

void MainWidget::on_chkTransparency_stateChanged(int value)
{
    engine.enableTransparency(value != 0);
}

void MainWidget::on_btnActive_clicked()
{
    // auto color = QColorDialog::getColor(Qt::white, this);
}

void MainWidget::on_btnInactive_clicked()
{

}

void MainWidget::handleAppTheme()
{
    engine.setAppsLightTheme(ui->rbAppLight->isChecked());
}

void MainWidget::handleSystemTheme()
{
    engine.setSysUsesLightTheme(ui->rbSysLight->isChecked());
}