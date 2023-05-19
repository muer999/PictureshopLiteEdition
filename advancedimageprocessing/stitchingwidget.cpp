#include "stitchingwidget.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

stitchingWidget::stitchingWidget(QWidget *parent) : QWidget(parent)
{
    this->parent = parent;

    instantiationWidgets();
    initWidgets();
}

void stitchingWidget::instantiationWidgets(void)
{
    TipsLabel = new QLabel(this);
    TipsLabel->setText("Choose mode and set parameters, then press \"start\" to continue.");
    TipsLabel->setWordWrap(true);
    ModeChoose = new QComboBox(this);
    ModeChoose->addItem("Full automatic(Very slow)");
    ModeChoose->addItem("Semi automatic(Faster, but requires you to sort images by hand)");
    ModeChoose->setMaximumWidth(300);
    //fullautomaticstitchingWidget = new class fullautomaticstitchingWidget(this);
    //semiautomaticstitchingWidget = new class semiautomaticstitchingWidget(this);
    //semiautomaticstitchingWidget->hide();

    FilePath1 = new QLabel(this);
    FilePath1->setText("FilePath1");
    FilePath1Edit = new QLineEdit(this);
    FilePath1DialogEdit = new QPushButton(this);
    FilePath1DialogEdit->setText("...");
    FilePath1DialogEdit->setMaximumWidth(25);
    FilePath2 = new QLabel(this);
    FilePath2->setText("FilePath2");
    FilePath2Edit = new QLineEdit(this);
    FilePath2DialogEdit = new QPushButton(this);
    FilePath2DialogEdit->setText("...");
    FilePath2DialogEdit->setMaximumWidth(25);
    FilePath3 = new QLabel(this);
    FilePath3->setText("FilePath3");
    FilePath3Edit = new QLineEdit(this);
    FilePath3DialogEdit = new QPushButton(this);
    FilePath3DialogEdit->setText("...");
    FilePath3DialogEdit->setMaximumWidth(25);
    FilePath4 = new QLabel(this);
    FilePath4->setText("FilePath4");
    FilePath4Edit = new QLineEdit(this);
    FilePath4DialogEdit = new QPushButton(this);
    FilePath4DialogEdit->setText("...");
    FilePath4DialogEdit->setMaximumWidth(25);

    start = new QPushButton(this);
    start->setText("Start");

    layout = new QGridLayout(this);
}
void stitchingWidget::initWidgets(void)
{
    layout->addWidget(TipsLabel,0,0,1,3);
    layout->addWidget(ModeChoose,1,0,1,3);

    layout->addWidget(FilePath1,2,0);
    layout->addWidget(FilePath1Edit,2,1);
    layout->addWidget(FilePath1DialogEdit,2,2);
    layout->addWidget(FilePath2,3,0);
    layout->addWidget(FilePath2Edit,3,1);
    layout->addWidget(FilePath2DialogEdit,3,2);
    layout->addWidget(FilePath3,4,0);
    layout->addWidget(FilePath3Edit,4,1);
    layout->addWidget(FilePath3DialogEdit,4,2);
    layout->addWidget(FilePath4,5,0);
    layout->addWidget(FilePath4Edit,5,1);
    layout->addWidget(FilePath4DialogEdit,5,2);

    //layout->addWidget(fullautomaticstitchingWidget,2,0,1,2);
    //layout->addWidget(semiautomaticstitchingWidget,2,0,1,2);
    layout->addWidget(start,6,0);


    layout->setRowStretch(0,2);
    layout->setRowStretch(1,1);
    layout->setRowStretch(2,1);
    layout->setRowStretch(3,1);
    layout->setRowStretch(4,1);
    layout->setRowStretch(5,1);
    layout->setRowStretch(6,1);
    layout->setColumnStretch(0,3);
    layout->setColumnStretch(1,4);
    layout->setColumnStretch(2,1);

    connect(start, SIGNAL(clicked(bool)), this, SLOT(OnBtnStartTriggered()));
    connect(FilePath1DialogEdit, SIGNAL(clicked(bool)), this, SLOT(OnBtnFilePath1DialogEditTriggered()));
    connect(FilePath2DialogEdit, SIGNAL(clicked(bool)), this, SLOT(OnBtnFilePath2DialogEditTriggered()));
    connect(FilePath3DialogEdit, SIGNAL(clicked(bool)), this, SLOT(OnBtnFilePath3DialogEditTriggered()));
    connect(FilePath4DialogEdit, SIGNAL(clicked(bool)), this, SLOT(OnBtnFilePath4DialogEditTriggered()));
    //connect(ModeChoose,SIGNAL(currentTextChanged(const QString)),this,SLOT(OnModeChanged()));
}
void stitchingWidget::OnBtnFilePath1DialogEditTriggered(void)
{
    FilePathDialogEditProcess(FilePath1Edit);
}
void stitchingWidget::OnBtnFilePath2DialogEditTriggered(void)
{
    FilePathDialogEditProcess(FilePath2Edit);
}
void stitchingWidget::OnBtnFilePath3DialogEditTriggered(void)
{
    FilePathDialogEditProcess(FilePath3Edit);
}
void stitchingWidget::OnBtnFilePath4DialogEditTriggered(void)
{
    FilePathDialogEditProcess(FilePath4Edit);
}
void stitchingWidget::FilePathDialogEditProcess(QLineEdit* FilePathEdit)
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Select image:"),"D:\\Documents\\Pictures",tr("Images (*.jpg *.jpeg *.png *.bmp *.gif)"));
    if (filename.isEmpty()) {
        QMessageBox::information(this, tr("Error"), tr("Open a file failed!"));
        return;
    }
    FilePathEdit->setText(filename);
}
/*void stitchingWidget::OnModeChanged(void)
{
    if(ModeChoose->currentText()=="Full automatic(Very slow)")
    {
        fullautomaticstitchingWidget->show();
        semiautomaticstitchingWidget->hide();
    }
    else if(ModeChoose->currentText()=="Semi automatic(Faster, but requires you to sort images by hand)")
    {
        fullautomaticstitchingWidget->hide();
        semiautomaticstitchingWidget->show();
    }
}*/
void stitchingWidget::OnBtnStartTriggered(void)
{
    int imagenumbers;
    cv::Mat src[4],dstImage;
    std::vector<cv::Mat> srcImage;
    if(FilePath1Edit->text()=="")
    {
        QMessageBox::information(this, tr("Error"), tr("You must input at least two images!"));
        return;
    }
    else if(FilePath2Edit->text()=="")
    {
        QMessageBox::information(this, tr("Error"), tr("You must input at least two images!"));
        return;
    }
    else if(FilePath3Edit->text()=="")
    {
        imagenumbers=2;
    }
    else if(FilePath4Edit->text()=="")
    {
        imagenumbers=3;
    }
    else
    {
        imagenumbers=4;
    }
    QLineEdit *FilePath[4] = {FilePath1Edit,FilePath2Edit,FilePath3Edit,FilePath4Edit};
    for(int i=0;i<imagenumbers;i++)
    {
        if(loadimage(src[i],FilePath[i])==-1)
            return;
        else if(ModeChoose->currentText()=="Full automatic(Very slow)")
        {
            srcImage.push_back(src[i]);
        }
    }

    if(ModeChoose->currentText()=="Full automatic(Very slow)")
    {
        cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(cv::Stitcher::PANORAMA);
        // 使用stitch函数进行拼接

        cv::Stitcher::Status status = stitcher->stitch(srcImage, dstImage);

        if (status != cv::Stitcher::OK)
        {
            qDebug() << "Can't stitch images, error code = " << int(status) << endl;
            return;
        }
        *image = cvMat2QImage(dstImage);
        *imagemat = dstImage;
    }
    else if(ModeChoose->currentText()=="Semi automatic(Faster, but requires you to sort images by hand)")
    {
        if(imagenumbers!=4)
        {
            QMessageBox::information(this, tr("Error"), tr("Please enter at least four images!"));
            return;
        }
        int i=0,j=0;
        cv::Mat combine;
        cv::Mat row[2][2], column[2];
        cv::Mat com_row[2][2], con_column[2];
        com_row[0][0] = src[0];
        com_row[1][0] = src[2];
        for(i=0;i<2;i++)
        {
            for(j=0;j<1;j++)
            {
                cv::hconcat(com_row[i][j], row[i][j+1], com_row[i][j+1]);
            }
            column[i]=com_row[i][1];   //产生的每一行的拼接图片
        }
        //垂直拼接
        con_column[0] = column[0];
        for (i = 0; i < 2; i++)
        {
            cv::vconcat(con_column[i], column[i + 1], con_column[i + 1]);
        }
        combine = con_column[1];
        //namedWindow("Combine", cv::WINDOW_AUTOSIZE);
        //imshow("Combine", combine);

        *image = cvMat2QImage(combine);
        *imagemat = combine;
    }

    *imgColorSpace = "RGB";
    emit stitchingtoExecute();
}

