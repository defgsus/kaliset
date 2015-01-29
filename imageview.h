#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>

class ImageView : public QWidget
{
    Q_OBJECT
public:
    explicit ImageView(QWidget *parent = 0);
    ~ImageView();

signals:

    void clicked(double x, double y);

public slots:

    void setImage(const QImage &img);

protected:

    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;

private:

    void createWidgets_();

    QLabel * display_;
};

#endif // IMAGEVIEW_H
