#include "convertcolorspacewidget.h"
#include <math.h>
#include <QDebug>
#include <QMessageBox>

convertColorSpaceWidget::convertColorSpaceWidget(QWidget *parent) : QWidget(parent)
{
    this->parent = parent;

    instantiationWidgets();
    initWidgets();
}

void convertColorSpaceWidget::instantiationWidgets(void)
{
    TipsLabel = new QLabel(this);
    TipsLabel->setWordWrap(true);
    TipsLabel->setText("Please choose a color space at the combobox below and then press \"start\".");
    CurrentColorSpaceLabel = new QLabel(this);
    CurrentColorSpaceLabel->setText("Current Space");
    CurrentColorSpace = new QLabel(this);
    TargetColorSpaceLabel = new QLabel(this);
    TargetColorSpaceLabel->setText("Target Space");
    TargetColorSpace = new QComboBox(this);
    TargetColorSpace->insertItem(1,"RGB");
    TargetColorSpace->insertItem(2,"GRAY");
    TargetColorSpace->insertItem(3,"YUV");
    TargetColorSpace->insertItem(4,"YCrCb");
    TargetColorSpace->insertItem(5,"HLS");
    TargetColorSpace->insertItem(6,"HSV");
    TargetColorSpace->insertItem(7,"HSI");
    TargetColorSpace->setMaxVisibleItems(4);

    start = new QPushButton(this);
    start->setText("Start");

    layout = new QGridLayout(this);
}
void convertColorSpaceWidget::initWidgets(void)
{
    layout->addWidget(TipsLabel,0,0,1,2);
    layout->addWidget(CurrentColorSpaceLabel,1,0);
    layout->addWidget(TargetColorSpaceLabel,1,1);
    layout->addWidget(CurrentColorSpace,2,0);
    layout->addWidget(TargetColorSpace,2,1);
    layout->addWidget(start,3,1);
    layout->setRowStretch(0,3);
    layout->setRowStretch(1,1);
    layout->setRowStretch(2,1);
    layout->setRowStretch(3,1);

    connect(start, SIGNAL(clicked(bool)), this, SLOT(OnBtnStartTriggered(bool)));
}
void convertColorSpaceWidget::OnBtnStartTriggered(bool)
{
    if(*imgColorSpace=="None")
    {
        QMessageBox::information(this, tr("Error"), tr("You didn't load any image!"));
        return;
    }

    cv::Mat src =*imagemat;

    int index = getindex();
    if(*imgColorSpace=="RGB")
    {
        switch(index)
        {
            case RGB:break;
            case HSV:cv::cvtColor(src,src,cv::COLOR_BGR2HSV);break;
            case YUV:cv::cvtColor(src,src,cv::COLOR_BGR2YUV);break;
            case YCrCb:cv::cvtColor(src,src,cv::COLOR_BGR2YCrCb);break;
            case HLS:cv::cvtColor(src,src,cv::COLOR_BGR2HLS);break;
            case HSI:cvtColorBGR2HSI(src,src);break;
            case GRAY:cv::cvtColor(src,src,cv::COLOR_BGR2GRAY);break;
        }
    }
    else if(*imgColorSpace=="HSV")
    {
        switch(index)
        {
            case RGB:cv::cvtColor(src,src,cv::COLOR_HSV2BGR);break;
            case HSV:break;
            case YUV:cv::cvtColor(src,src,cv::COLOR_HSV2BGR);cv::cvtColor(src,src,cv::COLOR_BGR2YUV);break;
            case YCrCb:cv::cvtColor(src,src,cv::COLOR_HSV2BGR);cv::cvtColor(src,src,cv::COLOR_BGR2YCrCb);break;
            case HLS:cv::cvtColor(src,src,cv::COLOR_HSV2BGR);cv::cvtColor(src,src,cv::COLOR_BGR2HLS);break;
            case HSI:cv::cvtColor(src,src,cv::COLOR_HSV2BGR);cvtColorBGR2HSI(src,src);break;
            case GRAY:cv::cvtColor(src,src,cv::COLOR_HSV2BGR);cv::cvtColor(src,src,cv::COLOR_BGR2GRAY);break;
        }
    }
    else if(*imgColorSpace=="YUV")
    {
        switch(index)
        {
            case RGB:cv::cvtColor(src,src,cv::COLOR_YUV2BGR);break;
            case HSV:cv::cvtColor(src,src,cv::COLOR_YUV2BGR);cv::cvtColor(src,src,cv::COLOR_BGR2HSV);break;
            case YUV:break;
            case YCrCb:cv::cvtColor(src,src,cv::COLOR_YUV2BGR);cv::cvtColor(src,src,cv::COLOR_BGR2YCrCb);break;
            case HLS:cv::cvtColor(src,src,cv::COLOR_YUV2BGR);cv::cvtColor(src,src,cv::COLOR_BGR2HLS);break;
            case HSI:cv::cvtColor(src,src,cv::COLOR_YUV2BGR);cvtColorBGR2HSI(src,src);break;
            case GRAY:cv::cvtColor(src,src,cv::COLOR_YUV2BGR);cv::cvtColor(src,src,cv::COLOR_BGR2GRAY);break;
        }
    }
    else if(*imgColorSpace=="YCrCb")
    {
        switch(index)
        {
            case RGB:cv::cvtColor(src,src,cv::COLOR_YCrCb2BGR);break;
            case HSV:cv::cvtColor(src,src,cv::COLOR_YCrCb2BGR);cv::cvtColor(src,src,cv::COLOR_BGR2HSV);break;
            case YUV:cv::cvtColor(src,src,cv::COLOR_YCrCb2BGR);cv::cvtColor(src,src,cv::COLOR_BGR2YUV);break;
            case YCrCb:break;
            case HLS:cv::cvtColor(src,src,cv::COLOR_YCrCb2BGR);cv::cvtColor(src,src,cv::COLOR_BGR2HLS);break;
            case HSI:cv::cvtColor(src,src,cv::COLOR_YCrCb2BGR);cvtColorBGR2HSI(src,src);break;
            case GRAY:cv::cvtColor(src,src,cv::COLOR_YCrCb2BGR);cv::cvtColor(src,src,cv::COLOR_BGR2GRAY);break;
        }
    }
    else if(*imgColorSpace=="HLS")
    {
        switch(index)
        {
            case RGB:cv::cvtColor(src,src,cv::COLOR_HLS2BGR);break;
            case HSV:cv::cvtColor(src,src,cv::COLOR_HLS2BGR);cv::cvtColor(src,src,cv::COLOR_BGR2HSV);break;
            case YUV:cv::cvtColor(src,src,cv::COLOR_HLS2BGR);cv::cvtColor(src,src,cv::COLOR_BGR2YUV);break;
            case YCrCb:cv::cvtColor(src,src,cv::COLOR_HLS2BGR);cv::cvtColor(src,src,cv::COLOR_BGR2YCrCb);break;
            case HLS:break;
            case HSI:cv::cvtColor(src,src,cv::COLOR_HLS2BGR);cvtColorBGR2HSI(src,src);break;
            case GRAY:cv::cvtColor(src,src,cv::COLOR_HLS2BGR);cv::cvtColor(src,src,cv::COLOR_BGR2GRAY);break;
        }
    }
    else if(*imgColorSpace=="HSI")
    {
        switch(index)
        {
            case RGB:cvtColorHSI2BGR(src,src);break;
            case HSV:cvtColorHSI2BGR(src,src);cv::cvtColor(src,src,cv::COLOR_BGR2HSV);break;
            case YUV:cvtColorHSI2BGR(src,src);cv::cvtColor(src,src,cv::COLOR_BGR2YUV);break;
            case YCrCb:cvtColorHSI2BGR(src,src);cv::cvtColor(src,src,cv::COLOR_BGR2YCrCb);break;
            case HLS:cvtColorHSI2BGR(src,src);cv::cvtColor(src,src,cv::COLOR_BGR2HLS);break;
            case HSI:break;
            case GRAY:cvtColorHSI2BGR(src,src);cv::cvtColor(src,src,cv::COLOR_BGR2GRAY);break;
        }
    }
    else if(*imgColorSpace=="GRAY")
    {
        switch(index)
        {
            case RGB:cv::cvtColor(src,src,cv::COLOR_GRAY2BGR);break;
            case HSV:cv::cvtColor(src,src,cv::COLOR_GRAY2BGR);cv::cvtColor(src,src,cv::COLOR_BGR2HSV);break;
            case YUV:cv::cvtColor(src,src,cv::COLOR_GRAY2BGR);cv::cvtColor(src,src,cv::COLOR_BGR2YUV);break;
            case YCrCb:cv::cvtColor(src,src,cv::COLOR_GRAY2BGR);cv::cvtColor(src,src,cv::COLOR_BGR2YCrCb);break;
            case HLS:cv::cvtColor(src,src,cv::COLOR_GRAY2BGR);cv::cvtColor(src,src,cv::COLOR_BGR2HLS);break;
            case HSI:cv::cvtColor(src,src,cv::COLOR_GRAY2BGR);cvtColorBGR2HSI(src,src);break;
            case GRAY:break;
        }
    }
    *imgColorSpace = TargetColorSpace->currentText();

    *image = cvMat2QImage(src);
    *imagemat = src;

    emit convertColorSpacetoExecute();
}
int convertColorSpaceWidget::getindex(void)
{
    int index;
    if(TargetColorSpace->currentText()=="RGB")
        index = RGB;
    else if (TargetColorSpace->currentText()=="HSV")
        index = HSV;
    else if (TargetColorSpace->currentText()=="YUV")
        index = YUV;
    else if (TargetColorSpace->currentText()=="YCrCb")
        index = YCrCb;
    else if (TargetColorSpace->currentText()=="HLS")
        index = HLS;
    else if (TargetColorSpace->currentText()=="HSI")
        index = HSI;
    else if (TargetColorSpace->currentText()=="GRAY")
        index = GRAY;
    return index;
}
void convertColorSpaceWidget::cvtColorHSI2BGR(cv::Mat &src,cv::Mat &dst)
{
    cv::Mat result = cv::Mat::zeros(src.size(),src.type());
    int nWidth = src.cols;
    int nHeight = src.rows;
    for (int row = 0;row < nHeight;row++)
    {
        for (int col = 0;col < nWidth;col++)
        {
            double B,G,R;
            double H = src.at<cv::Vec3b>(row,col)[0]*2;
            double S = src.at<cv::Vec3b>(row,col)[1]/255.0;
            double I = src.at<cv::Vec3b>(row,col)[2]/255.0;
            if(H>=0 && H<120)
            {
                B=I*(1-S);
                R=I*(1+(S*cos((H)/180*PI)/cos((60-H)/180*PI)));
                G=3*I-R-B;
            }
            else if(H>=120 && H<240)
            {
                R=I*(1-S);
                G=I*(1+(S*cos((H)/180*PI)/cos((60-H)/180*PI)));
                B=3*I-R-G;
            }
            else if(H>=240 && H<=360)
            {
                G=I*(1-S);
                B=I*(1+(S*cos((H)/180*PI)/cos((60-H)/180*PI)));
                R=3*I-G-B;
            }
            result.at<cv::Vec3b>(row,col)[0]=cv::saturate_cast<uchar>(B*255);
            result.at<cv::Vec3b>(row,col)[1]=cv::saturate_cast<uchar>(G*255);
            result.at<cv::Vec3b>(row,col)[2]=cv::saturate_cast<uchar>(R*255);
        }
    }
    result.copyTo(dst);
}
void convertColorSpaceWidget::cvtColorBGR2HSI(cv::Mat &src,cv::Mat &dst)
{
    cv::Mat result = cv::Mat::zeros(src.size(),src.type());
    int nWidth = src.cols;
    int nHeight = src.rows;
    for (int row = 0;row < nHeight;row++)
    {
        for (int col = 0;col < nWidth;col++)
        {
            double B = src.at<cv::Vec3b>(row,col)[0]/255.0;
            double G = src.at<cv::Vec3b>(row,col)[1]/255.0;
            double R = src.at<cv::Vec3b>(row,col)[2]/255.0;
            double H,S,I;
            if(B<=G)
            {
                H=acos((((R-G)+(R-B))/2)/(sqrt((R-G)*(R-G)+(R-B)*(G-B))));
                H=H/PI*180;
            }
            else
            {
                H=2*PI-acos((((R-G)+(R-B))/2)/(sqrt((R-G)*(R-G)+(R-B)*(G-B))));
                H=H/PI*180;
            }
            S=1-3.0/(R+G+B)*MIN(MIN(R,G),B);
            I=1/3.0*(R+G+B);
            result.at<cv::Vec3b>(row,col)[0] = cv::saturate_cast<uchar>(H/2);
            result.at<cv::Vec3b>(row,col)[1] = cv::saturate_cast<uchar>(S*255);
            result.at<cv::Vec3b>(row,col)[2] = cv::saturate_cast<uchar>(I*255);
        }
    }
    result.copyTo(dst);
}
