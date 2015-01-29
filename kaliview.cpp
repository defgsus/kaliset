/** @file kaliview.cpp

    @brief

    <p>(c) 2015, stefan.berke@modular-audio-graphics.com</p>
    <p>All rights reserved</p>

    <p>created 29.01.2015</p>
*/

#include "kaliview.h"

KaliView::KaliView(bool createKaliControls, bool createRenderControls, QWidget *parent)
    : QWidget       (parent),
      doKali_       (createKaliControls),
      doRender_     (createRenderControls),
      ignoreWidgets_(false)
{
    setObjectName("_KaliView");

    render_ = new RenderThread(this);
    connect(render_, SIGNAL(finished()), this, SLOT(updateImage_()));

    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(updateImage_()));
    timer_->setSingleShot(false);
    timer_->setInterval(1000);

    createWidgets_();
}

KaliView::~KaliView()
{
    stopRender();
}


void KaliView::createWidgets_()
{
    auto lv = new QVBoxLayout(this);

        imageView_ = new ImageView(this);
        lv->addWidget(imageView_);
        connect(imageView_, SIGNAL(clicked(double,double)), this, SLOT(onViewClicked_(double,double)));


        layoutSet_ = lv;

        if (doKali_)
        {
            sbParamX_ = createDoubleSpinBox_(tr("kali param x"), render_->kaliSettings().param.x(), -10000, 10000, 0.01, true);
            sbParamY_ = createDoubleSpinBox_(tr("kali param y"), render_->kaliSettings().param.y(), -10000, 10000, 0.01, true);
            sbParamZ_ = createDoubleSpinBox_(tr("kali param z"), render_->kaliSettings().param.z(), -10000, 10000, 0.01, true);

            sbX_ = createDoubleSpinBox_(tr("position x"), render_->kaliSettings().pos.x(), -10000, 10000, 0.01, true);
            sbY_ = createDoubleSpinBox_(tr("position y"), render_->kaliSettings().pos.y(), -10000, 10000, 0.01, true);
            sbZ_ = createDoubleSpinBox_(tr("position z"), render_->kaliSettings().pos.z(), -10000, 10000, 0.01, true);
        }

        // --- render settings ---

        if (doRender_)
        {
            cbMode_ = new QComboBox(this);
            cbMode_->addItems(KaliSet::RenderModeName);
            cbMode_->setCurrentText(KaliSet::RenderModeName[render_->renderSettings().mode]);
            lv->addWidget(cbMode_);
            connect(cbMode_, SIGNAL(currentIndexChanged(int)), this, SLOT(onRenderChange_()));

            sbScale_ = createDoubleSpinBox_(tr("scale"), render_->renderSettings().scale.x(), 0., 100000., 0.01);

            sbStepMin_ = createDoubleSpinBox_(tr("min step"), render_->renderSettings().volumeMin, 0.000000001, 1000., 0.001, true);
            sbStepMax_ = createDoubleSpinBox_(tr("max step"), render_->renderSettings().volumeMax, 0.000000001, 1000., 0.001, true);
            sbStep_ = createDoubleSpinBox_(tr("step scale"), render_->renderSettings().volumeStep, 0.000000001, 1000., 0.01, true);
        }
}

void KaliView::updateWidgets_()
{
    ignoreWidgets_ = true;

    if (doKali_)
    {
        auto kset = kaliSettings();
        sbParamX_->setValue(kset.param.x());
        sbParamY_->setValue(kset.param.y());
        sbParamZ_->setValue(kset.param.z());
        sbX_->setValue(kset.pos.x());
        sbY_->setValue(kset.pos.y());
        sbZ_->setValue(kset.pos.z());
    }

    if (doRender_)
    {
        auto rset = renderSettings();
        sbStep_->setValue(rset.volumeStep);
        sbStepMin_->setValue(rset.volumeMin);
        sbStepMax_->setValue(rset.volumeMax);
        cbMode_->setCurrentText(KaliSet::RenderModeName[rset.mode]);
    }

    ignoreWidgets_ = false;
}

QDoubleSpinBox * KaliView::createDoubleSpinBox_(const QString &name, double val, double range_min, double range_max, double step, bool is_kali)
{
    auto lh = new QHBoxLayout();
    layoutSet_->addLayout(lh);

    lh->addWidget( new QLabel(name, this) );

    auto w = new QDoubleSpinBox(this);
    w->setRange(range_min, range_max);
    w->setDecimals(8);
    w->setSingleStep(step);
    w->setValue(val);
    lh->addWidget(w);

    if (is_kali)
        connect(w, SIGNAL(valueChanged(double)), this, SLOT(onKaliChange_()));
    else
        connect(w, SIGNAL(valueChanged(double)), this, SLOT(onRenderChange_()));

    return w;
}


void KaliView::startRender()
{
    if (!render_->isRunning())
    {
        render_->start();
        timer_->start();
    }
}

void KaliView::stopRender()
{
    timer_->stop();
    if (render_->isRunning())
    {
        render_->stop();
        updateImage_();
    }
}

void KaliView::setKaliSettings(const KaliSet::KaliSettings & s)
{
    render_->setKaliSettings(s);
    updateWidgets_();
    startRender();
}

void KaliView::setRenderSettings(const KaliSet::RenderSettings & s)
{
    render_->setRenderSettings(s);
    updateWidgets_();
    startRender();
}

/** Updates renderer with widget settings */
void KaliView::onKaliChange_()
{
    if (ignoreWidgets_ || !doKali_)
        return;

    KaliSet::KaliSettings s = render_->kaliSettings();
    s.param = KaliSet::vec3(sbParamX_->value(), sbParamY_->value(), sbParamZ_->value());
    s.pos = KaliSet::vec3(sbX_->value(), sbY_->value(), sbZ_->value());

    render_->setKaliSettings(s);
    startRender();

    emit kaliSettingsChanged();
}

void KaliView::onRenderChange_()
{
    if (ignoreWidgets_ || !doRender_)
        return;

    KaliSet::RenderSettings s = render_->renderSettings();
    s.scale = KaliSet::vec2(sbScale_->value(), sbScale_->value());
    s.volumeStep = sbStep_->value();
    s.volumeMin = sbStepMin_->value();
    s.volumeMax = sbStepMax_->value();
    s.mode = (KaliSet::RenderMode)KaliSet::RenderModeName.indexOf(cbMode_->currentText());

    render_->setRenderSettings(s);
    startRender();

    emit renderSettingsChanged();
}

void KaliView::onViewClicked_(double x, double y)
{
    KaliSet::vec3 pos = render_->kaliSettings().pos;
    KaliSet::vec2 sc = render_->renderSettings().scale;
    emit viewClicked( x * sc.x() + pos.x(), y * sc.y() + pos.y() );
}

void KaliView::updateImage_()
{
    render_->getImage(img_);
    // XXX probably some size policy in ImageView
    imageView_->setMinimumSize(img_.size());
    imageView_->setImage(img_);
}


