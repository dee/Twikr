#include "engine.h"
#include <QString>
#include <windows.h>
#include <QDebug>
#include <QProcess>
#include <QtGlobal>
#include <QMessageBox>
#include <QApplication>

constexpr const char* DwmPath{"SOFTWARE\\Microsoft\\Windows\\DWM"};
constexpr const char* PersonalizePath{"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize"};
constexpr const char* ExplorerAdvPath{"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced"};

Engine::Engine(QObject* parent)
    : QObject(parent)
{};

bool Engine::isColorPrevalenceEnabled()
{
    return getDWord(HKEY_CURRENT_USER, DwmPath, "ColorPrevalence") == 1 ||
        getDWord(HKEY_CURRENT_USER, PersonalizePath, "ColorPrevalence") == 1;
}

void Engine::enableColorPrevalence(bool enable)
{
    DWORD value = enable ? 1: 0;
    setOrCreateDWord(HKEY_CURRENT_USER, DwmPath, "ColorPrevalence",
                     value);
    setOrCreateDWord(HKEY_CURRENT_USER, PersonalizePath, "ColorPrevalence",
                     value);
}

bool Engine::isTransparencyEnabled()
{
    return getDWord(HKEY_CURRENT_USER, PersonalizePath, "EnableTransparency") == 1
        /*|| getDWord(HKEY_CURRENT_USER, DwmPath, "EnableTransparency") == 1*/;
}

void Engine::enableTransparency(bool enable)
{
    DWORD value = enable ? 1: 0;
    setOrCreateDWord(HKEY_CURRENT_USER, PersonalizePath, "EnableTransparency", value);
    //setOrCreateDWord(HKEY_CURRENT_USER, DwmPath, "EnableTransparency", value);
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

bool Engine::areSmallIconsUsed()
{
    return getDWord(HKEY_CURRENT_USER, ExplorerAdvPath, "TaskbarSmallIcons") != 0;
}

void Engine::enableSmallIcons(bool enable)
{
    auto oldValue = (getDWord(HKEY_CURRENT_USER, ExplorerAdvPath, "TaskbarSmallIcons") != 0);
    setOrCreateDWord(HKEY_CURRENT_USER, ExplorerAdvPath, "TaskbarSmallIcons", enable ? 1 : 0);
    if (oldValue != enable)
    {
        restartExplorer();
    }
}

void Engine::restartExplorer()
{
    QScopedPointer<QProcess> stopper(new QProcess());
    connect(stopper.data(), SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
    stopper.data()->start("taskkill", QStringList() << "/f" << "/im" << "explorer.exe");
    stopper.data()->waitForFinished();

    QProcess starter;
    starter.setProgram("explorer.exe");
    auto ok = starter.startDetached();
    if (!ok)
    {
        QMessageBox::warning(QApplication::activeWindow(), "Error",
             QString("Explorer failed to start: %1").arg(starter.errorString()),
             QMessageBox::Button::Ok);
    }
}

void Engine::processError(QProcess::ProcessError error)
{
    QMessageBox::warning(QApplication::activeWindow(), "Error",
         QString("Taskkill process failed!"),
         QMessageBox::Button::Ok);
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
        qDebug() << "Error reading registry, path =" << path << "value =" << name;
    }
    // qDebug() << "getDWord returned:" << val << "path =" << path << "value =" << name;
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
