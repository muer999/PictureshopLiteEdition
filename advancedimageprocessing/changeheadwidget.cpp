#include "changeheadwidget.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

using namespace cv;
using namespace dlib;
using namespace std;

changeHeadWidget::changeHeadWidget(QWidget *parent) : QWidget(parent)
{
    this->parent = parent;

    instantiationWidgets();
    initWidgets();
}

void changeHeadWidget::instantiationWidgets(void)
{
    TipsLabel = new QLabel(this);
    TipsLabel->setText("Enter the path of the image file and press \"start\" to continue.");
    TipsLabel->setWordWrap(true);
    StatusLabel = new QLabel(this);
    StatusLabel->setText("");
    ProgressBar = new QProgressBar(this);
    ProgressBar->setRange(0,100);
    ProgressBar->setValue(0);
    ProgressBar->hide();
    FilePath = new QLabel(this);
    FilePath->setText("Enter image path of the new face.");
    FilePathEdit = new QLineEdit(this);
    FilePathDialogEdit = new QPushButton(this);
    FilePathDialogEdit->setText("...");
    FilePathDialogEdit->setMaximumWidth(25);
    start = new QPushButton(this);
    start->setText("Start");

    layout = new QGridLayout(this);
}
void changeHeadWidget::initWidgets(void)
{
    layout->addWidget(TipsLabel,0,0,1,2);
    layout->addWidget(StatusLabel,1,0,1,2);
    layout->addWidget(ProgressBar,2,0,1,2);
    layout->addWidget(FilePath,3,0,1,2);
    layout->addWidget(FilePathEdit,4,0);
    layout->addWidget(FilePathDialogEdit,4,1);
    layout->addWidget(start,5,0,1,1);
    layout->setRowStretch(0,3);
    layout->setRowStretch(1,1);
    layout->setRowStretch(2,1);
    layout->setRowStretch(3,1);
    layout->setRowStretch(4,1);
    layout->setRowStretch(5,1);

    connect(start, SIGNAL(clicked(bool)), this, SLOT(OnBtnStartTriggered()));
    connect(FilePathDialogEdit, SIGNAL(clicked(bool)), this, SLOT(OnBtnFilePathDialogEditTriggered()));
}
void changeHeadWidget::OnBtnStartTriggered(void)
{
    if(imgColorSpace=="None")
    {
        QMessageBox::information(this, tr("Error"), tr("You didn't load any image!"));
        return;
    }

    if (!NewFaceImage.load(FilePathEdit->text())) {
        QMessageBox::information(this, tr("Error"), tr("Open the new face image failed!"));
        return;
    }
    cv::Mat imgCV1 = QImage2cvMat(NewFaceImage);
    cv::cvtColor(imgCV1,imgCV1,cv::COLOR_RGBA2RGB);

    cv::Mat imgCV2 = *imagemat;

    if(imgColorSpace=="GRAY")
    {
        cv::cvtColor(imgCV2,imgCV2,cv::COLOR_GRAY2BGR);
    }

    /*cv::CascadeClassifier face_cascade;
    if (!face_cascade.load(cv::samples::findFileOrKeep("haarcascade_frontalface_alt.xml")))
    {
        qDebug() << "WARNING: Could not load classifier cascade for nested objects" << endl;
        return;
    }
    cv::Mat gray;
    cv::cvtColor(src,gray,cv::COLOR_BGR2GRAY);

    std::vector<cv::Rect> faces;

    face_cascade.detectMultiScale(gray,faces,1.15,5,0,cv::Size(5,5),src.size());

    //qDebug()<<"发现"<<faces.size()<<"个人脸!";
    if(faces.size()<1)
    {
        QMessageBox::information(this, tr("Message"), tr("Can't find a face in this image!"));
        return;
    }

    for ( size_t i = 0; i < faces.size(); i++ )
    {
        cv::Rect r = faces[i];
        int x = r.x, y = r.y, w = r.width, h = r.height;
        int w1 = int(w*1.2);
        cv::Mat newface_resized = NewFaceImageMat.clone();
        cv::resize(newface_resized,newface_resized,cv::Size(w1,w1));
        int top = y - 20;
        int left = x - 20;
        cv::Mat roi = src(cv::Rect(left, top, w1, w1));
        cv::Mat newface_resized_gray;
        cv::cvtColor(newface_resized, newface_resized_gray, cv::COLOR_BGR2GRAY);
        cv::Mat mask;
        cv::threshold(newface_resized_gray, mask, 254, 255, cv::THRESH_BINARY);
        cv::Mat mask_inv;
        cv::bitwise_not(mask,mask_inv);
        cv::Mat img1_bg;
        cv::bitwise_and(roi, roi, img1_bg, mask);
        cv::Mat img2_fg;
        cv::bitwise_and(newface_resized, newface_resized, img2_fg, mask_inv);
        cv::add(img1_bg, img2_fg, dst);
        dst.copyTo(roi);
    }*/

    ProgressValue = 0;
    ProgressBar->setValue(ProgressValue);
    ProgressBar->show();
    StatusLabel->setText("Processing!");

    dlib::array2d<unsigned char> imgDlib1,imgDlib2;
    dlib::cv_image<rgb_pixel> imgDlib1rgb(imgCV1);
    dlib::cv_image<rgb_pixel> imgDlib2rgb(imgCV2);
    dlib::assign_image(imgDlib1, imgDlib1rgb);
    dlib::assign_image(imgDlib2, imgDlib2rgb);

    ProgressValue+=10;
    ProgressBar->setValue(ProgressValue);

    if(!imgCV1.data || !imgCV2.data)
    {
        printf("No image data \n");
        return;
    }
    else
    {
        //cout<<"image readed by opencv"<<endl;
    }



    //---------------------- step 2. detect face landmarks -----------------------------------
    shape_predictor sp;
    deserialize("shape_predictor_68_face_landmarks.dat") >> sp;
    std::vector<Point2f> points1, points2;

    int result=faceLandmarkDetection(imgDlib1,sp,points1);
    ProgressValue+=30;
    ProgressBar->setValue(ProgressValue);
    if(result<1)
    {
        QMessageBox::information(this, tr("Error"), tr("No face can be found in the source image!"));
        ProgressBar->hide();
        StatusLabel->setText("");
        return;
    }
    else if(result>1)
    {
        QMessageBox::information(this, tr("Warning"), tr("More than one face is found in the source image, will choose the first one."));
    }
    result=faceLandmarkDetection(imgDlib2,sp,points2);
    if(result<1)
    {
        QMessageBox::information(this, tr("Error"), tr("No face can be found in the target image!"));
        ProgressBar->hide();
        StatusLabel->setText("");
        return;
    }
    else if(result>1)
    {
        QMessageBox::information(this, tr("Warning"), tr("More than one face is found in the target image, will choose the first one."));
    }
    ProgressValue+=30;
    ProgressBar->setValue(ProgressValue);
    //---------------------step 3. find convex hull -------------------------------------------
    Mat imgCV1Warped = imgCV2.clone();
    imgCV1.convertTo(imgCV1, CV_32F);
    imgCV1Warped.convertTo(imgCV1Warped, CV_32F);

    std::vector<Point2f> hull1;
    std::vector<Point2f> hull2;
    std::vector<int> hullIndex;

    cv::convexHull(points2, hullIndex, false, false);
    ProgressValue+=5;
    ProgressBar->setValue(ProgressValue);
    for(int i = 0; i < hullIndex.size(); i++)
    {
        hull1.push_back(points1[hullIndex[i]]);
        hull2.push_back(points2[hullIndex[i]]);
    }

    ProgressValue+=5;
    ProgressBar->setValue(ProgressValue);
    //-----------------------step 4. delaunay triangulation -------------------------------------
    std::vector<correspondens> delaunayTri;
    Rect rect(0, 0, imgCV1Warped.cols, imgCV1Warped.rows);
    delaunayTriangulation(hull2,delaunayTri,rect);

    ProgressValue+=5;
    ProgressBar->setValue(ProgressValue);
    for(size_t i=0;i<delaunayTri.size();++i)
    {
        std::vector<Point2f> t1,t2;
        correspondens corpd=delaunayTri[i];
        for(size_t j=0;j<3;++j)
        {
            t1.push_back(hull1[corpd.index[j]]);
            t2.push_back(hull2[corpd.index[j]]);
        }

        warpTriangle(imgCV1,imgCV1Warped,t1,t2);
    }

    ProgressValue+=5;
    ProgressBar->setValue(ProgressValue);
    //------------------------step 5. clone seamlessly -----------------------------------------------

    //calculate mask
    std::vector<Point> hull8U;

    for(int i=0; i< hull2.size();++i)
    {
        Point pt(hull2[i].x,hull2[i].y);
        hull8U.push_back(pt);
    }

    Mat mask = Mat::zeros(imgCV2.rows,imgCV2.cols,imgCV2.depth());
    fillConvexPoly(mask, &hull8U[0], hull8U.size(), Scalar(255,255,255));

    ProgressValue+=5;
    ProgressBar->setValue(ProgressValue);
    Rect r = boundingRect(hull2);
    Point center = (r.tl() +r.br()) / 2;

    Mat output;
    imgCV1Warped.convertTo(imgCV1Warped, CV_8UC3);
    seamlessClone(imgCV1Warped,imgCV2,mask,center,output,NORMAL_CLONE);

    ProgressValue+=5;
    ProgressBar->setValue(ProgressValue);

    if(imgColorSpace=="GRAY")
    {
        cv::cvtColor(output,output,cv::COLOR_BGR2GRAY);
    }
    *imagemat = output;
    *image = cvMat2QImage(output);

    ProgressBar->hide();
    StatusLabel->setText("");
    emit changeHeadtoExecute();
}
void changeHeadWidget::OnBtnFilePathDialogEditTriggered(void)
{
    filename = QFileDialog::getOpenFileName(this,tr("Select image:"),"D:\\Documents\\Pictures",tr("Images (*.jpg *.jpeg *.png *.bmp *.gif)"));
    if (filename.isEmpty()) {
        QMessageBox::information(this, tr("Error"), tr("Open a file failed!"));
        return;
    }
    FilePathEdit->setText(filename);
}
int changeHeadWidget::faceLandmarkDetection(dlib::array2d<unsigned char>& img, shape_predictor sp, std::vector<Point2f>& landmark)
{
    dlib::frontal_face_detector detector = get_frontal_face_detector();
    //dlib::pyramid_up(img);

    std::vector<dlib::rectangle> dets = detector(img);
    //cout << "Number of faces detected: " << dets.size() << endl;

    if(int(dets.size()==0))
    {
        return 0;
    }

    full_object_detection shape = sp(img, dets[0]);
    //image_window win;
    //win.clear_overlay();
    //win.set_image(img);
    //win.add_overlay(render_face_detections(shape));
    for (int i = 0; i < shape.num_parts(); ++i)
    {
        float x=shape.part(i).x();
        float y=shape.part(i).y();
        landmark.push_back(Point2f(x,y));
    }
    return int(dets.size());
}

