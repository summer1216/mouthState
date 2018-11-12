#include "MouthStateDetector.h"

MouthStateDetector::MouthStateDetector()
{
}

MouthStateDetector::MouthStateDetector(std::string cascadeName)
{
    mouth_cascade.load(cascadeName);
}

void MouthStateDetector::detectMouths(const cv::Mat& greyImg,cv::Size minSize, cv::Size maxSize)
{
    mouth_cascade.detectMultiScale(greyImg,mouthRects,reject_levels,level_weights,
                                   1.1, 3, 0,minSize,maxSize,true);

    std::cout<<"------------------------------"<<std::endl;
    std::cout<<"size of mouths "<<mouthRects.size()<<std::endl;

//    for(int i=0;i<mouthRects.size();i++)
//    {
//        std::cout<<"rect"<<mouthRects[i]<<std::endl;
//        std::cout<<"levels"<<reject_levels[i]<<std::endl;
//        std::cout<<"weights"<<level_weights[i]<<std::endl;

//    }
}

std::vector<cv::Rect> MouthStateDetector::getMouthRects()
{
    return mouthRects;
}
