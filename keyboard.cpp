#include "keyboard.h"
#include <QPainter>
#include <QDebug>
Keyboard::Keyboard(QGraphicsScene *scene, int x, int y)
{
    this->scene = scene;
    scene->update();
    this->x = x;
    this->y = y;
    const QVector<char> row0 = {'`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '+'};
    for (int i = 0; i < row0.size(); i++)
        buttons[QString(row0[i])] = QRect(x + 27 + 53 * i, y + 27, 50, 50);
    //backspace
    buttons["BSP"] = QRect(x + 27 + 53 * 13, y + 27, 800 - (27 + 53 * 13), 50);

    const QVector<char>row1 = {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']'};
    for (int i = 0; i < row1.size(); i++)
        buttons[QString(row1[i])] = QRect(x + 107 + 53 * i, y + 80, 50, 50);
    buttons["Tab"] = QRect(x + 27, y + 80, 77, 50);
    buttons["\\"] = QRect(x + 107 + 53 * 12, y + 80, 800 - (107 + 53 * 12), 50);

    const QVector<char>row2 = {'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\"'};
    for (int i = 0; i < row2.size(); i++)
        buttons[QString(row2[i])] = QRect(x + 117 + 53 * i, y + 133, 50, 50);
    buttons["Caps\nLock"] =  QRect(x + 27, y + 133, 87, 50);
    buttons["Enter"] =  QRect(x + 117 + 53 * row2.size(), y + 133, 800 - (117 + 53 * row2.size()), 50);

    const QVector<char>row3 = {'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '?'};
    for (int i = 0; i < row3.size(); i++)
        buttons[QString(row3[i])] = QRect(x + 144 + 53 * i, y + 186, 50, 50);
    buttons["Shift"] = QRect(x + 27, y + 186, 114, 50);
    buttons["Shift "] = QRect(x + 144 + 53 * row3.size(), y + 186, 800 - (144 + 53 * row3.size()), 50);

    buttons["Ctrl"] = QRect(x + 27, y + 239, 77, 50);
    buttons["Ctrl "] = QRect(x + 724, y + 239, 76, 50);

    buttons["Alt"] = QRect(x + 27 + 77 + 3, y + 239, 77, 50);
    buttons["Alt "] = QRect(x + 644, y + 239, 77, 50);

    //Space
    buttons[" "] = QRect(x + 27 + 77 + 3 + 77 + 3, y + 239, 644 - (27 + 77 + 3 + 77 + 3) - 3, 50);

}


QRectF Keyboard::boundingRect() const
{
    return QRect(x, y, 500, 500);
}

void Keyboard::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QFont *font = new QFont(QFont("Comic Sans MC", 17));
    QFontMetrics *fm = new QFontMetrics(*font);
    painter->setFont(*font);

    for(auto button = buttons.begin(); button != buttons.end(); button++) {
        painter->setPen(QColor(64, 64, 64));
        painter->drawRect(button.value());
        painter->fillRect(button.value(), QColor(64, 64, 64));
        painter->setPen(QColor(192, 192, 192));
        painter->drawText(button.value(), Qt::AlignCenter, button.key());
    }

    for (int i = 0; i < pressed.size(); i++) {
        painter->setPen(QColor(245, 245, 245));
        painter->drawRect(buttons[pressed[i]]);
        painter->fillRect(buttons[pressed[i]], QColor(245, 245, 245));
        painter->setPen(QColor(40, 40, 40));
        painter->drawText(buttons[pressed[i]], Qt::AlignCenter, pressed[i]);
    }

}

void Keyboard::press_key(int key) {
    if (!pressed.contains(QString(key))) pressed.push_back(QString(key));
    scene->update();
}

void Keyboard::release_key(int key) {
    pressed.removeOne(QString(key));
    scene->update();
}

