#ifndef HISTEQUALIZATIONWIDGET_H
#define HISTEQUALIZATIONWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>

#include "advancedimageprocessing.h"

class histEqualizationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit histEqualizationWidget(QWidget *parent = nullptr);

    QWidget *parent;

    QString imgColorSpace;
    QImage *image;
    cv::Mat *imagemat;
    QPoint MousePressPosition;

    QLabel *TipsLabel;

    QPushButton *start;

    QLabel *colorChannelLabel;
    QLabel *colorChannelSelectLabel;
    QButtonGroup *colorChannelSelect;
    QRadioButton *colorChannelAll;
    QRadioButton *colorChannel1;
    QRadioButton *colorChannel2;
    QRadioButton *colorChannel3;

    QGridLayout *layout;

    void colorSpaceChanged(void);

private:
    void instantiationWidgets(void);
    void initWidgets(void);

signals:
    void histEqualizationtoExecute();

public slots:
    void OnBtnStartTriggered(void);
};

#endif // HISTEQUALIZATIONWIDGET_H
