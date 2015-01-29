#include <QDebug>

#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow   (parent),
      img2_         (256, 256, QImage::Format_RGB32),
      img2s_        (256, 256, QImage::Format_RGB32),
      ignorePosChanged_(false)
{
    createWidgets_();

    connect(&thread2_, SIGNAL(finished()),
            this, SLOT(onThread2Finished_()));

    updateBase();
}

MainWindow::~MainWindow()
{
    thread2_.stop();
}



void MainWindow::createWidgets_()
{
    auto w = new QWidget();
    setCentralWidget(w);

    auto lh = new QHBoxLayout(w);

        auto lv = new QVBoxLayout();
        lh->addLayout(lv);

            view2_ = new ImageView(w);
            lv->addWidget(view2_);
            connect(view2_, SIGNAL(clicked(double,double)),
                    this, SLOT(onClicked2_(double,double)));

            sbIter_ = new QSpinBox(w);
            sbIter_->setRange(2, 10000);
            sbIter_->setValue(kali_.iters());
            lv->addWidget(sbIter_);
            connect(sbIter_, SIGNAL(valueChanged(int)),
                    this, SLOT(onParamChanged_()));

            sbPX_ = new QDoubleSpinBox(w);
            sbPX_->setRange(-10000, 10000);
            sbPX_->setDecimals(5);
            sbPX_->setSingleStep(0.01);
            sbPX_->setValue(kali_.param().x());
            lv->addWidget(sbPX_);
            connect(sbPX_, SIGNAL(valueChanged(double)),
                    this, SLOT(onParamChanged_()));

            sbPY_ = new QDoubleSpinBox(w);
            sbPY_->setRange(-10000, 10000);
            sbPY_->setDecimals(5);
            sbPY_->setSingleStep(0.01);
            sbPY_->setValue(kali_.param().y());
            lv->addWidget(sbPY_);
            connect(sbPY_, SIGNAL(valueChanged(double)),
                    this, SLOT(onParamChanged_()));

            sbPZ_ = new QDoubleSpinBox(w);
            sbPZ_->setRange(-10000, 10000);
            sbPZ_->setDecimals(5);
            sbPZ_->setSingleStep(0.01);
            sbPZ_->setValue(kali_.param().z());
            lv->addWidget(sbPZ_);
            connect(sbPZ_, SIGNAL(valueChanged(double)),
                    this, SLOT(onParamChanged_()));

            lv->addStretch(1);

        lv = new QVBoxLayout();
        lh->addLayout(lv);

            view2s_ = new ImageView(w);
            lv->addWidget(view2s_);

            sbX_ = new QDoubleSpinBox(w);
            sbX_->setRange(-10000, 10000);
            sbX_->setDecimals(5);
            sbX_->setSingleStep(0.01);
            lv->addWidget(sbX_);
            connect(sbX_, SIGNAL(valueChanged(double)),
                    this, SLOT(onPosChanged_()));

            sbY_ = new QDoubleSpinBox(w);
            sbY_->setRange(-10000, 10000);
            sbY_->setDecimals(5);
            sbY_->setSingleStep(0.01);
            lv->addWidget(sbY_);
            connect(sbY_, SIGNAL(valueChanged(double)),
                    this, SLOT(onPosChanged_()));

            sbZ_ = new QDoubleSpinBox(w);
            sbZ_->setRange(-10000, 10000);
            sbZ_->setDecimals(5);
            sbZ_->setSingleStep(0.01);
            lv->addWidget(sbZ_);
            connect(sbZ_, SIGNAL(valueChanged(double)),
                    this, SLOT(onPosChanged_()));

            lv->addStretch(1);

}

void MainWindow::onClicked2_(double x, double y)
{
    pos_ = vec3(x, y, 0.);

    ignorePosChanged_ = true;
    sbX_->setValue(pos_.x());
    sbY_->setValue(pos_.y());
    sbZ_->setValue(pos_.z());
    ignorePosChanged_ = false;

    updateAll();
}

void MainWindow::onPosChanged_()
{
    if (ignorePosChanged_)
        return;

    pos_ = vec3(sbX_->value(), sbY_->value(), sbZ_->value());

    updateAll();
}

void MainWindow::onParamChanged_()
{
    kali_.setIters(sbIter_->value());
    kali_.setParam(KaliSet::vec3(sbPX_->value(), sbPY_->value(), sbPZ_->value()));

    updateBase();
    updateAll();
}

void MainWindow::onThread2Finished_()
{
    thread2_.getImage(img2s_);
    view2s_->setImage(img2s_);
}

void MainWindow::updateBase()
{
    kali_.plotImage3(img2_, vec3(0., 0., 0.), 1.);
    view2_->setImage(img2_);
}


void MainWindow::updateAll()
{
    startThread_(thread2_);
}


void MainWindow::getSettings_(RenderThread::Settings& set)
{
    set.numIters = sbIter_->value();
    set.param = KaliSet::vec3(sbPX_->value(), sbPY_->value(), sbPZ_->value());
    set.pos = KaliSet::vec3(sbX_->value(), sbY_->value(), sbZ_->value());
}

void MainWindow::startThread_(RenderThread& t)
{
    RenderThread::Settings set = t.settings();
    getSettings_(set);
    t.setSettings(set);

    if (!t.isRunning())
        t.start();
}

