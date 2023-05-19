#ifndef ROTATEWIDGET_H
#define ROTATEWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QButtonGroup>

#include "advancedimageprocessing.h"

class rotateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit rotateWidget(QWidget *parent = nullptr);

    QWidget *parent;

    QString imgColorSpace;
    QImage *image;
    cv::Mat *imagemat;
    QPoint MousePressPosition;

    QLabel *TipsLabel;
    QLabel *AngelLabel;
    QLabel *BackgroundColorLabel;
    QButtonGroup *DirectionSelect;
    QRadioButton *clockwise;
    QRadioButton *counterclockwise;
    QSpinBox *Angel;
    QLineEdit *BackgroundColor;

    QPushButton *start;

    QGridLayout *layout;

private:
    void instantiationWidgets(void);
    void initWidgets(void);

signals:
    void rotatetoExecute();

public slots:
    void OnBtnStartTriggered(void);
};

#endif // ROTATEWIDGET_H
