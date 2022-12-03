#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "rect.h"
#include <Windows.h>
#include <wingdi.h>
using namespace cv;
std::vector <square> rects;
cv::Scalar whitelow = cv::Scalar(30, 24, 233);
cv::Scalar whitehigh = cv::Scalar(36, 255, 255);
cv::Scalar blacklow = cv::Scalar(40, 102, 143);
cv::Scalar blackhigh = cv::Scalar(46, 255, 255);
void findRects(cv::Mat img, cv::Scalar low, cv::Scalar high, cv::Scalar low1, cv::Scalar high1, COLOR color, COLOR color1,board board) {
    cv::Mat mask;
    cv::inRange(img, low, high, mask);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    //cv::imshow("maska", mask);
    for (size_t i = 0; i < contours.size(); ++i) {
        cv::Rect boundRect = cv::boundingRect(contours[i]);
        if (abs(boundRect.width - boundRect.height) <= 2 && boundRect.width > 15) {//adds only squares
            rects.emplace_back(color, boundRect, boundRect.x + boundRect.width / 2, boundRect.y + boundRect.height / 2);
        
       }
    }
    //
    cv::inRange(img, low1, high1, mask);
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    //cv::imshow("maska", mask);
    for (size_t i = 0; i < contours.size(); ++i) {
        cv::Rect boundRect = cv::boundingRect(contours[i]);
        if (abs(boundRect.width - boundRect.height) <= 2 && boundRect.width > 15) {//adds only squares
            rects.emplace_back(color1, boundRect, boundRect.x + boundRect.width / 2, boundRect.y + boundRect.height / 2);
            

        }
    }

}
void drawRects(cv::Mat img) { //draws highlight squares on the image of the board
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
    board board;
    //Takes the input image
    cv::Mat img = cv::imread("C:\\Users\\dimit\\Desktop\\scr2.png");
    cv::Mat orig;
    //Switches to HSV
    img.copyTo(orig);
    //cv::imshow("test", img);
    cv::cvtColor(img, img, cv::COLOR_BGR2HSV);
    //White field detection
    findRects(img, blacklow, blackhigh,whitelow,whitehigh, COLOR::BLACK, COLOR::WHITE,board);
    drawRects(orig);
    for (int i = 0; i < rects.size(); i++) {
        if (rects[i].rec.tl().x < board.left) {
            board.left = rects[i].rec.tl().x;
        }
        if (rects[i].rec.tl().y < board.top) {
            board.top = rects[i].rec.tl().y;
        }
        if (rects[i].rec.br().x > board.right) {
            board.right = rects[i].rec.br().x;
        }
        if (rects[i].rec.br().y > board.bottom) {
            board.bottom = rects[i].rec.br().y;
        }

    };



    //cv::imshow("output", mask);
    std::cout << board.left << std::endl;
    Point p1(board.left, board.top + 50);
    Point p2(board.left + 50, board.top);
    Point p3(board.right, board.bottom + 20);
    Point p4(board.right + 20, board.bottom);
    rectangle(orig,p1,p2, Scalar(255, 0, 0) ,2,LINE_8);

    rectangle(orig, p3, p4, Scalar(255, 0, 0), 2, LINE_8);
    cv::imshow("conts", orig);
    cv::waitKey(0);
}