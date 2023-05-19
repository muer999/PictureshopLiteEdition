#include "histogramwidget.h"
#include <math.h>
#include <QDebug>

HistogramWidget::HistogramWidget(QWidget *parent) : QWidget(parent)
{
    this->parent = parent;

    instantiationWidgets();
    initWidgets();
}

void HistogramWidget::instantiationWidgets(void)
{
    imgColorSpace = "None";
    HistPixmap = nullptr;
    image = nullptr;
    HistChannelAll = nullptr;
    HistChannel1 = nullptr;
    HistChannel2 = nullptr;
    HistChannel3 = nullptr;
    imgColorSpaceLabel = new QLabel(this);
    imgColorSpaceLabel->setMaximumHeight(40);
    colorChannelAll = new QRadioButton(this);
    colorChannel1 = new QRadioButton(this);
    colorChannel2 = new QRadioButton(this);
    colorChannel3 = new QRadioButton(this);
    colorChannelSelect = new QButtonGroup(this);
    colorChannelSelect->setExclusive(true);
    colorChannelSelect->addButton(colorChannelAll);
    colorChannelSelect->addButton(colorChannel1);
    colorChannelSelect->addButton(colorChannel2);
    colorChannelSelect->addButton(colorChannel3);
    connect(colorChannelSelect,SIGNAL(buttonClicked(int)),this,SLOT(onButtonClicked(int)));
    HistogramLabel = new My_QLabel(this);
    HistogramLabel->setMinimumSize(300,300);
    HistogramLabel->setMaximumHeight(300);
    HistogramLabel->setFrameShape (QFrame::Box);
    HistogramLabel->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(0, 0, 0);");
    connect(HistogramLabel,SIGNAL(Label_Resized()),this,SLOT(onResized()));
    layout = new QGridLayout(this);

    colorChannelAll->setChecked(true);

    initWidgets();
    updateHistogram();
}
void HistogramWidget::initWidgets(void)
{
    layout->addWidget(imgColorSpaceLabel,0,0,1,4);
    layout->addWidget(colorChannelAll,1,0);
    layout->addWidget(colorChannel1,1,1);
    layout->addWidget(colorChannel2,1,2);
    layout->addWidget(colorChannel3,1,3);
    layout->addWidget(HistogramLabel,2,0,1,4);
    layout->setRowStretch(0,1);
    layout->setRowStretch(1,1);
    layout->setRowStretch(2,6);

}
void HistogramWidget::updateHistogram(void)
{
    colorChannelAll->show();
    if(imgColorSpace=="RGB")
    {
        imgColorSpaceLabel->setText("ColorSpace:RGB");
        colorChannelAll->setText("ALL");
        colorChannel1->setEnabled(true);
        colorChannel1->setText("R");
        colorChannel1->setStyleSheet("color: red");
        colorChannel2->setEnabled(true);
        colorChannel2->setText("G");
        colorChannel2->setStyleSheet("color: green");
        colorChannel3->setEnabled(true);
        colorChannel3->setText("B");
        colorChannel3->setStyleSheet("color: blue");
        calcHistRGB();
        showHist();
    }
    else if(imgColorSpace=="HSV")
    {
        imgColorSpaceLabel->setText("ColorSpace:HSV");
        colorChannelAll->setText("ALL");
        colorChannel1->setEnabled(true);
        colorChannel1->setText("H");
        colorChannel1->setStyleSheet("color: black");
        colorChannel2->setEnabled(true);
        colorChannel2->setText("S");
        colorChannel2->setStyleSheet("color: black");
        colorChannel3->setEnabled(true);
        colorChannel3->setText("V");
        colorChannel3->setStyleSheet("color: black");
        calcHistHSV();
        showHist();
    }
    else if(imgColorSpace=="YUV")
    {
        imgColorSpaceLabel->setText("ColorSpace:YUV");
        colorChannelAll->setText("ALL");
        colorChannel1->setEnabled(true);
        colorChannel1->setText("Y");
        colorChannel1->setStyleSheet("color: black");
        colorChannel2->setEnabled(true);
        colorChannel2->setText("U");
        colorChannel2->setStyleSheet("color: black");
        colorChannel3->setEnabled(true);
        colorChannel3->setText("V");
        colorChannel3->setStyleSheet("color: black");
        calcHistYUV();
        showHist();
    }
    else if(imgColorSpace=="HLS")
    {
        imgColorSpaceLabel->setText("ColorSpace:HLS");
        colorChannelAll->setText("ALL");
        colorChannel1->setEnabled(true);
        colorChannel1->setText("H");
        colorChannel1->setStyleSheet("color: black");
        colorChannel2->setEnabled(true);
        colorChannel2->setText("L");
        colorChannel2->setStyleSheet("color: black");
        colorChannel3->setEnabled(true);
        colorChannel3->setText("S");
        colorChannel3->setStyleSheet("color: black");
        calcHistHLS();
        showHist();
    }
    else if(imgColorSpace=="HSI")
    {
        imgColorSpaceLabel->setText("ColorSpace:HSI");
        colorChannelAll->setText("ALL");
        colorChannel1->setEnabled(true);
        colorChannel1->setText("H");
        colorChannel1->setStyleSheet("color: black");
        colorChannel2->setEnabled(true);
        colorChannel2->setText("S");
        colorChannel2->setStyleSheet("color: black");
        colorChannel3->setEnabled(true);
        colorChannel3->setText("I");
        colorChannel3->setStyleSheet("color: black");
        calcHistHSI();
        showHist();
    }
    else if(imgColorSpace=="YCrCb")
    {
        imgColorSpaceLabel->setText("ColorSpace:HLS");
        colorChannelAll->setText("ALL");
        colorChannel1->setEnabled(true);
        colorChannel1->setText("Y");
        colorChannel1->setStyleSheet("color: black");
        colorChannel2->setEnabled(true);
        colorChannel2->setText("Cr");
        colorChannel2->setStyleSheet("color: black");
        colorChannel3->setEnabled(true);
        colorChannel3->setText("Cb");
        colorChannel3->setStyleSheet("color: black");
        calcHistYCrCb();
        showHist();
    }
    else if(imgColorSpace=="GRAY")
    {
        imgColorSpaceLabel->setText("ColorSpace:GRAY");
        colorChannelAll->setText("ALL");
        colorChannel1->setEnabled(false);
        colorChannel1->setText("");
        colorChannel1->setStyleSheet("color: black");
        colorChannel2->setEnabled(false);
        colorChannel2->setText("");
        colorChannel2->setStyleSheet("color: black");
        colorChannel3->setEnabled(false);
        colorChannel3->setText("");
        colorChannel3->setStyleSheet("color: black");
        colorChannelAll->setChecked(true);
        calcHistGRAY();
        showHist();
    }
    else if(imgColorSpace=="None")
    {
        imgColorSpaceLabel->setText("No Picture.");
        colorChannelAll->hide();
        colorChannel1->hide();
        colorChannel2->hide();
        colorChannel3->hide();
        HistogramLabel->hide();
    }
    else
    {
        imgColorSpaceLabel->setText("Unsupported format.");
        colorChannelAll->hide();
        colorChannel1->hide();
        colorChannel2->hide();
        colorChannel3->hide();
        HistogramLabel->hide();
    }
}

