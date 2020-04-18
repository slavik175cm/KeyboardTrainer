#include "keyboard.h"
#include <QPainter>
#include <QDebug>
#include <Windows.h>
#include <QKeyEvent>
Keyboard::Keyboard(QGraphicsScene *scene, int x, int y)
{
    this->scene = scene;
    this->x = x;
    this->y = y;

    const QVector<QString> row0 = {"`\n~", "!\n1", "@\n2", "#\n3", "$\n4", "%\n5", "^\n6", "&\n7", "*\n8", "(\n9", ")\n0", "-\n_", "=\n+"};
    for (int i = 0; i < row0.size(); i++)
        buttons[QString(row0[i])] = QRect(x + 27 + 53 * i, y + 27, 50, 50);
    //backspace
    buttons["Backspace"] = QRect(x + 27 + 53 * 13, y + 27, 800 - (27 + 53 * 13), 50);

    const QVector<QString>row1 = {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[\n{", "]\n}"};
    for (int i = 0; i < row1.size(); i++)
        buttons[QString(row1[i])] = QRect(x + 107 + 53 * i, y + 80, 50, 50);
    buttons["Tab"] = QRect(x + 27, y + 80, 77, 50);
    buttons["\\\n|"] = QRect(x + 107 + 53 * 12, y + 80, 800 - (107 + 53 * 12), 50);

    const QVector<QString>row2 = {"A", "S", "D", "F", "G", "H", "J", "K", "L", ";\n:", "\'\n\""};
    for (int i = 0; i < row2.size(); i++)
        buttons[QString(row2[i])] = QRect(x + 117 + 53 * i, y + 133, 50, 50);
    buttons["Caps Lock"] =  QRect(x + 27, y + 133, 87, 50);
    buttons["Enter"] =  QRect(x + 117 + 53 * row2.size(), y + 133, 800 - (117 + 53 * row2.size()), 50);

    const QVector<QString>row3 = {"Z", "X", "C", "V", "B", "N", "M", ",\n<", ".\n>", "/\n?"};
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

    special_keys[VK_LSHIFT] = "Shift";
    special_keys[VK_RSHIFT] = "Shift ";
    special_keys[VK_LCONTROL] = "Ctrl";
    special_keys[VK_RCONTROL] = "Ctrl ";
    special_keys[VK_LMENU] = "Alt";
    special_keys[VK_RMENU] = "Alt ";

    special_keys[Qt::Key_CapsLock] = "Caps Lock";
    special_keys[Qt::Key_Return] = "Enter";
    special_keys[Qt::Key_Backspace] = "Backspace";
    special_keys[Qt::Key_Tab] = "Tab";
    special_keys[Qt::Key_Space] = " ";

    scene->update();
}


QRectF Keyboard::boundingRect() const
{
    return QRect(x, y, 1, 1);
}

void Keyboard::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QFont *normal_font = new QFont(QFont("Comic Sans MC", 17));
    QFont *small_font = new QFont(QFont("Comic Sans MC", 10));

    for(auto button = buttons.begin(); button != buttons.end(); button++) {
        painter->setFont((modifiers.contains(button.key()) || (double_buttons.contains(button.key()))) ? *small_font : *normal_font);
        painter->setPen(QColor(64, 64, 64));
        painter->drawRect(button.value());
        painter->fillRect(button.value(), QColor(64, 64, 64));
        painter->setPen(QColor(192, 192, 192));
        painter->drawText(button.value(), Qt::AlignCenter, button.key());
    }

    for (int i = 0; i < pressed.size(); i++) {
        painter->setFont((modifiers.contains(pressed[i]) || (double_buttons.contains(pressed[i]))) ? *small_font : *normal_font);
        painter->setPen(QColor(245, 245, 245));
        painter->drawRect(buttons[pressed[i]]);
        painter->fillRect(buttons[pressed[i]], QColor(245, 245, 245));
        painter->setPen(QColor(40, 40, 40));
        painter->drawText(buttons[pressed[i]], Qt::AlignCenter, pressed[i]);
    }

}

QString Keyboard::get_button_name(int key){
    if (key >= 'A' && key <= 'Z') return QString(key);
    if (special_keys.contains(key)) return special_keys[key];
    for (int i = 0; i < double_buttons.size(); i++)
        if (double_buttons[i].contains(key)) return double_buttons[i];
    return "-1";
}

void Keyboard::press_key(QKeyEvent *event) {
    int key = event->key();
    if (event->key() == Qt::Key_Shift || event->key() == Qt::Key_Alt || event->key() == Qt::Key_Control) {
        key = interpretKeyEvent(event);
    }
    if (pressed.contains(QString(key))) return;
    pressed.push_back(get_button_name(key));
    if (pressed[pressed.size() - 1] == "-1") pressed.pop_back();
    scene->update();
}

void Keyboard::release_key(QKeyEvent *event) {
    int key = event->key();
    if (event->key() == Qt::Key_Shift || event->key() == Qt::Key_Alt || event->key() == Qt::Key_Control) {
        key = interpretKeyEvent(event);
    }
    pressed.removeAll(get_button_name(key));
    scene->update();
}


//this method is used to differentiate left and right modifiers
// still don't know how it works
#define EXTENDED_KEY_MASK   0x01000000
#define LSHIFT_MASK         0x2a
#define RSHIFT_MASK         0X36

quint32 Keyboard::interpretKeyEvent(QKeyEvent* e) {
    quint32 vk = e->nativeVirtualKey();
    quint32 mods = e->nativeModifiers();
    quint32 scode = e->nativeScanCode();
    bool extended = (bool) (mods & EXTENDED_KEY_MASK);

    switch (vk) {
        case VK_CONTROL:
            vk = extended ? VK_RCONTROL : VK_LCONTROL;
            break;
        case VK_MENU:
            // VK_MENU = ALT virtual key
            vk = extended ? VK_RMENU : VK_LMENU;
            break;
        case VK_SHIFT:
            if ((scode ^ LSHIFT_MASK) == 0) {
                vk = VK_LSHIFT;
            } else if ((scode ^ RSHIFT_MASK) == 0) {
                vk = VK_RSHIFT;
            }
            break;
    };
    return vk;
}


