#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "Engine.h"
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWidget;
}
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

public slots:
    void on_chkEnabled_stateChanged(int);
    void on_chkTransparency_stateChanged(int);
    void on_chkSmallIcons_stateChanged(int);
    void on_btnActive_clicked();
    void on_btnInactive_clicked();
    void handleAppTheme();
    void handleSystemTheme();

    void on_btnTaskbarBg_clicked();

private:
    Ui::MainWidget *ui;
    Engine engine;
    QColor active, inactive, taskbarBg;
};
#endif // MAINWIDGET_H
