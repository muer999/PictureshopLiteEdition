#ifndef RESIZEWIDGET_H
#define RESIZEWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

#include "advancedimageprocessing.h"

class resizeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit resizeWidget(QWidget *parent = nullptr);

    QWidget *parent;

    QString imgColorSpace;
    QImage *image;
    cv::Mat *imagemat;
    QPoint MousePressPosition;

    QLabel *TipsLabel;
    QLabel *newWidth;
    QLabel *newHeight;
    QCheckBox *KeepAspectRatio;
    QLineEdit *newWidthEdit;
    QLineEdit *newHeightEdit;
    QPushButton *start;

    QGridLayout *layout;

    bool isResized;

private:
    void instantiationWidgets(void);
    void initWidgets(void);

signals:
    void resizetoExecute();

public slots:
    void OnBtnStartTriggered(void);
    void OnWidthChanged(void);
    void OnHeightChanged(void);
};

#endif // RESIZEWIDGET_H
