#ifndef CHANGEHEADWIDGET_H
#define CHANGEHEADWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>

#include "advancedimageprocessing.h"

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv/cv_image_abstract.h>
#include <dlib/opencv/cv_image.h>

#include <iostream>
#include <vector>

using namespace cv;
using namespace dlib;
using namespace std;

struct correspondens{
    std::vector<int> index;
};

class changeHeadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit changeHeadWidget(QWidget *parent = nullptr);

    QWidget *parent;

    QString imgColorSpace;
    QImage *image;
    cv::Mat *imagemat;
    QPoint MousePressPosition;

    QImage NewFaceImage;
    //cv::Mat NewFaceImageMat;
    QString filename;

    QLabel *TipsLabel;
    QLabel *StatusLabel;
    QProgressBar *ProgressBar;
    QLabel *FilePath;
    QLineEdit *FilePathEdit;
    QPushButton *FilePathDialogEdit;
    QPushButton *start;

    QGridLayout *layout;

private:
    void instantiationWidgets(void);
    void initWidgets(void);
    int faceLandmarkDetection(dlib::array2d<unsigned char>& img, shape_predictor sp, std::vector<Point2f>& landmark);
    void delaunayTriangulation(const std::vector<Point2f>& hull,std::vector<correspondens>& delaunayTri,Rect rect);
    void applyAffineTransform(Mat &warpImage, Mat &src, std::vector<Point2f> &srcTri, std::vector<Point2f> &dstTri);
    void warpTriangle(Mat &img1, Mat &img2, std::vector<Point2f> &t1, std::vector<Point2f> &t2);

    int ProgressValue;

signals:
    void changeHeadtoExecute();

public slots:
    void OnBtnStartTriggered(void);
    void OnBtnFilePathDialogEditTriggered(void);
};

#endif // CHANGEHEADWIDGET_H
