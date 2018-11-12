#include "MouthStateClassifier.h"



MouthStateClassifier::MouthStateClassifier(std::string openCascadeName,
                                           std::string closeCascadeName, std::string randomCascadeName)
    :openDetector(openCascadeName),closeDetector(closeCascadeName),randomDetector(randomCascadeName)
{

}

void MouthStateClassifier::classifyMouths(const cv::Mat &greyImg,
                                          const cv::Rect &mouthBbox,
                                          cv::Rect &newMouthBox)
{

    currentMouthState = MouthState::UNKNOWN;

    std::cout<<"=============================="<<std::endl;
    std::cout<<"mouth box "<<mouthBbox<<std::endl;

    mouthCandidates.clear();


    float roi_x = mouthBbox.x-0.5*mouthBbox.width>0?mouthBbox.x-0.5*mouthBbox.width:0;
    float roi_y =mouthBbox.y-0.5*mouthBbox.height>0?mouthBbox.y-0.5*mouthBbox.height:0;
    float roi_width = 2*mouthBbox.width<(greyImg.cols-roi_x)?2*mouthBbox.width:(greyImg.cols-roi_x);
    float roi_height = 2*mouthBbox.height<(greyImg.rows-roi_y)?2*mouthBbox.height:(greyImg.rows-roi_y);

    cv::Rect roi = cv::Rect(roi_x,roi_y,roi_width,roi_height);
    std::cout<<"roi box "<<roi<<std::endl;

    cv::Mat roiImg = greyImg(roi);


    detectCandidates(closeDetector,mouthBbox,MouthState::CLOSE,roiImg);
    detectCandidates(randomDetector,mouthBbox,MouthState::RANDOM,roiImg);
    detectCandidates(openDetector,mouthBbox,MouthState::OPEN,roiImg);

    if(mouthCandidates.size()!=0)
    {
        selectNearestCand(mouthBbox,selectedMouth);
        currentMouthState = selectedMouth.state;
        newMouthBox = cv::Rect(selectedMouth.boundingBox.x+mouthBbox.x-0.5*mouthBbox.width,
                               selectedMouth.boundingBox.y+mouthBbox.y-0.5*mouthBbox.height,
                               selectedMouth.boundingBox.width,selectedMouth.boundingBox.height);
    }

    switch (currentMouthState) {
    case MouthState::OPEN:
        std::cout<<"OPEN"<<std::endl;
        break;
    case MouthState::CLOSE:
        std::cout<<"CLOSE"<<std::endl;
        break;
    case MouthState::RANDOM:
        std::cout<<"RANDOM"<<std::endl;
        break;
    default:
        break;
    }


}

MouthState MouthStateClassifier::getCurrentState()
{
    return currentMouthState;
}


void MouthStateClassifier::detectCandidates( MouthStateDetector &detector,const cv::Rect &mouthBbox,
                                             const MouthState state, const cv::Mat &roiImg)
{
    cv::Size minSize(mouthBbox.width*0.7,mouthBbox.height*0.7);
    cv::Size maxSize(mouthBbox.width*1.3,mouthBbox.height*1.3);
    MouthCandidate candidate;


    detector.detectMouths(roiImg,minSize,maxSize);
    std::vector<cv::Rect> detectedRects = detector.getMouthRects();

    for(auto rect:detectedRects)
    {
        candidate.boundingBox = rect;
        candidate.state = state;
        mouthCandidates.push_back(candidate);
    }

}

void MouthStateClassifier::selectNearestCand(const cv::Rect &mouthBbox,MouthCandidate& selected_cand)
{
    std::cout<<"........................"<<std::endl;

    float minRatio = 100;

    float ratioMouth = 1.0*mouthBbox.width/(1.0*mouthBbox.height);
    std::cout<<"ratio mouth "<<ratioMouth<<std::endl;

    for(auto cand:mouthCandidates)
    {

        float ratioDetected = 1.0*cand.boundingBox.width/(1.0*cand.boundingBox.height);
        std::cout<<"ratio detected "<<ratioDetected<<std::endl;


        float dRatio = fabs(ratioDetected-ratioMouth);

        if(dRatio<minRatio)
        {
            minRatio = dRatio;
            selected_cand = cand;
        }

    }


}
