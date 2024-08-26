#ifndef ENGINE_H
#define ENGINE_H

#include <QColor>
#include <QProcess>
#include <QString>
#include <windows.h>

class Engine : public QObject
{
    Q_OBJECT
public:
    Engine(QObject* parent);

    bool isColorPrevalenceEnabled();
    void enableColorPrevalence(bool enable);

    bool isTransparencyEnabled();
    void enableTransparency(bool enable);

    bool appsUseLightTheme();
    void setAppsLightTheme(bool set);

    bool systemUsesLightTheme();
    void setSysUsesLightTheme(bool set);

    QColor getAccentColor(bool active);
    void setAccentColor(QColor color, bool active);
    
    bool areSmallIconsUsed();
    void enableSmallIcons(bool);

    void restartExplorer();

public slots:
    void processError(QProcess::ProcessError error);

private:
    int getDWord(HKEY handle, const QString& path, const QString& name);
    void setOrCreateDWord(HKEY handle, const QString& path, const QString& name, unsigned int value);
    QString getColorKey(bool active);
};

#endif // ENGINE_H
