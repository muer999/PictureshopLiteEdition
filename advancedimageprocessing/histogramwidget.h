#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>

#include "advancedimageprocessing.h"

class My_QLabel;

class HistogramWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HistogramWidget(QWidget *parent = nullptr);

    QWidget *parent;

    QString imgColorSpace;
    QImage *image;
    cv::Mat *imagemat;
    QPoint MousePressPosition;

    QPixmap *HistPixmap;

    QImage *HistChannelAll;
    QImage *HistChannel1;
    QImage *HistChannel2;
    QImage *HistChannel3;

    QLabel *imgColorSpaceLabel;
    My_QLabel *HistogramLabel;
    QButtonGroup *colorChannelSelect;
    QRadioButton *colorChannelAll;
    QRadioButton *colorChannel1;
    QRadioButton *colorChannel2;
    QRadioButton *colorChannel3;

    QGridLayout *layout;
    void updateHistogram(void);

private:
    void instantiationWidgets(void);
    void initWidgets(void);
    void calcHistGRAY(void);
    void calcHistRGB(void);
    void calcHistHSV(void);
    void calcHistYUV(void);
    void calcHistHLS(void);
    void calcHistHSI(void);
    void calcHistYCrCb(void);
    void showHist(void);

    cv::Scalar HSVValuetoRGBValue(cv::Scalar);

signals:
    void HistogramtoExecute();

public slots:
    void onButtonClicked(int);
    void onResized(void);
};

class My_QLabel : public QLabel
{
    Q_OBJECT
public:
    explicit My_QLabel(QWidget *parent = nullptr);
    bool eventAccept;
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
signals:
    void Label_Resized(void);
};

#endif // HISTOGRAMWIDGET_H
