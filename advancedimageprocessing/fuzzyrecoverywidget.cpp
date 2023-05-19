#include "fuzzyrecoverywidget.h"

#include <QDebug>
#include <QMessageBox>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QFile>

#include <stdio.h>
#include <windows.h>
#include <tchar.h>

fuzzyRecoveryWidget::fuzzyRecoveryWidget(QWidget *parent) : QWidget(parent)
{
    this->parent = parent;

    instantiationWidgets();
    initWidgets();
}

void fuzzyRecoveryWidget::instantiationWidgets(void)
{
    TipsLabel = new QLabel(this);
    TipsLabel->setText("Press \"start\" to begin motion blur recovery.");
    TipsLabel->setWordWrap(true);
    /*RadiusLabel = new QLabel(this);
    RadiusLabel->setText("R:");
    RadiusEdit = new QLineEdit(this);
    RadiusEdit->setValidator(new QIntValidator(-65535, 65535, this));
    SNRLabel = new QLabel(this);
    SNRLabel->setText("SNR:");
    SNREdit = new QLineEdit(this);
    SNREdit->setValidator(new QDoubleValidator(-65535, 65535, 5, this));*/
    StatusLabel = new QLabel(this);
    StatusLabel->setText("");
    StatusLabel->hide();
    ProgressBar = new QProgressBar(this);
    ProgressBar->setRange(0,3);
    ProgressBar->setValue(0);
    ProgressBar->hide();
    start = new QPushButton(this);
    start->setText("Start");

    layout = new QGridLayout(this);
}
void fuzzyRecoveryWidget::initWidgets(void)
{
    layout->addWidget(TipsLabel,0,0,1,2);
    /*layout->addWidget(RadiusLabel,1,0);
    layout->addWidget(RadiusEdit,1,1);
    layout->addWidget(SNRLabel,2,0);
    layout->addWidget(SNREdit,2,1);*/
    layout->addWidget(StatusLabel,1,0,1,2);
    layout->addWidget(ProgressBar,2,0,1,2);
    layout->addWidget(start,3,0);
    layout->setRowStretch(0,4);
    layout->setRowStretch(1,1);
    layout->setRowStretch(2,1);
    layout->setRowStretch(3,1);

    connect(start, SIGNAL(clicked(bool)), this, SLOT(OnBtnStartTriggered()));
}
void fuzzyRecoveryWidget::OnBtnStartTriggered(void)
{
    if(imgColorSpace=="None")
    {
        QMessageBox::information(this, tr("Error"), tr("You didn't load any image!"));
        return;
    }
    if(imgColorSpace=="GRAY")
    {
        //GRAYFuzzyRecovery();
        cv::Mat src=*imagemat;

        StatusLabel->show();
        ProgressBar->show();
        StatusLabel->setText("Processing!(1/1)");
        ProgressBar->setValue(0);
        openMatlabMotionBlurRecoveryEXE(src,src);
        StatusLabel->setText("Done!");
        ProgressBar->setValue(3);
        ProgressBar->hide();

        *imagemat = src;
        *image = cvMat2QImage(src);
    }
    else
    {
        //RGBFuzzyRecovery();
        cv::Mat src=*imagemat;

        cv::Mat rgb[3];
        cv::split(src,rgb);

        StatusLabel->show();
        ProgressBar->show();
        StatusLabel->setText("Processing!(1/3)");
        ProgressBar->setValue(0);
        openMatlabMotionBlurRecoveryEXE(rgb[0],rgb[0]);
        StatusLabel->setText("Processing!(2/3)");
        ProgressBar->setValue(1);
        openMatlabMotionBlurRecoveryEXE(rgb[1],rgb[1]);
        StatusLabel->setText("Processing!(3/3)");
        ProgressBar->setValue(2);
        openMatlabMotionBlurRecoveryEXE(rgb[2],rgb[2]);
        StatusLabel->setText("Done!");
        ProgressBar->setValue(3);

        cv::merge(rgb,3,src);
        ProgressBar->hide();

        *imagemat = src;
        *image = cvMat2QImage(src);
    }

    emit fuzzyRecoverytoExecute();
}
void fuzzyRecoveryWidget::RGBFuzzyRecovery(void)
{
    int adjust_r = RadiusEdit->text().toInt();
    double snr = SNREdit->text().toDouble();

    cv::Mat src = *imagemat;
    cv::Mat rgb[3];
    cv::Mat rgb_out[3];
    cv::Mat imgOut;

    cv::split(src,rgb);

    // 偶数处理，神级操作
    cv::Rect roi = cv::Rect(0, 0, src.cols & -2, src.rows & -2);
    //printf("roi.x=%d, y=%d, w=%d, h=%d\n", roi.x, roi.y, roi.width, roi.height);

    // 生成PSF与维纳滤波器
    cv::Mat Hw, h;
    calcPSF(h, roi.size(), adjust_r);
    calcWnrFilter(h, Hw, 1.0 / double(snr));

    // 反模糊
    for(int i=0;i<3;i++)
    {
        filter2DFreq(rgb[i](roi), rgb_out[i], Hw);
    }

    cv::merge(rgb_out,3,imgOut);
    // 归一化显示
    imgOut.convertTo(imgOut, CV_8U);
    cv::normalize(imgOut, imgOut, 0, 255, cv::NORM_MINMAX);

    *image = cvMat2QImage(imgOut);
    *imagemat = imgOut;
}
void fuzzyRecoveryWidget::GRAYFuzzyRecovery(void)
{
    int adjust_r = RadiusEdit->text().toInt();
    double snr = SNREdit->text().toDouble();

    cv::Mat src = *imagemat;
    cv::Mat imgOut;

    // 偶数处理，神级操作
    cv::Rect roi = cv::Rect(0, 0, src.cols & -2, src.rows & -2);
    //printf("roi.x=%d, y=%d, w=%d, h=%d\n", roi.x, roi.y, roi.width, roi.height);

    // 生成PSF与维纳滤波器
    cv::Mat Hw, h;
    calcPSF(h, cv::Size(15,1), adjust_r);
    calcWnrFilter(h, Hw, 1.0 / double(snr));

    // 反模糊
    filter2DFreq(src(roi), imgOut, Hw);

    // 归一化显示
    imgOut.convertTo(imgOut, CV_8U);
    cv::normalize(imgOut, imgOut, 0, 255, cv::NORM_MINMAX);

    *image = cvMat2QImage(imgOut);
    *imagemat = imgOut;
}

