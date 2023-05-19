#include "histpullwidget.h"

#include <QDebug>
#include <QMessageBox>
#include <QDoubleValidator>
#include <QIntValidator>
#include <math.h>

histPullWidget::histPullWidget(QWidget *parent) : QWidget(parent)
{
    this->parent = parent;

    instantiationWidgets();
    initWidgets();

}

void histPullWidget::instantiationWidgets(void)
{
    TipsLabel = new QLabel(this);
    TipsLabel->setText("Choose a mode and set parameters, then press \"start\" to continue.");
    TipsLabel->setWordWrap(true);
    FunctionChoose = new QLabel(this);
    FunctionChoose->setText("Choose function");
    FunctionChooseEdit = new QComboBox(this);
    FunctionChooseEdit->addItem("Linear");
    FunctionChooseEdit->addItem("Quadratic");
    FunctionChooseEdit->addItem("Logistic");
    FunctionChooseEdit->addItem("Piecewise");
    FunctionChooseEdit->addItem("In Range");
    reserveOtherChannels = new QCheckBox(this);
    reserveOtherChannels->setText("Reserve Other Channels");
    reserveOtherChannels->setChecked(true);
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

    LinearFunctionWidget = new class LinearFunctionWidget(this);
    QuadraticFunctionWidget = new class QuadraticFunctionWidget(this);
    LogisticFunctionWidget = new class LogisticFunctionWidget(this);
    PiecewiseFunctionWidget = new class PiecewiseFunctionWidget(this);
    inRangeFunctionWidget = new class inRangeFunctionWidget(this);

    start = new QPushButton(this);
    start->setText("Start");

    layout = new QGridLayout(this);
}
void histPullWidget::initWidgets(void)
{
    layout->addWidget(TipsLabel,0,0,1,5);
    layout->addWidget(FunctionChoose,1,0,1,1);
    layout->addWidget(FunctionChooseEdit,1,1,1,2);
    layout->addWidget(reserveOtherChannels,2,1,1,4);
    layout->addWidget(colorChannelSelectLabel,3,0);
    layout->addWidget(colorChannelAll,3,1);
    layout->addWidget(colorChannel1,3,2);
    layout->addWidget(colorChannel2,3,3);
    layout->addWidget(colorChannel3,3,4);
    layout->addWidget(LinearFunctionWidget,4,0,1,5);
    layout->addWidget(QuadraticFunctionWidget,4,0,1,5);
    layout->addWidget(LogisticFunctionWidget,4,0,1,5);
    layout->addWidget(PiecewiseFunctionWidget,4,0,1,5);
    layout->addWidget(inRangeFunctionWidget,4,0,1,5);
    layout->addWidget(start,5,0,1,2);

    layout->setRowStretch(0,2);
    layout->setRowStretch(1,1);
    layout->setRowStretch(2,1);
    layout->setRowStretch(3,1);
    layout->setRowStretch(4,5);
    layout->setRowStretch(5,1);

    LogisticFunctionWidget->hide();
    PiecewiseFunctionWidget->hide();
    inRangeFunctionWidget->hide();
    QuadraticFunctionWidget->hide();
    LinearFunctionWidget->show();

    connect(FunctionChooseEdit,SIGNAL(currentTextChanged(const QString)),this,SLOT(onFunctionChoosed()));
    connect(start, SIGNAL(clicked(bool)), this, SLOT(OnBtnStartTriggered()));
}
void histPullWidget::onFunctionChoosed(void)
{
    if(FunctionChooseEdit->currentText()=="Linear")
    {
        QuadraticFunctionWidget->hide();
        LogisticFunctionWidget->hide();
        PiecewiseFunctionWidget->hide();
        inRangeFunctionWidget->hide();
        LinearFunctionWidget->show();

        if(*imgColorSpace=="GRAY")
        {
            colorChannelAll->setEnabled(true);
            colorChannel1->setEnabled(false);
            colorChannel2->setEnabled(false);
            colorChannel3->setEnabled(false);

            colorChannelAll->setChecked(true);

            reserveOtherChannels->setEnabled(false);
        }
        else
        {
            colorChannelAll->setEnabled(true);
            colorChannel1->setEnabled(true);
            colorChannel2->setEnabled(true);
            colorChannel3->setEnabled(true);

            reserveOtherChannels->setEnabled(true);
        }

    }
    else if(FunctionChooseEdit->currentText()=="Quadratic")
    {
        LogisticFunctionWidget->hide();
        PiecewiseFunctionWidget->hide();
        inRangeFunctionWidget->hide();
        LinearFunctionWidget->hide();
        QuadraticFunctionWidget->show();

        if(*imgColorSpace=="GRAY")
        {
            colorChannelAll->setEnabled(true);
            colorChannel1->setEnabled(false);
            colorChannel2->setEnabled(false);
            colorChannel3->setEnabled(false);

            colorChannelAll->setChecked(true);

            reserveOtherChannels->setEnabled(false);
        }
        else
        {
            colorChannelAll->setEnabled(true);
            colorChannel1->setEnabled(true);
            colorChannel2->setEnabled(true);
            colorChannel3->setEnabled(true);

            reserveOtherChannels->setEnabled(true);
        }
    }
    else if(FunctionChooseEdit->currentText()=="Logistic")
    {
        PiecewiseFunctionWidget->hide();
        inRangeFunctionWidget->hide();
        LinearFunctionWidget->hide();
        QuadraticFunctionWidget->hide();
        LogisticFunctionWidget->show();
        if(*imgColorSpace=="GRAY")
        {
            colorChannelAll->setEnabled(true);
            colorChannel1->setEnabled(false);
            colorChannel2->setEnabled(false);
            colorChannel3->setEnabled(false);

            colorChannelAll->setChecked(true);

            reserveOtherChannels->setEnabled(false);
        }
        else
        {
            colorChannelAll->setEnabled(true);
            colorChannel1->setEnabled(true);
            colorChannel2->setEnabled(true);
            colorChannel3->setEnabled(true);

            reserveOtherChannels->setEnabled(true);
        }
    }
    else if(FunctionChooseEdit->currentText()=="Piecewise")
    {
        LogisticFunctionWidget->hide();
        inRangeFunctionWidget->hide();
        LinearFunctionWidget->hide();
        QuadraticFunctionWidget->hide();
        PiecewiseFunctionWidget->show();
        if(*imgColorSpace=="GRAY")
        {
            colorChannelAll->setEnabled(true);
            colorChannel1->setEnabled(false);
            colorChannel2->setEnabled(false);
            colorChannel3->setEnabled(false);

            colorChannelAll->setChecked(true);
        }
        else
        {
            colorChannelAll->setEnabled(false);
            colorChannel1->setEnabled(true);
            colorChannel2->setEnabled(true);
            colorChannel3->setEnabled(true);

            if(colorChannelAll->isChecked())
                colorChannel1->setChecked(true);
        }

        reserveOtherChannels->setEnabled(false);
    }
    else if(FunctionChooseEdit->currentText()=="In Range")
    {
        LogisticFunctionWidget->hide();
        PiecewiseFunctionWidget->hide();
        LinearFunctionWidget->hide();
        QuadraticFunctionWidget->hide();
        inRangeFunctionWidget->show();

        colorChannelAll->setEnabled(true);
        colorChannelAll->setChecked(true);
        colorChannel1->setEnabled(false);
        colorChannel2->setEnabled(false);
        colorChannel3->setEnabled(false);

        reserveOtherChannels->setEnabled(false);
    }
}
LinearFunctionWidget::LinearFunctionWidget(QWidget *parent) : QWidget(parent)
{
    PleaseEnterFunctionLabel = new QLabel(this);
    PleaseEnterFunctionLabel->setText("Please Enter Function:");
    LinearFunctionLabel1 = new QLabel(this);
    LinearFunctionLabel1->setText("f(x)=");
    LinearFunctionEdit1 = new QLineEdit(this);
    LinearFunctionEdit1->setValidator(new QDoubleValidator(-65535,65535,5,this));
    LinearFunctionEdit1->setMaximumWidth(30);
    LinearFunctionLabel2 = new QLabel(this);
    LinearFunctionLabel2->setText("x+");
    LinearFunctionEdit2 = new QLineEdit(this);
    LinearFunctionEdit2->setValidator(new QDoubleValidator(-65535,65535,5,this));
    LinearFunctionEdit2->setMaximumWidth(30);

    LinearFunctionLayout = new QGridLayout(this);
    LinearFunctionLayout->addWidget(PleaseEnterFunctionLabel,0,0,1,4);
    LinearFunctionLayout->addWidget(LinearFunctionLabel1,1,0);
    LinearFunctionLayout->addWidget(LinearFunctionEdit1,1,1);
    LinearFunctionLayout->addWidget(LinearFunctionLabel2,1,2);
    LinearFunctionLayout->addWidget(LinearFunctionEdit2,1,3);

    this->setMaximumSize(200,180);
    this->setMinimumSize(10,180);
}
QuadraticFunctionWidget::QuadraticFunctionWidget(QWidget *parent) : QWidget(parent)
{
    PleaseEnterFunctionLabel = new QLabel(this);
    PleaseEnterFunctionLabel->setText("Please Enter Function:");
    QuadraticFunctionLabel1 = new QLabel(this);
    QuadraticFunctionLabel1->setText("f(x)=");
    QuadraticFunctionEdit1 = new QLineEdit(this);
    QuadraticFunctionEdit1->setValidator(new QDoubleValidator(-65535,65535,5,this));
    QuadraticFunctionEdit1->setMaximumWidth(30);
    QuadraticFunctionLabel2 = new QLabel(this);
    QuadraticFunctionLabel2->setText("x^2+");
    QuadraticFunctionEdit2 = new QLineEdit(this);
    QuadraticFunctionEdit2->setValidator(new QDoubleValidator(-65535,65535,5,this));
    QuadraticFunctionEdit2->setMaximumWidth(30);
    QuadraticFunctionLabel3 = new QLabel(this);
    QuadraticFunctionLabel3->setText("x+");
    QuadraticFunctionEdit3 = new QLineEdit(this);
    QuadraticFunctionEdit3->setValidator(new QDoubleValidator(-65535,65535,5,this));
    QuadraticFunctionEdit3->setMaximumWidth(30);

    QuadraticFunctionLayout = new QGridLayout(this);
    QuadraticFunctionLayout->addWidget(PleaseEnterFunctionLabel,0,0,1,5);
    QuadraticFunctionLayout->addWidget(QuadraticFunctionLabel1,1,0);
    QuadraticFunctionLayout->addWidget(QuadraticFunctionEdit1,1,1);
    QuadraticFunctionLayout->addWidget(QuadraticFunctionLabel2,1,2);
    QuadraticFunctionLayout->addWidget(QuadraticFunctionEdit2,1,3);
    QuadraticFunctionLayout->addWidget(QuadraticFunctionLabel3,1,4);
    QuadraticFunctionLayout->addWidget(QuadraticFunctionEdit3,1,5);

    this->setMaximumSize(300,180);
    this->setMinimumSize(10,180);
}

