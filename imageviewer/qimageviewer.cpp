#include "qimageviewer.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QImageReader>



QImageViewer::QImageViewer(QWidget *parent) : QWidget(parent)
{
    this->parent = parent;

    setupWidgets();

    initImageResource();
}

QImageViewer::QImageViewer(QWidget *parent,
                           QString &caption,
                           QString &dir,
                           QString &filer)
{
    this->parent = parent;
    initImageResource();
    loadImageResource(caption, dir, filer);
}

QImageViewer::~QImageViewer(void)
{
    this->parent = nullptr;
}

int QImageViewer::openImageFile(const QString &caption,
                                const QString &dir,
                                const QString &filer)
{
    return loadImageResource(caption, dir, filer);
}

int QImageViewer::closeImageFile(void)
{
    initImageResource();
    return 0;
}
int QImageViewer::saveImageFile(const QString &,const QString &,const QString &)
{
    if(image.isNull())
    {
        return -1;
    }
    int temp = filename.length()-filename.lastIndexOf('.')-1;
    image.save(filename,(filename.right(temp)).toStdString().c_str());

    /* get file list */
    getFileInfoList();

    /* load file info */
    upgradeFileInfo(filename, angle, 10);

    return 0;
}
int QImageViewer::saveImageFileas(const QString &caption,const QString &,const QString &filer)
{
    if(image.isNull())
    {
        return -1;
    }
    filename = QFileDialog::getSaveFileName(this,caption,filename,filer);
    if (filename.isEmpty()) {
        return -2;
    }
    int temp = filename.length()-filename.lastIndexOf('.')-1;
    image.save(filename,(filename.right(temp)).toStdString().c_str());

    /* get file list */
    getFileInfoList();

    /* load file info */
    upgradeFileInfo(filename, angle, 10);

    return 0;
}

int QImageViewer::delImageFile(void)
{
    if (filename.isEmpty()) {
        return -1;
    }

    if (QFile::remove(filename)) {
        qDebug() << "remove success: " << filename;
    } else {
        qDebug() << "remove failed: " << filename;
        return -1;
    }

    /* delete from list */
    fileInfoList.removeAt(index);

    return 0;
}

int QImageViewer::last(void)
{
    if (index < 0) {
        return -1;
    }

    while (1) {
        index = index - 1;
        int count = fileInfoList.count();
        if (index < 0) {
            QMessageBox::information(this, tr("Tip"), tr("This is the first image."));
            index = count - 1;
        }

        filename.clear();
        filename.append(path);
        filename += "/";
        filename += fileInfoList.at(index).fileName();

        if (!QFile(filename).exists()) {
            fileInfoList.removeAt(index);
            continue;
        } else {
            break;
        }
    }

    angle = 0;
    size = QSize(0, 0);

    /* load file info */
    return upgradeFileInfo(filename, angle, 10);
}

int QImageViewer::next(void)
{
    if (index < 0) {
        return -1;
    }

    while (1) {
        index = index + 1;
        int count = fileInfoList.count();
        if (index == count) {
            QMessageBox::information(this, tr("Tip"), tr("This is the last image."));
            index = 0;
        }

        filename.clear();
        filename.append(path);
        filename += "/";
        filename += fileInfoList.at(index).fileName();

        if (!QFile(filename).exists()) {
            fileInfoList.removeAt(index);
            continue;
        } else {
            break;
        }
    }

    angle = 0;
    size = QSize(0, 0);

    /* load file info */
    return upgradeFileInfo(filename, angle, 10);
}

int QImageViewer::zoomIn(void)
{
    if(image.isNull())
    {
        return -1;
    }
    return upgradeFileInfo(image, angle, 11);
}

int QImageViewer::zoomOut(void)
{
    if(image.isNull())
    {
        return -1;
    }
    return upgradeFileInfo(image, angle, 9);
}

int QImageViewer::spinToRight(void)
{
    angle += 1;
    angle = angle % 4;
    if(image.isNull())
    {
        return -1;
    }

    /* load file info */
    return upgradeFileInfo(image, angle, 10);
}

int QImageViewer::spinToLeft(void)
{
    angle += 3;
    angle = angle % 4;
    if(image.isNull())
    {
        return -1;
    }

    /* load file info */
    return upgradeFileInfo(image, angle, 10);
}

void QImageViewer::initImageResource(void)
{
    index = -1;
    angle = 0;
    size = QSize(0, 0);
    imgColorSpace="None";

    imageShowScale = 1;

    filename.clear();
    path.clear();

    sendImageAddress_ColorSpace_toChildrens();
}

