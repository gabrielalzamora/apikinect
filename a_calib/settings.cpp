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
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "settings.h"
/*
//These write and read functions must be defined for the serialization in FileStorage to work
static void write(FileStorage& fs, const std::string&, const Settings& x)
{
    x.write(fs);
}
static void read(const FileNode& node, Settings& x, const Settings& default_value = Settings())
{
    if(node.empty())
        x = default_value;
    else
        x.read(node);
}
*/

Settings::Settings()
{
    goodInput=true;
    boardSize.width=9;
    boardSize.height=6;                 // The size of the board -> Number of items by width and height
    calibrationPattern=CHESSBOARD;      // One of the Chessboard, circles, or asymmetric circle pattern
    squareSize=24.0;                    // The size of a square in your defined unit (point, millimeter,etc).
    nrFrames=10;                        // The number of frames to use from the input for calibration
    aspectRatio=0;                      // The aspect ratio
    delay=100;                          // In case of a video input
    bwritePoints=1;                     //  Write detected feature points
    bwriteExtrinsics=1;                 // Write extrinsic parameters
    calibZeroTangentDist=0;             // Assume zero tangential distortion
    calibFixPrincipalPoint=0;           // Fix the principal point at the center
    flipVertical=0;                     // Flip the captured images around the horizontal axis
    outputFileName="calib_out_video.xml";// The name of the file where to write
    showUndistorsed=1;                  // Show undistorted images after calibration
    input="calib_img_list.xml";               // The input ->
    inputType=IMAGE_LIST;
/*    imageList = { "calib_video/img1.jpg",
                  "calib_video/img2.jpg",
                  "calib_video/img3.jpg",
                  "calib_video/img4.jpg",
                  "calib_video/img5.jpg",
                  "calib_video/img6.jpg",
                  "calib_video/img7.jpg",
                  "calib_video/img8.jpg",
                  "calib_video/img9.jpg",
                  "calib_video/img10.jpg",
                  "calib_video/img11.jpg",
                  "calib_video/img12.jpg",
                  "calib_video/img13.jpg",
                  "calib_video/img14.jpg",
                  "calib_video/img15.jpg",
                  "calib_video/img16.jpg",
                  "calib_video/img17.jpg",
                  "calib_video/img18.jpg",
                  "calib_video/img19.jpg",
                  "calib_video/img20.jpg"
                };
*/

}

Settings::~Settings()
{

}
/*
void Settings::write(FileStorage& fs) const  //Write serialization for this class
{
    fs << "{" << "BoardSize_Width"  << boardSize.width
              << "BoardSize_Height" << boardSize.height
              << "Square_Size"         << squareSize
              << "Calibrate_Pattern" << patternToUse
              << "Calibrate_NrOfFrameToUse" << nrFrames
              << "Calibrate_FixAspectRatio" << aspectRatio
              << "Calibrate_AssumeZeroTangentialDistortion" << calibZeroTangentDist
              << "Calibrate_FixPrincipalPointAtTheCenter" << calibFixPrincipalPoint

              << "Write_DetectedFeaturePoints" << bwritePoints
              << "Write_extrinsicParameters"   << bwriteExtrinsics
              << "Write_outputFileName"  << outputFileName

              << "Show_UndistortedImage" << showUndistorsed

              << "Input_FlipAroundHorizontalAxis" << flipVertical
              << "Input_Delay" << delay
              << "Input" << input
       << "}";
}

void Settings::read(const FileNode& node)   //Read serialization for this class
{
    node["BoardSize_Width" ] >> boardSize.width;
    node["BoardSize_Height"] >> boardSize.height;
    node["Calibrate_Pattern"] >> patternToUse;
    node["Square_Size"]  >> squareSize;
    node["Calibrate_NrOfFrameToUse"] >> nrFrames;
    node["Calibrate_FixAspectRatio"] >> aspectRatio;
    node["Write_DetectedFeaturePoints"] >> bwritePoints;
    node["Write_extrinsicParameters"] >> bwriteExtrinsics;
    node["Write_outputFileName"] >> outputFileName;
    node["Calibrate_AssumeZeroTangentialDistortion"] >> calibZeroTangentDist;
    node["Calibrate_FixPrincipalPointAtTheCenter"] >> calibFixPrincipalPoint;
    node["Input_FlipAroundHorizontalAxis"] >> flipVertical;
    node["Show_UndistortedImage"] >> showUndistorsed;
    node["Input"] >> input;
    node["Input_Delay"] >> delay;
    interprate();
}

void Settings::interprate()
{
    goodInput = true;
    if (boardSize.width <= 0 || boardSize.height <= 0){
        cerr << "Invalid Board size: " << boardSize.width << " " << boardSize.height << endl;
        goodInput = false;
    }

    if (squareSize <= 10e-6){
        cerr << "Invalid square size " << squareSize << endl;
        goodInput = false;
    }

    if (nrFrames <= 0){
        cerr << "Invalid number of frames " << nrFrames << endl;
        goodInput = false;
    }

    if (input.empty())      // Check for valid input
            inputType = INVALID;
    else{
        if (input[0] >= '0' && input[0] <= '9'){
            stringstream ss(input);
            ss >> cameraID;
            inputType = CAMERA;
        }else{
            if (readStringList(input, imageList)){
                    inputType = IMAGE_LIST;
                    nrFrames = (nrFrames < (int)imageList.size()) ? nrFrames : (int)imageList.size();
            }else
                inputType = VIDEO_FILE;
        }
        if (inputType == CAMERA)
            inputCapture.open(cameraID);
        if (inputType == VIDEO_FILE)
            inputCapture.open(input);
        if (inputType != IMAGE_LIST && !inputCapture.isOpened())
                inputType = INVALID;
    }

    if (inputType == INVALID)    {
        cerr << " Inexistent input: " << input;
        goodInput = false;
    }

    flag = 0;
    if(calibFixPrincipalPoint) flag |= CV_CALIB_FIX_PRINCIPAL_POINT;
    if(calibZeroTangentDist)   flag |= CV_CALIB_ZERO_TANGENT_DIST;
    if(aspectRatio)            flag |= CV_CALIB_FIX_ASPECT_RATIO;

    calibrationPattern = NOT_EXISTING;
    if (!patternToUse.compare("CHESSBOARD")) calibrationPattern = CHESSBOARD;
    if (!patternToUse.compare("CIRCLES_GRID")) calibrationPattern = CIRCLES_GRID;
    if (!patternToUse.compare("ASYMMETRIC_CIRCLES_GRID")) calibrationPattern = ASYMMETRIC_CIRCLES_GRID;
    if (calibrationPattern == NOT_EXISTING){
            cerr << " Inexistent camera calibration mode: " << patternToUse << endl;
            goodInput = false;
    }
    atImageList = 0;
}
*/
Mat Settings::nextImage()
{
    Mat result;
    if( inputCapture.isOpened() ){
        Mat view0;
        inputCapture >> view0;
        view0.copyTo(result);
    }else if( atImageList < (int)imageList.size() )
        result = imread(imageList[atImageList++], CV_LOAD_IMAGE_COLOR);

    return result;
}

bool Settings::readStringList( const string& filename, vector<string>& l )
{
    l.clear();
    FileStorage fs(filename, FileStorage::READ);
    if( !fs.isOpened() )
        return false;
    FileNode n = fs.getFirstTopLevelNode();
    if( n.type() != FileNode::SEQ )
        return false;
    FileNodeIterator it = n.begin(), it_end = n.end();
    for( ; it != it_end; ++it )
        l.push_back((string)*it);
    return true;
}