LogisticFunctionWidget::LogisticFunctionWidget(QWidget *parent) : QWidget(parent)
{
    PleaseEnterFunctionLabel = new QLabel(this);
    PleaseEnterFunctionLabel->setText("Please Enter Function:");
    LogisticFunctionLabel1 = new QLabel(this);
    LogisticFunctionLabel1->setText("f(x)=");
    LogisticFunctionEdit1 = new QLineEdit(this);
    LogisticFunctionEdit1->setValidator(new QDoubleValidator(-65535,65535,5,this));
    LogisticFunctionEdit1->setMaximumWidth(30);
    LogisticFunctionLabel2 = new QLabel(this);
    LogisticFunctionLabel2->setText("log(");
    LogisticFunctionEdit2 = new QLineEdit(this);
    LogisticFunctionEdit2->setValidator(new QDoubleValidator(0,65535,5,this));
    LogisticFunctionEdit2->setMaximumWidth(30);
    LogisticFunctionLabel3 = new QLabel(this);
    LogisticFunctionLabel3->setText(")(");
    LogisticFunctionEdit3 = new QLineEdit(this);
    LogisticFunctionEdit3->setValidator(new QDoubleValidator(-65535,65535,5,this));
    LogisticFunctionEdit3->setMaximumWidth(30);
    LogisticFunctionLabel4 = new QLabel(this);
    LogisticFunctionLabel4->setText("x+");
    LogisticFunctionEdit4 = new QLineEdit(this);
    LogisticFunctionEdit4->setValidator(new QDoubleValidator(-65535,65535,5,this));
    LogisticFunctionEdit4->setMaximumWidth(30);
    LogisticFunctionLabel5 = new QLabel(this);
    LogisticFunctionLabel5->setText(")+");
    LogisticFunctionEdit5 = new QLineEdit(this);
    LogisticFunctionEdit5->setValidator(new QDoubleValidator(-65535,65535,5,this));
    LogisticFunctionEdit5->setMaximumWidth(30);

    LogisticFunctionLayout = new QGridLayout(this);
    LogisticFunctionLayout->addWidget(PleaseEnterFunctionLabel,0,0,1,8);
    LogisticFunctionLayout->addWidget(LogisticFunctionLabel1,1,0);
    LogisticFunctionLayout->addWidget(LogisticFunctionEdit1,1,1);
    LogisticFunctionLayout->addWidget(LogisticFunctionLabel2,1,2);
    LogisticFunctionLayout->addWidget(LogisticFunctionEdit2,1,3);
    LogisticFunctionLayout->addWidget(LogisticFunctionLabel3,1,4);
    LogisticFunctionLayout->addWidget(LogisticFunctionEdit3,1,5);
    LogisticFunctionLayout->addWidget(LogisticFunctionLabel4,1,6);
    LogisticFunctionLayout->addWidget(LogisticFunctionEdit4,1,7);
    LogisticFunctionLayout->addWidget(LogisticFunctionLabel5,1,8);
    LogisticFunctionLayout->addWidget(LogisticFunctionEdit5,1,9);

    this->setMaximumSize(450,180);
    this->setMinimumSize(10,180);
}

