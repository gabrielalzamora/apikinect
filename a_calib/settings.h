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
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <iostream>
#include <sstream>
#include <string>
//#include <ctime>
//#include <cstdio>

#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

enum Pattern { NOT_EXISTING, CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };
enum InputType {INVALID, CAMERA, VIDEO_FILE, IMAGE_LIST};
enum { DETECTION = 0, CAPTURING = 1, CALIBRATED = 2 };


class Settings : QObject
{

    Q_OBJECT

public:
    explicit Settings();
    ~Settings();

    Size boardSize;            // The size of the board -> Number of items by width and height
    Pattern calibrationPattern;// One of the Chessboard, circles, or asymmetric circle pattern
    float squareSize;          // The size of a square in your defined unit (point, millimeter,etc).
    int nrFrames;              // The number of frames to use from the input for calibration
    float aspectRatio;         // The aspect ratio
    int delay;                 // In case of a video input
    bool bwritePoints;         //  Write detected feature points
    bool bwriteExtrinsics;     // Write extrinsic parameters
    bool calibZeroTangentDist; // Assume zero tangential distortion
    bool calibFixPrincipalPoint;// Fix the principal point at the center
    bool flipVertical;          // Flip the captured images around the horizontal axis
    string outputFileName;      // The name of the file where to write
    bool showUndistorsed;       // Show undistorted images after calibration
    string input;               // The input ->

    int cameraID;
    vector<string> imageList;
    int atImageList;
    VideoCapture inputCapture;
    InputType inputType;
    bool goodInput;
    int flag;
    string patternToUse;

public slots:
//    void write(FileStorage& fs) const;
//    void read(const FileNode& node);
//    void interprate();
    Mat nextImage();
    static bool readStringList( const string& filename, vector<string>& l );

private:

};

static inline void read(const FileNode& node, Settings& x, const Settings& default_value = Settings())
{
    if(node.empty())
        //x. = default_value;
        x
    else
        x.read(node);
}

static inline void write(FileStorage& fs, const String&, const Settings& s )
{
    s.write(fs);
}


#endif // SETTINGS_H
