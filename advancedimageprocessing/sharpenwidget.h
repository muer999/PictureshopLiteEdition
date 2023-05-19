#ifndef SHARPENWIDGET_H
#define SHARPENWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QCheckBox>
#include <QComboBox>

#include "advancedimageprocessing.h"

class sharpenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit sharpenWidget(QWidget *parent = nullptr);

    QWidget *parent;

    QString imgColorSpace;
    QImage *image;
    cv::Mat *imagemat;
    QPoint MousePressPosition;

    QLabel *TipsLabel;
    QLabel *KernelSize;
    QLineEdit *KernelSizeEdit;
    QLabel *KernelXOrder;
    QLineEdit *KernelXOrderEdit;
    QLabel *KernelYOrder;
    QLineEdit *KernelYOrderEdit;
    QLabel *CustomKernel;
    QTextEdit *CustomKernelEdit;
    QLabel *FilterChoose;
    QComboBox *FilterChooseEdit;
    QLabel *BorderChoose;
    QComboBox *BorderChooseEdit;
    QCheckBox *KeepColorCheckBox;
    QPushButton *start;

    QGridLayout *layout;

private:
    void instantiationWidgets(void);
    void initWidgets(void);

signals:
    void sharpentoExecute();

public slots:
    void OnBtnStartTriggered(void);
    void OnFilterChanged(void);
};

#endif // SHARPENWIDGET_H
