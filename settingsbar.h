#ifndef SETTINGSBAR_H
#define SETTINGSBAR_H

#include <QWidget>

namespace Ui {
class SettingsBar;
}

class SettingsBar : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsBar(QWidget *parent = nullptr);
    ~SettingsBar();

private slots:

private:
    Ui::SettingsBar *ui;
};

#endif // SETTINGSBAR_H
