#ifndef STATISTICSPROVIDER_H
#define STATISTICSPROVIDER_H
#include <QVector>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "user.h"
class StatisticsProvider : public QGraphicsItem
{
public:
    StatisticsProvider(QGraphicsScene *scene, User *user);
    void display_statistics();
private:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QGraphicsScene *scene;
    void display_graph(QPainter *painter, int x, int y, int width, int height);
    void display_bar_chart(QPainter *painter, int x, int y, int width, int height);
    void display_labels(QPainter *painter);
    User *user;
};

#endif // STATISTICSPROVIDER_H