int QImageViewer::loadImageResource(void)
{
    QString username = QDir::home().dirName();
    filename = QFileDialog::getOpenFileName(this, tr("Select image:"),
        "C://Users//"+username+"//Pictures", tr("Images (*.jpg *.jpeg *.png *.bmp *.gif *.tif)"));
    if (filename.isEmpty()) {
        return -1;
    }

    /* get file list */
    getFileInfoList();

    /* load file info */
    upgradeFileInfo(filename, angle, 10);

    return 0;
}

int QImageViewer::loadImageResource(const QString &caption,
                                    const QString &directory,
                                    const QString &filer)
{
    QString tempfilename;
    tempfilename = QFileDialog::getOpenFileName(this,caption,directory,filer);
    if (tempfilename.isEmpty()) {
        return 0;
    }

    initImageResource();
    filename = tempfilename;

    /* get file list */
    getFileInfoList();

    /* load file info */
    upgradeFileInfo(filename, angle, 10);

    return 0;
}

int QImageViewer::reloadImageResource(void)
{
    if (filename.isEmpty()) {
        return -1;
    }
    QString tempfilename = filename;
    initImageResource();
    filename = tempfilename;
    /* get file list */
    getFileInfoList();

    /* load file info */
    upgradeFileInfo(filename, angle, 10);

    return 0;
}

int QImageViewer::upgradeFileInfo(QString &filename,int angle,int sizeScale)
{
    QImage imgRotate;
    QMatrix matrix;
    QImage imgScaled;

    if (filename.isEmpty()) {
        return -1;
    }

    fileInfo = QFileInfo(filename);
    if (!image.load(filename)) {
        return -1;
    }

    imagemat = QImage2cvMat(image);

    //imagemat = cv::imread(filename.toStdString(),1);
    //cv::imshow("load",imagemat);

    if(image.format()==QImage::Format_ARGB32 ||
       image.format()==QImage::Format_RGB32 ||
       image.format()==QImage::Format_ARGB32_Premultiplied ||
       image.format()==QImage::Format_RGB888)
    {
        imgColorSpace="RGB";
        cv::cvtColor(imagemat,imagemat,cv::COLOR_RGBA2RGB);
    }

    else if(image.format()==QImage::Format_Indexed8 ||
            image.format()==QImage::Format_Grayscale8)
    {
        imgColorSpace="GRAY";
    }


    if (size == QSize(0, 0)) {
        size = image.size();
    }

    /* modify size */
    imgScaled = image.scaled(size.width() * sizeScale / 10,
                             size.height() * sizeScale / 10,
                             Qt::KeepAspectRatio);
    if (sizeScale != 10) {
        size = imgScaled.size();
    }

    /* modify angle */
    matrix.rotate(angle * 90);
    imgRotate = imgScaled.transformed(matrix);

    size = imgRotate.size();

    imageShowScale = 1;

    pixmap = QPixmap::fromImage(imgRotate);
    /* upgrade index */
    index = getFileCurIndex();

    sendImageAddress_ColorSpace_toChildrens();
    update_ColorSpaceText_Histogram_FFT_DCT();

    return 0;
}

int QImageViewer::upgradeFileInfo(QImage image,int angle,int sizeScale)
{
    QImage imgRotate;
    QMatrix matrix;
    QImage imgScaled;

    if(resizeWidget->isResized)
    {
        size=QSize(0, 0);
        resizeWidget->isResized=false;
    }

    if (size == QSize(0, 0)) {
        size = image.size();
    }

    int lastsize=qMax(size.width(),size.height());

    /* modify angle */
    matrix.rotate(angle * 90);
    imgRotate = image.transformed(matrix);

    size = imgRotate.size();

    int initialsize=qMax(size.width(),size.height());

    if((size.width() * lastsize / initialsize * sizeScale / 10)  > 6000 ||
       (size.height() * lastsize / initialsize * sizeScale / 10) > 6000 ||
       (size.width() * lastsize / initialsize * sizeScale / 10)  < 10 ||
       (size.height() * lastsize / initialsize * sizeScale / 10) < 10)
        sizeScale=10;

    /* modify size */
    imgScaled = imgRotate.scaled(size.width() * lastsize / initialsize * sizeScale / 10,
                                 size.height() * lastsize / initialsize * sizeScale / 10,
                                 Qt::KeepAspectRatio);

    size = imgScaled.size();

    imageShowScale = (double(qMax(size.width(),size.height()))/qMax(image.width(),image.height()));

    pixmap = QPixmap::fromImage(imgScaled);
    /* upgrade index */
    index = getFileCurIndex();

    sendImageAddress_ColorSpace_toChildrens();

    emit imageChanged();

    return 0;
}

