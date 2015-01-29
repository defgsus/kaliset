#include <cassert>

#include "renderthread.h"

RenderThread::RenderThread(QObject * o)
    : QThread   (o),
      stop_     (false),
      restart_  (false)
{
    kset_ = kali_.defaultKaliSettings();
    rset_ = kali_.defaultRenderSettings();
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
    if (img.width() != rset_.width
        || img.height() != rset_.height
        || img.format() != QImage::Format_RGB32)
        img = QImage(rset_.width, rset_.height, QImage::Format_RGB32);

    const int s = rset_.height * rset_.height;

    for (uint i = 0; i < buf_.size(); ++i)
        *reinterpret_cast<QRgb*>(&img.bits()[i<<2]) = KaliSet::toColor(buf_[i]);

    // fill rest
    for (int i = buf_.size(); i < s; ++i)
        *reinterpret_cast<QRgb*>(&img.bits()[i<<2]) = 0xff000000;
}


void RenderThread::run()
{
    stop_ = false;

    do
    {
        restart_ = false;
        // update settings of runtime object
        kali_.setKaliSettings(kset_);
        kali_.setRenderSettings(rset_);
        // above part should probably be locked
        // from now on, only use settings in kali_

        buf_.resize(kali_.renderSettings().width * kali_.renderSettings().height);

        vec3 * buf = &buf_[0];
        for (int j = 0; j < kali_.renderSettings().height; ++j)
        for (int i = 0; i < kali_.renderSettings().width; ++i)
        {
            if (stop_ || restart_)
                break;

            *buf++ = kali_.color(i, j);
        }

    }
    while (restart_);
}
