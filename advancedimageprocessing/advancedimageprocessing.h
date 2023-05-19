#ifndef ADVANCED_IMAGE_PROCESSING_H
#define ADVANCED_IMAGE_PROCESSING_H

#include <QImage>
#include <QDebug>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/stitching.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/imgproc/imgproc_c.h>

QImage cvMat2QImage(const cv::Mat& mat);
cv::Mat QImage2cvMat(QImage image);

#endif // ADVANCED_IMAGE_PROCESSING_H
