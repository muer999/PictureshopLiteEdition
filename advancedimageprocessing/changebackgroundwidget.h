#ifndef CHANGEBACKGROUNDWIDGET_H
#define CHANGEBACKGROUNDWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QButtonGroup>

#include "advancedimageprocessing.h"

class changeBackgroundWidget : public QWidget
{
    Q_OBJECT
public:
    explicit changeBackgroundWidget(QWidget *parent = nullptr);

    QWidget *parent;

    QString imgColorSpace;
    QImage *image;
    cv::Mat *imagemat;
    QPoint MousePressPosition;

    QImage BackgroundImage;
    cv::Mat BackgroundImageMat;
    QString filename;

    QLabel *TipsLabel;
    QLabel *BackgroundType;
    QCheckBox *ImageCheckBox;
    QLabel *FilePath;
    QLineEdit *FilePathEdit;
    QPushButton *FilePathDialogEdit;
    QLabel *BackgroundFillingMode;
    QComboBox *BackgroundFillingModeEdit;
    QCheckBox *PureColorCheckBox;
    QLabel *PureColor;
    QLineEdit *PureColorEdit;
    QButtonGroup *BackgroundTypeGroup;
    QPushButton *start;

    QGridLayout *layout;

private:
    void instantiationWidgets(void);
    void initWidgets(void);

    cv::Mat mat_to_samples(cv::Mat &);

signals:
    void changeBackgroundtoExecute();

public slots:
    void OnBtnStartTriggered(void);
    void OnBackgroundTypeChanged(void);
    void OnBtnFilePathDialogEditTriggered(void);
};

#endif // CHANGEBACKGROUNDWIDGET_H
