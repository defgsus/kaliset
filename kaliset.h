#ifndef KALISET_H
#define KALISET_H

#include <QVector2D>
#include <QVector3D>
#include <QImage>
#include <QStringList>

class KaliSet
{
public:

    // ------- types ---------

    typedef float Float;
    typedef QVector3D vec3;
    typedef QVector2D vec2;

    // persistence required
    enum RenderMode
    {
        RM_PLOT_2D = 0,
        RM_VOLUME
    };
    static const QStringList RenderModeName;

    /** The main parameter space */
    struct KaliSettings
    {
        uint numIters;
        vec3 param;
        vec3 pos;
    };

    struct RenderSettings
    {
        int width, height;
        vec2 scale;
        RenderMode mode;
        Float volumeStep, volumeMin, volumeMax;
    };

    // ------ vector helper -----

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

    KaliSet();
    KaliSet(const KaliSettings&, const RenderSettings&);
    ~KaliSet();

    // ------- getter --------

    KaliSettings defaultKaliSettings() const;
    RenderSettings defaultRenderSettings() const;

    const KaliSettings& kaliSettings() const { return kset_; }
    const RenderSettings& renderSettings() const { return rset_; }

    // ------- setter --------

    void setKaliSettings(const KaliSettings& s) { kset_ = s; }
    void setRenderSettings(const RenderSettings& s) { rset_ = s; }

    // ------- calc ----------

    /** Returns the value of p after the last step */
    vec3 value3(const vec3& p) const;
    /** Returns the average value of p */
    vec3 value3_av(const vec3& p) const;

    /** Returns the ray color */
    vec3 trace3(const vec3& ro, const vec3& rd, Float stepSize) const;

    /** Returns the color of pixel x,y as defined in the current settings */
    vec3 color(int x, int y) const;

    // ------- image ---------

    /** Returns a fresh and empty image with the correct settings */
    QImage getNewImage() const;

    /** Renders the image as defined by KaliSettings and RenderSettings.
        The image will be adjusted to the correct format if necessary. */
    void plotImage(QImage &img) { plotImage(img, 0, 0, rset_.width, rset_.height); }
    /** Renders a portion of the image as defined by KaliSettings and RenderSettings.
        The image will be adjusted to the correct format if necessary. */
    void plotImage(QImage &img, int x, int y, int width, int height);

#if 0
    /** Plots a 2d view of the set [pos, pos + scale)
        using the vec3 value3(). */
    void plotImage3(QImage &img, const vec3& pos, const Float scale);

    /** Plots a 2d view of the set [pos, pos + scale)
        using the vec3 value3_av(). */
    void plotImage3_av(QImage &img, const vec3& pos, const Float scale);
#endif

private:

    KaliSettings kset_;
    RenderSettings rset_;
};

#endif // KALISET_H
