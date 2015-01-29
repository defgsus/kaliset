#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <vector>

#include <QThread>
#include <QImage>

#include "kaliset.h"

class RenderThread : public QThread
{
public:

    // ----------- types ----------------

    typedef KaliSet::Float Float;
    typedef KaliSet::vec3 vec3;

    struct Settings
    {
        uint numIters;
        vec3 param;
        vec3 pos;
        Float scale;
    };

    // ------------- ctor ---------------

    explicit RenderThread(QObject *parent = 0);

    // -------- thread interface --------

    void run() Q_DECL_OVERRIDE;

    /** Quit the thread - blocking */
    void stop();

    // ------------- getter -------------

    const Settings& settings() const { return set_; }

    /** Received current image data */
    void getImage(QImage& img);

    // ------------- setter -------------

    void setSettings(const Settings& s) { set_ = s; restart_ = true; }

private:

    KaliSet kali_;
    std::vector<vec3> buf_;
    int w_, h_;

    Settings set_;

    volatile bool stop_, restart_, running_;
};

#endif // RENDERTHREAD_H
