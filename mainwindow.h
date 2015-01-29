#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QPixmap>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>

#include "renderthread.h"
#include "imageview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    typedef KaliSet::Float Float;
    typedef KaliSet::vec3 vec3;

    MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:

    void calc2();
    void calc2s();

private slots:

    void onClicked2_(double x, double y);
    void onPosChanged_();
    void onParamChanged_();

private:

    void createWidgets_();

    KaliSet kali_;
    vec3 pos_;

    QImage img2_, img2s_;

    ImageView * view2_, * view2s_;

    bool ignorePosChanged_;
    QSpinBox * sbIter_;
    QDoubleSpinBox
        * sbPX_, * sbPY_, * sbPZ_,
        * sbX_, * sbY_, * sbZ_;
};

#endif // MAINWINDOW_H