/*
//perform Delaunay Triangulation on the keypoints of the convex hull.
*/
void changeHeadWidget::delaunayTriangulation(const std::vector<Point2f>& hull,std::vector<correspondens>& delaunayTri,Rect rect)
{

    cv::Subdiv2D subdiv(rect);
    for (int it = 0; it < hull.size(); it++)
        subdiv.insert(hull[it]);
    //cout<<"done subdiv add......"<<endl;
    std::vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    //cout<<"traingleList number is "<<triangleList.size()<<endl;



    //std::vector<Point2f> pt;
    //correspondens ind;
    for (size_t i = 0; i < triangleList.size(); ++i)
    {

        std::vector<Point2f> pt;
        correspondens ind;
        Vec6f t = triangleList[i];
        pt.push_back( Point2f(t[0], t[1]) );
        pt.push_back( Point2f(t[2], t[3]) );
        pt.push_back( Point2f(t[4], t[5]) );
        //cout<<"pt.size() is "<<pt.size()<<endl;

        if (rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2]))
        {
            //cout<<t[0]<<" "<<t[1]<<" "<<t[2]<<" "<<t[3]<<" "<<t[4]<<" "<<t[5]<<endl;
            int count = 0;
            for (int j = 0; j < 3; ++j)
                for (size_t k = 0; k < hull.size(); k++)
                    if (abs(pt[j].x - hull[k].x) < 1.0   &&  abs(pt[j].y - hull[k].y) < 1.0)
                    {
                        ind.index.push_back(k);
                        count++;
                    }
            if (count == 3)
                //cout<<"index is "<<ind.index[0]<<" "<<ind.index[1]<<" "<<ind.index[2]<<endl;
                delaunayTri.push_back(ind);
        }
        //pt.resize(0);
        //cout<<"delaunayTri.size is "<<delaunayTri.size()<<endl;
    }
}


