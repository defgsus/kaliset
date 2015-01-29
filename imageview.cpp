#include <QLabel>
#include <QLayout>
#include <QMouseEvent>

#include "imageview.h"



ImageView::ImageView(QWidget *parent)
    : QWidget   (parent)
{
    setMinimumSize(256, 256);

    createWidgets_();
}

ImageView::~ImageView()
{

}



void ImageView::createWidgets_()
{
    auto lv = new QVBoxLayout(this);
    lv->setMargin(0);
    lv->setContentsMargins(0,0,0,0);

        display_ = new QLabel(this);
        lv->addWidget(display_);

}

void ImageView::mousePressEvent(QMouseEvent * e)
{
    if (e->buttons() & Qt::LeftButton)
        emit clicked(double(e->x()) / width() * 2. - 1.,
                     double(height() - 1 - e->y()) / height() * 2. - 1.);
    QWidget::mousePressEvent(e);
}

void ImageView::mouseMoveEvent(QMouseEvent * e)
{
    if (e->buttons() & Qt::LeftButton)
        emit clicked(double(e->x()) / width() * 2. - 1.,
                     double(height() - 1 - e->y()) / height() * 2. - 1.);
    QWidget::mouseMoveEvent(e);
}


void ImageView::setImage(const QImage &img)
{
    display_->setMinimumSize(img.size());
    display_->setPixmap(QPixmap::fromImage(img));
}
