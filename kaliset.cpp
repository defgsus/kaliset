#include <cassert>

#include "kaliset.h"

KaliSet::KaliSet(const vec3 &param, uint iters)
    : param_    (param)
    , iters_    (iters)
{

}

KaliSet::~KaliSet()
{

}


KaliSet::vec3 KaliSet::value3(const vec3 &pos) const
{
    vec3 p(pos);

    for (uint i=1; i<iters_; ++i)
        p = abs(p) / dot(p, p) - param_;

    return abs(p) / dot(p, p);
}

KaliSet::vec3 KaliSet::value3_av(const vec3 &pos) const
{
    vec3 p(pos), av(0,0,0);

    for (uint i=1; i<iters_; ++i)
    {
        p = abs(p) / dot(p, p);
        av += p;
        p -= param_;
    }

    return av / iters_;
}

KaliSet::vec3 KaliSet::trace3(const vec3 &ro, const vec3 &rd, Float stepSize) const
{
    vec3 col = vec3(0,0,0);
    Float t = 0.0;
    for (int i=0; i<60; ++i)
    {
        vec3 p = ro + rd * t;

        vec3 k = value3(p);

        Float d = dot(k, k);
        d = Float(1) - std::abs(Float(1) - d);

        t += std::max(Float(0.0001), std::min(Float(0.01), d )) * stepSize;

        col += k / (Float(1) + Float(100) * d * d);
    }

    return col / 10.;
}

void KaliSet::plotImage3(QImage &img, const vec3 &pos, const Float scale)
{
    assert(img.format() == QImage::Format_RGB32);

    for (int y = 0; y < img.height(); ++y)
    for (int x = 0; x < img.width(); ++x)
    {
        vec3 p = vec3(pos.x() + Float(x) / img.width() * scale,
                      pos.y() + Float(y) / img.height() * scale,
                      pos.z());
        vec3 k = value3(p);

        img.setPixel(x, y, toColor(k) );
    }
}

void KaliSet::plotImage3_av(QImage &img, const vec3 &pos, const Float scale)
{
    assert(img.format() == QImage::Format_RGB32);

    for (int y = 0; y < img.height(); ++y)
    for (int x = 0; x < img.width(); ++x)
    {
        vec3 p = vec3(pos.x() + Float(x) / img.width() * scale,
                      pos.y() + Float(y) / img.height() * scale,
                      pos.z());
        vec3 k = value3_av(p);

        img.setPixel(x, y, toColor(k) );
    }
}
