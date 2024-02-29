#include "engine.h"
#include <QString>
#include <windows.h>
#include <QDebug>

constexpr const char* DwmPath{"SOFTWARE\\Microsoft\\Windows\\DWM"};
constexpr const char* PersonalizePath{"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize"};

Engine::Engine() {};

bool Engine::isColorPrevalenceEnabled()
{
    return getDWord(HKEY_CURRENT_USER, DwmPath, "ColorPrevalence") == 1;
}

void Engine::enableColorPrevalence(bool enable)
{
    DWORD value = enable ? 1: 0;
    setOrCreateDWord(HKEY_CURRENT_USER, DwmPath, "ColorPrevalence",
                     value);
}

bool Engine::isTransparencyEnabled()
{
    return getDWord(HKEY_CURRENT_USER, PersonalizePath, "EnableTransparency");
}

void Engine::enableTransparency(bool enable)
{
    DWORD value = enable ? 1: 0;
    setOrCreateDWord(HKEY_CURRENT_USER, PersonalizePath, "EnableTransparency", value);
}

bool Engine::appsUseLightTheme()
{
    return getDWord(HKEY_CURRENT_USER, PersonalizePath, "AppsUseLightTheme") == 1;
}

void Engine::setAppsLightTheme(bool set)
{
    DWORD value = set ? 1: 0;
    setOrCreateDWord(HKEY_CURRENT_USER, PersonalizePath, "AppsUseLightTheme", value);
}

bool Engine::systemUsesLightTheme()
{
    return getDWord(HKEY_CURRENT_USER, PersonalizePath, "SystemUsesLightTheme") == 1;
}

void Engine::setSysUsesLightTheme(bool set)
{
    DWORD value = set ? 1: 0;
    setOrCreateDWord(HKEY_CURRENT_USER, PersonalizePath, "SystemUsesLightTheme", value);
}

QColor Engine::getAccentColor(bool active)
{
    QString key = getColorKey(active);
    unsigned int regValue = getDWord(HKEY_CURRENT_USER, DwmPath, key);
    // this value is a 32-bit abgr, so we have to convert
    QColor color{QRgb(regValue)};
    return QColor::fromRgb(color.blue(), color.green(), color.red(), color.alpha());
}

void Engine::setAccentColor(QColor color, bool active)
{
    QString key = getColorKey(active);
    QRgb r = qRgba(color.blue(), color.green(), color.red(), color.alpha());
    unsigned int dwColor{r};
    setOrCreateDWord(HKEY_CURRENT_USER, DwmPath, key, dwColor);
}

int Engine::getDWord(HKEY handle, const QString& path, const QString& name)
{
    DWORD val;
    unsigned long size{sizeof(DWORD)};
    auto result = ::RegGetValue(handle, path.toStdString().c_str(),
                                name.toStdString().c_str(),
                                RRF_RT_DWORD,
                                nullptr,
                                &val,
                                &size);
    if (result != 0)
    {
        qDebug() << "Error reading registry";
    }
    return val;
}

void Engine::setOrCreateDWord(HKEY handle, const QString& path, const QString& name, unsigned int value)
{
    DWORD val{value};
    unsigned int size{sizeof(val)};
    auto result = ::RegSetKeyValue(handle,
                     path.toStdString().c_str(),
                     name.toStdString().c_str(),
                     REG_DWORD,
                     &val,
                     size);
    if (result != 0)
    {
        qDebug() << "Error saving value";
    }
}

QString Engine::getColorKey(bool active)
{
    return active ? "AccentColor" : "AccentColorInactive";
}
