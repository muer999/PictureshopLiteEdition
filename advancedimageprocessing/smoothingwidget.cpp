#include "smoothingwidget.h"

#include <QDebug>
#include <QMessageBox>
#include <QIntValidator>

smoothingWidget::smoothingWidget(QWidget *parent) : QWidget(parent)
{
    this->parent = parent;

    instantiationWidgets();
    initWidgets();
}

void smoothingWidget::instantiationWidgets(void)
{
    TipsLabel = new QLabel(this);
    TipsLabel->setWordWrap(true);
    TipsLabel->setText("Choose a Filter and set size of it, then press \"start\".");
    KernelWidth = new QLabel(this);
    KernelWidth->setText("Kernel width");
    KernelHeight = new QLabel(this);
    KernelHeight->setText("Kernel height");
    KernelWidthEdit = new QLineEdit(this);
    KernelWidthEdit->setValidator(new QIntValidator(1, 101, this));
    KernelWidthEdit->setMaximumWidth(30);
    KernelHeightEdit = new QLineEdit(this);
    KernelHeightEdit->setValidator(new QIntValidator(1, 101, this));
    KernelHeightEdit->setMaximumWidth(30);
    OtherKernelpara = new QLabel(this);
    OtherKernelpara->setText("");
    OtherKernelparaEdit = new QLineEdit(this);
    OtherKernelparaEdit->setValidator(new QIntValidator(1, 100, this));
    OtherKernelparaEdit->setMaximumWidth(30);
    OtherKernelparaEdit->setEnabled(false);
    FilterChoose = new QLabel(this);
    FilterChoose->setText("Filter");
    FilterChooseEdit = new QComboBox(this);
    FilterChooseEdit->addItem("Box Filter");
    FilterChooseEdit->addItem("Median Filter");
    FilterChooseEdit->addItem("Gaussian Filter");
    FilterChooseEdit->addItem("Bilateral Filter");
    FilterChooseEdit->addItem("Custom Filter");
    BorderChoose = new QLabel(this);
    BorderChoose->setText("Border policy");
    BorderChooseEdit = new QComboBox(this);
    BorderChooseEdit->addItem("Reflect 101");
    BorderChooseEdit->addItem("Reflect");
    BorderChooseEdit->addItem("Replicate");
    //BorderChooseEdit->addItem("Wrap");
    BorderChooseEdit->addItem("Constant");
    CustomKernel = new QLabel(this);
    CustomKernel->setText("Custom Kernel");
    CustomKernelEdit = new QTextEdit(this);
    CustomKernelEdit->setWordWrapMode(QTextOption::NoWrap);
    CustomKernelEdit->setEnabled(false);

    start = new QPushButton(this);
    start->setText("Start");

    layout = new QGridLayout(this);
}
void smoothingWidget::initWidgets(void)
{
    layout->addWidget(TipsLabel,0,0,1,4);
    layout->addWidget(FilterChoose,1,0,1,2,Qt::AlignBottom);
    layout->addWidget(FilterChooseEdit,2,0,1,2);
    layout->addWidget(KernelWidth,3,0);
    layout->addWidget(KernelWidthEdit,3,1);
    layout->addWidget(KernelHeight,4,0);
    layout->addWidget(KernelHeightEdit,4,1);
    layout->addWidget(OtherKernelpara,5,0);
    layout->addWidget(OtherKernelparaEdit,5,1);
    layout->addWidget(CustomKernel,1,2,1,2,Qt::AlignBottom);
    layout->addWidget(CustomKernelEdit,2,2,6,2);
    layout->addWidget(BorderChoose,6,0,1,2);
    layout->addWidget(BorderChooseEdit,7,0,1,2);
    layout->addWidget(start,8,0,1,2);
    layout->setRowStretch(0,2);
    layout->setRowStretch(1,1);
    layout->setRowStretch(2,1);
    layout->setRowStretch(3,1);
    layout->setRowStretch(4,1);
    layout->setRowStretch(5,1);
    layout->setRowStretch(6,1);
    layout->setRowStretch(7,1);
    layout->setRowStretch(8,1);

    connect(start, SIGNAL(clicked(bool)), this, SLOT(OnBtnStartTriggered()));
    connect(FilterChooseEdit,SIGNAL(currentTextChanged(QString)),this,SLOT(OnFilterChanged()));
}
void smoothingWidget::OnFilterChanged(void)
{
    if(FilterChooseEdit->currentText()=="Box Filter")
    {
        KernelHeight->setText("Kernel height");
        KernelHeightEdit->setEnabled(true);
        KernelWidth->setText("Kernel width");
        KernelWidthEdit->setEnabled(true);
        CustomKernelEdit->setEnabled(false);
        BorderChooseEdit->setEnabled(true);
        OtherKernelparaEdit->setEnabled(false);
        OtherKernelpara->setText("");
    }
    else if(FilterChooseEdit->currentText()=="Median Filter")
    {
        KernelHeight->setText("");
        KernelHeightEdit->setEnabled(false);
        KernelWidth->setText("Kernel size");
        KernelWidthEdit->setEnabled(true);
        CustomKernelEdit->setEnabled(false);
        BorderChooseEdit->setEnabled(false);
        OtherKernelparaEdit->setEnabled(false);
        OtherKernelpara->setText("");
    }
    else if(FilterChooseEdit->currentText()=="Gaussian Filter")
    {
        KernelHeight->setText("Kernel height");
        KernelHeightEdit->setEnabled(true);
        KernelWidth->setText("Kernel width");
        KernelWidthEdit->setEnabled(true);
        CustomKernelEdit->setEnabled(false);
        BorderChooseEdit->setEnabled(true);
        OtherKernelparaEdit->setEnabled(false);
        OtherKernelpara->setText("");
    }
    else if(FilterChooseEdit->currentText()=="Bilateral Filter")
    {
        KernelHeight->setText("SigmaColor");
        KernelHeightEdit->setEnabled(true);
        KernelWidth->setText("Neighborhood size");
        KernelWidthEdit->setEnabled(true);
        CustomKernelEdit->setEnabled(false);
        BorderChooseEdit->setEnabled(true);
        OtherKernelparaEdit->setEnabled(true);
        OtherKernelpara->setText("SigmaSpace");
    }
    else if(FilterChooseEdit->currentText()=="Custom Filter")
    {
        KernelHeight->setText("Kernel height");
        KernelHeightEdit->setEnabled(true);
        KernelWidth->setText("Kernel width");
        KernelWidthEdit->setEnabled(true);
        CustomKernelEdit->setEnabled(true);
        BorderChooseEdit->setEnabled(true);
        OtherKernelparaEdit->setEnabled(false);
        OtherKernelpara->setText("");
    }
}
void smoothingWidget::OnBtnStartTriggered(void)
{
    if(imgColorSpace=="None")
    {
        QMessageBox::information(this, tr("Error"), tr("You didn't load any image!"));
        return;
    }

    cv::Mat dst;

    int borderpolicy;
    if(BorderChooseEdit->currentText()=="Reflect 101")
        borderpolicy=cv::BORDER_REFLECT_101;
    else if(BorderChooseEdit->currentText()=="Reflect")
        borderpolicy=cv::BORDER_REFLECT;
    else if(BorderChooseEdit->currentText()=="Replicate")
        borderpolicy=cv::BORDER_REPLICATE;
    else if(BorderChooseEdit->currentText()=="Wrap")
        borderpolicy=cv::BORDER_WRAP;
    else if(BorderChooseEdit->currentText()=="Constant")
        borderpolicy=cv::BORDER_CONSTANT;
    else
        borderpolicy=cv::BORDER_DEFAULT;

    if(FilterChooseEdit->currentText()=="Box Filter")
    {
        int width = KernelWidthEdit->text().toInt();
        int height = KernelHeightEdit->text().toInt();
        if(width<1 || width>101 || width%2!=1 || height<1 || height>101 || height%2!=1)
        {
            QMessageBox::information(this, tr("Error"), tr("Please enter a vaild kernel size!"));
            return;
        }
        cv::Size size(width,height);

        cv::blur(*imagemat,dst,size,cv::Point(-1,-1),borderpolicy);
    }
    else if(FilterChooseEdit->currentText()=="Median Filter")
    {
        int width = KernelWidthEdit->text().toInt();
        if(width<1 || width>101 || width%2!=1)
        {
            QMessageBox::information(this, tr("Error"), tr("Please enter a vaild kernel size!"));
            return;
        }

        cv::medianBlur(*imagemat,dst,width);
    }
    else if(FilterChooseEdit->currentText()=="Gaussian Filter")
    {
        int width = KernelWidthEdit->text().toInt();
        int height = KernelHeightEdit->text().toInt();
        if(width<1 || width>101 || width%2!=1 || height<1 || height>101 || height%2!=1)
        {
            QMessageBox::information(this, tr("Error"), tr("Please enter a vaild kernel size!"));
            return;
        }
        cv::Size size(width,height);

        cv::GaussianBlur(*imagemat,dst,size,0,0,borderpolicy);
    }
    else if(FilterChooseEdit->currentText()=="Bilateral Filter")
    {
        int neighborhood = KernelWidthEdit->text().toInt();
        int sigmaColor = KernelHeightEdit->text().toInt();
        int sigmaSpace = OtherKernelparaEdit->text().toInt();
        if(neighborhood<0 || neighborhood>200 || sigmaColor<0 || sigmaColor>200 || sigmaSpace<-1 || sigmaSpace>200)
        {
            QMessageBox::information(this, tr("Error"), tr("Please enter a vaild kernel size!"));
            return;
        }

        cv::bilateralFilter(*imagemat,dst,neighborhood,sigmaColor,sigmaSpace,borderpolicy);
    }
    else if(FilterChooseEdit->currentText()=="Custom Filter")
    {
        int width = KernelWidthEdit->text().toInt();
        int height = KernelHeightEdit->text().toInt();
        if(width<1 || width>101 || width%2!=1 || height<1 || height>101 || height%2!=1)
        {
            QMessageBox::information(this, tr("Error"), tr("Please enter a vaild kernel size!"));
            return;
        }
        cv::Size size(width,height);

        QString kernaltext = CustomKernelEdit->toPlainText();
        kernaltext.replace('\n',',');
        kernaltext.replace(' ',',');
        QStringList list = kernaltext.split(",");
        int length=width*height;
        if(list.length()!=length)
        {
            QMessageBox::information(this, tr("Error"), tr("Please enter a vaild kernel!"));
            return;
        }
        cv::Mat kernal(height, width, CV_64F, cv::Scalar(0));
        for(int i=0;i<length;i++)
        {
            kernal.at<double>(i)=list.at(i).toDouble();
        }
        cv::filter2D(*imagemat,dst,CV_8U,kernal,cv::Point(-1,-1),0,borderpolicy);
    }
    *imagemat = dst;
    *image = cvMat2QImage(dst);
    emit smoothingtoExecute();
}
