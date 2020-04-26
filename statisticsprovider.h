#ifndef STATISTICSPROVIDER_H
#define STATISTICSPROVIDER_H
#include <QVector>
#include <QGraphicsScene>
#include <QGraphicsItem>
class StatisticsProvider : public QGraphicsItem
{
public:
    StatisticsProvider(QGraphicsScene *scene);
    void add_new_sample(int current_speed);
    void display_statistics();
private:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QGraphicsScene *scene;
    void display_graph(QPainter *painter, int x, int y, int width, int height);
    QVector<int> speed;
};

#endif // STATISTICSPROVIDER_H