int stitchingWidget::loadimage(cv::Mat &src,QLineEdit* FilePathEdit)
{
    QImage image;
    if (!image.load(FilePathEdit->text())) {
        QMessageBox::information(this, tr("Error"), tr("Open a file failed!"));
        return -1;
    }
    src = QImage2cvMat(image);
    if(image.format()==QImage::Format_Indexed8)
        cv::cvtColor(src,src,cv::COLOR_GRAY2BGR);
    else
        cv::cvtColor(src,src,cv::COLOR_RGBA2RGB);
    return 0;
}
/*fullautomaticstitchingWidget::fullautomaticstitchingWidget(QWidget *parent) : QWidget(parent)
{
    this->parent = parent;

    FilePath1 = new QLabel(this);
    FilePath1->setText("FilePath1");
    FilePath1Edit = new QLineEdit(this);
    FilePath1DialogEdit = new QPushButton(this);
    FilePath1DialogEdit->setText("...");
    FilePath1DialogEdit->setMaximumWidth(25);
    FilePath2 = new QLabel(this);
    FilePath2->setText("FilePath2");
    FilePath2Edit = new QLineEdit(this);
    FilePath2DialogEdit = new QPushButton(this);
    FilePath2DialogEdit->setText("...");
    FilePath2DialogEdit->setMaximumWidth(25);
    FilePath3 = new QLabel(this);
    FilePath3->setText("FilePath3");
    FilePath3Edit = new QLineEdit(this);
    FilePath3DialogEdit = new QPushButton(this);
    FilePath3DialogEdit->setText("...");
    FilePath3DialogEdit->setMaximumWidth(25);
    FilePath4 = new QLabel(this);
    FilePath4->setText("FilePath4");
    FilePath4Edit = new QLineEdit(this);
    FilePath4DialogEdit = new QPushButton(this);
    FilePath4DialogEdit->setText("...");
    FilePath4DialogEdit->setMaximumWidth(25);

    layout = new QGridLayout(this);
    layout->addWidget(FilePath1,0,0);
    layout->addWidget(FilePath1Edit,0,1);
    layout->addWidget(FilePath1DialogEdit,0,2);
    layout->addWidget(FilePath2,1,0);
    layout->addWidget(FilePath2Edit,1,1);
    layout->addWidget(FilePath2DialogEdit,1,2);
    layout->addWidget(FilePath3,2,0);
    layout->addWidget(FilePath3Edit,2,1);
    layout->addWidget(FilePath3DialogEdit,2,2);
    layout->addWidget(FilePath4,3,0);
    layout->addWidget(FilePath4Edit,3,1);
    layout->addWidget(FilePath4DialogEdit,3,2);

    layout->setColumnStretch(0,3);
    layout->setColumnStretch(1,4);
    layout->setColumnStretch(2,1);
}
semiautomaticstitchingWidget::semiautomaticstitchingWidget(QWidget *parent) : QWidget(parent)
{
    this->parent = parent;

    NumberOfLines = new QLabel(this);
    NumberOfLines->setText("Number of lines");
    NumberOfLinesEdit = new QSpinBox(this);
    NumberOfLinesEdit->setMinimum(1);
    NumberOfLinesEdit->setMaximum(5);
    NumberOfColumns = new QLabel(this);
    NumberOfColumns->setText("Number of columns");
    NumberOfColumnsEdit = new QSpinBox(this);
    NumberOfColumnsEdit->setMinimum(1);
    NumberOfColumnsEdit->setMaximum(5);

    layout = new QGridLayout(this);
    layout->addWidget(NumberOfLines,0,0);
    layout->addWidget(NumberOfLinesEdit,0,1);
    layout->addWidget(NumberOfColumns,1,0);
    layout->addWidget(NumberOfColumnsEdit,1,1);

    layout->setColumnStretch(0,3);
    layout->setColumnStretch(1,4);
}*/
