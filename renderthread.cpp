#include <cassert>

#include "renderthread.h"

RenderThread::RenderThread(QObject * o)
    : QThread   (o),
      stop_     (false),
      restart_  (false)
{
    w_ = 256;
    h_ = 256;
    buf_.resize(w_ * h_);

    set_.numIters = 32;
    set_.param = vec3(.4, .5, 1.5);
    set_.pos = vec3(0, 0, 0);
    set_.scale = 0.03;

    set_.volumeTrace = false;
    set_.volumeStep = 0.001;
}

/*RenderThread::~RenderThread()
{

}*/

void RenderThread::stop()
{
    if (!isRunning() || stop_)
        return;

    stop_ = true;
    wait();
}


void RenderThread::getImage(QImage &img)
{
    assert(img.format() == QImage::Format_RGB32
           && img.width() == w_
           && img.height() == h_);

    for (int j = 0; j < h_; ++j)
    for (int i = 0; i < w_; ++i)
        img.setPixel(i, j, KaliSet::toColor(buf_[j*w_+i]));
}


void RenderThread::run()
{
    stop_ = false;

again_:
    restart_ = false;
    kali_.setIters(set_.numIters);
    kali_.setParam(set_.param);

    if (!set_.volumeTrace)
    {
        for (int j = 0; j < h_; ++j)
        for (int i = 0; i < w_; ++i)
        {
            if (stop_ || restart_)
                break;

            Float x = Float(i) / w_ * set_.scale,
                  y = Float(j) / h_ * set_.scale;

            buf_[j*w_ + i] = kali_.value3(vec3(x, y, 0.) + set_.pos);
        }
    }
    else
    {
        for (int j = 0; j < h_; ++j)
        for (int i = 0; i < w_; ++i)
        {
            if (stop_ || restart_)
                break;

            Float x = Float(i) / w_ * set_.scale,
                  y = Float(j) / h_ * set_.scale;

            vec3 dir = vec3(x-.5, y-.5, 1.);
            dir.normalize();

            buf_[j*w_ + i] = kali_.trace3(set_.pos, dir, set_.volumeStep);
        }
    }

    if (restart_)
        goto again_;
}
