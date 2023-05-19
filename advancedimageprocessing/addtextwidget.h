#ifndef ADDTEXTWIDGET_H
#define ADDTEXTWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFontComboBox>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QColor>
#include <QFont>

#include "advancedimageprocessing.h"

class addTextWidget : public QWidget
{
    Q_OBJECT
public:
    explicit addTextWidget(QWidget *parent = nullptr);

    QWidget *parent;

    QString imgColorSpace;
    QImage *image;
    cv::Mat *imagemat;
    QPoint MousePressPosition;

    QLabel *TipsLabel;
    QLabel *TextLabel;
    QLineEdit *TextEdit;
    QLabel *fontName;
    QLabel *fontSize;
    QLabel *fontColor;
    QLabel *MouseGetPoint;
    QCheckBox *setMouseGetPoint;
    QLineEdit *MouseGetPointEdit;
    QFontComboBox *fontNameEdit;
    QSpinBox *fontSizeEdit;
    QComboBox *fontColorEdit;
    QPushButton *start;

    QGridLayout *layout;

private:
    void instantiationWidgets(void);
    void initWidgets(void);

signals:
    void addTexttoExecute();

public slots:
    void OnBtnStartTriggered(void);
    void OnsetMouseGetPointChanged(void);
};

#endif // ADDTEXTWIDGET_H
