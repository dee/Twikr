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
    ui->chkSmallIcons->setChecked(engine.areSmallIconsUsed());

    active = engine.getAccentColor(true);
    inactive = engine.getAccentColor(false);

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
    // qDebug() << "ColorPrevalence state changed, value =" << value;
    bool enable = (value != 0);

    ui->btnActive->setEnabled(enable);
    ui->btnInactive->setEnabled(enable);
    engine.enableColorPrevalence(enable);
}

void MainWidget::on_chkTransparency_stateChanged(int value)
{
    engine.enableTransparency(value != 0);
}

void MainWidget::on_chkSmallIcons_stateChanged(int value)
{
    qDebug() << "State changed!";
    engine.enableSmallIcons(value != 0);
}

void MainWidget::on_btnActive_clicked()
{
    auto color = QColorDialog::getColor(active, this, tr("Pick active accent color"));
    if (color.isValid())
    {
        engine.setAccentColor(color, true);
    }
}

void MainWidget::on_btnInactive_clicked()
{
    auto color = QColorDialog::getColor(inactive, this, tr("Pick inactive accent color"));
    if (color.isValid())
    {
        engine.setAccentColor(color, false);
    }
}

void MainWidget::handleAppTheme()
{
    engine.setAppsLightTheme(ui->rbAppLight->isChecked());
}

void MainWidget::handleSystemTheme()
{
    engine.setSysUsesLightTheme(ui->rbSysLight->isChecked());
}
