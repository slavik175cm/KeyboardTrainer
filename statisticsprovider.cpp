#include "statisticsprovider.h"
#include <QGraphicsView>
#include <QDebug>
#include <QtMath>
StatisticsProvider::StatisticsProvider(QGraphicsScene *scene, User *user)
{
    this->scene = scene;
    this->user = user;
}

QRectF StatisticsProvider::boundingRect() const
{
    return QRect(100, 100, 1, 1);
}

void StatisticsProvider::display_statistics() {
    if (scene != NULL) scene->update();
}

void StatisticsProvider::display_graph(QPainter *painter, int x, int y, int width, int height) {
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setPen(Qt::PenStyle::CustomDashLine);
    painter->drawLine(x, y, x, y - height);
    painter->drawLine(x, y, x + width, y);

    int mx = 0;
    for (int i = 0; i < user->speed.size(); i++)
        mx = qMax(mx, user->speed[i]);
    double coeff = (double)height / mx;
    int step = width / qMax(user->speed.size() - 1, 1);

    double current_y = y, step_y = (double)height / 9;
    for (int i = 0; i < 10; i++) {
        painter->drawLine(x, current_y, x + width, current_y);
        painter->drawText(x - 35, current_y + 5, QString::number((int)(ceil((y - current_y) / coeff))));
        current_y -= step_y;
    }

    int current_x = x, how_many = 1, number = qMin(10, user->speed.size());
    int step_x = width / qMax(number - 1, 1);
    for (int i = 0; i < number; i++) {
        painter->drawLine(current_x, y - 5, current_x, y + 5);
        painter->drawText(current_x - 5, y + 35, QString::number(how_many));
        current_x += step_x;
        while (how_many < user->speed.size() && x + how_many * (step - 1) <= current_x) how_many++;
    }

    QPen pen(Qt::gray, 5);
    painter->setPen(pen);

    current_x = x;
    QPoint prev(-1, -1);
    for (int i = 0; i < user->speed.size(); i++) {
        QPoint current(current_x, (y - user->speed[i] * coeff));
        if (prev.x() != -1) painter->drawLine(prev, current);
        painter->drawEllipse(current, 5, 5);
        prev = current;
        current_x += step;
    }
}

void StatisticsProvider::display_bar_chart(QPainter *painter, int x, int y, int width, int height) {
    //painter->setFont(QFont("Comic Sans MC", 15));
    painter->setPen(Qt::PenStyle::CustomDashLine);
    painter->drawLine(x, y, x + width, y);
    int step = width / 25;
    const int bar_width = 15;
    int current_x = x ;
    for (int i = 0; i < 26; i++) {
        painter->drawRect(current_x - bar_width, y - height, bar_width * 2, height);
        double coeff = (double)user->letter_mistakes[i] / qMax(user->letter_pressed_count[i], 1);
        painter->fillRect(current_x - bar_width + 1, qCeil(y - height * (1 - coeff)) + 1, bar_width * 2 - 1, height * (1 - coeff) - 1, Qt::darkGreen);
        //painter->drawText(current_x - 10, y - height / 2, QString::number(letter_mistakes[i]) + '/');
        //painter->drawText(current_x - 10, y - height / 2 + 15, QString::number(letter_pressed_count[i]));
        //QPen pen(Qt::gray); painter->setPen(pen);
        painter->drawText(current_x - 10, y + 20, QString::number((int)((1 - coeff) * 100)) + '%');
        painter->drawText(current_x - 5, y + 40, QString('A' + i));
        //    painter->setPen(Qt::PenStyle::CustomDashLine);
        current_x += step;
    }
}

void StatisticsProvider::display_labels(QPainter *painter) {
    QPen pen(Qt::gray);
    painter->setFont(QFont("Comic Sans MC", 40));
    painter->setPen(pen);

    int sum = 0;
    for (int i = 0; i < user->speed.size(); i++)
        sum += user->speed[i];
    painter->drawText(100, 100, "Average speed");
    painter->drawText(230, 180, QString::number(sum / qMax(user->speed.size(), 1)));

    int mx = 0;
    for (int i = 0; i < user->speed.size(); i++)
        mx = qMax(mx, user->speed[i]);
    painter->drawText(660, 100, "Max speed");
    painter->drawText(750, 180, QString::number(mx));

    painter->drawText(1100, 100, "Total samples");
    painter->drawText(1240, 180, QString::number(user->speed.size()));

}

void StatisticsProvider::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    display_graph(painter, 300, 600, 1000, 300);
    display_bar_chart(painter, 150, 900, 1300, 200);
    display_labels(painter);
}
