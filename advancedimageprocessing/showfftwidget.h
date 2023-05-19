#ifndef SHOWFFTWIDGET_H
#define SHOWFFTWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFontComboBox>
#include <QComboBox>
#include <QSpinBox>
#include <QColor>
#include <QFont>

#include "advancedimageprocessing.h"

#define FFTIMAGEWIDTH 400
#define FFTIMAGEHEIGHT 400

class showFFTWidget : public QWidget
{
    Q_OBJECT
public:
    explicit showFFTWidget(QWidget *parent = nullptr);

    QWidget *parent;

    QString imgColorSpace;
    QImage *image;
    cv::Mat *imagemat;
    QPoint MousePressPosition;

    QLabel *fftImageLabel;

    QImage *fftImage;
    QPixmap *fftPixmap;

    QGridLayout *layout;

    void caculateFFTandShow(void);

private:
    void instantiationWidgets(void);
    void initWidgets(void);

    void caculateFFTRGB(void);
    void caculateFFTYUV(void);
    void caculateFFTYCrCb(void);
    void caculateFFTHSV(void);
    void caculateFFTHLS(void);
    void caculateFFTGRAY(void);

signals:
    void showFFTtoExecute();

public slots:

};

#endif // SHOWFFTWIDGET_H
