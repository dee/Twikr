#include "engine.h"
#include <QString>
#include <windows.h>
#include <QDebug>
#include <QProcess>
#include <QtGlobal>
#include <QMessageBox>
#include <QApplication>

const QString DwmPath{"SOFTWARE\\Microsoft\\Windows\\DWM"};
const QString CurrentVersion{"SOFTWARE\\Microsoft\\Windows\\CurrentVersion"};
const QString PersonalizePath = CurrentVersion + "\\Themes\\Personalize";
const QString ExplorerAdvPath{CurrentVersion + "\\Explorer\\Advanced"};
const QString ExplorerAccent{CurrentVersion + "\\Explorer\\Accent"};

Engine::Engine(QObject* parent)
    : QObject(parent)
{};

bool Engine::isColorPrevalenceEnabled()
{
    return getDWord(HKEY_CURRENT_USER, DwmPath, Constants::ColorPrevalence) == 1 ||
        getDWord(HKEY_CURRENT_USER, PersonalizePath, Constants::ColorPrevalence) == 1;
}

void Engine::enableColorPrevalence(bool enable)
{
    DWORD value = enable ? 1: 0;
    setOrCreateDWord(HKEY_CURRENT_USER, DwmPath, Constants::ColorPrevalence,
                     value);
    setOrCreateDWord(HKEY_CURRENT_USER, PersonalizePath, Constants::ColorPrevalence,
                     value);
}

bool Engine::isTransparencyEnabled()
{
    return getDWord(HKEY_CURRENT_USER, PersonalizePath, Constants::EnableTransparency) == 1;
}

void Engine::enableTransparency(bool enable)
{
    DWORD value = enable ? 1: 0;
    setOrCreateDWord(HKEY_CURRENT_USER, PersonalizePath, Constants::EnableTransparency, value);
}

bool Engine::appsUseLightTheme()
{
    return getDWord(HKEY_CURRENT_USER, PersonalizePath, Constants::AppsUseLightTheme) == 1;
}

void Engine::setAppsLightTheme(bool set)
{
    DWORD value = set ? 1: 0;
    setOrCreateDWord(HKEY_CURRENT_USER, PersonalizePath, Constants::AppsUseLightTheme, value);
}

bool Engine::systemUsesLightTheme()
{
    return getDWord(HKEY_CURRENT_USER, PersonalizePath, Constants::SystemUsesLightTheme) == 1;
}

void Engine::setSysUsesLightTheme(bool set)
{
    DWORD value = set ? 1: 0;
    setOrCreateDWord(HKEY_CURRENT_USER, PersonalizePath, Constants::SystemUsesLightTheme, value);
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
    return getDWord(HKEY_CURRENT_USER, ExplorerAdvPath, Constants::TaskbarSmallIcons) != 0;
}

void Engine::enableSmallIcons(bool enable)
{
    auto oldValue = (getDWord(HKEY_CURRENT_USER, ExplorerAdvPath, Constants::TaskbarSmallIcons) != 0);
    setOrCreateDWord(HKEY_CURRENT_USER, ExplorerAdvPath, Constants::TaskbarSmallIcons, enable ? 1 : 0);
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

QColor Engine::getTaskbarColor()
{
    auto buf = getBinary(HKEY_CURRENT_USER, ExplorerAccent, Constants::AccentPalette);
    // qDebug() << buf;
    // qDebug() << "Length is" << buf.length();
    return QColor(qRgba(buf[20],buf[21],buf[22],buf[23]));
}

void Engine::setTaskbarColor(QColor color)
{
    auto buf = getBinary(HKEY_CURRENT_USER, ExplorerAccent, Constants::AccentPalette);
    buf[20] = (byte)color.red();
    buf[21] = (byte)color.green();
    buf[22] = (byte)color.blue();
    buf[23] = (byte)color.alpha();
    setBinary(HKEY_CURRENT_USER, ExplorerAccent, Constants::AccentPalette, buf);
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

QByteArray Engine::getBinary(HKEY handle, const QString &path, const QString &name)
{
    QByteArray buf{255, '\0'};
    unsigned long size = 255;
    HKEY keyHandle;
    if (::RegOpenKey(handle, path.toStdString().c_str(), &keyHandle) != 0)
    {
        qDebug() << "Failed to open" << path;
        return QByteArray();
    }

    auto result = ::RegQueryValueEx(keyHandle,
                                    name.toStdString().c_str(),
                                    NULL,
                                    NULL,
                                    (LPBYTE)buf.data(),
                                    &size);
    if (result != 0)
    {
        qDebug() << "Error getting binary value from" << path << name;
        qDebug() << "Return code" << result;
        return QByteArray();
    }

    ::RegCloseKey(keyHandle);
    buf.resize(size);
    return buf;
}

void Engine::setBinary(HKEY handle, const QString &path, const QString &name, const QByteArray &buf)
{
    HKEY keyHandle;
    if (::RegOpenKey(handle, path.toStdString().c_str(), &keyHandle) != 0)
    {
        qDebug() << "Failed to open" << path;
        return;
    }
    auto result = ::RegSetValueEx(keyHandle, name.toStdString().c_str(), 0, REG_BINARY, (const byte*)buf.data(), buf.length());
    if (result != 0)
    {
        qDebug() << "Error saving binary value to" << path << name;
        qDebug() << "Return code" << result;
        return;
    }
    ::RegCloseKey(handle);
}

QString Engine::getColorKey(bool active)
{
    return active ? Constants::AccentColor : Constants::AccentColorInactive;
}