void fuzzyRecoveryWidget::calcPSF(cv::Mat& outputImg, cv::Size filterSize, int R)
{
    cv::Mat h(filterSize, CV_32F, cv::Scalar(0));
    cv::Point point(filterSize.width / 2, filterSize.height / 2);
    circle(h, point, R, 255, -1, 8);
    cv::Scalar summa = sum(h);
    outputImg = h / summa[0];
}

void fuzzyRecoveryWidget::calcWnrFilter(const cv::Mat& input_h_PSF, cv::Mat& output_G, double nsr)
{
    cv::Mat h_PSF_shifted;
    fftshift(input_h_PSF, h_PSF_shifted);
    cv::Mat planes[2] = { cv::Mat_<float>(h_PSF_shifted.clone()), cv::Mat::zeros(h_PSF_shifted.size(), CV_32F) };
    cv::Mat complexI;
    merge(planes, 2, complexI);
    dft(complexI, complexI);
    split(complexI, planes);
    cv::Mat denom;
    pow(abs(planes[0]), 2, denom);
    denom += nsr;
    divide(planes[0], denom, output_G);
}

void fuzzyRecoveryWidget::filter2DFreq(const cv::Mat& inputImg, cv::Mat& outputImg, const cv::Mat& H)
{
    cv::Mat planes[2] = { cv::Mat_<float>(inputImg.clone()), cv::Mat::zeros(inputImg.size(), CV_32F) };
    cv::Mat complexI;
    cv::merge(planes, 2, complexI);
    cv::dft(complexI, complexI, cv::DFT_SCALE);
    cv::Mat planesH[2] = { cv::Mat_<float>(H.clone()), cv::Mat::zeros(H.size(), CV_32F) };
    cv::Mat complexH;
    cv::merge(planesH, 2, complexH);
    cv::Mat complexIH;
    cv::mulSpectrums(complexI, complexH, complexIH, 0);
    cv::idft(complexIH, complexIH);
    cv::split(complexIH, planes);
    outputImg = planes[0];
}

void fuzzyRecoveryWidget::fftshift(const cv::Mat& inputImg, cv::Mat& outputImg)
{
    outputImg = inputImg.clone();
    int cx = outputImg.cols / 2;
    int cy = outputImg.rows / 2;
    cv::Mat q0(outputImg, cv::Rect(0, 0, cx, cy));
    cv::Mat q1(outputImg, cv::Rect(cx, 0, cx, cy));
    cv::Mat q2(outputImg, cv::Rect(0, cy, cx, cy));
    cv::Mat q3(outputImg, cv::Rect(cx, cy, cx, cy));
    cv::Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

void fuzzyRecoveryWidget::openMatlabMotionBlurRecoveryEXE(cv::Mat& inputImg,cv::Mat& outputImg)
{
    cv::imwrite("origin.jpg",inputImg);//这里偷懒了，直接把要处理的图片保存在硬盘里了，过一会让matlab程序里用自己的打开函数图像的函数就可以了，这样就不用处理从opencv的图像数据类型转换到matlab的图像数据类型的问题了

    SHELLEXECUTEINFO sei;//用c语言里一个很奇怪库,在<windows.h>里，这样就可以和操作系统打交道了，这个东西可以控制打开其他的文件和程序
    memset(&sei, 0, sizeof(SHELLEXECUTEINFO));

    sei.cbSize = sizeof(SHELLEXECUTEINFO);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb = _T("open");                    //指定要进行的操作
    sei.lpFile = _T("MotionBlurRecovery.exe");  //指定要打开的东西在哪里
    sei.nShow = SW_SHOWDEFAULT;
    ShellExecuteEx(&sei);                       //让操作系统根据刚刚指定的信息进行操作

    WaitForSingleObject(sei.hProcess, INFINITE);//监听操作系统创建的新进程的运行情况，等这个进程结束（也就是程序退出）
    CloseHandle(sei.hProcess);

    outputImg = cv::imread("recover.jpg",0);//matlab把处理好的图像又存在硬盘里，然后用opencv打开

    QFile::remove("origin.jpg");//打开之后把硬盘里的文件删了
    QFile::remove("recover.jpg");
}
