#include "sharpenwidget.h"

#include <QDebug>
#include <QMessageBox>
#include <QIntValidator>

sharpenWidget::sharpenWidget(QWidget *parent) : QWidget(parent)
{
    this->parent = parent;

    instantiationWidgets();
    initWidgets();
}

void sharpenWidget::instantiationWidgets(void)
{
    TipsLabel = new QLabel(this);
    TipsLabel->setWordWrap(true);
    TipsLabel->setText("Choose a Filter and set parameter of it, then press \"start\".");
    KernelSize = new QLabel(this);
    KernelSize->setText("Kernel size");
    KernelSizeEdit = new QLineEdit(this);
    KernelSizeEdit->setMaximumWidth(30);
    KernelSizeEdit->setValidator(new QIntValidator(1, 31, this));
    KernelXOrder = new QLabel(this);
    KernelXOrder->setText("Order of corresponding derivative in x");
    KernelXOrderEdit = new QLineEdit(this);
    KernelXOrderEdit->setMaximumWidth(30);
    KernelXOrderEdit->setValidator(new QIntValidator(0, 2, this));
    KernelYOrder = new QLabel(this);
    KernelYOrder->setText("Order of corresponding derivative in y");
    KernelYOrderEdit = new QLineEdit(this);
    KernelYOrderEdit->setMaximumWidth(30);
    KernelYOrderEdit->setValidator(new QIntValidator(0, 2, this));
    FilterChoose = new QLabel(this);
    FilterChoose->setText("Filter");
    FilterChooseEdit = new QComboBox(this);
    FilterChooseEdit->addItem("Sobel");
    FilterChooseEdit->addItem("Scharr");
    FilterChooseEdit->addItem("Laplacian");
    FilterChooseEdit->addItem("Custom Filter");
    KeepColorCheckBox = new QCheckBox(this);
    KeepColorCheckBox->setText("Keep color");
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
void sharpenWidget::initWidgets(void)
{
    layout->addWidget(TipsLabel,0,0,1,4);

    layout->addWidget(FilterChoose,1,0,1,2,Qt::AlignBottom);
    layout->addWidget(FilterChooseEdit,2,0,1,2);
    layout->addWidget(KernelSize,3,0);
    layout->addWidget(KernelSizeEdit,3,1);
    layout->addWidget(KernelXOrder,4,0);
    layout->addWidget(KernelXOrderEdit,4,1);
    layout->addWidget(KernelYOrder,5,0);
    layout->addWidget(KernelYOrderEdit,5,1);
    layout->addWidget(BorderChoose,6,0,1,2);
    layout->addWidget(BorderChooseEdit,7,0,1,2);
    layout->addWidget(CustomKernel,1,2,1,2,Qt::AlignBottom);
    layout->addWidget(CustomKernelEdit,2,2,5,2);
    layout->addWidget(KeepColorCheckBox,7,2);
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
void sharpenWidget::OnFilterChanged(void)
{
    if(FilterChooseEdit->currentText()=="Sobel")
    {
        KernelSize->setText("Kernel size");
        KernelSizeEdit->setEnabled(true);
        KernelXOrder->setText("Order of corresponding derivative in x");
        KernelXOrderEdit->setEnabled(true);
        KernelXOrderEdit->setValidator(new QIntValidator(0, 2, this));
        KernelYOrder->setText("Order of corresponding derivative in y");
        KernelYOrderEdit->setEnabled(true);
        KernelYOrderEdit->setValidator(new QIntValidator(0, 2, this));
        CustomKernelEdit->setEnabled(false);
    }
    else if(FilterChooseEdit->currentText()=="Scharr")
    {
        KernelSize->setText("Kernel size");
        KernelSizeEdit->setEnabled(false);
        KernelXOrder->setText("Order of corresponding derivative in x");
        KernelXOrderEdit->setEnabled(true);
        KernelXOrderEdit->setValidator(new QIntValidator(0, 2, this));
        KernelYOrder->setText("Order of corresponding derivative in y");
        KernelYOrderEdit->setEnabled(true);
        KernelYOrderEdit->setValidator(new QIntValidator(0, 2, this));
        CustomKernelEdit->setEnabled(false);

    }
    else if(FilterChooseEdit->currentText()=="Laplacian")
    {
        KernelSize->setText("Kernel size");
        KernelSizeEdit->setEnabled(true);
        KernelXOrder->setText("Order of corresponding derivative in x");
        KernelXOrderEdit->setEnabled(false);
        KernelXOrderEdit->setValidator(new QIntValidator(0, 2, this));
        KernelYOrder->setText("Order of corresponding derivative in y");
        KernelYOrderEdit->setEnabled(false);
        KernelYOrderEdit->setValidator(new QIntValidator(0, 2, this));
        CustomKernelEdit->setEnabled(false);
    }
    else if(FilterChooseEdit->currentText()=="Custom Filter")
    {
        KernelSize->setText("");
        KernelSizeEdit->setEnabled(false);
        KernelXOrder->setText("Kernel width");
        KernelXOrderEdit->setEnabled(true);
        KernelXOrderEdit->setValidator(new QIntValidator(1, 101, this));
        KernelYOrder->setText("Kernal height");
        KernelYOrderEdit->setEnabled(true);
        KernelYOrderEdit->setValidator(new QIntValidator(1, 101, this));
        CustomKernelEdit->setEnabled(true);
    }
}
void sharpenWidget::OnBtnStartTriggered(void)
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

    if(FilterChooseEdit->currentText()=="Sobel")
    {
        int xorder = KernelXOrderEdit->text().toInt();
        int yorder = KernelYOrderEdit->text().toInt();
        int size = KernelSizeEdit->text().toInt();
        if(xorder<0 || xorder>2 || yorder<0 || yorder>2)
        {
            QMessageBox::information(this, tr("Error"), tr("Please enter vaild number for order in x and y!"));
            return;
        }
        if(size<1 || size>31 || size%2!=1)
        {
            QMessageBox::information(this, tr("Error"), tr("Please enter a vaild kernel size!"));
            return;
        }
        cv::Sobel(*imagemat,dst,CV_8U,xorder,yorder,size,1,0,borderpolicy);

        if(KeepColorCheckBox->isChecked())
        {
            cv::add(*imagemat,dst,dst);
        }
    }
    else if(FilterChooseEdit->currentText()=="Scharr")
    {
        int xorder = KernelXOrderEdit->text().toInt();
        int yorder = KernelYOrderEdit->text().toInt();

        if(xorder<0 || xorder>1 || yorder<0 || yorder>1 || xorder*yorder!=0)
        {
            QMessageBox::information(this, tr("Error"), tr("Please enter vaild number for order in x and y!"));
            return;
        }

        cv::Scharr(*imagemat,dst,CV_8U,xorder,yorder,1,0,borderpolicy);

        if(KeepColorCheckBox->isChecked())
        {
            cv::add(*imagemat,dst,dst);
        }
    }
    else if(FilterChooseEdit->currentText()=="Laplacian")
    {
        int size = KernelSizeEdit->text().toInt();
        if(size<1 || size>31 || size%2!=1)
        {
            QMessageBox::information(this, tr("Error"), tr("Please enter a vaild kernel size!"));
            return;
        }
        cv::Laplacian(*imagemat,dst,CV_8U,size,1,0,borderpolicy);

        if(KeepColorCheckBox->isChecked())
        {
            cv::add(*imagemat,dst,dst);
        }
    }
    else if(FilterChooseEdit->currentText()=="Custom Filter")
    {
        int width = KernelXOrderEdit->text().toInt();
        int height = KernelYOrderEdit->text().toInt();
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
    emit sharpentoExecute();
}