int QImageViewer::getFileInfoList(void)
{
    QFileInfo info;
    QFileInfoList infoList;

    path = QFileInfo(filename).absolutePath();
    dir = QFileInfo(filename).absoluteDir();

    /* clear list */
    fileInfoList.clear();

    infoList = dir.entryInfoList(QDir::Files);
    for (int i = 0; i < infoList.count(); i++) {
        info = infoList.at(i);
        QString suffix = info.suffix();

        if (suffix == "jpg" || suffix == "bmp" || suffix == "png"
            || suffix == "gif" || suffix == "jpeg" || suffix == "tif") {
            fileInfoList.append(info);
        }
    }

    return 0;
}

int QImageViewer::getFileCurIndex(void)
{
    QFileInfo info;
    int j;

    if (fileInfoList.count() <= 0) {
        qDebug() << "fileInfoList is NULL!";
        return -1;
    }

    for (j = 0; j < fileInfoList.count(); j++) {
        info = fileInfoList.at(j);
        if (info.fileName() == fileInfo.fileName()) {
            break;
        }
    }

    if (j >= fileInfoList.count()) {
        qDebug() << "Not find current file!";
        return -1;
    }

    index = j;
    //qDebug() << "Current fileInfo index: " << index;

    return index;
}

void QImageViewer::addTextExecute()
{
    upgradeFileInfo(image, angle, 10);
    update_ColorSpaceText_Histogram_FFT_DCT();
}
void QImageViewer::convertColorSpaceExecute()
{
    upgradeFileInfo(image, angle, 10);
    update_ColorSpaceText_Histogram_FFT_DCT();
}
void QImageViewer::showDCTExecute()
{

}
void QImageViewer::showFFTExecute()
{

}

void QImageViewer::histPullExecute()
{
    upgradeFileInfo(image, angle, 10);
    update_ColorSpaceText_Histogram_FFT_DCT();
}
void QImageViewer::histEqualizationExecute()
{
    upgradeFileInfo(image, angle, 10);
    update_ColorSpaceText_Histogram_FFT_DCT();
}
void QImageViewer::smoothingExecute()
{
    upgradeFileInfo(image, angle, 10);
    update_ColorSpaceText_Histogram_FFT_DCT();
}
void QImageViewer::sharpenExecute()
{
    upgradeFileInfo(image, angle, 10);
    update_ColorSpaceText_Histogram_FFT_DCT();
}
void QImageViewer::beautyExecute()
{
    upgradeFileInfo(image, angle, 10);
    update_ColorSpaceText_Histogram_FFT_DCT();
}

void QImageViewer::rotateExecute()
{
    upgradeFileInfo(image, angle, 10);
    update_ColorSpaceText_Histogram_FFT_DCT();
}
void QImageViewer::resizeExecute()
{
    upgradeFileInfo(image, angle, 10);
    update_ColorSpaceText_Histogram_FFT_DCT();
}
void QImageViewer::geometricCalibrationExecute()
{
    upgradeFileInfo(image, angle, 10);
    update_ColorSpaceText_Histogram_FFT_DCT();
}
void QImageViewer::fuzzyRecoveryExecute()
{
    upgradeFileInfo(image, angle, 10);
    update_ColorSpaceText_Histogram_FFT_DCT();
}

