#ifndef SETTINGSBAR_H
#define SETTINGSBAR_H
#include <QWidget>
#include <mainwindow.h>
#include <textprovider.h>
#include <statisticsprovider.h>
#include <QStackedWidget>
#include "form.h"
#include "user.h"
namespace Ui {
class SettingsBar;
}
class MainWindow;
class StatisticsProvider;
class Form;
class SettingsBar : public QWidget
{
    Q_OBJECT

public:
    SettingsBar(MainWindow *mainwindow, TextProvider *text_provider, StatisticsProvider *statistics_provider,
                QStackedWidget *main_stackedWidget, Form *form, User *user, QWidget *parent = nullptr);
    ~SettingsBar();
    void on_user_changed();

private slots:

    void on_settings_button_clicked();

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_font_size_comboBox_currentIndexChanged(const QString &arg1);

    void on_statistics_button_clicked();

    void on_log_in_button_clicked();

    void on_sign_up_button_clicked();

    void on_log_out_button_clicked();

    void on_information_button_clicked();

private:
    MainWindow *mainwindow;
    TextProvider *text_provider;
    StatisticsProvider *statistics_provider;
    QStackedWidget *main_stackedWidget;
    Form *form;
    User *user;
    Ui::SettingsBar *ui;
};

#endif // SETTINGSBAR_H
