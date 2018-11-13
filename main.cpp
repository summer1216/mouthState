

//https://www.learnopencv.com/training-better-haar-lbp-cascade-eye-detector-opencv/
#include <iostream>

#include <iostream>
#include <stdio.h>

#include "FaceDetector.h"
#include "MouthStateClassifier.h"
#include "MouthStateDetector.h"
#include "MouthTracker.h"



int main(int argc, char** argv)
{


    MouthStateClassifier mouthClassifier("cascade_open_2520.xml","cascade_close_3015.xml",
                                         "cascade_random_2518.xml");


    bool foundFace = false;
    bool isTracking = false;

    cv::Rect2d bbox(0, 0, 0, 0);

    MouthTracker tracker;
    cv::VideoCapture cap;

    FaceDetector faceDetetor("shape_predictor_68_face_landmarks.dat");



    if(!cap.open(0))
        return 0;
    cv::Mat frame;


    while(cap.read(frame))
    {
        cap >> frame;

        if( frame.empty() ) break; // end of video stream

        cv::Mat frame_gray;
        cv::cvtColor( frame, frame_gray, cv::COLOR_BGR2GRAY ); //灰度变换
        cv::equalizeHist( frame_gray, frame_gray ); //直方图均衡


        cv::Rect mouthBox;
        cv::Rect newMouthBox;

        foundFace =faceDetetor.findFaceAndLandmarks(frame,mouthBox);

//        std::cout<<foundFace<<mouthBox<<std::endl;

        if(foundFace)
        {
            isTracking=false;

            bbox = mouthBox;

        }

        else if(!isTracking)
        {
            tracker.reinitializeTracekr(frame,bbox);
            isTracking = true;
        }
        bool isTracked;
        if(isTracking)
        {
             isTracked = tracker.updateTrack(frame,bbox);
        }

        if(foundFace || isTracked)
        {
            cv::rectangle(frame,bbox,cv::Scalar(255,255,0),2,8,0); //open blue

            mouthClassifier.classifyMouths(frame_gray,bbox,newMouthBox);

            MouthState currentState = mouthClassifier.getCurrentState();

            if(currentState == MouthState::OPEN)
            {
                cv::rectangle(frame,newMouthBox,cv::Scalar(0,255,0),2,8,0);
                cv::putText(frame, std::string("open"),cv::Point(240,80),
                            cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,255,0));
            }
            else if(currentState == MouthState::CLOSE)
            {
                cv::rectangle(frame,newMouthBox,cv::Scalar(0,0,255),2,8,0);
                cv::putText(frame, std::string("closed"),cv::Point(240,80),
                            cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,0,255));
            }
            else if(currentState == MouthState::RANDOM)
            {
                cv::rectangle(frame,newMouthBox,cv::Scalar(0,0,255),2,8,0);
                cv::putText(frame, std::string("random"),cv::Point(240,80),
                            cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,0,255));
            }
        }
        else
            cv::putText(frame, std::string("no mouth detected..."),cv::Point(20,20),
                        cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,0,255));

        cv::imshow("Do you want to drink water :)", frame);

        char c=(char)cv::waitKey(1);

        if(c==27)
            break;

    }

    return 0;
}
