#include <QDebug>
#include <QThread>

#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow   (parent)
#if 0
      img2_         (256, 256, QImage::Format_RGB32),
      img2s_        (256, 256, QImage::Format_RGB32),
      img3_         (256, 256, QImage::Format_RGB32),
      ignorePosChanged_(false)
#endif
{
    setWindowTitle(tr("KaliSet Explorer"));
    createWidgets_();
#if 0

    auto s = thread3_.settings();
    s.volumeTrace = true;
    s.volumeStep = 0.01;
    thread3_.setSettings(s);

    connect(&thread2_, SIGNAL(finished()),
            this, SLOT(onThread2Finished_()));
    connect(&thread3_, SIGNAL(finished()),
            this, SLOT(onThread3Finished_()));
    connect(&timer_, SIGNAL(timeout()),
            this, SLOT(onTimer_()));
    timer_.setInterval(500);

    updateBase();
#endif
}

MainWindow::~MainWindow()
{
    //timer_.stop();
    //thread3_.stop();
    //thread2_.stop();
}


void MainWindow::createWidgets_()
{
    auto w = new QWidget();
    setCentralWidget(w);

    auto lh = new QHBoxLayout(w);

        // create some viewer
        for (int i=0; i<3; ++i)
        {
            auto lv = new QVBoxLayout();
            lh->addLayout(lv);

            int numThreads = i == 2 ? QThread::idealThreadCount() : 1;

            auto kview = new KaliView(i == 0, true, numThreads, this);
            kview->setObjectName(QString("_KaliView%1").arg(i+1));
            lv->addWidget(kview);

            lv->addStretch(1);

            kview->startRender();

            kviews_.push_back(kview);
        }


        // propagate 1st kali settings to other views
        connect(kviews_[0], &KaliView::kaliSettingsChanged, [=]()
        {
            auto kset = kviews_[0]->kaliSettings();

            for (uint i=1; i<kviews_.size(); ++i)
                kviews_[i]->setKaliSettings(kset);
        });

        // send click/drag position to either next views
        for (uint j=0; j<kviews_.size() - 1; ++j)
        {
            connect(kviews_[j], &KaliView::viewClicked, [=](double x, double y)
            {
                for (uint i=j+1; i<kviews_.size(); ++i)
                {
                    auto kset = kviews_[i]->kaliSettings();
                    kset.pos = KaliSet::vec3(x, y, kset.pos.z());
                    kviews_[i]->setKaliSettings(kset);
                }
            });
        }
}

