#ifndef GEOMETRICCALIBRATIONWIDGET_H
#define GEOMETRICCALIBRATIONWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QButtonGroup>
#include <QSpinBox>

#include "advancedimageprocessing.h"

class geometricCalibrationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit geometricCalibrationWidget(QWidget *parent = nullptr);

    QWidget *parent;

    QString imgColorSpace;
    QImage *image;
    cv::Mat *imagemat;
    QPoint MousePressPosition;

    QLabel *TipsLabel;
    QCheckBox *LeftTopPointCheckBox;
    QLineEdit *LeftTopPointEdit;
    QCheckBox *RightTopPointCheckBox;
    QLineEdit *RightTopPointEdit;
    QCheckBox *LeftBottomPointCheckBox;
    QLineEdit *LeftBottomPointEdit;
    QCheckBox *RightBottomPointCheckBox;
    QLineEdit *RightBottomPointEdit;
    QButtonGroup *PointSelectionButtonGroup;
    QPushButton *start;

    QGridLayout *layout;

private:
    void instantiationWidgets(void);
    void initWidgets(void);

    int GetCoordinateFromLineEdit(QLineEdit *PointEdit,cv::Point &point);

signals:
    void geometricCalibrationtoExecute();

public slots:
    void OnBtnStartTriggered(void);
};

#endif // GEOMETRICCALIBRATIONWIDGET_H