void QImageViewer::changeHeadExecute()
{
    upgradeFileInfo(image, angle, 10);
    update_ColorSpaceText_Histogram_FFT_DCT();
}
void QImageViewer::changeBackgroundExecute()
{
    upgradeFileInfo(image, angle, 10);
    update_ColorSpaceText_Histogram_FFT_DCT();
}
void QImageViewer::stitchingExecute()
{
    upgradeFileInfo(image, angle, 10);
    update_ColorSpaceText_Histogram_FFT_DCT();
}
void QImageViewer::HistogramExecute()
{
    upgradeFileInfo(image, angle, 10);
    update_ColorSpaceText_Histogram_FFT_DCT();
}
void QImageViewer::setupWidgets(void)
{
    addTextWidget = new class addTextWidget(this);
    connect(addTextWidget,SIGNAL(addTexttoExecute())
            ,this,SLOT(addTextExecute()));
    convertColorSpaceWidget = new class convertColorSpaceWidget(this);
    connect(convertColorSpaceWidget,SIGNAL(convertColorSpacetoExecute())
            ,this,SLOT(convertColorSpaceExecute()));
    showDCTWidget = new class showDCTWidget(this);
    connect(showDCTWidget,SIGNAL(showDCTtoExecute())
            ,this,SLOT(showDCTExecute()));
    showFFTWidget = new class showFFTWidget(this);
    connect(showFFTWidget,SIGNAL(showFFTtoExecute())
            ,this,SLOT(showFFTExecute()));

    histPullWidget = new class histPullWidget(this);
    connect(histPullWidget,SIGNAL(histPulltoExecute())
            ,this,SLOT(histPullExecute()));
    histEqualizationWidget = new class histEqualizationWidget(this);
    connect(histEqualizationWidget,SIGNAL(histEqualizationtoExecute())
            ,this,SLOT(histEqualizationExecute()));
    smoothingWidget = new class smoothingWidget(this);
    connect(smoothingWidget,SIGNAL(smoothingtoExecute())
            ,this,SLOT(smoothingExecute()));
    sharpenWidget = new class sharpenWidget(this);
    connect(sharpenWidget,SIGNAL(sharpentoExecute())
            ,this,SLOT(sharpenExecute()));
    beautyWidget = new class beautyWidget(this);
    connect(beautyWidget,SIGNAL(beautytoExecute())
            ,this,SLOT(beautyExecute()));

    rotateWidget = new class rotateWidget(this);
    connect(rotateWidget,SIGNAL(rotatetoExecute())
            ,this,SLOT(rotateExecute()));
    resizeWidget = new class resizeWidget(this);
    connect(resizeWidget,SIGNAL(resizetoExecute())
            ,this,SLOT(resizeExecute()));
    geometricCalibrationWidget = new class geometricCalibrationWidget(this);
    connect(geometricCalibrationWidget,SIGNAL(geometricCalibrationtoExecute())
            ,this,SLOT(geometricCalibrationExecute()));
    fuzzyRecoveryWidget = new class fuzzyRecoveryWidget(this);
    connect(fuzzyRecoveryWidget,SIGNAL(fuzzyRecoverytoExecute())
            ,this,SLOT(fuzzyRecoveryExecute()));

    changeHeadWidget = new class changeHeadWidget(this);
    connect(changeHeadWidget,SIGNAL(changeHeadtoExecute())
            ,this,SLOT(changeHeadExecute()));
    changeBackgroundWidget = new class changeBackgroundWidget(this);
    connect(changeBackgroundWidget,SIGNAL(changeBackgroundtoExecute())
            ,this,SLOT(changeBackgroundExecute()));
    stitchingWidget = new class stitchingWidget(this);
    connect(stitchingWidget,SIGNAL(stitchingtoExecute())
            ,this,SLOT(stitchingExecute()));

    HistogramWidget = new class HistogramWidget(this);
    connect(HistogramWidget,SIGNAL(HistogramtoExecute())
            ,this,SLOT(HistogramExecute()));

}

