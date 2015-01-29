#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QPixmap>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTimer>

#include "kaliview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    typedef KaliSet::Float Float;
    typedef KaliSet::vec3 vec3;

    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    void createWidgets_();

    std::vector<KaliView*> kviews_;

};

#endif // MAINWINDOW_H
