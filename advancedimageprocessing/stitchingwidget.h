#ifndef STITCHINGWIDGET_H
#define STITCHINGWIDGET_H

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

//class fullautomaticstitchingWidget;
//class semiautomaticstitchingWidget;

class stitchingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit stitchingWidget(QWidget *parent = nullptr);

    QWidget *parent;

    QString *imgColorSpace;
    QImage *image;
    cv::Mat *imagemat;
    QPoint MousePressPosition;

    QLabel *TipsLabel;
    QComboBox *ModeChoose;
    QPushButton *start;

    QLabel *FilePath1;
    QLineEdit *FilePath1Edit;
    QPushButton *FilePath1DialogEdit;
    QLabel *FilePath2;
    QLineEdit *FilePath2Edit;
    QPushButton *FilePath2DialogEdit;
    QLabel *FilePath3;
    QLineEdit *FilePath3Edit;
    QPushButton *FilePath3DialogEdit;
    QLabel *FilePath4;
    QLineEdit *FilePath4Edit;
    QPushButton *FilePath4DialogEdit;

    QGridLayout *layout;

    //fullautomaticstitchingWidget *fullautomaticstitchingWidget;
    //semiautomaticstitchingWidget *semiautomaticstitchingWidget;

private:
    void instantiationWidgets(void);
    void initWidgets(void);

    void FilePathDialogEditProcess(QLineEdit* FilePathEdit);

    int loadimage(cv::Mat &src,QLineEdit* FilePathEdit);
signals:
    void stitchingtoExecute();

public slots:
    void OnBtnStartTriggered(void);
    void OnBtnFilePath1DialogEditTriggered(void);
    void OnBtnFilePath2DialogEditTriggered(void);
    void OnBtnFilePath3DialogEditTriggered(void);
    void OnBtnFilePath4DialogEditTriggered(void);
    //void OnModeChanged(void);
};

/*class fullautomaticstitchingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit fullautomaticstitchingWidget(QWidget *parent = nullptr);

    QWidget *parent;

    QLabel *FilePath1;
    QLineEdit *FilePath1Edit;
    QPushButton *FilePath1DialogEdit;
    QLabel *FilePath2;
    QLineEdit *FilePath2Edit;
    QPushButton *FilePath2DialogEdit;
    QLabel *FilePath3;
    QLineEdit *FilePath3Edit;
    QPushButton *FilePath3DialogEdit;
    QLabel *FilePath4;
    QLineEdit *FilePath4Edit;
    QPushButton *FilePath4DialogEdit;

    QGridLayout *layout;
};
class semiautomaticstitchingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit semiautomaticstitchingWidget(QWidget *parent = nullptr);

    QWidget *parent;

    QLabel *NumberOfLines;
    QSpinBox *NumberOfLinesEdit;
    QLabel *NumberOfColumns;
    QSpinBox *NumberOfColumnsEdit;

    QGridLayout *layout;
};*/
#endif // STITCHINGWIDGET_H
