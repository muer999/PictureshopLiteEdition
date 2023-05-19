#ifndef HISTPULLWIDGET_H
#define HISTPULLWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QButtonGroup>
#include <QComboBox>
#include <QRadioButton>
#include <QCheckBox>

#include "advancedimageprocessing.h"

class LinearFunctionWidget;
class QuadraticFunctionWidget;
class LogisticFunctionWidget;
class PiecewiseFunctionWidget;
class inRangeFunctionWidget;

class histPullWidget : public QWidget
{
    Q_OBJECT
public:
    explicit histPullWidget(QWidget *parent = nullptr);

    void colorSpaceChanged(void);

    QWidget *parent;

    QString *imgColorSpace;
    QImage *image;
    cv::Mat *imagemat;
    QPoint MousePressPosition;

    QGridLayout *layout;
    QLabel *TipsLabel;
    QLabel *FunctionChoose;
    QComboBox *FunctionChooseEdit;
    QPushButton *start;
    QLabel *colorChannelSelectLabel;
    QButtonGroup *colorChannelSelect;
    QRadioButton *colorChannelAll;
    QRadioButton *colorChannel1;
    QRadioButton *colorChannel2;
    QRadioButton *colorChannel3;
    QCheckBox *reserveOtherChannels;

    LinearFunctionWidget *LinearFunctionWidget;
    QuadraticFunctionWidget *QuadraticFunctionWidget;
    LogisticFunctionWidget *LogisticFunctionWidget;
    PiecewiseFunctionWidget *PiecewiseFunctionWidget;
    inRangeFunctionWidget *inRangeFunctionWidget;

private:
    void instantiationWidgets(void);
    void initWidgets(void);

    cv::Mat LinearTransformation(cv::Mat&);
    cv::Mat QuadraticTransformation(cv::Mat&);
    cv::Mat LogisticTransformation(cv::Mat&);
    cv::Mat PiecewiseTransformation(cv::Mat&);
    cv::Mat inRangeTransformation(cv::Mat&);

signals:
    void histPulltoExecute();

public slots:
    void OnBtnStartTriggered(void);
    void onFunctionChoosed(void);
};

class LinearFunctionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LinearFunctionWidget(QWidget *parent = nullptr);
    QLabel *PleaseEnterFunctionLabel;
    QGridLayout *LinearFunctionLayout;
    QLabel *LinearFunctionLabel1;
    QLineEdit *LinearFunctionEdit1;
    QLabel *LinearFunctionLabel2;
    QLineEdit *LinearFunctionEdit2;
};
class QuadraticFunctionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QuadraticFunctionWidget(QWidget *parent = nullptr);
    QLabel *PleaseEnterFunctionLabel;
    QGridLayout *QuadraticFunctionLayout;
    QLabel *QuadraticFunctionLabel1;
    QLineEdit *QuadraticFunctionEdit1;
    QLabel *QuadraticFunctionLabel2;
    QLineEdit *QuadraticFunctionEdit2;
    QLabel *QuadraticFunctionLabel3;
    QLineEdit *QuadraticFunctionEdit3;
};
class LogisticFunctionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LogisticFunctionWidget(QWidget *parent = nullptr);
    QLabel *PleaseEnterFunctionLabel;
    QGridLayout *LogisticFunctionLayout;
    QLabel *LogisticFunctionLabel1;
    QLineEdit *LogisticFunctionEdit1;
    QLabel *LogisticFunctionLabel2;
    QLineEdit *LogisticFunctionEdit2;
    QLabel *LogisticFunctionLabel3;
    QLineEdit *LogisticFunctionEdit3;
    QLabel *LogisticFunctionLabel4;
    QLineEdit *LogisticFunctionEdit4;
    QLabel *LogisticFunctionLabel5;
    QLineEdit *LogisticFunctionEdit5;
};
class PiecewiseFunctionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PiecewiseFunctionWidget(QWidget *parent = nullptr);
    QLabel *PleaseEnterFunctionLabel;
    QGridLayout *PiecewiseFunctionLayout;
    QLabel *PiecewiseFunctionLabel1;
    QLabel *PiecewiseFunctionLabel2;
    QLabel *PiecewiseFunctionLabel3;
    QLabel *PiecewiseFunctionLabel4;
    QLabel *PiecewiseFunctionLabel5;
    QLabel *PiecewiseFunctionLabel6;
    QLabel *PiecewiseFunctionLabel7;
    QLabel *PiecewiseFunctionLabel8;
    QLabel *PiecewiseFunctionLabel9;
    QLabel *PiecewiseFunctionLabel10;
    QLabel *PiecewiseFunctionLabel11;
    QLineEdit *PiecewiseFunctionEdit1;
    QLineEdit *PiecewiseFunctionEdit2;

public slots:
    void onPiecewiseFunctionEdit1Changed(void);
    void onPiecewiseFunctionEdit2Changed(void);
};
class inRangeFunctionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit inRangeFunctionWidget(QWidget *parent = nullptr);
    QLabel *PleaseEnterFunctionLabel;
    QGridLayout *inRangeFunctionLayout;
    QLabel *inRangeFunctionLabel1;
    QLabel *inRangeFunctionLabel2;
    QLabel *inRangeFunctionLabel3;
    QLabel *inRangeFunctionLabel4;
    QLabel *inRangeFunctionLabel5;
    QLabel *inRangeFunctionLabel6;
    QLabel *inRangeFunctionLabel7;
    QLabel *inRangeFunctionLabel8;
    QLabel *inRangeFunctionLabel9;
    QLineEdit *inRangeFunctionEdit1;
    QLineEdit *inRangeFunctionEdit2;
    QLineEdit *inRangeFunctionEdit3;
    QLineEdit *inRangeFunctionEdit4;
    QLineEdit *inRangeFunctionEdit5;
    QLineEdit *inRangeFunctionEdit6;
};

#endif // HISTPULLWIDGET_H
