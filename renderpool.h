/** @file renderpool.h

    @brief

    <p>(c) 2015, stefan.berke@modular-audio-graphics.com</p>
    <p>All rights reserved</p>

    <p>created 29.01.2015</p>
*/

#ifndef RENDERPOOL_H
#define RENDERPOOL_H

#include <atomic>

#include <QObject>
#include <QMutex>

#include "kaliset.h"

/** Encapsulates multiple worker threads */
class RenderPool : public QObject
{
    Q_OBJECT
public:
    // ------------- ctor ---------------

    explicit RenderPool(int numThreads, QObject *parent = 0);
    ~RenderPool();

    // -------- thread interface --------

    void start();

    /** Quit the threads - block till end */
    void stop();

    // ------------- getter -------------

    const KaliSet::KaliSettings& kaliSettings() const { return kset_; }
    const KaliSet::RenderSettings& renderSettings() const { return rset_; }

    /** Receive current image data.
        Not thread-locked, some pixels might by crap */
    void getImage(QImage& img);

    // ------------- setter -------------

    // Threadsafe

    void setKaliSettings(const KaliSet::KaliSettings& s);
    void setRenderSettings(const KaliSet::RenderSettings& s);

signals:

    void finished();

private slots:

    void onFinished_();

private:

    void createBuffers_();

    class Worker;
    struct Block;

    KaliSet kali_;
    //std::vector<KaliSet::vec3> buf_;
    std::vector<Worker*> worker_;
    std::vector<Block*> blocks_;
    QMutex * mutex_;
    int curBlock_;
    std::atomic_int runningThreads_;

    KaliSet::KaliSettings kset_;
    KaliSet::RenderSettings rset_;

    volatile bool stop_, restart_;

};

#endif // RENDERPOOL_H
