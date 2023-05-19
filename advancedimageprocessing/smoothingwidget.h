#ifndef SMOOTHINGWIDGET_H
#define SMOOTHINGWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QComboBox>


#include "advancedimageprocessing.h"

class smoothingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit smoothingWidget(QWidget *parent = nullptr);

    QWidget *parent;

    QString imgColorSpace;
    QImage *image;
    cv::Mat *imagemat;
    QPoint MousePressPosition;

    QLabel *TipsLabel;
    QLabel *KernelWidth;
    QLabel *KernelHeight;
    QLabel *OtherKernelpara;
    QLabel *CustomKernel;
    QLineEdit *KernelWidthEdit;
    QLineEdit *KernelHeightEdit;
    QLineEdit *OtherKernelparaEdit;
    QTextEdit *CustomKernelEdit;
    QLabel *FilterChoose;
    QComboBox *FilterChooseEdit;
    QLabel *BorderChoose;
    QComboBox *BorderChooseEdit;
    QPushButton *start;

    QGridLayout *layout;

private:
    void instantiationWidgets(void);
    void initWidgets(void);

signals:
    void smoothingtoExecute();

public slots:
    void OnBtnStartTriggered(void);
    void OnFilterChanged(void);
};

#endif // SMOOTHINGWIDGET_H
