#ifndef SHOWDCTWIDGET_H
#define SHOWDCTWIDGET_H

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

#define DCTIMAGEWIDTH 400
#define DCTIMAGEHEIGHT 400

class showDCTWidget : public QWidget
{
    Q_OBJECT
public:
    explicit showDCTWidget(QWidget *parent = nullptr);

    QWidget *parent;

    QString imgColorSpace;
    QImage *image;
    cv::Mat *imagemat;
    QPoint MousePressPosition;

    QLabel *dctImageLabel;

    QImage *dctImage;
    QPixmap *dctPixmap;

    QGridLayout *layout;

    void caculateDCTandShow(void);

private:
    void instantiationWidgets(void);
    void initWidgets(void);

    void caculateDCTRGB(void);
    void caculateDCTYUV(void);
    void caculateDCTYCrCb(void);
    void caculateDCTHSV(void);
    void caculateDCTHLS(void);
    void caculateDCTGRAY(void);

signals:
    void showDCTtoExecute();

public slots:

};

#endif // SHOWDCTWIDGET_H
