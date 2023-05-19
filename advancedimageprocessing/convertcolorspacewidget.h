#ifndef CONVERTCOLORSPACEWIDGET_H
#define CONVERTCOLORSPACEWIDGET_H

#define PI 3.14159265359

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

enum Space{
    RGB = 1,
    HSV = 2,
    YUV = 3,
    YCrCb = 4,
    HLS = 5,
    GRAY = 6,
    HSI = 7
};

class convertColorSpaceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit convertColorSpaceWidget(QWidget *parent = nullptr);

    QWidget *parent;

    QString *imgColorSpace;
    QImage *image;
    cv::Mat *imagemat;
    QPoint MousePressPosition;

    QLabel *TipsLabel;
    QLabel *CurrentColorSpaceLabel;
    QLabel *CurrentColorSpace;
    QLabel *TargetColorSpaceLabel;
    QComboBox *TargetColorSpace;
    QPushButton *start;

    QGridLayout *layout;

private:
    void instantiationWidgets(void);
    void initWidgets(void);
    int getindex(void);

    void cvtColorHSI2BGR(cv::Mat &,cv::Mat &);
    void cvtColorBGR2HSI(cv::Mat &,cv::Mat &);

    IplImage* catHSImage(CvMat* HSI_H, CvMat* HSI_S, CvMat* HSI_I);
    IplImage* HSI2RGBImage(CvMat* HSI_H, CvMat* HSI_S, CvMat* HSI_I);

signals:
    void convertColorSpacetoExecute();

public slots:
    void OnBtnStartTriggered(bool);
};

#endif // CONVERTCOLORSPACEWIDGET_H
