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

}

std::vector<cv::Rect> MouthStateDetector::getMouthRects()
{
    return mouthRects;
}
