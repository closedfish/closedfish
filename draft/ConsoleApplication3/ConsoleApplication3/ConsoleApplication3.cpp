#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "rect.h"
using namespace cv;
std::vector <square> rects; 
cv::Scalar whitelow = cv::Scalar(31,30,234);
cv::Scalar whitehigh = cv::Scalar(33, 255, 255);
cv::Scalar blacklow = cv::Scalar(44, 107, 147);
cv::Scalar blackhigh = cv::Scalar(56, 255, 255);

void findRects(cv::Mat img, cv::Scalar low, cv::Scalar high, COLOR color) {
    cv::Mat mask;
    cv::inRange(img, low, high, mask);
    //Drawing contours around white squares
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    for (size_t i = 0; i < contours.size(); ++i) {
        cv::Rect boundRect = cv::boundingRect(contours[i]);
        if (abs(boundRect.width-boundRect.height)<=2 && boundRect.width>10) {//adds only squares
            rects.emplace_back(color, boundRect, boundRect.x + boundRect.width / 2, boundRect.y + boundRect.height / 2);
        }
    }
}
void drawRects(cv::Mat img) {
    for (size_t i = 0; i < rects.size(); ++i) {
        switch (rects[i].color) {
        case WHITE:
            cv::rectangle(img, rects[i].rec.tl(), rects[i].rec.br(), CV_RGB(255, 0, 0), 2);
            break;
        case BLACK:
            cv::rectangle(img, rects[i].rec.tl(), rects[i].rec.br(), CV_RGB(0, 0, 0), 2);
            break;
        };
    };
};
int main(){  
    //Takes the input image
    cv::Mat img = cv::imread("C:\\Users\\dimit\\Desktop\\scr.png");
    cv::Mat orig;
    //Switches to HSV
    img.copyTo(orig);
    cv::cvtColor(img, img, cv::COLOR_BGR2HSV);
    //White field detection
    findRects(img, whitelow, whitehigh, COLOR::WHITE);
    findRects(img, blacklow, blackhigh, COLOR::BLACK);
    drawRects(orig);


    //cv::imshow("output", mask);
    cv::imshow("conts", orig);
    cv::waitKey(0);
    return 0;
}