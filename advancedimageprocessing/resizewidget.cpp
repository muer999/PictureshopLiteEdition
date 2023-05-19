#include "resizewidget.h"

#include <QDebug>
#include <QIntValidator>
#include <QMessageBox>

resizeWidget::resizeWidget(QWidget *parent) : QWidget(parent)
{
    this->parent = parent;
    isResized=false;

    instantiationWidgets();
    initWidgets();
}

void resizeWidget::instantiationWidgets(void)
{
    TipsLabel = new QLabel(this);
    TipsLabel->setWordWrap(true);
    TipsLabel->setText("Enter new size of picture in the input box below and then press the \"Start\" button. Check \"Keep Aspect Ratio\" to automatically caculate new size.\n\nThe width and height are restricted between 2 pixels to 6000 pixels.");
    newWidth = new QLabel(this);
    newWidth->setText("New Width");
    newHeight = new QLabel(this);
    newHeight->setText("New Height");
    KeepAspectRatio = new QCheckBox(this);
    KeepAspectRatio->setText("Keep Aspect Ratio");
    newWidthEdit = new QLineEdit(this);
    newWidthEdit->setValidator(new QIntValidator(10, 6000, this));
    newHeightEdit = new QLineEdit(this);
    newHeightEdit->setValidator(new QIntValidator(10, 6000, this));
    start = new QPushButton(this);
    start->setText("Start");

    layout = new QGridLayout(this);
}
void resizeWidget::initWidgets(void)
{
    layout->addWidget(TipsLabel,0,0,1,2);
    layout->addWidget(KeepAspectRatio,1,0,1,2);
    layout->addWidget(newWidth,2,0);
    layout->addWidget(newWidthEdit,3,0);
    layout->addWidget(newHeight,2,1);
    layout->addWidget(newHeightEdit,3,1);
    layout->addWidget(start,4,1);
    layout->setRowStretch(0,2);
    layout->setRowStretch(1,1);
    layout->setRowStretch(2,1);
    layout->setRowStretch(3,1);
    layout->setRowStretch(4,1);

    connect(start, SIGNAL(clicked(bool)), this, SLOT(OnBtnStartTriggered()));
    connect(newWidthEdit, SIGNAL(textChanged(const QString)), this, SLOT(OnWidthChanged()));
    connect(newHeightEdit, SIGNAL(textChanged(const QString)), this, SLOT(OnHeightChanged()));
}
void resizeWidget::OnWidthChanged(void)
{
    newHeightEdit->blockSignals(true);
    if(KeepAspectRatio->isChecked())
    {
        int width = newWidthEdit->text().toInt();
        int newheight = int(double(width)/image->width()*image->height());
        if(newheight > 6000 || newheight < 10)
        {
            newHeightEdit->clear();
            newHeightEdit->setStyleSheet("color: red");
        }
        else
        {
            newHeightEdit->setText(QString::number(newheight));
            newHeightEdit->setStyleSheet("color: black");
        }
    }
    newHeightEdit->blockSignals(false);
}
void resizeWidget::OnHeightChanged(void)
{
    newWidthEdit->blockSignals(true);
    if(KeepAspectRatio->isChecked())
    {
        int height = newHeightEdit->text().toInt();
        int newwidth = int(double(height)/image->height()*image->width());
        if(newwidth > 6000 || newwidth < 10)
        {
            newWidthEdit->clear();
            newWidthEdit->setStyleSheet("color: red");
        }
        else
        {
            newWidthEdit->setText(QString::number(newwidth));
            newWidthEdit->setStyleSheet("color: black");
        }
    }
    newWidthEdit->blockSignals(false);
}
void resizeWidget::OnBtnStartTriggered(void)
{
    if(imgColorSpace=="None")
    {
        QMessageBox::information(this, tr("Error"), tr("You didn't load any image!"));
        return;
    }
    if(newHeightEdit->text()=="" || newWidthEdit->text()=="")
    {
        QMessageBox::information(this, tr("Error"), tr("Please enter a vaild size!"));
        return;
    }
    int height = newHeightEdit->text().toInt();
    int width = newWidthEdit->text().toInt();
    if(height > 6000 || height < 10 || width > 6000 || width < 10)
    {
        QMessageBox::information(this, tr("Error"), tr("Please enter a vaild size!"));
        return;
    }

    cv::Mat src = *imagemat;

    cv::resize(src,src,cv::Size(width,height));

    *imagemat = src;
    *image = cvMat2QImage(src);

    isResized=true;
    emit resizetoExecute();
}
