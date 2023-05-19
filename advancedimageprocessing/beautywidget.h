#ifndef BEAUTYWIDGET_H
#define BEAUTYWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFontComboBox>
#include <QComboBox>
#include <QSpinBox>
#include <QColor>
#include <QFont>

#include "advancedimageprocessing.h"

class beautyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit beautyWidget(QWidget *parent = nullptr);

    QWidget *parent;

    QString imgColorSpace;
    QImage *image;
    cv::Mat *imagemat;
    QPoint MousePressPosition;

    QLabel *TipsLabel;

    QPushButton *start;

    QGridLayout *layout;

private:
    void instantiationWidgets(void);
    void initWidgets(void);

signals:
    void beautytoExecute();

public slots:
    void OnBtnStartTriggered(void);
};

#endif // BEAUTYWIDGET_H
