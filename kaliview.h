/** @file kaliview.h

    @brief

    <p>(c) 2015, stefan.berke@modular-audio-graphics.com</p>
    <p>All rights reserved</p>

    <p>created 29.01.2015</p>
*/

#ifndef KALIVIEW_H
#define KALIVIEW_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTimer>
#include <QComboBox>

#include "renderpool.h"
#include "imageview.h"

/** A complete renderer/display with all settings */
class KaliView : public QWidget
{
    Q_OBJECT
public:

    // ------------- ctor ---------------

    explicit KaliView(bool createKaliControls, bool createRenderControls, uint numThreads, QWidget *parent = 0);
    ~KaliView();

    KaliSet::KaliSettings kaliSettings() const { return render_->kaliSettings(); }
    KaliSet::RenderSettings renderSettings() const { return render_->renderSettings(); }

signals:

    /** Emitted when the kali settings where changed by the user */
    void kaliSettingsChanged();
    /** Dito for render settings */
    void renderSettingsChanged();

    /** Forward from ImageView. Returns the actual fractal coordinates. */
    void viewClicked(double x, double y);

public slots:

    /** Forces new start or restart of the rendering with current settings. */
    void startRender();
    /** Stops rendering */
    void stopRender();

    /** Overrides current settings and (re-)starts rendering */
    void setKaliSettings(const KaliSet::KaliSettings&);
    /** Overrides current settings and (re-)starts rendering */
    void setRenderSettings(const KaliSet::RenderSettings&);

private slots:

    void onKaliChange_();
    void onRenderChange_();
    void onViewClicked_(double, double);

    void updateImage_();

private:

    void createWidgets_();
    void updateWidgets_();

    QDoubleSpinBox * createDoubleSpinBox_(const QString& name, double val, double range_min, double range_max, double step, bool is_kaliset = false);

    ImageView * imageView_;
    RenderPool * render_;
    QTimer * timer_;

    bool doKali_, doRender_, ignoreWidgets_;
    QImage img_;
    QVBoxLayout * layoutSet_;
    QDoubleSpinBox *sbParamX_, *sbParamY_, *sbParamZ_,
                   *sbX_, *sbY_, *sbZ_, *sbScale_,
                    *sbStep_, *sbStepMin_, *sbStepMax_;
    QComboBox * cbMode_;
};

#endif // KALIVIEW_H