void HistogramWidget::showHist(void)
{
    colorChannelAll->show();
    colorChannel1->show();
    colorChannel2->show();
    colorChannel3->show();
    HistogramLabel->show();
    delete HistPixmap;
    HistPixmap = new QPixmap();
    HistogramLabel->setMaximumWidth(HistogramLabel->width()-10);
    if(colorChannelAll->isChecked())
    {
        *HistPixmap = QPixmap::fromImage((*HistChannelAll).scaled(HistogramLabel->width(),HistogramLabel->height()));
        HistogramLabel->setPixmap(*HistPixmap);
    }
    else if(colorChannel1->isChecked())
    {
        *HistPixmap = QPixmap::fromImage((*HistChannel1).scaled(HistogramLabel->width(),HistogramLabel->height()));
        HistogramLabel->setPixmap(*HistPixmap);
    }
    else if(colorChannel2->isChecked())
    {
        *HistPixmap = QPixmap::fromImage((*HistChannel2).scaled(HistogramLabel->width(),HistogramLabel->height()));
        HistogramLabel->setPixmap(*HistPixmap);
    }
    else if(colorChannel3->isChecked())
    {
        *HistPixmap = QPixmap::fromImage((*HistChannel3).scaled(HistogramLabel->width(),HistogramLabel->height()));
        HistogramLabel->setPixmap(*HistPixmap);
    }
    HistogramLabel->setMaximumWidth(16777215);
}

