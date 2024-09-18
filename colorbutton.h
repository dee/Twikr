#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QPushButton>

class ColorButton : public QPushButton
{
    Q_OBJECT
public:
    ColorButton(QWidget* parent = nullptr);
    QColor color() const { return _color; }
    void setColor(QColor arg);

signals:
    void colorChanged(QColor color);

private:
    QColor _color;

    QColor getForeground(QColor background);
};

#endif // COLORBUTTON_H
