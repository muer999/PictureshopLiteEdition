#include "showfftwidget.h"

#include <QDebug>

showFFTWidget::showFFTWidget(QWidget *parent) : QWidget(parent)
{
    this->parent = parent;

    instantiationWidgets();
    initWidgets();
}

void showFFTWidget::instantiationWidgets(void)
{
    fftImageLabel = new QLabel(this);
    fftImageLabel->setMaximumSize(FFTIMAGEWIDTH,FFTIMAGEHEIGHT);
    fftImageLabel->resize(FFTIMAGEWIDTH,FFTIMAGEHEIGHT);
    fftImage=nullptr;
    fftPixmap=nullptr;

    layout = new QGridLayout(this);
}
void showFFTWidget::initWidgets(void)
{
    layout->addWidget(fftImageLabel,0,0);
}
void showFFTWidget::caculateFFTandShow(void)
{
    if(imgColorSpace=="None")
    {
        fftImageLabel->setText("No Picture.");
    }
    else if(imgColorSpace=="RGB")
    {
        delete fftImage;
        fftImage=new QImage();
        caculateFFTRGB();
        fftImageLabel->resize(FFTIMAGEWIDTH,FFTIMAGEHEIGHT);
        delete fftPixmap;
        fftPixmap=new QPixmap();
        *fftPixmap = QPixmap::fromImage(fftImage->scaled(fftImageLabel->width(),fftImageLabel->height()));
        fftImageLabel->setPixmap(*fftPixmap);
    }
    else if(imgColorSpace=="HSV")
    {
        delete fftImage;
        fftImage=new QImage();
        caculateFFTHSV();
        fftImageLabel->resize(FFTIMAGEWIDTH,FFTIMAGEHEIGHT);
        delete fftPixmap;
        fftPixmap=new QPixmap();
        *fftPixmap = QPixmap::fromImage(fftImage->scaled(fftImageLabel->width(),fftImageLabel->height()));
        fftImageLabel->setPixmap(*fftPixmap);
    }
    else if(imgColorSpace=="YUV")
    {
        delete fftImage;
        fftImage=new QImage();
        caculateFFTYUV();
        fftImageLabel->resize(FFTIMAGEWIDTH,FFTIMAGEHEIGHT);
        delete fftPixmap;
        fftPixmap=new QPixmap();
        *fftPixmap = QPixmap::fromImage(fftImage->scaled(fftImageLabel->width(),fftImageLabel->height()));
        fftImageLabel->setPixmap(*fftPixmap);
    }
    else if(imgColorSpace=="YCrCb")
    {
        delete fftImage;
        fftImage=new QImage();
        caculateFFTYCrCb();
        fftImageLabel->resize(FFTIMAGEWIDTH,FFTIMAGEHEIGHT);
        delete fftPixmap;
        fftPixmap=new QPixmap();
        *fftPixmap = QPixmap::fromImage(fftImage->scaled(fftImageLabel->width(),fftImageLabel->height()));
        fftImageLabel->setPixmap(*fftPixmap);
    }
    else if(imgColorSpace=="HLS")
    {
        delete fftImage;
        fftImage=new QImage();
        caculateFFTHLS();
        fftImageLabel->resize(FFTIMAGEWIDTH,FFTIMAGEHEIGHT);
        delete fftPixmap;
        fftPixmap=new QPixmap();
        *fftPixmap = QPixmap::fromImage(fftImage->scaled(fftImageLabel->width(),fftImageLabel->height()));
        fftImageLabel->setPixmap(*fftPixmap);
    }
    else if(imgColorSpace=="GRAY")
    {
        delete fftImage;
        fftImage=new QImage();
        caculateFFTGRAY();
        fftImageLabel->resize(FFTIMAGEWIDTH,FFTIMAGEHEIGHT);
        delete fftPixmap;
        fftPixmap=new QPixmap();
        *fftPixmap = QPixmap::fromImage(fftImage->scaled(fftImageLabel->width(),fftImageLabel->height()));
        fftImageLabel->setPixmap(*fftPixmap);
    }
    else
    {
        fftImageLabel->setText("Unsupported color space.");
    }
}

