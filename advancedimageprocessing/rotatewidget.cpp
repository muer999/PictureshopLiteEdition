#include "rotatewidget.h"

#include <QDebug>
#include <QMessageBox>
#include <math.h>

rotateWidget::rotateWidget(QWidget *parent) : QWidget(parent)
{
    this->parent = parent;

    instantiationWidgets();
    initWidgets();
}

void rotateWidget::instantiationWidgets(void)
{
    TipsLabel = new QLabel(this);
    TipsLabel->setWordWrap(true);
    TipsLabel->setText("Enter the rotation angle in the spinbox and press \"start\". Enter background color in RGB format (R,G,B) to spicify the color of unfilled areas.");
    AngelLabel = new QLabel(this);
    AngelLabel->setText("Angel(degree):");
    BackgroundColorLabel = new QLabel(this);
    BackgroundColorLabel->setText("Set background color(Optional):");
    clockwise = new QRadioButton(this);
    clockwise->setText("Clockwise");
    clockwise->setChecked(true);
    counterclockwise = new QRadioButton(this);
    counterclockwise->setText("Counterclockwise");
    DirectionSelect = new QButtonGroup(this);
    DirectionSelect->setExclusive(true);
    DirectionSelect->addButton(clockwise);
    DirectionSelect->addButton(counterclockwise);
    BackgroundColor = new QLineEdit(this);
    Angel = new QSpinBox(this);
    Angel->setMaximum(359);
    Angel->setMinimum(0);
    Angel->setValue(0);

    start = new QPushButton(this);
    start->setText("Start");

    layout = new QGridLayout(this);
}
void rotateWidget::initWidgets(void)
{
    layout->addWidget(TipsLabel,0,0,1,2);
    layout->addWidget(AngelLabel,1,0);
    layout->addWidget(Angel,1,1);
    layout->addWidget(clockwise,2,0);
    layout->addWidget(counterclockwise,2,1);
    layout->addWidget(BackgroundColorLabel,3,0);
    layout->addWidget(BackgroundColor,3,1);
    layout->addWidget(start,4,1);
    layout->setRowStretch(0,2);
    layout->setRowStretch(1,1);
    layout->setRowStretch(2,1);
    layout->setRowStretch(3,1);
    layout->setRowStretch(4,1);

    connect(start, SIGNAL(clicked(bool)), this, SLOT(OnBtnStartTriggered()));
}
void rotateWidget::OnBtnStartTriggered(void)
{
    if(imgColorSpace=="None")
    {
        QMessageBox::information(this, tr("Error"), tr("You didn't load any image!"));
        return;
    }
    cv::Mat src = *imagemat;
    if(imgColorSpace=="GRAY")
    {
        cv::cvtColor(src,src,cv::COLOR_GRAY2BGR);
    }
    int r,g,b;
    if(BackgroundColor->text()=="")
        BackgroundColor->setText("0,0,0");
    QStringList list = BackgroundColor->text().split(",");
    if(list.length()!=3)
    {
        QMessageBox::information(this, tr("Error"), tr("Please enter a vaild RGB value!"));
        return;
    }
    r=list.at(0).toInt();
    g=list.at(1).toInt();
    b=list.at(2).toInt();
    if(r>255||r<0||g>255||g<0||b>255||b<0)
    {
        QMessageBox::information(this, tr("Error"), tr("Please enter a vaild RGB value!"));
        return;
    }



    int degree = Angel->value();
    if(clockwise->isChecked())
        degree=-degree;
    int width = src.cols;
    int height = src.rows;
    cv::Point2f center = cv::Point2f(width / 2, height / 2);
    cv::Mat map_matrix = cv::getRotationMatrix2D(center, degree, 1.0);

    double cos = fabs(map_matrix.at<double>(0,0));
    double sin = fabs(map_matrix.at<double>(0,1));

    int nW = int(height*sin+width*cos);
    int nH = int(height*cos+width*sin);

    map_matrix.at<double>(0,2) += nW/2 - width/2;
    map_matrix.at<double>(1,2) += nH/2 - height/2;

    cv::warpAffine(src, src, map_matrix, cv::Size(nW, nH), 1, 0, cv::Scalar(r,g,b));

    if(imgColorSpace=="GRAY")
    {
        cv::cvtColor(src,src,cv::COLOR_BGR2GRAY);
    }
    *imagemat = src;
    *image = cvMat2QImage(src);
    emit rotatetoExecute();
}
