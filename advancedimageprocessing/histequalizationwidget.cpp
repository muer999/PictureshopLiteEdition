#include "histequalizationwidget.h"

#include <QDebug>
#include <QMessageBox>

histEqualizationWidget::histEqualizationWidget(QWidget *parent) : QWidget(parent)
{
    this->parent = parent;

    instantiationWidgets();
    initWidgets();
}

void histEqualizationWidget::instantiationWidgets(void)
{
    TipsLabel = new QLabel(this);
    TipsLabel->setText("Press \"start\" to begin histogram equalization.");

    start = new QPushButton(this);
    start->setText("Start");

    colorChannelLabel = new QLabel(this);
    colorChannelLabel->setText("Choose target label.");
    colorChannelSelectLabel = new QLabel(this);
    colorChannelSelectLabel->setText("Color Space:None");
    colorChannelSelect = new QButtonGroup(this);
    colorChannelAll = new QRadioButton(this);
    colorChannelAll->setText("");
    colorChannel1 = new QRadioButton(this);
    colorChannel1->setText("");
    colorChannel2 = new QRadioButton(this);
    colorChannel2->setText("");
    colorChannel3 = new QRadioButton(this);
    colorChannel3->setText("");
    colorChannelAll->setEnabled(false);
    colorChannel1->setEnabled(false);
    colorChannel2->setEnabled(false);
    colorChannel3->setEnabled(false);
    colorChannelSelect->addButton(colorChannelAll);
    colorChannelSelect->addButton(colorChannel1);
    colorChannelSelect->addButton(colorChannel2);
    colorChannelSelect->addButton(colorChannel3);
    colorChannelSelect->setExclusive(true);
    colorChannelAll->setChecked(true);

    layout = new QGridLayout(this);
}
void histEqualizationWidget::initWidgets(void)
{
    layout->addWidget(TipsLabel,0,0,1,5);
    layout->addWidget(colorChannelLabel,1,0,1,5);
    layout->addWidget(colorChannelSelectLabel,2,0);
    layout->addWidget(colorChannelAll,2,1);
    layout->addWidget(colorChannel1,2,2);
    layout->addWidget(colorChannel2,2,3);
    layout->addWidget(colorChannel3,2,4);
    layout->addWidget(start,3,0,1,3);
    layout->setRowStretch(0,4);
    layout->setRowStretch(1,1);
    layout->setRowStretch(2,1);
    layout->setRowStretch(3,1);

    connect(start, SIGNAL(clicked(bool)), this, SLOT(OnBtnStartTriggered()));
}
void histEqualizationWidget::colorSpaceChanged(void)
{
    if(imgColorSpace=="None")
    {
        colorChannelSelectLabel->setText("Color Space:None");
        colorChannelAll->setText("");
        colorChannel1->setText("");
        colorChannel2->setText("");
        colorChannel3->setText("");
        colorChannelAll->setEnabled(false);
        colorChannel1->setEnabled(false);
        colorChannel2->setEnabled(false);
        colorChannel3->setEnabled(false);
    }
    else if(imgColorSpace=="RGB")
    {
        colorChannelSelectLabel->setText("Color Space:RGB");
        colorChannelAll->setText("ALL");
        colorChannel1->setText("B");
        colorChannel2->setText("G");
        colorChannel3->setText("R");
        colorChannelAll->setEnabled(true);
        colorChannel1->setEnabled(true);
        colorChannel2->setEnabled(true);
        colorChannel3->setEnabled(true);
    }
    else if(imgColorSpace=="HSV")
    {
        colorChannelSelectLabel->setText("Color Space:HSV");
        colorChannelAll->setText("ALL");
        colorChannel1->setText("H");
        colorChannel2->setText("S");
        colorChannel3->setText("V");
        colorChannelAll->setEnabled(true);
        colorChannel1->setEnabled(true);
        colorChannel2->setEnabled(true);
        colorChannel3->setEnabled(true);
    }
    else if(imgColorSpace=="HLS")
    {
        colorChannelSelectLabel->setText("Color Space:HLS");
        colorChannelAll->setText("ALL");
        colorChannel1->setText("H");
        colorChannel2->setText("L");
        colorChannel3->setText("S");
        colorChannelAll->setEnabled(true);
        colorChannel1->setEnabled(true);
        colorChannel2->setEnabled(true);
        colorChannel3->setEnabled(true);
    }
    else if(imgColorSpace=="HSI")
    {
        colorChannelSelectLabel->setText("Color Space:HSI");
        colorChannelAll->setText("ALL");
        colorChannel1->setText("H");
        colorChannel2->setText("S");
        colorChannel3->setText("I");
        colorChannelAll->setEnabled(true);
        colorChannel1->setEnabled(true);
        colorChannel2->setEnabled(true);
        colorChannel3->setEnabled(true);
    }
    else if(imgColorSpace=="YUV")
    {
        colorChannelSelectLabel->setText("Color Space:YUV");
        colorChannelAll->setText("ALL");
        colorChannel1->setText("Y");
        colorChannel2->setText("U");
        colorChannel3->setText("V");
        colorChannelAll->setEnabled(true);
        colorChannel1->setEnabled(true);
        colorChannel2->setEnabled(true);
        colorChannel3->setEnabled(true);
    }
    else if(imgColorSpace=="YCrCb")
    {
        colorChannelSelectLabel->setText("Color Space:YCrCb");
        colorChannelAll->setText("ALL");
        colorChannel1->setText("Y");
        colorChannel2->setText("Cr");
        colorChannel3->setText("Cb");
        colorChannelAll->setEnabled(true);
        colorChannel1->setEnabled(true);
        colorChannel2->setEnabled(true);
        colorChannel3->setEnabled(true);
    }
    else if(imgColorSpace=="GRAY")
    {
        colorChannelSelectLabel->setText("Color Space:GRAY");
        colorChannelAll->setText("ALL");
        colorChannel1->setText("");
        colorChannel2->setText("");
        colorChannel3->setText("");
        colorChannelAll->setEnabled(true);
        colorChannelAll->setChecked(true);
        colorChannel1->setEnabled(false);
        colorChannel2->setEnabled(false);
        colorChannel3->setEnabled(false);
    }
}
void histEqualizationWidget::OnBtnStartTriggered(void)
{
    if(imgColorSpace=="None")
    {
        QMessageBox::information(this, tr("Error"), tr("You didn't load any image!"));
        return;
    }
    if(imgColorSpace=="GRAY")
    {
        cv::Mat src=*imagemat;

        cv::equalizeHist(src,src);

        *imagemat = src;
        *image = cvMat2QImage(src);
    }
    else
    {
        cv::Mat src=*imagemat;
        cv::Mat rgb[3];
        cv::split(src,rgb);
        if(colorChannelAll->isChecked())
        {
            cv::equalizeHist(rgb[0],rgb[0]);
            cv::equalizeHist(rgb[1],rgb[1]);
            cv::equalizeHist(rgb[2],rgb[2]);
            if(imgColorSpace=="HSV" || imgColorSpace=="HSI" || imgColorSpace=="HLS")
            {
                double temp;
                for (int row = 0;row < rgb[0].rows;row++)
                {
                    for (int col = 0;col < rgb[0].cols;col++)
                    {
                        temp = rgb[0].at<uchar>(row,col);
                        rgb[0].at<uchar>(row,col) = cv::saturate_cast<uchar>(temp*180/255);
                    }
                }
            }
        }
        else if(colorChannel1->isChecked())
        {
            cv::equalizeHist(rgb[0],rgb[0]);
            if(imgColorSpace=="HSV" || imgColorSpace=="HSI" || imgColorSpace=="HLS")
            {
                double temp;
                for (int row = 0;row < rgb[0].rows;row++)
                {
                    for (int col = 0;col < rgb[0].cols;col++)
                    {
                        temp = rgb[0].at<uchar>(row,col);
                        rgb[0].at<uchar>(row,col) = cv::saturate_cast<uchar>(temp*180/255);
                    }
                }
            }
        }
        else if(colorChannel2->isChecked())
        {
            cv::equalizeHist(rgb[1],rgb[1]);
        }
        else if(colorChannel3->isChecked())
        {
            cv::equalizeHist(rgb[2],rgb[2]);
        }
        cv::merge(rgb,3,src);
        *imagemat = src;
        *image = cvMat2QImage(src);
    }

    emit histEqualizationtoExecute();
}
