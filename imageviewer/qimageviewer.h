#ifndef QIMAGEVIEWER_H
#define QIMAGEVIEWER_H

#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QDir>
#include <QSize>
#include <QFileInfo>
#include <QFileInfoList>

#include "addtextwidget.h"
#include "convertcolorspacewidget.h"
#include "showdctwidget.h"
#include "showfftwidget.h"
#include "histpullwidget.h"
#include "histequalizationwidget.h"
#include "smoothingwidget.h"
#include "sharpenwidget.h"
#include "beautywidget.h"
#include "rotatewidget.h"
#include "resizewidget.h"
#include "geocalwidget.h"
#include "fuzzyrecoverywidget.h"
#include "changeheadwidget.h"
#include "changebackgroundwidget.h"
#include "stitchingwidget.h"
#include "histogramwidget.h"

class QImageViewer : public QWidget
{
    Q_OBJECT
public:
    explicit QImageViewer(QWidget *parent = nullptr);
    explicit QImageViewer(QWidget *parent,QString &caption,QString &dir,QString &filer);
    ~QImageViewer();

    QWidget *parent;

    QPoint MousePressPosition;

    int index;
    int angle;
    double imageShowScale;
    QSize size;
    QString filename;
    QString path;
    QDir dir;
    QFileInfo fileInfo;
    QFileInfoList fileInfoList;

    QImage image;
    cv::Mat imagemat;
    QPixmap pixmap;

    QString imgColorSpace;

    /* open a file */
    int openImageFile(const QString &caption,const QString &dir,const QString &filer);

    /* close file */
    int closeImageFile(void);

    int saveImageFile(const QString &caption,const QString &dir,const QString &filer);
    int saveImageFileas(const QString &caption,const QString &dir,const QString &filer);

    /* delete file */
    int delImageFile(void);

    int reloadImageResource(void);
    /* file skipping */
    int last(void);
    int next(void);

    /* change image */
    int zoomIn(void);
    int zoomOut(void);
    int spinToRight(void);
    int spinToLeft(void);

    QPoint caculatePixelPosition(QPoint);
    void deliverPixelPosition(void);

    addTextWidget *addTextWidget;
    convertColorSpaceWidget *convertColorSpaceWidget;
    showDCTWidget *showDCTWidget;
    showFFTWidget *showFFTWidget;

    histPullWidget *histPullWidget;
    histEqualizationWidget *histEqualizationWidget;
    smoothingWidget *smoothingWidget;
    sharpenWidget *sharpenWidget;
    beautyWidget *beautyWidget;

    rotateWidget *rotateWidget;
    resizeWidget *resizeWidget;
    geometricCalibrationWidget *geometricCalibrationWidget;
    fuzzyRecoveryWidget *fuzzyRecoveryWidget;

    changeHeadWidget *changeHeadWidget;
    changeBackgroundWidget *changeBackgroundWidget;
    stitchingWidget *stitchingWidget;

    HistogramWidget *HistogramWidget;

private:
    /* init param */
    void initImageResource(void);
    void setupWidgets(void);

    /* open a image */
    int loadImageResource(void);
    int loadImageResource(const QString &caption,const QString &dir,const QString &filer);

    /* get file info list from current path */
    int getFileInfoList(void);
    int getFileCurIndex(void);
    int upgradeFileInfo(QString &filename,int angle,int sizeScale);
    int upgradeFileInfo(QImage src_image,int angle,int sizeScale);

    void sendImageAddress_ColorSpace_toChildrens(void);
    void update_ColorSpaceText_Histogram_FFT_DCT(void);

public slots:
    void addTextExecute();
    void convertColorSpaceExecute();
    void showDCTExecute();
    void showFFTExecute();

    void histPullExecute();
    void histEqualizationExecute();
    void smoothingExecute();
    void sharpenExecute();
    void beautyExecute();

    void rotateExecute();
    void resizeExecute();
    void geometricCalibrationExecute();
    void fuzzyRecoveryExecute();

    void changeHeadExecute();
    void changeBackgroundExecute();
    void stitchingExecute();

    void HistogramExecute();

signals:
    void imageChanged(void);
};

#endif // QIMAGEVIEWER_H
