#ifndef KALISET_H
#define KALISET_H

#include <QVector3D>
#include <QImage>

class KaliSet
{
public:

    // ------- types ---------

    typedef float Float;
    typedef QVector3D vec3;

    static vec3 abs(const vec3& p)
    { return vec3(std::abs(p.x()), std::abs(p.y()), std::abs(p.z())); }

    static Float dot(const vec3& a, const vec3& b)
    { return vec3::dotProduct(a, b); }

    /** vec3 [0,1] -> QRgb [0,255] */
    static QRgb toColor(const vec3& p)
    { return qRgb(std::max(0, std::min(255, int(p.x() * 255))),
                  std::max(0, std::min(255, int(p.y() * 255))),
                  std::max(0, std::min(255, int(p.z() * 255))) ); }

    // ------- ctor ----------

    KaliSet(const vec3& param = vec3(.5, .4, 1.5), uint iters = 32);
    ~KaliSet();

    // ------- getter --------

    const vec3& param() const { return param_; }

    uint iters() const { return iters_; }

    // ------- setter --------

    void setParam(const vec3& p) { param_ = p; }
    void setIters(uint iters) { iters_ = iters; }

    // ------- calc ----------

    /** Returns the value of p after the last step */
    vec3 value3(const vec3& p) const;
    /** Returns the average value of p */
    vec3 value3_av(const vec3& p) const;


    // ------- image ---------

    /** Plots a 2d view of the set [pos, pos + scale)
        using the vec3 value3(). */
    void plotImage3(QImage &img, const vec3& pos, const Float scale);

    /** Plots a 2d view of the set [pos, pos + scale)
        using the vec3 value3_av(). */
    void plotImage3_av(QImage &img, const vec3& pos, const Float scale);

private:

    vec3 param_;
    uint iters_;
};

#endif // KALISET_H
