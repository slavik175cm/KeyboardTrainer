#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPair>
class Keyboard : public QGraphicsItem
{
public:
    int x, y;
    Keyboard(QGraphicsScene *scene, int x, int y);
    QRectF boundingRect() const;
    QGraphicsScene *scene;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QMap<QString, QRect> buttons;
    void press_key(int key);
    void release_key(int key);
    QVector<QString> pressed;
};

#endif // KEYBOARD_H
