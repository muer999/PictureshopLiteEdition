#include "beautywidget.h"

#include <QDebug>
#include <QMessageBox>

beautyWidget::beautyWidget(QWidget *parent) : QWidget(parent)
{
    this->parent = parent;

    instantiationWidgets();
    initWidgets();
}

void beautyWidget::instantiationWidgets(void)
{
    TipsLabel = new QLabel(this);
    TipsLabel->setText("Press \"start\" to continue.");
    TipsLabel->setWordWrap(true);
    start = new QPushButton(this);
    start->setText("Start");

    layout = new QGridLayout(this);
}
void beautyWidget::initWidgets(void)
{
    layout->addWidget(TipsLabel,0,0,1,2);
    layout->addWidget(start,1,0,1,1);
    layout->setRowStretch(0,6);
    layout->setRowStretch(1,1);

    connect(start, SIGNAL(clicked(bool)), this, SLOT(OnBtnStartTriggered()));
}
void beautyWidget::OnBtnStartTriggered(void)
{
    if(imgColorSpace=="None")
    {
        QMessageBox::information(this, tr("Error"), tr("You didn't load any image!"));
        return;
    }

    cv::Mat src = *imagemat;
    cv::Mat dst;

    cv::bilateralFilter(src,dst,0,50,10);

    *imagemat = dst;
    *image = cvMat2QImage(dst);
    emit beautytoExecute();
}
