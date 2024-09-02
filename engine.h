#ifndef ENGINE_H
#define ENGINE_H

#include <QColor>
#include <QProcess>
#include <QString>
#include <windows.h>

namespace Constants
{
    const QString ColorPrevalence = "ColorPrevalence";
    const QString EnableTransparency = "EnableTransparency";
    const QString AccentColor = "AccentColor";
    const QString AccentColorInactive = "AccentColorInactive";
    const QString AccentPalette = "AccentPalette";
    const QString TaskbarSmallIcons = "TaskbarSmallIcons";
    const QString AppsUseLightTheme = "AppsUseLightTheme";
    const QString SystemUsesLightTheme = "SystemUsesLightTheme";
}

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

    QColor getTaskbarColor();
    void setTaskbarColor(QColor color);

public slots:
    void processError(QProcess::ProcessError error);

private:
    int getDWord(HKEY handle, const QString& path, const QString& name);
    void setOrCreateDWord(HKEY handle, const QString& path, const QString& name, unsigned int value);

    QByteArray getBinary(HKEY handle, const QString& path, const QString& name);
    void setBinary(HKEY handle, const QString& path, const QString& name, const QByteArray& buf);

    QColor getPaletteColor(QByteArray& src, ushort startIndex);
    void setPaletteColor(QByteArray& src, ushort startIndex, QColor color);

    QString getColorKey(bool active);
};

#endif // ENGINE_H