void showFFTWidget::caculateFFTRGB(void)
{
    cv::Mat I =*imagemat;
    if(I.empty())
    {
        qDebug() << "From [void showFFTWidget::caculateFFTRGB(void)]:  ERROR: Could not load image." << endl;
    }
    cv::cvtColor(I, I, cv::COLOR_BGR2GRAY);
    cv::Mat padded;                 //以0填充输入图像矩阵
    int m = cv::getOptimalDFTSize(I.rows);
    int n = cv::getOptimalDFTSize(I.cols);

    //填充输入图像I，输入矩阵为padded，上方和左方不做填充处理
    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    cv::Mat planes[] = { cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(),CV_32F) };
    cv::Mat complexI;
    cv::merge(planes, 2, complexI);     //将planes融合合并成一个多通道数组complexI

    cv::dft(complexI, complexI);        //进行傅里叶变换

    //计算幅值，转换到对数尺度(logarithmic scale)
    //=> log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    cv::split(complexI, planes);        //planes[0] = Re(DFT(I),planes[1] = Im(DFT(I))
                                   //即planes[0]为实部,planes[1]为虚部
    cv::magnitude(planes[0], planes[1], planes[0]);     //planes[0] = magnitude
    cv::Mat magI = planes[0];

    magI += cv::Scalar::all(1);
    log(magI, magI);                //转换到对数尺度(logarithmic scale)

    //如果有奇数行或列，则对频谱进行裁剪
    magI = magI(cv::Rect(0, 0, magI.cols&-2, magI.rows&-2));

    //重新排列傅里叶图像中的象限，使得原点位于图像中心
    int cx = magI.cols / 2;
    int cy = magI.rows / 2;

    cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));       //左上角图像划定ROI区域
    cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));      //右上角图像
    cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));      //左下角图像
    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy));     //右下角图像

    //变换左上角和右下角象限
    cv::Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    //变换右上角和左下角象限
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    //归一化处理，用0-1之间的浮点数将矩阵变换为可视的图像格式
    normalize(magI, magI, 0, 255, cv::NORM_MINMAX);
    magI.convertTo(magI, CV_8UC1);
    cv::resize(magI, magI, cv::Size(512, 512));
    *fftImage = cvMat2QImage(magI);
}
void showFFTWidget::caculateFFTHSV(void)
{
    caculateFFTRGB();
}
void showFFTWidget::caculateFFTYUV(void)
{
    caculateFFTRGB();
}
void showFFTWidget::caculateFFTYCrCb(void)
{
    caculateFFTRGB();
}
void showFFTWidget::caculateFFTHLS(void)
{
    caculateFFTRGB();
}
void showFFTWidget::caculateFFTGRAY(void)
{
    cv::Mat I =*imagemat;
    if(I.empty())
    {
        qDebug() << "From [void showFFTWidget::caculateFFTRGB(void)]:  ERROR: Could not load image." << endl;
    }
    cv::Mat padded;                 //以0填充输入图像矩阵
    int m = cv::getOptimalDFTSize(I.rows);
    int n = cv::getOptimalDFTSize(I.cols);

    //填充输入图像I，输入矩阵为padded，上方和左方不做填充处理
    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    cv::Mat planes[] = { cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(),CV_32F) };
    cv::Mat complexI;
    cv::merge(planes, 2, complexI);     //将planes融合合并成一个多通道数组complexI

    cv::dft(complexI, complexI);        //进行傅里叶变换

    //计算幅值，转换到对数尺度(logarithmic scale)
    //=> log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    cv::split(complexI, planes);        //planes[0] = Re(DFT(I),planes[1] = Im(DFT(I))
                                        //即planes[0]为实部,planes[1]为虚部
    cv::magnitude(planes[0], planes[1], planes[0]);     //planes[0] = magnitude
    cv::Mat magI = planes[0];

    magI += cv::Scalar::all(1);
    log(magI, magI);                //转换到对数尺度(logarithmic scale)

    //如果有奇数行或列，则对频谱进行裁剪
    magI = magI(cv::Rect(0, 0, magI.cols&-2, magI.rows&-2));

    //重新排列傅里叶图像中的象限，使得原点位于图像中心
    int cx = magI.cols / 2;
    int cy = magI.rows / 2;

    cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));       //左上角图像划定ROI区域
    cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));      //右上角图像
    cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));      //左下角图像
    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy));     //右下角图像

    //变换左上角和右下角象限
    cv::Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    //变换右上角和左下角象限
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    //归一化处理，用0-1之间的浮点数将矩阵变换为可视的图像格式
    normalize(magI, magI, 0, 255, cv::NORM_MINMAX);
    magI.convertTo(magI, CV_8UC1);
    cv::resize(magI, magI, cv::Size(512, 512));
    *fftImage = cvMat2QImage(magI);
}
