#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QPixmap>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTimer>

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

    /** Rerenders the base (first) view */
    void updateBase();
    /** Starts threads for all other views */
    void updateAll();

private slots:

    void onClicked2_(double x, double y);
    void onPosChanged_();
    void onParamChanged_();
    void onThread2Finished_();
    void onThread3Finished_();
    void onTimer_();

private:

    void createWidgets_();

    /** Overwrite the settings in @p set with current widget values */
    void getSettings_(RenderThread::Settings& set);
    void startThread_(RenderThread& t);

    RenderThread thread2_, thread3_;
    QTimer timer_;
    KaliSet kali_;
    vec3 pos_;

    QImage img2_, img2s_, img3_;

    ImageView * view2_, * view2s_, * view3_;

    bool ignorePosChanged_;
    QSpinBox * sbIter_;
    QDoubleSpinBox
        * sbPX_, * sbPY_, * sbPZ_,
        * sbX_, * sbY_, * sbZ_,
        * sbStep_;
};

#endif // MAINWINDOW_H
