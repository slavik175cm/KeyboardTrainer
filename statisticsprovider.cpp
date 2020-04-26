#include "statisticsprovider.h"
#include <QGraphicsView>
#include <QDebug>
StatisticsProvider::StatisticsProvider(QGraphicsScene *scene)
{
    this->scene = scene;
    speed.push_back(380);
    speed.push_back(250);
    speed.push_back(228);
    speed.push_back(400);
    for (int i = 0; i < 30; i++)
        speed.push_back(rand() % 600);
}

QRectF StatisticsProvider::boundingRect() const
{
    return QRect(100, 100, 1, 1);
}

void StatisticsProvider::add_new_sample(int current_speed) {
    speed.push_back(current_speed);
}

void StatisticsProvider::display_statistics() {
    if (scene != NULL) scene->update();
}

void StatisticsProvider::display_graph(QPainter *painter, int x, int y, int width, int height) {
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawLine(x, y, x, y - height);
    painter->drawLine(x, y, x + width, y);

    int mx = 0;
    for (int i = 0; i < speed.size(); i++)
        mx = qMax(mx, speed[i]);
    double coeff = (double)height / mx;
    int step = width / (speed.size() - 1);

    double current_y = y, step_y = (double)height / 9;
    for (int i = 0; i < 10; i++) {
        painter->drawLine(x, current_y, x + width, current_y);
        painter->drawText(x - 35, current_y + 5, QString::number((int)((y - current_y) / coeff)));
        current_y -= step_y;
    }

    int current_x = x, how_many = 1, number = qMin(10, speed.size());
    int step_x = width / (number - 1);
    for (int i = 0; i < number; i++) {
        painter->drawLine(current_x, y - 5, current_x, y + 5);
        painter->drawText(current_x - 5, y + 35, QString::number(how_many));
        current_x += step_x;
        while (how_many < speed.size() && x + how_many * (step - 1) <= current_x) how_many++;
    }

    QPen pen(Qt::gray, 5);
    painter->setPen(pen);

    current_x = x;
    QPoint prev(-1, -1);
    for (int i = 0; i < speed.size(); i++) {
        QPoint current(current_x, (y - speed[i] * coeff));
        if (prev.x() != -1) painter->drawLine(prev, current);
        painter->drawEllipse(current, 5, 5);
        prev = current;
        current_x += step;
    }
}

void StatisticsProvider::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    display_graph(painter, 300, 900, 1000, 300);
}
