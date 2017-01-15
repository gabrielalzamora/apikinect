/*
 * Copyright (c) 2017  Gabriel Alzamora.
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 *
 * As it is based on OpenCV samples
 *                           License Agreement
 *              For Open Source Computer Vision Library
 *                      (3-clause BSD License)
 * Copyright (C) 2000-2016, Intel Corporation, all rights reserved.
 * Copyright (C) 2009-2011, Willow Garage Inc., all rights reserved.
 * Copyright (C) 2009-2016, NVIDIA Corporation, all rights reserved.
 * Copyright (C) 2010-2013, Advanced Micro Devices, Inc., all rights reserved.
 * Copyright (C) 2015-2016, OpenCV Foundation, all rights reserved.
 * Copyright (C) 2015-2016, Itseez Inc., all rights reserved.
 * Third party copyrights are property of their respective owners.
 */
#ifndef CVWORK_H
#define CVWORK_H

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <cstdio>

#include <opencv2/core/core.hpp>
//#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "settings.h"


class CVwork : QObject
{

    Q_OBJECT

public:
    explicit CVwork(QObject *parent=0);
    ~CVwork();

    Settings s;

public slots:
    int mainFunction(int argc, char *argv[]);
    bool runCalibrationAndSave(Settings& s, Size imageSize, Mat&  cameraMatrix, Mat& distCoeffs,
                               vector<vector<Point2f> > imagePoints );

    double computeReprojectionErrors(const vector<vector<Point3f> > &objectPoints, const vector<vector<Point2f> > &imagePoints, const vector<Mat> &rvecs, const vector<Mat> &tvecs, const Mat &cameraMatrix, const Mat &distCoeffs, vector<float> &perViewErrors, bool fisheye);
    void calcBoardCornerPositions(Size boardSize, float squareSize, vector<Point3f> &corners, Settings::Pattern patternType);
    bool runCalibration(Settings &s, Size &imageSize, Mat &cameraMatrix, Mat &distCoeffs, vector<vector<Point2f> > imagePoints, vector<Mat> &rvecs, vector<Mat> &tvecs, vector<float> &reprojErrs, double &totalAvgErr);
    void saveCameraParams(Settings &s, Size &imageSize, Mat &cameraMatrix, Mat &distCoeffs, const vector<Mat> &rvecs, const vector<Mat> &tvecs, const vector<float> &reprojErrs, const vector<vector<Point2f> > &imagePoints, double totalAvgErr);
private slots:

private:


};
#endif // CVWORK_H