void QImageViewer::sendImageAddress_ColorSpace_toChildrens(void)
{
    addTextWidget->imgColorSpace=this->imgColorSpace;
    addTextWidget->image=&(this->image);
    addTextWidget->imagemat=&(this->imagemat);

    convertColorSpaceWidget->imgColorSpace=&(this->imgColorSpace);
    convertColorSpaceWidget->image=&(this->image);
    convertColorSpaceWidget->imagemat=&(this->imagemat);

    showDCTWidget->imgColorSpace=this->imgColorSpace;
    showDCTWidget->image=&(this->image);
    showDCTWidget->imagemat=&(this->imagemat);

    showFFTWidget->imgColorSpace=this->imgColorSpace;
    showFFTWidget->image=&(this->image);
    showFFTWidget->imagemat=&(this->imagemat);


    histPullWidget->imgColorSpace=&(this->imgColorSpace);
    histPullWidget->image=&(this->image);
    histPullWidget->imagemat=&(this->imagemat);

    histEqualizationWidget->imgColorSpace=this->imgColorSpace;
    histEqualizationWidget->image=&(this->image);
    histEqualizationWidget->imagemat=&(this->imagemat);

    smoothingWidget->imgColorSpace=this->imgColorSpace;
    smoothingWidget->image=&(this->image);
    smoothingWidget->imagemat=&(this->imagemat);

    sharpenWidget->imgColorSpace=this->imgColorSpace;
    sharpenWidget->image=&(this->image);
    sharpenWidget->imagemat=&(this->imagemat);

    beautyWidget->imgColorSpace=this->imgColorSpace;
    beautyWidget->image=&(this->image);
    beautyWidget->imagemat=&(this->imagemat);


    rotateWidget->imgColorSpace=this->imgColorSpace;
    rotateWidget->image=&(this->image);
    rotateWidget->imagemat=&(this->imagemat);

    resizeWidget->imgColorSpace=this->imgColorSpace;
    resizeWidget->image=&(this->image);
    resizeWidget->imagemat=&(this->imagemat);

    geometricCalibrationWidget->imgColorSpace=this->imgColorSpace;
    geometricCalibrationWidget->image=&(this->image);
    geometricCalibrationWidget->imagemat=&(this->imagemat);

    fuzzyRecoveryWidget->imgColorSpace=this->imgColorSpace;
    fuzzyRecoveryWidget->image=&(this->image);
    fuzzyRecoveryWidget->imagemat=&(this->imagemat);


    changeHeadWidget->imgColorSpace=this->imgColorSpace;
    changeHeadWidget->image=&(this->image);
    changeHeadWidget->imagemat=&(this->imagemat);

    changeBackgroundWidget->imgColorSpace=this->imgColorSpace;
    changeBackgroundWidget->image=&(this->image);
    changeBackgroundWidget->imagemat=&(this->imagemat);

    stitchingWidget->imgColorSpace=&(this->imgColorSpace);
    stitchingWidget->image=&(this->image);
    stitchingWidget->imagemat=&(this->imagemat);


    HistogramWidget->imgColorSpace=this->imgColorSpace;
    HistogramWidget->image=&(this->image);
    HistogramWidget->imagemat=&(this->imagemat);
}
void QImageViewer::update_ColorSpaceText_Histogram_FFT_DCT(void)
{
    convertColorSpaceWidget->CurrentColorSpace->setText(imgColorSpace);
    histPullWidget->colorSpaceChanged();
    histEqualizationWidget->colorSpaceChanged();

    showDCTWidget->caculateDCTandShow();
    showFFTWidget->caculateFFTandShow();
    HistogramWidget->updateHistogram();
}


void QImageViewer::deliverPixelPosition(void)
{
    addTextWidget->MousePressPosition=this->MousePressPosition;
    convertColorSpaceWidget->MousePressPosition=this->MousePressPosition;
    showDCTWidget->MousePressPosition=this->MousePressPosition;
    showFFTWidget->MousePressPosition=this->MousePressPosition;
    histPullWidget->MousePressPosition=this->MousePressPosition;
    histEqualizationWidget->MousePressPosition=this->MousePressPosition;
    smoothingWidget->MousePressPosition=this->MousePressPosition;
    sharpenWidget->MousePressPosition=this->MousePressPosition;
    beautyWidget->MousePressPosition=this->MousePressPosition;
    rotateWidget->MousePressPosition=this->MousePressPosition;
    resizeWidget->MousePressPosition=this->MousePressPosition;
    geometricCalibrationWidget->MousePressPosition=this->MousePressPosition;
    fuzzyRecoveryWidget->MousePressPosition=this->MousePressPosition;
    changeHeadWidget->MousePressPosition=this->MousePressPosition;
    changeBackgroundWidget->MousePressPosition=this->MousePressPosition;
    stitchingWidget->MousePressPosition=this->MousePressPosition;
    HistogramWidget->MousePressPosition=this->MousePressPosition;
}

QPoint QImageViewer::caculatePixelPosition(QPoint mousePosition)
{
    QPoint retPosition;
    switch(angle)
    {
        case 0: retPosition.setX(int(mousePosition.x()/imageShowScale));
                retPosition.setY(int(mousePosition.y()/imageShowScale));
                break;
        case 1: retPosition.setX(int(mousePosition.y()/imageShowScale));
                retPosition.setY(int(image.height()-mousePosition.x()/imageShowScale));
                break;
        case 2: retPosition.setX(int(image.width()-mousePosition.x()/imageShowScale));
                retPosition.setY(int(image.height()-mousePosition.y()/imageShowScale));
                break;
        case 3: retPosition.setX(int(image.width()-mousePosition.y()/imageShowScale));
                retPosition.setY(int(mousePosition.x()/imageShowScale));
                break;
    }
    return retPosition;
}
