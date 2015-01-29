#include <cassert>

#include "kaliset.h"

const QStringList KaliSet::RenderModeName =
{
    "plot 2D",
    "volume trace"
};

KaliSet::KaliSet()
    : kset_ (defaultKaliSettings()),
      rset_ (defaultRenderSettings())
{

}

KaliSet::KaliSet(const KaliSettings & k, const RenderSettings& r)
    : kset_ (k),
      rset_ (r)
{

}

KaliSet::~KaliSet()
{

}


KaliSet::KaliSettings KaliSet::defaultKaliSettings() const
{
    KaliSettings s;
    s.numIters = 32;
    s.param = vec3(0.4, 0.5, 1.5);
    s.pos = vec3(0, 0, 0);
    return s;
}

KaliSet::RenderSettings KaliSet::defaultRenderSettings() const
{
    RenderSettings s;
    s.width = s.height = 512;//384;
    s.scale = vec2(1., 1.);
    s.mode = RM_PLOT_2D;
    s.volumeStep = 0.1;
    s.volumeMin = 0.001;
    s.volumeMax = 0.01;
    s.bright = 1.;
    return s;
}



KaliSet::vec3 KaliSet::value3(const vec3 &pos) const
{
    vec3 p(pos);

    for (uint i=1; i<kset_.numIters; ++i)
        p = abs(p) / dot(p, p) - kset_.param;

    return abs(p) / dot(p, p);
}

KaliSet::vec3 KaliSet::value3_av(const vec3 &pos) const
{
    vec3 p(pos), av(0,0,0);

    for (uint i=0; i<kset_.numIters; ++i)
    {
        p = abs(p) / dot(p, p);
        av += p;
        p -= kset_.param;
    }

    return av / kset_.numIters;
}

KaliSet::vec3 KaliSet::getRayDir(double x, double y) const
{
    vec3 d = vec3(x, y, 2.);
    d.normalize();
    return d;
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

        t += std::max(Float(rset_.volumeMin), std::min(Float(rset_.volumeMax), d )) * stepSize;

        col += k / (Float(1) + Float(rset_.bright * 100.) * d * d);
    }

    return col / 10.;
}

KaliSet::vec3 KaliSet::color(int i, int j) const
{
    Float y = Float(rset_.height - 1 - j) / rset_.height * Float(2) - Float(1),
          x = Float(i) / rset_.width * Float(2) - Float(1);

    vec3 pos = kset_.pos + vec3(x * rset_.scale.x(), y * rset_.scale.y(), 0.);

    switch (rset_.mode)
    {
        default:
        case RM_PLOT_2D:
            return value3( pos );

        case RM_VOLUME:
            return trace3( pos, getRayDir(x, y), rset_.volumeStep);
    }
}


QImage KaliSet::getNewImage() const
{
    return QImage(rset_.width, rset_.height, QImage::Format_RGB32);
}



void KaliSet::plotImage(QImage &img, int ox, int oy, int w, int h)
{
    assert(ox >= 0 && ox < rset_.width);
    assert(oy >= 0 && oy < rset_.height);
    assert(w > 0 && w < rset_.width - ox);
    assert(h > 0 && h < rset_.height - oy);

    // correct image type
    if (img.width() != rset_.width
        || img.height() != rset_.height
        || img.format() != QImage::Format_RGB32)
        img = getNewImage();

    for (int j = oy; j < h; ++j)
    {
        for (int i = ox; i < w; ++i)
        {
            img.setPixel(i, j, toColor(color(i, j)));
        }
    }
}



#if 0
void KaliSet::plotImage3(QImage &img, const vec3 &pos, const Float scale)
{
    // correct image type
    if (img.width() != rset_.width
        || img.height() != rset_.height
        || img.format() != QImage::Format_RGB32)
        img = getNewImage();

    for (int y = 0; y < rset_.height; ++y)
    for (int x = 0; x < rset_.width; ++x)
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
    // correct image type
    if (img.width() != rset_.width
        || img.height() != rset_.height
        || img.format() != QImage::Format_RGB32)
        img = getNewImage();

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
#endif
