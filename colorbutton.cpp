#include "colorbutton.h"

ColorButton::ColorButton(QWidget* parent)
    : QPushButton(parent),
    _color(nullptr)
{
}

void ColorButton::setColor(QColor arg)
{
    _color = arg;

    if (_color != nullptr)
    {
        this->setStyleSheet(QString("background-color: %1; color: %2")
                            .arg(_color.name(QColor::NameFormat::HexRgb))
                            .arg(getForeground(_color).name(QColor::NameFormat::HexRgb)));
    }
    else
    {
        this->setStyleSheet("");
    }

    emit colorChanged(_color);
}

QColor ColorButton::getForeground(QColor background)
{
    qreal sum = (background.red()*0.299 + background.green()*0.587+background.blue()*0.114);
    if (sum > 186)
    {
        return QColor::fromRgb(0x0, 0x0, 0x0);
    }
    return QColor::fromRgb(0xFF, 0xFF, 0xFF);
}