void HistogramWidget::onButtonClicked(int)
{
    showHist();
}

void HistogramWidget::calcHistRGB(void)
{
    cv::Mat src=*imagemat;

    //加载图像
    //src = QImage2cvMat(*image);
    if (!src.data)
    {
        qDebug() << "From [void HistogramWidget::calcHistRGB(void)]:  ERROR: Could not load image.";

    }
    //cv::namedWindow("INPUT", cv::WINDOW_AUTOSIZE);
    //cv::imshow("INPUT", src);

    //分通道显示
    std::vector<cv::Mat> bgr_planes;
    split(src, bgr_planes);

    //设定像素取值范围
    int histSize = 256;
    float range[] = { 0,255 };
    const float *histRanges[] = { range };

    //三个通道分别计算直方图
    cv::Mat b_hist, g_hist, r_hist;
    cv::calcHist(&bgr_planes[0], 1, nullptr, cv::Mat(), b_hist, 1, &histSize, histRanges, true, false);
    cv::calcHist(&bgr_planes[1], 1, nullptr, cv::Mat(), g_hist, 1, &histSize, histRanges, true, false);
    cv::calcHist(&bgr_planes[2], 1, nullptr, cv::Mat(), r_hist, 1, &histSize, histRanges, true, false);

    //创建直方图画布并归一化处理
    int hist_w = 512; int hist_h = 200;
    int bin_w = cvRound( double(hist_w)/histSize );

    cv::Mat rgb_hist[3];
    for(int i=0; i<3; i++)
    {
        rgb_hist[i] = cv::Mat(hist_h, hist_w, CV_8UC3, cv::Scalar::all(100));
    }

    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(100,100,100));

    cv::normalize(r_hist, r_hist, 0, histImage.rows-10, cv::NORM_MINMAX);
    cv::normalize(g_hist, g_hist, 0, histImage.rows-10, cv::NORM_MINMAX);
    cv::normalize(b_hist, b_hist, 0, histImage.rows-10, cv::NORM_MINMAX);

    //render histogram chart  在直方图画布上画出直方图
    for( int i = 1; i <= histSize; i++ )
    {
        cv::line( histImage, cv::Point( bin_w*(i-1), hist_h-cvRound(r_hist.at<float>(i-1)) ) ,
            cv::Point( bin_w*(i), hist_h-cvRound(r_hist.at<float>(i)) ),
            cv::Scalar( 0, 0, 255), 1);
        cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
            cv::Point( bin_w*(i), hist_h-cvRound(g_hist.at<float>(i)) ),
            cv::Scalar( 0, 255, 0), 1);
        cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
            cv::Point( bin_w*(i), hist_h-cvRound(b_hist.at<float>(i)) ),
            cv::Scalar( 255, 0, 0), 1);
    }

    for (int j=0; j<histSize; j++)
    {
        int val = cv::saturate_cast<int>(r_hist.at<float>(j));
        cv::rectangle(rgb_hist[0], cv::Point(j*2, rgb_hist[0].rows), cv::Point(j*2+1, rgb_hist[0].rows-val), cv::Scalar(0,0,255),1,0);

        val = cv::saturate_cast<int>(g_hist.at<float>(j));
        cv::rectangle(rgb_hist[1], cv::Point(j*2, rgb_hist[1].rows), cv::Point(j*2+1, rgb_hist[1].rows-val), cv::Scalar(0,255,0),1,0);

        val = cv::saturate_cast<int>(b_hist.at<float>(j));
        cv::rectangle(rgb_hist[2], cv::Point(j*2, rgb_hist[2].rows), cv::Point(j*2+1, rgb_hist[2].rows-val), cv::Scalar(255,0,0),1,0);
    }

    /*cv::namedWindow("OUTPUT CHANNEL_ALL", cv::WINDOW_AUTOSIZE);
    cv::imshow("OUTPUT CHANNEL_ALL", histImage);
    cv::namedWindow("OUTPUT CHANNEL_1", cv::WINDOW_AUTOSIZE);
    cv::imshow("OUTPUT CHANNEL_1", rgb_hist[0]);
    cv::namedWindow("OUTPUT CHANNEL_2", cv::WINDOW_AUTOSIZE);
    cv::imshow("OUTPUT CHANNEL_2", rgb_hist[1]);
    cv::namedWindow("OUTPUT CHANNEL_3", cv::WINDOW_AUTOSIZE);
    cv::imshow("OUTPUT CHANNEL_3", rgb_hist[2]);*/


    delete HistChannelAll;
    delete HistChannel1;
    delete HistChannel2;
    delete HistChannel3;
    HistChannelAll = new QImage();
    HistChannel1 = new QImage();
    HistChannel2 = new QImage();
    HistChannel3 = new QImage();
    *HistChannelAll = cvMat2QImage(histImage);

    *HistChannel1 = cvMat2QImage(rgb_hist[0]);
    *HistChannel2 = cvMat2QImage(rgb_hist[1]);
    *HistChannel3 = cvMat2QImage(rgb_hist[2]);

}
void HistogramWidget::calcHistHSV(void)
{
    cv::Mat src=*imagemat;
    if (!src.data)
    {
        qDebug() << "From [void HistogramWidget::calcHistHSV(void)]:  ERROR: Could not load image.";

    }
    //cv::namedWindow("INPUT", cv::WINDOW_AUTOSIZE);
    //cv::imshow("INPUT", src);

    std::vector<cv::Mat> hsv_planes;
    split(src, hsv_planes);

    int h_histSize = 180;
    int s_histSize = 256;
    int v_histSize = 256;
    float h_ranges[] = {0, 180}; // hue is [0, 180]
    float s_ranges[] = {0, 255};
    float v_ranges[] = {0, 255};
    const float* h_Ranges[] = {h_ranges};
    const float* s_Ranges[] = {s_ranges};
    const float* v_Ranges[] = {v_ranges};

    cv::Mat h_hist,s_hist,v_hist;

    // Compute the histogram
    //
    cv::calcHist(&hsv_planes[0], 1, 0, cv::Mat(), h_hist, 1, &h_histSize, h_Ranges, true, false);
    cv::calcHist(&hsv_planes[1], 1, 0, cv::Mat(), s_hist, 1, &s_histSize, s_Ranges, true, false);
    cv::calcHist(&hsv_planes[2], 1, 0, cv::Mat(), v_hist, 1, &v_histSize, v_Ranges, true, false);

    //cv::normalize(hist, hist, 0, 255, cv::NORM_MINMAX);

    int hist_w = 512; int hist_h = 200;
    int h_bin_w = cvRound( (double) hist_w/h_histSize );
    int s_bin_w = cvRound( (double) hist_w/s_histSize );
    int v_bin_w = cvRound( (double) hist_w/v_histSize );

    cv::Mat H_hist,S_hist,V_hist;

    H_hist = cv::Mat(hist_h, hist_w, CV_8UC3, cv::Scalar::all(100));
    S_hist = cv::Mat(hist_h, hist_w, CV_8UC3, cv::Scalar::all(100));
    V_hist = cv::Mat(hist_h, hist_w, CV_8UC3, cv::Scalar::all(100));
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(100,100,100));

    cv::normalize(h_hist, h_hist, 0, histImage.rows-10, cv::NORM_MINMAX);
    cv::normalize(s_hist, s_hist, 0, histImage.rows-10, cv::NORM_MINMAX);
    cv::normalize(v_hist, v_hist, 0, histImage.rows-10, cv::NORM_MINMAX);

    // Draw our histogram.
    //
    for( int i = 1; i <= h_histSize; i++ )
    {
        cv::line( histImage, cv::Point( h_bin_w*(i-1), hist_h-cvRound(h_hist.at<float>(i-1)) ) ,
            cv::Point( h_bin_w*(i), hist_h-cvRound(h_hist.at<float>(i)) ),
            HSVValuetoRGBValue(cv::Scalar(i,255,255)), 1);
    }
    for( int i = 1; i <= s_histSize; i++ )
    {
        cv::line( histImage, cv::Point( s_bin_w*(i-1), hist_h - cvRound(s_hist.at<float>(i-1)) ) ,
            cv::Point( s_bin_w*(i), hist_h-cvRound(s_hist.at<float>(i)) ),
            cv::Scalar( 0, 255, 255), 1);
    }
    for( int i = 1; i <= v_histSize; i++ )
    {
        cv::line( histImage, cv::Point( v_bin_w*(i-1), hist_h - cvRound(v_hist.at<float>(i-1)) ) ,
            cv::Point( v_bin_w*(i), hist_h-cvRound(v_hist.at<float>(i)) ),
            cv::Scalar( 255, 255, 0), 1);
    }


    for (int j = 0; j < h_histSize; ++j)
    {
        int val = cv::saturate_cast<int>(h_hist.at<float>(j));
        cv::rectangle(H_hist, cv::Point(j*h_bin_w, H_hist.rows), cv::Point(j*h_bin_w+1, H_hist.rows-val), HSVValuetoRGBValue(cv::Scalar(j,255,255)),1,0);
    }
    for (int j = 0; j < s_histSize; ++j)
    {
        int val = cv::saturate_cast<int>(s_hist.at<float>(j));
        cv::rectangle(S_hist, cv::Point(j*s_bin_w, S_hist.rows), cv::Point(j*s_bin_w+1, S_hist.rows-val), cv::Scalar(0, 255, 255),1,0);
    }
    for (int j = 0; j < v_histSize; ++j)
    {
        int val = cv::saturate_cast<int>(v_hist.at<float>(j));
        cv::rectangle(V_hist, cv::Point(j*v_bin_w, V_hist.rows), cv::Point(j*v_bin_w+1, V_hist.rows-val), cv::Scalar(255,255,0),1,0);
    }

    //cv::namedWindow("OUTPUT CHANNEL_ALL", cv::WINDOW_AUTOSIZE);
    //cv::imshow("OUTPUT CHANNEL_ALL", histImage);

    delete HistChannelAll;
    delete HistChannel1;
    delete HistChannel2;
    delete HistChannel3;
    HistChannelAll = new QImage();
    HistChannel1 = new QImage();
    HistChannel2 = new QImage();
    HistChannel3 = new QImage();

    *HistChannelAll = cvMat2QImage(histImage);

    *HistChannel1 = cvMat2QImage(H_hist);
    *HistChannel2 = cvMat2QImage(S_hist);
    *HistChannel3 = cvMat2QImage(V_hist);
}
void HistogramWidget::calcHistYUV(void)
{
    calcHistRGB();
}
void HistogramWidget::calcHistHLS(void)
{
    calcHistHSV();
}
void HistogramWidget::calcHistYCrCb(void)
{
    calcHistRGB();
}
void HistogramWidget::calcHistHSI(void)
{
    calcHistHSV();
}
void HistogramWidget::calcHistGRAY(void)
{
    cv::Mat src;

    //加载图像
    src = *imagemat;
    if (!src.data)
    {
        qDebug() << "From [void HistogramWidget::calcHistGRAY(void)]:  ERROR: Could not load image.";

    }
    //cv::namedWindow("INPUT", cv::WINDOW_AUTOSIZE);
    //cv::imshow("INPUT", src);

    //为计算直方图配置变量
    //首先是需要计算的通道编号，就是需要计算哪个通道的直方图（BGR空间需要确定计算，计算方法见帖子中对相关参数的说明）
    //然后是定义直方图计算结果的存储空间
    cv::Mat dsthist;
    //接下来是直方图的每一个维度的数目（这个数目用于将每一维度的数值分组）
    int histSize = 256;
    //最后是确定每个维度的取值范围，就是每一维度的横坐标的取值范围
    //首先得定义一个数组用来存储单个维度的的取值范围
    float midRanges[] = { 0, 255 };
    //然后把这个数组再放到一个二维数组中
    const float *ranges[] = { midRanges };

    //准备工作做好后,就可以调用calcHis函数计算直方图数据了
    calcHist(&src, 1, 0, cv::Mat(), dsthist, 1, &histSize, ranges, true, false);

    //calcHist函数调用结束后，dstHist变量中将储存直方图数据

    //接下来绘制直方图
    //首先先创建一个黑底的图像，为了可以显示彩色，所以该绘制图像是一个8位的3通道图像

    int hist_w = 512; int hist_h = 200;
    int bin_w = cvRound( (double) hist_w/histSize );

    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(100,100,100));

    //因为图像中的某个灰度值的总个数可能会超出所定义的图像的尺寸，所以要对个数进行归一化处理
    cv::normalize(dsthist, dsthist, 0, histImage.rows-10, cv::NORM_MINMAX);

    //遍历直方图数据,对数据进行归一化和绘图处理
    for (int j=0; j<histSize; ++j)
    {
        int val = cv::saturate_cast<int>(dsthist.at<float>(j));
        cv::rectangle(histImage, cv::Point(j*2, histImage.rows), cv::Point(j*2+1, histImage.rows-val), cv::Scalar(255,255,255),1,0);
    }

    //cv::namedWindow("OUTPUT CHANNEL_ALL", cv::WINDOW_AUTOSIZE);
    //cv::imshow("OUTPUT CHANNEL_ALL", histImage);

    delete HistChannelAll;
    delete HistChannel1;
    delete HistChannel2;
    delete HistChannel3;
    HistChannelAll = new QImage();
    HistChannel1 = new QImage();
    HistChannel2 = new QImage();
    HistChannel3 = new QImage();

    *HistChannelAll = cvMat2QImage(histImage);
}

void HistogramWidget::onResized(void)
{
    showHist();
}
cv::Scalar HistogramWidget::HSVValuetoRGBValue(cv::Scalar hsv)
{
    double h=hsv.val[0]*2,s=hsv.val[1]/255,v=hsv.val[2];
    double f,p,q,t;
    int hi;
    hi=int(fabs(h/60))%6;
    f=h/60-hi;
    p=v*(1-s);
    q=v*(1-f*s);
    t=v*(1-(1-f)*s);
    switch(hi)
    {
        case 0:return cv::Scalar(p,t,v);
        case 1:return cv::Scalar(p,v,q);
        case 2:return cv::Scalar(t,v,p);
        case 3:return cv::Scalar(v,q,p);
        case 4:return cv::Scalar(v,p,t);
        case 5:return cv::Scalar(q,p,v);
    }
    return cv::Scalar(0,0,0);
}

My_QLabel::My_QLabel(QWidget *parent):QLabel(parent)
{
    eventAccept=false;
}

void My_QLabel::resizeEvent(QResizeEvent *)
{
    if(eventAccept==true)
    {
        eventAccept=false;
        emit Label_Resized();
    }
    else
        eventAccept=true;
}
