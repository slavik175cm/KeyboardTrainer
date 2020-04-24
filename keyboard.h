#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPair>
class Keyboard : public QGraphicsItem
{
public:
    Keyboard(QGraphicsScene *scene, int x, int y);
    void press_key(QKeyEvent *event);
    void release_key(QKeyEvent *event);
private:
    int x, y;
    QRectF boundingRect() const override;
    QGraphicsScene *scene;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QMap<QString, QRect> buttons;
    QMap<int, QString> special_keys;
    QString get_button_name(int key);
    quint32 interpretKeyEvent(QKeyEvent* e);
    QVector<QString> pressed;
    const QVector<int> available_symbols = {'.', ',', ';', '(', ')', ' ', '"', '`', '\'', '?', '-', '+', '\\', L'␣'};
    QVector<QString> modifiers = {"Tab", "Caps Lock", "Shift", "Shift ", "Ctrl", "Ctrl ", "Alt", "Alt ", "Backspace", "Enter"};
    const QVector<QString> double_buttons = {"`\n~", "-\n_", "[\n{", "]\n}", "\\\n|", ";\n:", ",\n<", ".\n>", "/\n?", "\'\n\"", "=\n+",
                                             "!\n1", "@\n2", "#\n3", "$\n4", "%\n5", "^\n6", "&\n7", "*\n8", "(\n9", ")\n0"};
};

#endif // KEYBOARD_H
