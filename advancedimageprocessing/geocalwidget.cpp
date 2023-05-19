#include "geocalwidget.h"

#include <QDebug>
#include <QMessageBox>
#include <math.h>

geometricCalibrationWidget::geometricCalibrationWidget(QWidget *parent) : QWidget(parent)
{
    this->parent = parent;

    instantiationWidgets();
    initWidgets();
}

void geometricCalibrationWidget::instantiationWidgets(void)
{
    TipsLabel = new QLabel(this);
    TipsLabel->setText("Input the coordinate of the four points and then press \"start\".");
    TipsLabel->setWordWrap(true);
    LeftTopPointCheckBox = new QCheckBox(this);
    LeftTopPointCheckBox->setText("Left Top point");
    LeftTopPointEdit = new QLineEdit(this);
    RightTopPointCheckBox = new QCheckBox(this);
    RightTopPointCheckBox->setText("Right Top point");
    RightTopPointEdit = new QLineEdit(this);
    LeftBottomPointCheckBox = new QCheckBox(this);
    LeftBottomPointCheckBox->setText("Left Bottom point");
    LeftBottomPointEdit = new QLineEdit(this);
    RightBottomPointCheckBox = new QCheckBox(this);
    RightBottomPointCheckBox->setText("Right Bottom point");
    RightBottomPointEdit = new QLineEdit(this);
    PointSelectionButtonGroup = new QButtonGroup(this);
    PointSelectionButtonGroup->addButton(LeftTopPointCheckBox);
    PointSelectionButtonGroup->addButton(RightTopPointCheckBox);
    PointSelectionButtonGroup->addButton(LeftBottomPointCheckBox);
    PointSelectionButtonGroup->addButton(RightBottomPointCheckBox);
    PointSelectionButtonGroup->setExclusive(true);
    start = new QPushButton(this);
    start->setText("Start");

    layout = new QGridLayout(this);
}
void geometricCalibrationWidget::initWidgets(void)
{
    layout->addWidget(TipsLabel,0,0,1,2);
    layout->addWidget(LeftTopPointCheckBox,1,0);
    layout->addWidget(LeftTopPointEdit,1,1);
    layout->addWidget(RightTopPointCheckBox,2,0);
    layout->addWidget(RightTopPointEdit,2,1);
    layout->addWidget(LeftBottomPointCheckBox,3,0);
    layout->addWidget(LeftBottomPointEdit,3,1);
    layout->addWidget(RightBottomPointCheckBox,4,0);
    layout->addWidget(RightBottomPointEdit,4,1);
    layout->addWidget(start,5,0);
    layout->setRowStretch(0,3);
    layout->setRowStretch(1,1);
    layout->setRowStretch(2,1);
    layout->setRowStretch(3,1);
    layout->setRowStretch(4,1);
    layout->setColumnStretch(0,1);
    layout->setColumnStretch(1,1);

    connect(start, SIGNAL(clicked(bool)), this, SLOT(OnBtnStartTriggered()));
}
void geometricCalibrationWidget::OnBtnStartTriggered(void)
{
    if(imgColorSpace=="None")
    {
        QMessageBox::information(this, tr("Error"), tr("You didn't load any image!"));
        return;
    }

    cv::Mat src = *imagemat;

    /*cv::Mat gray_src,binary,dst;
    cv::cvtColor(src,gray_src,cv::COLOR_BGR2GRAY);
    cv::threshold(gray_src,binary,0,255,cv::THRESH_BINARY_INV|cv::THRESH_OTSU);
    //cv::imshow("binary image",binary);


    qDebug()<<"1";

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(5,5),cv::Point(-1,-1));
    cv::morphologyEx(binary,dst,cv::MORPH_CLOSE,kernel,cv::Point(-1,-1),3);
    //cv::imshow("morphology",dst);

    qDebug()<<"2";

    cv::bitwise_not(dst,dst,cv::Mat());
    std::vector<std::vector<cv::Point>>contours;
    std::vector<cv::Vec4i>hireachy;
    cv::findContours(dst,contours,hireachy,cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE,cv::Point());

    qDebug()<<"3";

    int width = src.cols;
    int height = src.rows;
    cv::Mat drawImage = cv::Mat::zeros(src.size(),CV_8UC3);
    for(std::size_t t=0;t<contours.size();t++)
    {
        cv::Rect rect = cv::boundingRect(contours[t]);
        if(rect.width>width/2 && rect.height>height/2)
        {
            cv::drawContours(drawImage,contours,static_cast<int>(t),cv::Scalar(0,0,255),2,8,hireachy,0,cv::Point());
        }
    }
    //cv::imshow("contours",drawImage);

    qDebug()<<"4";

    std::vector<cv::Vec4i> lines;
    cv::Mat contoursImg;
    int accu = cv::min(width/2,height/2);
    cv::cvtColor(drawImage,contoursImg,cv::COLOR_BGR2GRAY);
    cv::HoughLinesP(contoursImg,lines,1,CV_PI/180.0,accu,accu,0);
    cv::Mat linesImage = cv::Mat::zeros(src.size(),CV_8UC3);
    for(std::size_t t=0;t<lines.size();t++)
    {
        cv::Vec4i ln = lines[t];
        cv::line(linesImage,cv::Point(ln[0],ln[1]),cv::Point(ln[2],ln[3]),cv::Scalar(0,0,255),2,8,0);
    }
    qDebug()<<"number of lines:"<<lines.size();
    //cv::imshow("lines image",linesImage);

    qDebug()<<"5";

    int deltah = 0;
    cv::Vec4i topLine,bottomLine;
    cv::Vec4i leftLine,rightLine;
    for(int i=0;i<lines.size();i++)
    {
        cv::Vec4i ln = lines[i];
        deltah = abs(ln[3]-ln[1]);
        if(ln[3]<height/2.0 && ln[1]<height/2.0 && deltah<accu-1)
        {
            topLine = lines[i];
        }
        if(ln[3]>height/2.0 && ln[1]>height/2.0 && deltah<accu-1)
        {
            bottomLine = lines[i];
        }
        if(ln[0]<width/2.0 && ln[2]<width/2.0)
        {
            leftLine = lines[i];
        }
        if(ln[0]>width/2.0 && ln[2]>width/2.0)
        {
            rightLine = lines[i];
        }
    }

    qDebug()<<"6";

    qDebug()<<"top line:p1(x,y)="<<topLine[0]<<","<<topLine[1]<<"p2(x,y)="<<topLine[2]<<","<<topLine[3];
    qDebug()<<"bottom line:p1(x,y)="<<bottomLine[0]<<","<<bottomLine[1]<<"p2(x,y)="<<bottomLine[2]<<","<<bottomLine[3];
    qDebug()<<"left line:p1(x,y)="<<leftLine[0]<<","<<leftLine[1]<<"p2(x,y)="<<leftLine[2]<<","<<leftLine[3];
    qDebug()<<"right line:p1(x,y)="<<rightLine[0]<<","<<rightLine[1]<<"p2(x,y)="<<rightLine[2]<<","<<rightLine[3];

    float k1,c1;
    k1 = float(topLine[3]-topLine[1])/float(topLine[2]-topLine[0]);
    c1 = topLine[1]-k1*topLine[0];
    qDebug()<<"k1="<<k1<<",c1="<<c1;
    float k2,c2;
    k2 = float(bottomLine[3]-bottomLine[1])/float(bottomLine[2]-bottomLine[0]);
    c2 = bottomLine[1]-k2*bottomLine[0];
    qDebug()<<"k2="<<k2<<",c2="<<c2;
    float k3,c3;
    k3 = float(leftLine[3]-leftLine[1])/float(leftLine[2]-leftLine[0]);
    c3 = leftLine[1]-k3*leftLine[0];
    qDebug()<<"k3="<<k3<<",c3="<<c3;
    float k4,c4;
    k4 = float(rightLine[3]-rightLine[1])/float(rightLine[2]-rightLine[0]);
    c4 = rightLine[1]-k4*rightLine[0];
    qDebug()<<"k4="<<k4<<",c4="<<c4;

    qDebug()<<"7";

    cv::Point p1;
    p1.x = static_cast<int>((c1-c3)/(k3-k1));
    p1.y = static_cast<int>(k1*p1.x+c1);
    cv::Point p2;
    p2.x = static_cast<int>((c1-c4)/(k4-k1));
    p2.y = static_cast<int>(k1*p2.x+c1);
    cv::Point p3;
    p3.x = static_cast<int>((c2-c3)/(k3-k2));
    p3.y = static_cast<int>(k2*p3.x+c2);
    cv::Point p4;
    p4.x = static_cast<int>((c2-c4)/(k4-k2));
    p4.y = static_cast<int>(k2*p4.x+c2);

    qDebug()<<"8";

    qDebug()<<"p1(x,y)="<<p1.x<<","<<p1.y;
    qDebug()<<"p2(x,y)="<<p2.x<<","<<p2.y;
    qDebug()<<"p3(x,y)="<<p3.x<<","<<p3.y;
    qDebug()<<"p4(x,y)="<<p4.x<<","<<p4.y;

    cv::circle(linesImage,p1,2,cv::Scalar(255,0,0),2,8,0);
    cv::circle(linesImage,p2,2,cv::Scalar(255,0,0),2,8,0);
    cv::circle(linesImage,p3,2,cv::Scalar(255,0,0),2,8,0);
    cv::circle(linesImage,p4,2,cv::Scalar(255,0,0),2,8,0);

    qDebug()<<"9";*/

    //cv::imshow("four corners",linesImage);

    int width = src.cols;
    int height = src.rows;
    cv::Point p1,p2,p3,p4;
    int temp=0;
    temp=temp+GetCoordinateFromLineEdit(LeftTopPointEdit,p1);
    temp=temp+GetCoordinateFromLineEdit(RightTopPointEdit,p2);
    temp=temp+GetCoordinateFromLineEdit(LeftBottomPointEdit,p3);
    temp=temp+GetCoordinateFromLineEdit(RightBottomPointEdit,p4);
    if(temp!=0)return;

    std::vector<cv::Point2f>src_corners(4);
    src_corners[0]=p1;
    src_corners[1]=p2;
    src_corners[2]=p3;
    src_corners[3]=p4;

    std::vector<cv::Point2f>dst_corners(4);
    dst_corners[0]=cv::Point(0,0);
    dst_corners[1]=cv::Point(width,0);
    dst_corners[2]=cv::Point(0,height);
    dst_corners[3]=cv::Point(width,height);

    /*qDebug()<<"src_corners[0]="<<p1.x<<","<<p1.y;
    qDebug()<<"src_corners[1]="<<p2.x<<","<<p2.y;
    qDebug()<<"src_corners[2]="<<p3.x<<","<<p3.y;
    qDebug()<<"src_corners[3]="<<p4.x<<","<<p4.y;
    qDebug()<<"dst_corners[0]="<<dst_corners[0].x<<","<<dst_corners[0].y;
    qDebug()<<"dst_corners[1]="<<dst_corners[1].x<<","<<dst_corners[1].y;
    qDebug()<<"dst_corners[2]="<<dst_corners[2].x<<","<<dst_corners[2].y;
    qDebug()<<"dst_corners[3]="<<dst_corners[3].x<<","<<dst_corners[3].y;*/

    cv::Mat resultImage;
    cv::Mat warpmatrix = cv::getPerspectiveTransform(src_corners,dst_corners);
    cv::warpPerspective(src,resultImage,warpmatrix,resultImage.size(),cv::INTER_LINEAR,cv::BORDER_CONSTANT);
    //cv::namedWindow("Final Result",cv::WINDOW_AUTOSIZE);
    //cv::imshow("Final Result",resultImage);

    *image=cvMat2QImage(resultImage);
    *imagemat=resultImage;

    emit geometricCalibrationtoExecute();
}
int geometricCalibrationWidget::GetCoordinateFromLineEdit(QLineEdit *PointEdit,cv::Point &point)
{
    QStringList list = PointEdit->text().split(",");
    if(list.length()!=2)
    {
        QMessageBox::information(this, tr("Error"), tr("Please enter a vaild coordinate!"));
        return -1;
    }
    point.x=list.at(0).toInt();
    point.y=list.at(1).toInt();
    if(point.x<0 || point.x>=image->width() || point.y<0 || point.y>=image->height())
    {
        QMessageBox::information(this, tr("Error"), tr("Location out of boundary!"));
        return -1;
    }
    return 0;
}
