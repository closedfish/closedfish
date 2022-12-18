#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "class_decl.h"
#include <Windows.h>
#include <wingdi.h>
using namespace cv;
using namespace std;
vector <Square> rects;
Scalar whitelow = cv::Scalar(30, 24, 233);
Scalar whitehigh = cv::Scalar(36, 255, 255);
Scalar blacklow = cv::Scalar(40, 102, 143);
Scalar blackhigh = cv::Scalar(46, 255, 255);
//getPosition takes as an imput a board object, player color and (x,y) coordinates on the board.
//It then converts the coordinates to a chess board coordinate system.
std::string getPosition(Board board, int x, int y,Player col) {
    char white[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
    char black[] = { 'h','g','f','e','d','c','b','a' };
    std::string position;
    double squareSize = (board.right - board.left) / 8;
    if (col.color) {
        int index = (x - board.left) / squareSize;
        position = position + white[index];
        int index1 = (y - board.top) / squareSize;
        int num = 8 - index1;
        position = position + std::to_string(num);
    }
    else {
        int index = (x - board.left) / squareSize;
        position = position + black[index];
        int num  = (y - board.top) / squareSize;
        position = position + std::to_string(num);
    };

    return position;
}
//findRects takes as imput the image and color boundaries for the chessboard square colors, and then it proceeds
//to find the squares and return the overall board position on the screen.
void findRects(cv::Mat img, cv::Scalar low, cv::Scalar high, cv::Scalar low1, cv::Scalar high1, COLOR color, COLOR color1,Board board) {
    cv::Mat mask;
    cv::inRange(img, low, high, mask);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    //cv::imshow("maska", mask);
    for (size_t i = 0; i < contours.size(); ++i) {
        cv::Rect boundRect = cv::boundingRect(contours[i]);
        if (abs(boundRect.width - boundRect.height) <= 2 && boundRect.width > 35) {//adds only squares
            rects.emplace_back(color, boundRect, boundRect.x + boundRect.width / 2, boundRect.y + boundRect.height / 2);
        
       }
    }
    
    cv::inRange(img, low1, high1, mask);
    cv::findContours(mask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    //cv::imshow("maska", mask);
    for (size_t i = 0; i < contours.size(); ++i) {
        cv::Rect boundRect = cv::boundingRect(contours[i]);
        if (abs(boundRect.width - boundRect.height) <= 2 && boundRect.width > 35) {//adds only squares
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

vector<Tile> addTiles(int size,bool col) { //adds tiles to a vector
    char white[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
    char black[] = { 'h','g','f','e','d','c','b','a' };
    int iter = size / 8;
    std::vector<Tile> Tiles;
    for (int i = 1; i <= 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (col) {
                Tile t(white[j], i);
                Tiles.push_back(t);
            }
            else {
                Tile t(black[j], i);
                Tiles.push_back(t);
            }
        }
    }
    return Tiles;
}
//The idea is to store the pieces just as values, and then detect their movements by color changes on the block.




int main(){  
    bool playerColor;
    Board board;
    //Takes the input image
    cv::Mat img = cv::imread("C:\\Users\\dimit\\Desktop\\scr3.png");
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
    ///storin the pieces
    board.width = board.right - board.left;
    board.height = board.bottom - board.top;
    //cv::imshow("output", mask);
    //std::cout << board.width << std::endl;
    //std::cout << board.height << std::endl; 
    Point p1(board.left + board.width / 16, board.top + 50);
    Point p3(board.left + board.width / 16, board.bottom + 20);
    Vec3b col1 =orig.at<Vec3b>(p1);
    Vec3b col2 = orig.at<Vec3b>(p3);
    cout << col1 << endl;
    cout << col2 << endl;
    if (col2[0] > col1[0]) {
        playerColor = true;
    }
    else {
        playerColor = false;
    }
    vector<Tile> tile_vect=addTiles(board.width,playerColor);
    board.Tiles = tile_vect;
    Point p2(board.left + 50, board.top+board.height/16);
    Point p4(board.left + 20, board.bottom - board.width / 16);
    rectangle(orig,p1,p2, Scalar(255, 0, 0) ,2,LINE_8);
    rectangle(orig, p3, p4, Scalar(255, 0, 0), 2, LINE_8);
    cv::imshow("conts", orig);
    cout << board.Tiles.size() << endl;
    cv::waitKey(0);
}