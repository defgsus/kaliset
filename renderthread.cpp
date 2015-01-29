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
}

/*RenderThread::~RenderThread()
{

}*/

void RenderThread::stop()
{
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

    while (!stop_)
    {
again_:
        restart_ = false;
        kali_.setIters(iters_);
        kali_.setParam(param_);

        for (int j = 0; j < h_; ++j)
        for (int i = 0; i < w_; ++i)
        {
            if (stop_ || restart_)
                break;

            Float x = Float(i) / w_,
                  y = Float(y) / h_;

            buf_[j*w_ + i] = kali_.value3(vec3(x, y, 0.));
        }


        if (restart_)
            goto again_;
    }
}
