#include "engine.h"
#include <QString>
#include <windows.h>
#include <QDebug>

Engine::Engine() {}

bool Engine::isColorPrevalenceEnabled()
{
    return getDWord(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\DWM", "ColorPrevalence") == 1;
}

void Engine::enableColorPrevalence(bool enable)
{
    DWORD value = enable ? 1: 0;
    setOrCreateDWord(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\DWM", "ColorPrevalence",
                     value);
}

bool Engine::isTransparencyEnabled()
{
    return getDWord(HKEY_CURRENT_USER,
                    "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                    "EnableTransparency");
}

void Engine::enableTransparency(bool enable)
{
    DWORD value = enable ? 1: 0;
    setOrCreateDWord(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                     "EnableTransparency", value);
}

bool Engine::appsUseLightTheme()
{
    return getDWord(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                    "AppsUseLightTheme") == 1;
}

void Engine::setAppsLightTheme(bool set)
{
    DWORD value = set ? 1: 0;
    setOrCreateDWord(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                     "AppsUseLightTheme", value);
}

bool Engine::systemUsesLightTheme()
{
    return getDWord(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                    "SystemUsesLightTheme") == 1;
}

void Engine::setSysUsesLightTheme(bool set)
{
    DWORD value = set ? 1: 0;
    setOrCreateDWord(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                     "SystemUsesLightTheme", value);
}

QColor Engine::getAccentColor(bool active)
{
    // QString key = active ? "AccentColor" : "AccentColorInactive";
    // getDWord(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\DWM", key);
    // QColor color;
    return QColor::from
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
