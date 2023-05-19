#include "changebackgroundwidget.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

changeBackgroundWidget::changeBackgroundWidget(QWidget *parent) : QWidget(parent)
{
    this->parent = parent;

    instantiationWidgets();
    initWidgets();
}

void changeBackgroundWidget::instantiationWidgets(void)
{
    TipsLabel = new QLabel(this);
    TipsLabel->setText("Choose a background type and set parameters. Then press \"start\" to continue.");
    TipsLabel->setWordWrap(true);
    BackgroundType = new QLabel(this);
    BackgroundType->setText("Background type");
    ImageCheckBox = new QCheckBox(this);
    ImageCheckBox->setText("Image");
    ImageCheckBox->setChecked(true);
    FilePath = new QLabel(this);
    FilePath->setText("Enter image path");
    FilePathEdit = new QLineEdit(this);
    FilePathDialogEdit = new QPushButton(this);
    FilePathDialogEdit->setText("...");
    FilePathDialogEdit->setMaximumWidth(25);
    BackgroundFillingMode = new QLabel(this);
    BackgroundFillingMode->setText("Filling mode");
    BackgroundFillingModeEdit = new QComboBox(this);
    BackgroundFillingModeEdit->addItem("Auto resize");
    BackgroundFillingModeEdit->addItem("Fill");
    PureColorCheckBox = new QCheckBox(this);
    PureColorCheckBox->setText("Plain Color");
    PureColor = new QLabel(this);
    PureColor->setText("Enter color");
    PureColorEdit = new QLineEdit(this);
    PureColorEdit->setEnabled(false);
    BackgroundTypeGroup = new QButtonGroup(this);
    BackgroundTypeGroup->addButton(ImageCheckBox);
    BackgroundTypeGroup->addButton(PureColorCheckBox);
    BackgroundTypeGroup->setExclusive(true);
    start = new QPushButton(this);
    start->setText("Start");

    layout = new QGridLayout(this);
}
void changeBackgroundWidget::initWidgets(void)
{
    layout->addWidget(TipsLabel,0,0,1,3);
    layout->addWidget(BackgroundType,1,0);
    layout->addWidget(ImageCheckBox,2,0);
    layout->addWidget(FilePath,2,1);
    layout->addWidget(FilePathEdit,2,2);
    layout->addWidget(FilePathDialogEdit,2,3);
    layout->addWidget(BackgroundFillingMode,3,1);
    layout->addWidget(BackgroundFillingModeEdit,3,2,1,2);
    layout->addWidget(PureColorCheckBox,4,0);
    layout->addWidget(PureColor,4,1);
    layout->addWidget(PureColorEdit,4,2,1,2);

    layout->addWidget(start,5,0);
    layout->setColumnStretch(0,4);
    layout->setColumnStretch(1,4);
    layout->setColumnStretch(2,4);
    layout->setColumnStretch(3,1);
    layout->setRowStretch(0,2);
    layout->setRowStretch(1,1);
    layout->setRowStretch(2,1);
    layout->setRowStretch(3,1);
    layout->setRowStretch(4,1);
    layout->setRowStretch(5,1);

    connect(start, SIGNAL(clicked(bool)), this, SLOT(OnBtnStartTriggered()));
    connect(FilePathDialogEdit, SIGNAL(clicked(bool)), this, SLOT(OnBtnFilePathDialogEditTriggered()));
    connect(BackgroundTypeGroup, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(OnBackgroundTypeChanged()));
}
void changeBackgroundWidget::OnBackgroundTypeChanged(void)
{
    if(ImageCheckBox->isChecked())
    {
        PureColorEdit->setEnabled(false);
        FilePathEdit->setEnabled(true);
        FilePathDialogEdit->setEnabled(true);
        BackgroundFillingModeEdit->setEnabled(true);
    }
    else if(PureColorCheckBox->isChecked())
    {
        PureColorEdit->setEnabled(true);
        FilePathEdit->setEnabled(false);
        FilePathDialogEdit->setEnabled(false);
        BackgroundFillingModeEdit->setEnabled(false);
    }
}
void changeBackgroundWidget::OnBtnFilePathDialogEditTriggered(void)
{
    filename = QFileDialog::getOpenFileName(this,tr("Select image:"),"D:\\Documents\\Pictures",tr("Images (*.jpg *.jpeg *.png *.bmp *.gif)"));
    if (filename.isEmpty()) {
        QMessageBox::information(this, tr("Error"), tr("Open a file failed!"));
        return;
    }
    FilePathEdit->setText(filename);
}
void changeBackgroundWidget::OnBtnStartTriggered(void)
{
    if(imgColorSpace=="None")
    {
        QMessageBox::information(this, tr("Error"), tr("You didn't load any image!"));
        return;
    }

    if(ImageCheckBox->isChecked())
    {
        if (!BackgroundImage.load(FilePathEdit->text())) {
            QMessageBox::information(this, tr("Error"), tr("Open a file failed!"));
            return;
        }
        BackgroundImageMat = QImage2cvMat(BackgroundImage);
        cv::cvtColor(BackgroundImageMat,BackgroundImageMat,cv::COLOR_RGBA2RGB);

        if(BackgroundFillingModeEdit->currentText()=="Auto resize")
        {
            cv::resize(BackgroundImageMat,BackgroundImageMat,imagemat->size());
        }
        else if(BackgroundFillingModeEdit->currentText()=="Fill")
        {
            if(imagemat->cols>BackgroundImageMat.cols || imagemat->rows>BackgroundImageMat.rows)
            {
                cv::copyMakeBorder(BackgroundImageMat,BackgroundImageMat,0,imagemat->rows-BackgroundImageMat.rows,0,imagemat->cols-BackgroundImageMat.cols,cv::BORDER_CONSTANT,cv::Scalar(0,0,0));
            }
            else
            {
                cv::Rect rect(0,0,imagemat->cols-1,imagemat->rows-1);
                BackgroundImageMat=BackgroundImageMat(rect);
            }
        }
    }


    cv::Mat src = *imagemat;

    if(imgColorSpace=="GRAY")
    {
        cv::cvtColor(src,src,cv::COLOR_GRAY2BGR);
    }
    //组装数据
    cv::Mat points = mat_to_samples(src);
    //运行KMeans
    int numCluster = 4;
    cv::Mat labels;
    cv::Mat centers;
    cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::EPS+cv::TermCriteria::COUNT,10,0.1);
    cv::kmeans(points,numCluster,labels,criteria,3,cv::KMEANS_PP_CENTERS,centers);

    //去背景+遮罩生成
    cv::Mat mask=cv::Mat::zeros(src.size(),CV_8UC1);
    int index = src.rows*2+2;
    int cindex = labels.at<int>(index,0);
    int height = src.rows;
    int width = src.cols;
    cv::Mat dst=src.clone();
    for(int row=0;row<height;row++)
    {
        for(int col=0;col<width;col++)
        {
            index = row*width+col;
            int label = labels.at<int>(index,0);
            if(label == cindex)
            {
                dst.at<cv::Vec3b>(row,col)[0] = 0;
                dst.at<cv::Vec3b>(row,col)[1] = 0;
                dst.at<cv::Vec3b>(row,col)[2] = 0;
                mask.at<uchar>(row,col)=0;
            }
            else
            {
                mask.at<uchar>(row,col)=255;
            }
        }
    }
    //cv::imshow("mask",mask);
    //cv::imshow("KMeansResult",dst);

    //腐蚀+高斯模糊
    cv::Mat k = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(3,3),cv::Point(-1,-1));
    cv::erode(mask,mask,k);
    //cv::imshow("erode-mask",mask);
    cv::GaussianBlur(mask,mask,cv::Size(3,3),0,0);
    //cv::imshow("Blur Mask",mask);

    //通道混合
    cv::Vec3b color;
    double w;
    int b,g,r;
    int b1,g1,r1;
    int b2,g2,r2;
    cv::Mat result(src.size(),src.type());

    if(ImageCheckBox->isChecked())
    {
        for(int row=0;row<height;row++)
        {
            for(int col=0;col<width;col++)
            {
                color[0]=BackgroundImageMat.at<cv::Vec3b>(row,col)[0];
                color[1]=BackgroundImageMat.at<cv::Vec3b>(row,col)[1];
                color[2]=BackgroundImageMat.at<cv::Vec3b>(row,col)[2];
                int m=mask.at<uchar>(row,col);
                if(m==255)
                {
                    result.at<cv::Vec3b>(row,col)=src.at<cv::Vec3b>(row,col);
                }
                else if(m==0)
                {
                    result.at<cv::Vec3b>(row,col)=color;
                }
                else
                {
                    w=m/255.0;
                    b1=src.at<cv::Vec3b>(row,col)[0];
                    g1=src.at<cv::Vec3b>(row,col)[1];
                    r1=src.at<cv::Vec3b>(row,col)[2];

                    b2=color[0];
                    g2=color[1];
                    r2=color[2];

                    b=int(b1*w+b2*(1.0-w));
                    g=int(g1*w+g2*(1.0-w));
                    r=int(r1*w+r2*(1.0-w));

                    result.at<cv::Vec3b>(row,col)[0]=b;
                    result.at<cv::Vec3b>(row,col)[1]=g;
                    result.at<cv::Vec3b>(row,col)[2]=r;
                }
            }
        }
    }
    else if(PureColorCheckBox->isChecked())
    {
        if(PureColorEdit->text()=="")
            PureColorEdit->setText("0,0,0");
        QStringList list = PureColorEdit->text().split(",");
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
        color[0]=b;
        color[1]=g;
        color[2]=r;

        for(int row=0;row<height;row++)
        {
            for(int col=0;col<width;col++)
            {
                int m=mask.at<uchar>(row,col);
                if(m==255)
                {
                    result.at<cv::Vec3b>(row,col)=src.at<cv::Vec3b>(row,col);
                }
                else if(m==0)
                {
                    result.at<cv::Vec3b>(row,col)=color;
                }
                else
                {
                    w=m/255.0;
                    b1=src.at<cv::Vec3b>(row,col)[0];
                    g1=src.at<cv::Vec3b>(row,col)[1];
                    r1=src.at<cv::Vec3b>(row,col)[2];

                    b2=color[0];
                    g2=color[1];
                    r2=color[2];

                    b=int(b1*w+b2*(1.0-w));
                    g=int(g1*w+g2*(1.0-w));
                    r=int(r1*w+r2*(1.0-w));

                    result.at<cv::Vec3b>(row,col)[0]=b;
                    result.at<cv::Vec3b>(row,col)[1]=g;
                    result.at<cv::Vec3b>(row,col)[2]=r;
                }
            }
        }
    }

    //cv::imshow("result",result);

    if(imgColorSpace=="GRAY")
    {
        cv::cvtColor(result,result,cv::COLOR_BGR2GRAY);
    }

    *image = cvMat2QImage(result);
    *imagemat = result;

    emit changeBackgroundtoExecute();
}

cv::Mat changeBackgroundWidget::mat_to_samples(cv::Mat &image)
{
    int w = image.cols;
    int h = image.rows;
    int samplecount = w*h;
    int dims = image.channels();
    cv::Mat points(samplecount,dims,CV_32F,cv::Scalar(10));

    int index = 0;
    for(int row = 0;row<h;row++)
    {
        for(int col = 0;col<w;col++)
        {
            index = row*w + col;
            cv::Vec3b bgr = image.at<cv::Vec3b>(row,col);
            points.at<float>(index,0) = static_cast<int>(bgr[0]);
            points.at<float>(index,1) = static_cast<int>(bgr[1]);
            points.at<float>(index,2) = static_cast<int>(bgr[2]);
        }
    }
    return points;
}
