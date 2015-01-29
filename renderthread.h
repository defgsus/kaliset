#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <vector>

#include <QThread>
#include <QImage>

#include "kaliset.h"

class RenderThread : public QThread
{
public:

    typedef KaliSet::Float Float;
    typedef KaliSet::vec3 vec3;

    explicit RenderThread(QObject *parent = 0);

    void run() Q_DECL_OVERRIDE;

    /** Quit the thread - blocking */
    void stop();

    // ------------- getter -------------

    /** Received current image data */
    void getImage(QImage& img);

    // ------------- setter -------------

    void setIters(uint iters) { iters_ = iters; restart_ = true; }
    void setParam(const vec3& p) { param_ = p; restart_ = true; }

private:

    KaliSet kali_;
    std::vector<vec3> buf_;
    int w_, h_;

    uint iters_;
    vec3 pos_, param_;

    volatile bool stop_, restart_;
};

#endif // RENDERTHREAD_H
