#include "addtextwidget.h"

#include <QDebug>
#include <QMessageBox>
#include <QPainter>
#include <QPen>

addTextWidget::addTextWidget(QWidget *parent) : QWidget(parent)
{
    this->parent = parent;

    instantiationWidgets();
    initWidgets();
}

void addTextWidget::instantiationWidgets(void)
{
    TipsLabel = new QLabel(this);
    TipsLabel->setText("Set parameters for the function and then press \"start\" to add text to the image. Check the\"Auto acquire\" box and press the location you want to add text on the image, the program will automaticlly caculate the position for you.");
    TipsLabel->setWordWrap(true);
    TextLabel = new QLabel(this);
    TextLabel->setText("Text");
    TextEdit = new QLineEdit(this);
    MouseGetPoint = new QLabel(this);
    MouseGetPoint->setText("Text position");
    setMouseGetPoint = new QCheckBox(this);
    setMouseGetPoint->setText("Auto acquire");
    MouseGetPointEdit = new QLineEdit(this);
    fontName = new QLabel(this);
    fontName->setText("Font:");
    fontSize = new QLabel(this);
    fontSize->setText("Size:");
    fontColor = new QLabel(this);
    fontColor->setText("Color:");
    fontNameEdit = new QFontComboBox(this);
    fontSizeEdit = new QSpinBox(this);
    fontSizeEdit->setMaximum(96);
    fontSizeEdit->setMinimum(8);
    fontSizeEdit->setSingleStep(2);
    fontSizeEdit->setValue(16);
    fontColorEdit = new QComboBox(this);
    fontColorEdit->addItem("Red");
    fontColorEdit->addItem("Green");
    fontColorEdit->addItem("Blue");
    fontColorEdit->addItem("Yellow");
    fontColorEdit->addItem("Orange");
    fontColorEdit->addItem("Purple");
    fontColorEdit->addItem("White");
    fontColorEdit->addItem("Black");
    start = new QPushButton(this);
    start->setText("Start");

    layout = new QGridLayout(this);
}
void addTextWidget::initWidgets(void)
{
    layout->addWidget(TipsLabel,0,0,1,2);
    layout->addWidget(TextLabel,1,0);
    layout->addWidget(TextEdit,1,1);
    layout->addWidget(setMouseGetPoint,2,0,1,2);
    layout->addWidget(MouseGetPoint,3,0);
    layout->addWidget(MouseGetPointEdit,3,1);
    layout->addWidget(fontName,4,0);
    layout->addWidget(fontNameEdit,4,1);
    layout->addWidget(fontSize,5,0);
    layout->addWidget(fontSizeEdit,5,1);
    layout->addWidget(fontColor,6,0);
    layout->addWidget(fontColorEdit,6,1);
    layout->addWidget(start,7,0);
    layout->setRowStretch(0,2);
    layout->setRowStretch(1,1);
    layout->setRowStretch(2,1);
    layout->setRowStretch(3,1);
    layout->setRowStretch(4,1);
    layout->setRowStretch(5,1);
    layout->setRowStretch(6,1);
    layout->setRowStretch(7,1);
    layout->setColumnStretch(0,1);
    layout->setColumnStretch(1,2);

    connect(setMouseGetPoint, SIGNAL(clicked(bool)), this, SLOT(OnsetMouseGetPointChanged()));
    connect(start, SIGNAL(clicked(bool)), this, SLOT(OnBtnStartTriggered()));
}
void addTextWidget::OnsetMouseGetPointChanged(void)
{
    if(setMouseGetPoint->isChecked())
        MouseGetPointEdit->setEnabled(false);
    else
        MouseGetPointEdit->setEnabled(true);
}
void addTextWidget::OnBtnStartTriggered(void)
{
    if(imgColorSpace=="None")
    {
        QMessageBox::information(this, tr("Error"), tr("You didn't load any image!"));
        return;
    }

    QColor color;
    if(fontColorEdit->currentText()=="Red")
        color=QColor(Qt::red);
    else if (fontColorEdit->currentText()=="Green")
        color=QColor(Qt::green);
    else if (fontColorEdit->currentText()=="Blue")
        color=QColor(Qt::blue);
    else if (fontColorEdit->currentText()=="Yellow")
        color=QColor(Qt::yellow);
    else if (fontColorEdit->currentText()=="Orange")
        color=QColor(255,128,0);
    else if (fontColorEdit->currentText()=="Purple")
        color=QColor(128,0,128);
    else if (fontColorEdit->currentText()=="White")
        color=QColor(Qt::white);
    else if (fontColorEdit->currentText()=="Black")
        color=QColor(Qt::black);

    QPen pen;
    pen.setWidth(5);
    pen.setColor(color);
    pen.setDashOffset(1);

    QStringList list = MouseGetPointEdit->text().split(",");
    if(list.length()!=2)
    {
        QMessageBox::information(this, tr("Error"), tr("Please enter a vaild coordinate!"));
        return;
    }

    int size=fontSizeEdit->value();
    QFont font=fontNameEdit->currentFont();
    font.setPointSize(size);
    font.setWeight(QFont::Normal);


    QPixmap imgpixtmp=QPixmap::fromImage(*image);
    QPainter pp(&imgpixtmp);
    pp.setRenderHint(QPainter::Antialiasing,true);
    pp.setPen(pen);
    pp.setFont(font);

    int x=list.at(0).toInt();
    int y=list.at(1).toInt();
    if(x<0 || x>=image->width() || y<0 || y>=image->height())
    {
        QMessageBox::information(this, tr("Error"), tr("Location out of boundary!"));
        return;
    }

    QPoint position(x,y);
    QString text=TextEdit->text();
    pp.drawText(position,text);

    *image = imgpixtmp.toImage();
    *imagemat = QImage2cvMat(*image);
    cv::cvtColor(*imagemat,*imagemat,cv::COLOR_RGBA2RGB);
    if(imgColorSpace=="GRAY")
    {
        cv::cvtColor(*imagemat,*imagemat,cv::COLOR_BGR2GRAY);
        *image = cvMat2QImage(*imagemat);
    }
    emit addTexttoExecute();
}
