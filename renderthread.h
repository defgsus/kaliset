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

    // ------------- ctor ---------------

    explicit RenderThread(QObject *parent = 0);

    // -------- thread interface --------

    void run() Q_DECL_OVERRIDE;

    /** Quit the thread - block till end */
    void stop();

    // ------------- getter -------------

    const KaliSet::KaliSettings& kaliSettings() const { return kset_; }
    const KaliSet::RenderSettings& renderSettings() const { return rset_; }

    /** Receive current image data.
        Not thread-locked, some pixels might by crap */
    void getImage(QImage& img);

    // ------------- setter -------------

    // The setters que a restart of the rendering when it's currently running

    void setKaliSettings(const KaliSet::KaliSettings& s) { kset_ = s; restart_ = true; }
    void setRenderSettings(const KaliSet::RenderSettings& s) { rset_ = s; restart_ = true; }

private:

    KaliSet kali_;
    std::vector<vec3> buf_;

    KaliSet::KaliSettings kset_;
    KaliSet::RenderSettings rset_;

    volatile bool stop_, restart_;
};

#endif // RENDERTHREAD_H
