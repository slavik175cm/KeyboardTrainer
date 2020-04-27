#ifndef STATISTICSPROVIDER_H
#define STATISTICSPROVIDER_H
#include <QVector>
#include <QGraphicsScene>
#include <QGraphicsItem>
class StatisticsProvider : public QGraphicsItem
{
public:
    StatisticsProvider(QGraphicsScene *scene);
    void add_new_sample(int current_speed, QVector<int> letter_pressed_count, QVector<int> letter_mistakes);
    void display_statistics();
private:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QGraphicsScene *scene;
    void display_graph(QPainter *painter, int x, int y, int width, int height);
    void display_bar_chart(QPainter *painter, int x, int y, int width, int height);
    void display_labels(QPainter *painter);
    QVector<int> speed;
    QVector<int> letter_pressed_count;
    QVector<int> letter_mistakes;
};

#endif // STATISTICSPROVIDER_H
