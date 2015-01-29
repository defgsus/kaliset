/** @file renderpool.cpp

    @brief

    <p>(c) 2015, stefan.berke@modular-audio-graphics.com</p>
    <p>All rights reserved</p>

    <p>created 29.01.2015</p>
*/

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>

#include "renderpool.h"

#define K_DEBUG(arg__) { qDebug() << arg__; }

struct RenderPool::Block
{
    KaliSet * kali;
    int x, y, x2, y2;
    std::vector<KaliSet::vec3> buffer;
};


class RenderPool::Worker : public QThread
{
public:

    Worker(RenderPool * p) : pool(p) { }

    RenderPool * pool;
    volatile bool stop;
    Block * block;

    void run() Q_DECL_OVERRIDE
    {
        K_DEBUG("Thread::run()");

        stop = false;
        while (!stop)
        {
            Block * b = 0;

            // fetch new work
            pool->mutex_->lock();
            if (pool->curBlock_ < (int)pool->blocks_.size())
                b = pool->blocks_[pool->curBlock_++];
            pool->mutex_->unlock();

            if (b)
                renderBlock(b);
            else
                break;
        }

        K_DEBUG("Thread::run() end");
    }

    void renderBlock(Block * b)
    {
        KaliSet::vec3 * buf = &b->buffer[0];

        for (int j = b->y; j < b->y2 && !stop; ++j)
            for (int i = b->x; i < b->x2 && !stop; ++i)
                *buf++ = b->kali->color(i, j);
    }
};




RenderPool::RenderPool(int numThreads, QObject *parent)
    : QObject           (parent),
      runningThreads_   (0)
{
    kset_ = kali_.kaliSettings();
    rset_ = kali_.renderSettings();
    createBuffers_();

    mutex_ = new QMutex();
    for (int i=0; i<numThreads; ++i)
        worker_.push_back(new Worker(this));

    for (Worker * w : worker_)
        connect(w, SIGNAL(finished()), this, SLOT(onFinished_()));
}

RenderPool::~RenderPool()
{
    stop();
    for (auto b : blocks_)
        delete b;
    delete mutex_;
}


void RenderPool::start()
{
    K_DEBUG("Pool::start()");

    mutex_->lock();
    curBlock_ = 0;
    kali_.setKaliSettings(kset_);
    kali_.setRenderSettings(rset_);
    mutex_->unlock();

    for (Worker * w : worker_)
        if (!w->isRunning())
        {
            w->start();
            runningThreads_++;
        }

}

void RenderPool::stop()
{
    K_DEBUG("Pool::stop()");

    for (Worker * w : worker_)
        w->stop = true;
    for (Worker * w : worker_)
        w->wait();

    runningThreads_ = 0;
}

void RenderPool::onFinished_()
{
    K_DEBUG("Pool::onFinished");

    bool fin = --runningThreads_ < 1;

    if (fin)
        emit finished();
}

void RenderPool::setKaliSettings(const KaliSet::KaliSettings &s)
{
    stop();
    kset_ = s;
}

void RenderPool::setRenderSettings(const KaliSet::RenderSettings &s)
{
    stop();
    bool changed = s.width != rset_.width || s.height != rset_.height;
    rset_ = s;
    if (changed)
        createBuffers_();
}

/** Create all necessary calculation space.
    Not locked */
void RenderPool::createBuffers_()
{
    // clear previous blocks
    for (auto b : blocks_)
        delete b;
    blocks_.clear();

    // create the blocks
    int bw = 32, bh = bw;
        //numx = rset_.width / bw,
        //numy = rset_.height / bh;
    for (int y = 0; y < rset_.height; y += bh)
    {
        int h = bh;
        if (y + h > rset_.height)
            h = rset_.height - y;

        for (int x = 0; x < rset_.width; x += bw)
        {
            int w = bw;
            if (x + w > rset_.width)
                w = rset_.width - x;

            auto b = new Block();
            b->x = x;
            b->y = y;
            b->x2 = x + w;
            b->y2 = y + h;
            b->buffer.resize(w * h);
            b->kali = &kali_;

            blocks_.push_back(b);
        }
    }

}

void RenderPool::getImage(QImage &img)
{
    if (img.width() != rset_.width
        || img.height() != rset_.height
        || img.format() != QImage::Format_RGB32)
        img = QImage(rset_.width, rset_.height, QImage::Format_RGB32);

    for (const Block * b : blocks_)
    {
        int w = b->x2 - b->x;
        for (int y=b->y; y < b->y2; ++y)
        for (int x=b->x; x < b->x2; ++x)
            img.setPixel(x, y, KaliSet::toColor(b->buffer[(y-b->y) * w + x - b->x]));
    }
}
