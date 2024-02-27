#ifndef ENGINE_H
#define ENGINE_H

#include <QColor>
#include <QString>
#include <windows.h>

class Engine
{
public:
    Engine();

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

private:
    int getDWord(HKEY handle, const QString& path, const QString& name);
    void setOrCreateDWord(HKEY handle, const QString& path, const QString& name, unsigned int value);
    QString getColorKey(bool active);
};

#endif // ENGINE_H
