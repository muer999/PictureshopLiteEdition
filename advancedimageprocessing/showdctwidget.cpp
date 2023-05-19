#include "showdctwidget.h"

#include <QDebug>

showDCTWidget::showDCTWidget(QWidget *parent) : QWidget(parent)
{
    this->parent = parent;

    instantiationWidgets();
    initWidgets();
}

void showDCTWidget::instantiationWidgets(void)
{
    dctImageLabel = new QLabel(this);
    dctImageLabel->setMaximumSize(DCTIMAGEWIDTH,DCTIMAGEHEIGHT);
    dctImageLabel->resize(DCTIMAGEWIDTH,DCTIMAGEHEIGHT);
    dctImage=nullptr;
    dctPixmap=nullptr;

    layout = new QGridLayout(this);
}
void showDCTWidget::initWidgets(void)
{
    layout->addWidget(dctImageLabel,0,0);
}
void showDCTWidget::caculateDCTandShow(void)
{
    if(imgColorSpace=="None")
    {
        dctImageLabel->setText("No Picture.");
    }
    else if(imgColorSpace=="RGB")
    {
        delete dctImage;
        dctImage=new QImage();
        dctImageLabel->resize(DCTIMAGEWIDTH,DCTIMAGEHEIGHT);
        caculateDCTRGB();
        delete dctPixmap;
        dctPixmap=new QPixmap();
        *dctPixmap = QPixmap::fromImage(dctImage->scaled(dctImageLabel->width(),dctImageLabel->height()));
        dctImageLabel->setPixmap(*dctPixmap);
    }
    else if(imgColorSpace=="HSV")
    {
        delete dctImage;
        dctImage=new QImage();
        caculateDCTHSV();
        dctImageLabel->resize(DCTIMAGEWIDTH,DCTIMAGEHEIGHT);
        delete dctPixmap;
        dctPixmap=new QPixmap();
        *dctPixmap = QPixmap::fromImage(dctImage->scaled(dctImageLabel->width(),dctImageLabel->height()));
        dctImageLabel->setPixmap(*dctPixmap);
    }
    else if(imgColorSpace=="YUV")
    {
        delete dctImage;
        dctImage=new QImage();
        caculateDCTYUV();
        dctImageLabel->resize(DCTIMAGEWIDTH,DCTIMAGEHEIGHT);
        delete dctPixmap;
        dctPixmap=new QPixmap();
        *dctPixmap = QPixmap::fromImage(dctImage->scaled(dctImageLabel->width(),dctImageLabel->height()));
        dctImageLabel->setPixmap(*dctPixmap);
    }
    else if(imgColorSpace=="YCrCb")
    {
        delete dctImage;
        dctImage=new QImage();
        caculateDCTYCrCb();
        dctImageLabel->resize(DCTIMAGEWIDTH,DCTIMAGEHEIGHT);
        delete dctPixmap;
        dctPixmap=new QPixmap();
        *dctPixmap = QPixmap::fromImage(dctImage->scaled(dctImageLabel->width(),dctImageLabel->height()));
        dctImageLabel->setPixmap(*dctPixmap);
    }
    else if(imgColorSpace=="HLS")
    {
        delete dctImage;
        dctImage=new QImage();
        caculateDCTHLS();
        dctImageLabel->resize(DCTIMAGEWIDTH,DCTIMAGEHEIGHT);
        delete dctPixmap;
        dctPixmap=new QPixmap();
        *dctPixmap = QPixmap::fromImage(dctImage->scaled(dctImageLabel->width(),dctImageLabel->height()));
        dctImageLabel->setPixmap(*dctPixmap);
    }
    else if(imgColorSpace=="GRAY")
    {
        delete dctImage;
        dctImage=new QImage();
        caculateDCTGRAY();
        dctImageLabel->resize(DCTIMAGEWIDTH,DCTIMAGEHEIGHT);
        delete dctPixmap;
        dctPixmap=new QPixmap();
        *dctPixmap = QPixmap::fromImage(dctImage->scaled(dctImageLabel->width(),dctImageLabel->height()));
        dctImageLabel->setPixmap(*dctPixmap);
    }
    else
    {
        dctImageLabel->setText("Unsupported color space.");
    }
}

void showDCTWidget::caculateDCTRGB(void)
{
    cv::Mat src=*imagemat;
    if(src.empty())
    {
        qDebug() << "From [void showDCTWidget::caculateDCTRGB(void)]:  ERROR: Could not load image." << endl;
    }
    cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);
    cv::resize(src, src, cv::Size(512, 512));
    src.convertTo(src, CV_32F);
    cv::Mat srcDCT;
    cv::dct(src, srcDCT);
    srcDCT.convertTo(srcDCT, CV_8UC1);
    *dctImage = cvMat2QImage(srcDCT);
}
void showDCTWidget::caculateDCTHSV(void)
{
    caculateDCTRGB();
}
void showDCTWidget::caculateDCTYUV(void)
{
    caculateDCTRGB();
}
void showDCTWidget::caculateDCTYCrCb(void)
{
    caculateDCTRGB();
}
void showDCTWidget::caculateDCTHLS(void)
{
    caculateDCTRGB();
}
void showDCTWidget::caculateDCTGRAY(void)
{
    cv::Mat src=*imagemat;
    if(src.empty())
    {
        qDebug() << "From [void showDCTWidget::caculateDCTRGB(void)]:  ERROR: Could not load image." << endl;
    }
    cv::resize(src, src, cv::Size(512, 512));
    src.convertTo(src, CV_32F);
    cv::Mat srcDCT;
    cv::dct(src, srcDCT);
    srcDCT.convertTo(srcDCT, CV_8UC1);
    *dctImage = cvMat2QImage(srcDCT);
}
