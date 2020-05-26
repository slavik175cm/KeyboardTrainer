#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>
#include <QtMath>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->custom_text->hide();
    ui->length_slider->hide();

    ui->text->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
    ui->text->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->spm->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
    ui->spm->setText("<p align=\"center\"><font color=#BBA8AD>-");

    ui->wrong_symbols->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
    ui->wrong_symbols->setText("<p align=\"center\"><font color=#FF4500>-");
    text_generator = new TextGenerator();

    QGraphicsScene *Scene = new QGraphicsScene(this);
    Scene->setSceneRect(ui->keyboard_graphicsView->x(), ui->keyboard_graphicsView->y(),
                        ui->keyboard_graphicsView->width() - 50, ui->keyboard_graphicsView->height() - 50);
    ui->keyboard_graphicsView->setScene(Scene);
    keyboard = new Keyboard(Scene, ui->keyboard_graphicsView->x(), ui->keyboard_graphicsView->y());
    Scene->addItem(keyboard);

    QGraphicsScene *statistics_scene = new QGraphicsScene();
    statistics_scene->setSceneRect(ui->graphics_view_for_statistics->x(), ui->graphics_view_for_statistics->y(),
                        ui->graphics_view_for_statistics->width() - 50, ui->graphics_view_for_statistics->height() - 50);
    ui->graphics_view_for_statistics->setScene(statistics_scene);

    database = new DataBase();
    user = new User(database);
    form = new Form(this, database, user);
    form->move(1920 / 2 - form->width() / 2, 1080 / 2 - form->height() / 2);
    form->setFixedSize(form->size());

    statistics_provider = new StatisticsProvider(statistics_scene, user);
    statistics_scene->addItem(statistics_provider);
    statistics_scene->update();

    text_provider = new TextProvider(ui->text, ui->spm, ui->wrong_symbols, user);
    on_length_slider_valueChanged(100);
    ui->length_slider->setValue(100);
    ui->random_text_button->setChecked(true);
    on_random_text_button_clicked();

    ui->main_stackedWidget->setCurrentIndex(0);
    settings_bar = new SettingsBar(this, text_provider, statistics_provider, ui->main_stackedWidget, form, user);
    ui->stackedWidget->insertWidget(1, settings_bar);
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget->setFocusPolicy(Qt::StrongFocus);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) return;
    keyboard->release_key(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    keyboard->press_key(event);
    text_provider->input_letter(event);

}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (this->focusWidget() != NULL)
        this->focusWidget()->clearFocus();
}

void MainWindow::on_custom_text_textChanged() {
    if (ui->custom_text->toPlainText().size() == 0) {
        QMessageBox::warning(NULL, "error", "это поле не должно быть пустым");
        return;
    }
    text_provider->restart(ui->custom_text->toPlainText());
}


void MainWindow::on_length_slider_valueChanged(int value) {
    text_provider->restart(text_generator->generate_random_text(value));
}

void MainWindow::on_custom_text_button_clicked() {
    ui->custom_text->show();
    ui->length_slider->hide();
    on_custom_text_textChanged();
    text_provider->random_mode = 0;
}

void MainWindow::on_random_text_button_clicked() {
    ui->length_slider->show();
    ui->custom_text->hide();
    on_length_slider_valueChanged(ui->length_slider->value());
    text_provider->random_mode = 1;
}

void MainWindow::on_user_changed() {
    ui->length_slider->setValue(100);
    on_random_text_button_clicked();
    ui->random_text_button->setChecked(true);
    ui->custom_text_button->setChecked(false);
    ui->spm->setText("<p align=\"center\"><font color=#BBA8AD>-");
    ui->wrong_symbols->setText("<p align=\"center\"><font color=#FF4500>-");
    settings_bar->on_user_changed();
}