PiecewiseFunctionWidget::PiecewiseFunctionWidget(QWidget *parent) : QWidget(parent)
{
    PleaseEnterFunctionLabel = new QLabel(this);
    PleaseEnterFunctionLabel->setText("Please Enter Function:");
    PiecewiseFunctionLabel1 = new QLabel(this);
    PiecewiseFunctionLabel1->setText("f(x)=");
    PiecewiseFunctionLabel2 = new QLabel(this);
    PiecewiseFunctionLabel2->setText("0    ,");
    PiecewiseFunctionLabel3 = new QLabel(this);
    PiecewiseFunctionLabel3->setText("<= x <");
    PiecewiseFunctionLabel8 = new QLabel(this);
    PiecewiseFunctionLabel8->setText("0");
    PiecewiseFunctionLabel9 = new QLabel(this);
    PiecewiseFunctionLabel9->setText("");
    PiecewiseFunctionLabel4 = new QLabel(this);
    PiecewiseFunctionLabel4->setText("255  ,");
    PiecewiseFunctionLabel5 = new QLabel(this);
    PiecewiseFunctionLabel5->setText("<= x <=");
    PiecewiseFunctionLabel6 = new QLabel(this);
    PiecewiseFunctionLabel6->setText("0    ,");
    PiecewiseFunctionLabel7 = new QLabel(this);
    PiecewiseFunctionLabel7->setText("<  x <=");
    PiecewiseFunctionLabel10 = new QLabel(this);
    PiecewiseFunctionLabel10->setText("");
    PiecewiseFunctionLabel11 = new QLabel(this);
    PiecewiseFunctionLabel11->setText("255");
    PiecewiseFunctionEdit1 = new QLineEdit(this);
    PiecewiseFunctionEdit1->setValidator(new QIntValidator(0,255,this));
    PiecewiseFunctionEdit1->setMaximumWidth(30);
    PiecewiseFunctionEdit2 = new QLineEdit(this);
    PiecewiseFunctionEdit2->setValidator(new QIntValidator(0,255,this));
    PiecewiseFunctionEdit2->setMaximumWidth(30);

    PiecewiseFunctionLayout = new QGridLayout(this);
    PiecewiseFunctionLayout->addWidget(PleaseEnterFunctionLabel,0,0,1,4);
    PiecewiseFunctionLayout->addWidget(PiecewiseFunctionLabel1,2,0);
    PiecewiseFunctionLayout->addWidget(PiecewiseFunctionLabel2,1,1);
    PiecewiseFunctionLayout->addWidget(PiecewiseFunctionLabel8,1,2);
    PiecewiseFunctionLayout->addWidget(PiecewiseFunctionLabel3,1,3);
    PiecewiseFunctionLayout->addWidget(PiecewiseFunctionLabel9,1,4);
    PiecewiseFunctionLayout->addWidget(PiecewiseFunctionLabel4,2,1);
    PiecewiseFunctionLayout->addWidget(PiecewiseFunctionEdit1,2,2);
    PiecewiseFunctionLayout->addWidget(PiecewiseFunctionLabel5,2,3);
    PiecewiseFunctionLayout->addWidget(PiecewiseFunctionEdit2,2,4);
    PiecewiseFunctionLayout->addWidget(PiecewiseFunctionLabel6,3,1);
    PiecewiseFunctionLayout->addWidget(PiecewiseFunctionLabel10,3,2);
    PiecewiseFunctionLayout->addWidget(PiecewiseFunctionLabel7,3,3);
    PiecewiseFunctionLayout->addWidget(PiecewiseFunctionLabel11,3,4);

    connect(PiecewiseFunctionEdit1,SIGNAL(textChanged(const QString)),this,SLOT(onPiecewiseFunctionEdit1Changed()));
    connect(PiecewiseFunctionEdit2,SIGNAL(textChanged(const QString)),this,SLOT(onPiecewiseFunctionEdit2Changed()));

    this->setMaximumSize(400,180);
    this->setMinimumSize(10,180);
}
void PiecewiseFunctionWidget::onPiecewiseFunctionEdit1Changed(void)
{
    PiecewiseFunctionLabel9->setText(PiecewiseFunctionEdit1->text());
}
void PiecewiseFunctionWidget::onPiecewiseFunctionEdit2Changed(void)
{
    PiecewiseFunctionLabel10->setText(PiecewiseFunctionEdit2->text());
}
inRangeFunctionWidget::inRangeFunctionWidget(QWidget *parent) : QWidget(parent)
{
    PleaseEnterFunctionLabel = new QLabel(this);
    PleaseEnterFunctionLabel->setText("Please Enter Function:");
    inRangeFunctionLabel1 = new QLabel(this);
    inRangeFunctionLabel1->setText("Upper:(");
    inRangeFunctionLabel2 = new QLabel(this);
    inRangeFunctionLabel2->setText(",");
    inRangeFunctionLabel3 = new QLabel(this);
    inRangeFunctionLabel3->setText(",");
    inRangeFunctionLabel4 = new QLabel(this);
    inRangeFunctionLabel4->setText(")");
    inRangeFunctionLabel5 = new QLabel(this);
    inRangeFunctionLabel5->setText("Lower:(");
    inRangeFunctionLabel6 = new QLabel(this);
    inRangeFunctionLabel6->setText(",");
    inRangeFunctionLabel7 = new QLabel(this);
    inRangeFunctionLabel7->setText(",");
    inRangeFunctionLabel8 = new QLabel(this);
    inRangeFunctionLabel8->setText(")");
    inRangeFunctionLabel9 = new QLabel(this);
    inRangeFunctionLabel9->setText("to");
    inRangeFunctionEdit1 = new QLineEdit(this);
    inRangeFunctionEdit1->setValidator(new QIntValidator(0,255,this));
    inRangeFunctionEdit1->setMaximumWidth(30);
    inRangeFunctionEdit2 = new QLineEdit(this);
    inRangeFunctionEdit2->setValidator(new QIntValidator(0,255,this));
    inRangeFunctionEdit2->setMaximumWidth(30);
    inRangeFunctionEdit3 = new QLineEdit(this);
    inRangeFunctionEdit3->setValidator(new QIntValidator(0,255,this));
    inRangeFunctionEdit3->setMaximumWidth(30);
    inRangeFunctionEdit4 = new QLineEdit(this);
    inRangeFunctionEdit4->setValidator(new QIntValidator(0,255,this));
    inRangeFunctionEdit4->setMaximumWidth(30);
    inRangeFunctionEdit5 = new QLineEdit(this);
    inRangeFunctionEdit5->setValidator(new QIntValidator(0,255,this));
    inRangeFunctionEdit5->setMaximumWidth(30);
    inRangeFunctionEdit6 = new QLineEdit(this);
    inRangeFunctionEdit6->setValidator(new QIntValidator(0,255,this));
    inRangeFunctionEdit6->setMaximumWidth(30);

    inRangeFunctionLayout = new QGridLayout(this);
    inRangeFunctionLayout->addWidget(PleaseEnterFunctionLabel,0,0,1,6);
    inRangeFunctionLayout->addWidget(inRangeFunctionLabel1,1,0);
    inRangeFunctionLayout->addWidget(inRangeFunctionEdit1,1,1);
    inRangeFunctionLayout->addWidget(inRangeFunctionLabel2,1,2);
    inRangeFunctionLayout->addWidget(inRangeFunctionEdit2,1,3);
    inRangeFunctionLayout->addWidget(inRangeFunctionLabel3,1,4);
    inRangeFunctionLayout->addWidget(inRangeFunctionEdit3,1,5);
    inRangeFunctionLayout->addWidget(inRangeFunctionLabel4,1,6);
    inRangeFunctionLayout->addWidget(inRangeFunctionLabel5,3,0);
    inRangeFunctionLayout->addWidget(inRangeFunctionEdit4,3,1);
    inRangeFunctionLayout->addWidget(inRangeFunctionLabel6,3,2);
    inRangeFunctionLayout->addWidget(inRangeFunctionEdit5,3,3);
    inRangeFunctionLayout->addWidget(inRangeFunctionLabel7,3,4);
    inRangeFunctionLayout->addWidget(inRangeFunctionEdit6,3,5);
    inRangeFunctionLayout->addWidget(inRangeFunctionLabel8,3,6);
    inRangeFunctionLayout->addWidget(inRangeFunctionLabel9,2,3);

    this->setMaximumSize(340,180);
    this->setMinimumSize(10,180);
}
void histPullWidget::colorSpaceChanged(void)
{
    if(*imgColorSpace=="None")
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

        reserveOtherChannels->setEnabled(false);
    }
    else if(*imgColorSpace=="RGB")
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

        reserveOtherChannels->setEnabled(true);
    }
    else if(*imgColorSpace=="HSV")
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

        reserveOtherChannels->setEnabled(true);
    }
    else if(*imgColorSpace=="HLS")
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

        reserveOtherChannels->setEnabled(true);
    }
    else if(*imgColorSpace=="HSI")
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

        reserveOtherChannels->setEnabled(true);
    }
    else if(*imgColorSpace=="YUV")
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

        reserveOtherChannels->setEnabled(true);
    }
    else if(*imgColorSpace=="YCrCb")
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

        reserveOtherChannels->setEnabled(true);
    }
    else if(*imgColorSpace=="GRAY")
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

        reserveOtherChannels->setEnabled(false);
    }
}
void histPullWidget::OnBtnStartTriggered(void)
{
    if(*imgColorSpace=="None")
    {
        QMessageBox::information(this, tr("Error"), tr("You didn't load any image!"));
        return;
    }
    cv::Mat dst;
    if(FunctionChooseEdit->currentText()=="Linear")
    {
        dst = LinearTransformation(*imagemat);
    }
    else if(FunctionChooseEdit->currentText()=="Quadratic")
    {
        dst = QuadraticTransformation(*imagemat);
    }
    else if(FunctionChooseEdit->currentText()=="Logistic")
    {
        dst = LogisticTransformation(*imagemat);
    }
    else if(FunctionChooseEdit->currentText()=="Piecewise")
    {
        dst = PiecewiseTransformation(*imagemat);
    }
    else if(FunctionChooseEdit->currentText()=="In Range")
    {
        dst = inRangeTransformation(*imagemat);
    }
    *imagemat = dst;
    *image = cvMat2QImage(dst);
    emit histPulltoExecute();
}
cv::Mat histPullWidget::LinearTransformation(cv::Mat &src)
{
    cv::Mat dst = cv::Mat::zeros(src.size(),src.type());
    int nWidth = src.cols;
    int nHeight = src.rows;
    float a = LinearFunctionWidget->LinearFunctionEdit1->text().toFloat();
    float b = LinearFunctionWidget->LinearFunctionEdit2->text().toFloat();

    if(colorChannelAll->isChecked())
    {
        if(*imgColorSpace == "GRAY")
        {
            for (int row = 0;row < nHeight;row++)
            {
                for (int col = 0;col < nWidth;col++)
                {
                    float val = src.at<uchar>(row,col);
                    dst.at<uchar>(row,col) = cv::saturate_cast<uchar>(val * a + b);
                }
            }
        }
        else
        {
            for (int row = 0;row < nHeight;row++)
            {
                for (int col = 0;col < nWidth;col++)
                {
                    float R = src.at<cv::Vec3b>(row,col)[0];
                    float G = src.at<cv::Vec3b>(row,col)[1];
                    float B = src.at<cv::Vec3b>(row,col)[2];

                    dst.at<cv::Vec3b>(row,col)[0] = cv::saturate_cast<uchar>(R * a + b);
                    dst.at<cv::Vec3b>(row,col)[1] = cv::saturate_cast<uchar>(G * a + b);
                    dst.at<cv::Vec3b>(row,col)[2] = cv::saturate_cast<uchar>(B * a + b);
                }
            }
        }
    }
    if(colorChannel1->isChecked())
    {
        if(reserveOtherChannels->isChecked())
            dst = src.clone();
        for (int row = 0;row < nHeight;row++)
        {
            for (int col = 0;col < nWidth;col++)
            {
                float B = src.at<cv::Vec3b>(row,col)[0];


                dst.at<cv::Vec3b>(row,col)[0] = cv::saturate_cast<uchar>(B * a + b);
            }
        }
    }
    if(colorChannel2->isChecked())
    {
        if(reserveOtherChannels->isChecked())
            dst = src.clone();
        for (int row = 0;row < nHeight;row++)
        {
            for (int col = 0;col < nWidth;col++)
            {
                float G = src.at<cv::Vec3b>(row,col)[1];

                dst.at<cv::Vec3b>(row,col)[1] = cv::saturate_cast<uchar>(G * a + b);
            }
        }
    }
    if(colorChannel3->isChecked())
    {
        if(reserveOtherChannels->isChecked())
            dst = src.clone();
        for (int row = 0;row < nHeight;row++)
        {
            for (int col = 0;col < nWidth;col++)
            {
                float R = src.at<cv::Vec3b>(row,col)[2];

                dst.at<cv::Vec3b>(row,col)[2] = cv::saturate_cast<uchar>(R * a + b);
            }
        }
    }

    return dst;
}
cv::Mat histPullWidget::QuadraticTransformation(cv::Mat &src)
{
    cv::Mat dst = cv::Mat::zeros(src.size(),src.type());
    int nWidth = src.cols;
    int nHeight = src.rows;
    float a = QuadraticFunctionWidget->QuadraticFunctionEdit1->text().toFloat();
    float b = QuadraticFunctionWidget->QuadraticFunctionEdit2->text().toFloat();
    float c = QuadraticFunctionWidget->QuadraticFunctionEdit2->text().toFloat();

    if(colorChannelAll->isChecked())
    {
        if(*imgColorSpace == "GRAY")
        {
            for (int row = 0;row < nHeight;row++)
            {
                for (int col = 0;col < nWidth;col++)
                {
                    float val = src.at<uchar>(row,col);
                    dst.at<uchar>(row,col) = cv::saturate_cast<uchar>(val * val * a + val * b + c);
                }
            }
        }
        else
        {
            for (int row = 0;row < nHeight;row++)
            {
                for (int col = 0;col < nWidth;col++)
                {
                    float R = src.at<cv::Vec3b>(row,col)[0];
                    float G = src.at<cv::Vec3b>(row,col)[1];
                    float B = src.at<cv::Vec3b>(row,col)[2];

                    dst.at<cv::Vec3b>(row,col)[0] = cv::saturate_cast<uchar>(R * R * a + R * b + c);
                    dst.at<cv::Vec3b>(row,col)[1] = cv::saturate_cast<uchar>(G * G * a + G * b + c);
                    dst.at<cv::Vec3b>(row,col)[2] = cv::saturate_cast<uchar>(B * B * a + B * b + c);
                }
            }
        }
    }
    if(colorChannel1->isChecked())
    {
        if(reserveOtherChannels->isChecked())
            dst = src.clone();
        for (int row = 0;row < nHeight;row++)
        {
            for (int col = 0;col < nWidth;col++)
            {
                float B = src.at<cv::Vec3b>(row,col)[0];

                dst.at<cv::Vec3b>(row,col)[0] = cv::saturate_cast<uchar>(B * B * a + B * b + c);
            }
        }
    }
    if(colorChannel2->isChecked())
    {
        if(reserveOtherChannels->isChecked())
            dst = src.clone();
        for (int row = 0;row < nHeight;row++)
        {
            for (int col = 0;col < nWidth;col++)
            {
                float G = src.at<cv::Vec3b>(row,col)[1];

                dst.at<cv::Vec3b>(row,col)[1] = cv::saturate_cast<uchar>(G * G * a + G * b + c);
            }
        }
    }
    if(colorChannel3->isChecked())
    {
        if(reserveOtherChannels->isChecked())
            dst = src.clone();
        for (int row = 0;row < nHeight;row++)
        {
            for (int col = 0;col < nWidth;col++)
            {
                float R = src.at<cv::Vec3b>(row,col)[2];

                dst.at<cv::Vec3b>(row,col)[2] = cv::saturate_cast<uchar>(R * R * a + R * b + c);
            }
        }
    }

    return dst;
}
cv::Mat histPullWidget::LogisticTransformation(cv::Mat &src)
{
    cv::Mat dst = cv::Mat::zeros(src.size(),src.type());
    int nWidth = src.cols;
    int nHeight = src.rows;
    double ratio = LogisticFunctionWidget->LogisticFunctionEdit1->text().toFloat();
    double base = LogisticFunctionWidget->LogisticFunctionEdit2->text().toDouble();
    if(base<=0)
    {
        QMessageBox::information(this, tr("Error"), tr("Base must be bigger than zero!"));
        return src;
    }
    double k = LogisticFunctionWidget->LogisticFunctionEdit3->text().toDouble();
    double a = LogisticFunctionWidget->LogisticFunctionEdit4->text().toDouble();
    double b = LogisticFunctionWidget->LogisticFunctionEdit5->text().toDouble();
    double logbase = log(base);

    if(colorChannelAll->isChecked())
    {
        if(*imgColorSpace == "GRAY")
        {
            for (int row = 0;row < nHeight;row++)
            {
                for (int col = 0;col < nWidth;col++)
                {
                    float val = src.at<uchar>(row,col);
                    dst.at<uchar>(row,col) = cv::saturate_cast<uchar>(ratio * log(k * val+a)/logbase+b);
                }
            }
        }
        else
        {
            for (int row = 0;row < nHeight;row++)
            {
                for (int col = 0;col < nWidth;col++)
                {
                    float R = src.at<cv::Vec3b>(row,col)[0];
                    float G = src.at<cv::Vec3b>(row,col)[1];
                    float B = src.at<cv::Vec3b>(row,col)[2];

                    dst.at<cv::Vec3b>(row,col)[0] = cv::saturate_cast<uchar>(ratio * log(k * R+a)/logbase+b);
                    dst.at<cv::Vec3b>(row,col)[1] = cv::saturate_cast<uchar>(ratio * log(k * G+a)/logbase+b);
                    dst.at<cv::Vec3b>(row,col)[2] = cv::saturate_cast<uchar>(ratio * log(k * B+a)/logbase+b);
                }
            }
        }
    }
    if(colorChannel1->isChecked())
    {
        if(reserveOtherChannels->isChecked())
            dst = src.clone();
        for (int row = 0;row < nHeight;row++)
        {
            for (int col = 0;col < nWidth;col++)
            {
                float B = src.at<cv::Vec3b>(row,col)[0];

                dst.at<cv::Vec3b>(row,col)[0] = cv::saturate_cast<uchar>(ratio * log(k * B+a)/logbase+b);
            }
        }
    }
    if(colorChannel2->isChecked())
    {
        if(reserveOtherChannels->isChecked())
            dst = src.clone();
        for (int row = 0;row < nHeight;row++)
        {
            for (int col = 0;col < nWidth;col++)
            {
                float G = src.at<cv::Vec3b>(row,col)[1];

                dst.at<cv::Vec3b>(row,col)[1] = cv::saturate_cast<uchar>(ratio * log(k * G+a)/logbase+b);
            }
        }
    }
    if(colorChannel3->isChecked())
    {
        if(reserveOtherChannels->isChecked())
            dst = src.clone();
        for (int row = 0;row < nHeight;row++)
        {
            for (int col = 0;col < nWidth;col++)
            {
                float R = src.at<cv::Vec3b>(row,col)[2];

                dst.at<cv::Vec3b>(row,col)[2] = cv::saturate_cast<uchar>(ratio * log(k * R+a)/logbase+b);
            }
        }
    }

    return dst;
}
cv::Mat histPullWidget::PiecewiseTransformation(cv::Mat &src)
{
    cv::Mat dst = cv::Mat::zeros(src.size(),src.type());

    int DOWN = PiecewiseFunctionWidget->PiecewiseFunctionEdit1->text().toInt();
    int UP = PiecewiseFunctionWidget->PiecewiseFunctionEdit2->text().toInt();

    if(colorChannelAll->isChecked())
    {
        if(*imgColorSpace == "GRAY")
        {
            cv::inRange(src,cv::Scalar(DOWN),cv::Scalar(UP),dst);
            *imgColorSpace = "GRAY";
        }
        else
        {
            QMessageBox::information(this, tr("Error"), tr("Can't use this function to process three channels at the same time! You may try to use inRange Function."));
            return src;
        }
    }
    if(colorChannel1->isChecked())
    {
        cv::inRange(src,cv::Scalar(0, 0, DOWN),cv::Scalar(UP, 255, 255),dst);
        *imgColorSpace = "GRAY";
    }
    if(colorChannel2->isChecked())
    {
        cv::inRange(src,cv::Scalar(0, DOWN, 0),cv::Scalar(255, UP, 255),dst);
        *imgColorSpace = "GRAY";
    }
    if(colorChannel3->isChecked())
    {
        cv::inRange(src,cv::Scalar(DOWN, 0, 0),cv::Scalar(255, 255, UP),dst);
        *imgColorSpace = "GRAY";
    }

    return dst;
}
cv::Mat histPullWidget::inRangeTransformation(cv::Mat &src)
{
    cv::Mat dst = cv::Mat::zeros(src.size(),src.type());

    int up_r = inRangeFunctionWidget->inRangeFunctionEdit1->text().toInt();
    int up_g = inRangeFunctionWidget->inRangeFunctionEdit2->text().toInt();
    int up_b = inRangeFunctionWidget->inRangeFunctionEdit3->text().toInt();
    int down_r = inRangeFunctionWidget->inRangeFunctionEdit4->text().toInt();
    int down_g = inRangeFunctionWidget->inRangeFunctionEdit5->text().toInt();
    int down_b = inRangeFunctionWidget->inRangeFunctionEdit6->text().toInt();

    if(colorChannelAll->isChecked())
    {
        if(*imgColorSpace == "GRAY")
        {
            QMessageBox::information(this, tr("Error"), tr("Can't use this function for GRAY image! You may try to use Piecewise Function."));
            return src;
        }
        else
        {
            cv::inRange(src,cv::Scalar(down_b, down_g, down_r),cv::Scalar(up_b, up_g, up_r),dst);
            *imgColorSpace = "GRAY";
        }
    }

    return dst;
}
