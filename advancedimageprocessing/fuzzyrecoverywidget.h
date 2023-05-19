#ifndef FUZZYRECOVERYWIDGET_H
#define FUZZYRECOVERYWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>

#include "advancedimageprocessing.h"

class fuzzyRecoveryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit fuzzyRecoveryWidget(QWidget *parent = nullptr);

    QWidget *parent;

    QString imgColorSpace;
    QImage *image;
    cv::Mat *imagemat;
    QPoint MousePressPosition;

    QLabel *TipsLabel;

    QLabel *RadiusLabel;
    QLineEdit *RadiusEdit;
    QLabel *SNRLabel;
    QLineEdit *SNREdit;

    QLabel *StatusLabel;
    QProgressBar *ProgressBar;
    QPushButton *start;

    QGridLayout *layout;

private:
    void instantiationWidgets(void);
    void initWidgets(void);

    void calcPSF(cv::Mat& outputImg, cv::Size filterSize, int R);
    void calcWnrFilter(const cv::Mat& input_h_PSF, cv::Mat& output_G, double nsr);
    void filter2DFreq(const cv::Mat& inputImg, cv::Mat& outputImg, const cv::Mat& H);
    void fftshift(const cv::Mat& inputImg, cv::Mat& outputImg);

    void RGBFuzzyRecovery(void);
    void GRAYFuzzyRecovery(void);

    void openMatlabMotionBlurRecoveryEXE(cv::Mat& inputImg,cv::Mat& outputImg);
signals:
    void fuzzyRecoverytoExecute();

public slots:
    void OnBtnStartTriggered(void);
};

#endif // FUZZYRECOVERYWIDGET_H
