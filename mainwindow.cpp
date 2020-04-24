#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>
#include <QtMath>
#include <QFileDialog>
#include <QGraphicsScene>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->spm->setFocusPolicy(Qt::NoFocus);
    ui->random_text_button->setFocusPolicy(Qt::NoFocus);
    ui->custom_text_button->setFocusPolicy(Qt::NoFocus);
    ui->wrong_symbols->setFocusPolicy(Qt::NoFocus);
    ui->custom_text->setFocusPolicy(Qt::ClickFocus);
    ui->length_slider->setFocusPolicy(Qt::NoFocus);
    ui->custom_text->hide();
    ui->length_slider->hide();
    this->setStyleSheet("background-color: #4D535E;");

    ui->text->setStyleSheet("background-color: #4D535E;");
    ui->text->setFrameStyle(QFrame::NoFrame);
    ui->text->setFont(QFont("Comic Sans MC", 25));
    ui->text->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
    ui->text->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->text->setFocusPolicy(Qt::NoFocus);

    ui->spm->setFont(QFont("Comic Sans MC", 40));
    ui->spm->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
    ui->spm->setFrameStyle(QFrame::NoFrame);
    ui->spm->setText("<p align=\"center\"><font color=#BBA8AD>-");

    ui->wrong_symbols->setFont(QFont("Comic Sans MC", 40));
    ui->wrong_symbols->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
    ui->wrong_symbols->setFrameStyle(QFrame::NoFrame);
    ui->wrong_symbols->setText("<p align=\"center\"><font color=#FF4500>-");
    text_generator = new TextGenerator();

    text_provider = new TextProvider(ui->text, ui->spm, ui->wrong_symbols);
    text_provider->restart(text_generator->generate_random_text(100));
    ui->length_slider->setValue(100);
    startTimer(300);

    QGraphicsScene *Scene = new QGraphicsScene(this);
    Scene->setSceneRect(ui->keyboard_graphicsView->x(), ui->keyboard_graphicsView->y(),
                        ui->keyboard_graphicsView->width() - 50, ui->keyboard_graphicsView->height() - 50);
    ui->keyboard_graphicsView->setScene(Scene);
    ui->keyboard_graphicsView->setFrameStyle(QFrame::NoFrame);
    keyboard = new Keyboard(Scene, ui->keyboard_graphicsView->x(), ui->keyboard_graphicsView->y());
    Scene->addItem(keyboard);
    settings_bar = new SettingsBar();
    ui->stackedWidget->insertWidget(1, settings_bar);
    ui->stackedWidget->setCurrentIndex(1);

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
    if (this->focusWidget() != NULL) this->focusWidget()->clearFocus();
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
}

void MainWindow::on_random_text_button_clicked() {
    ui->length_slider->show();
    ui->custom_text->hide();
}