// Apply affine transform calculated using srcTri and dstTri to src
void changeHeadWidget::applyAffineTransform(Mat &warpImage, Mat &src, std::vector<Point2f> &srcTri, std::vector<Point2f> &dstTri)
{
    // Given a pair of triangles, find the affine transform.
    Mat warpMat = getAffineTransform( srcTri, dstTri );

    // Apply the Affine Transform just found to the src image
    warpAffine( src, warpImage, warpMat, warpImage.size(), cv::INTER_LINEAR, BORDER_REFLECT_101);
}


/*
//morp a triangle in the one image to another image.
*/
void changeHeadWidget::warpTriangle(Mat &img1, Mat &img2, std::vector<Point2f> &t1, std::vector<Point2f> &t2)
{

    Rect r1 = boundingRect(t1);
    Rect r2 = boundingRect(t2);

    // Offset points by left top corner of the respective rectangles
    std::vector<Point2f> t1Rect, t2Rect;
    std::vector<Point> t2RectInt;
    for(int i = 0; i < 3; i++)
    {

        t1Rect.push_back( Point2f( t1[i].x - r1.x, t1[i].y -  r1.y) );
        t2Rect.push_back( Point2f( t2[i].x - r2.x, t2[i].y - r2.y) );
        t2RectInt.push_back( Point(t2[i].x - r2.x, t2[i].y - r2.y) ); // for fillConvexPoly

    }

    // Get mask by filling triangle
    Mat mask = Mat::zeros(r2.height, r2.width, CV_32FC3);
    fillConvexPoly(mask, t2RectInt, Scalar(1.0, 1.0, 1.0), 16, 0);

    // Apply warpImage to small rectangular patches
    Mat img1Rect;
    img1(r1).copyTo(img1Rect);

    Mat img2Rect = Mat::zeros(r2.height, r2.width, img1Rect.type());

    applyAffineTransform(img2Rect, img1Rect, t1Rect, t2Rect);

    multiply(img2Rect,mask, img2Rect);
    multiply(img2(r2), Scalar(1.0,1.0,1.0) - mask, img2(r2));
    img2(r2) = img2(r2) + img2Rect;
}
