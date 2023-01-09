#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "class_decl.h"
#include "ChessBotUI.h"

using namespace cv;

vector <Square> rects;
cv::Scalar whitelow = cv::Scalar(30, 24, 233);
cv::Scalar whitehigh = cv::Scalar(36, 255, 255);
cv::Scalar blacklow = cv::Scalar(40, 102, 143);
cv::Scalar blackhigh = cv::Scalar(46, 255, 255);

std::string getPosition(Board board, int x, int y, Player col) {
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
        int num = (y - board.top) / squareSize;
        position = position + std::to_string(num);
    };

    return position;
}

void findRects(cv::Mat img, cv::Scalar low, cv::Scalar high, cv::Scalar low1, cv::Scalar high1, COLOR color, COLOR color1, Board board) {
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

vector<Tile> addTiles(int size, bool col) {
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



/// <summary>
/// Gives the coordinates of the board
/// </summary>
/// <param name="filepath">path to the photo</param>
/// <returns>board left, top, right, bottom coordinates</returns>
std::pair < std::pair<int, int>, std::pair<int, int> > giveBoard(char* filepath, int colW, int colB)
{
    std::pair < std::pair<int, int>, std::pair<int, int> > ret;

    Board board;
    //Takes the input image
    std::string file;
    while (*filepath != '\0')
    {
        file += (*filepath);
        ++filepath;
    }
    cv::Mat img = cv::imread(filepath);
    
    cv::Mat orig;
    //Switches to HSV
    img.copyTo(orig);
    //cv::imshow("test", img);
    cv::cvtColor(img, img, cv::COLOR_BGR2HSV);
    //White field detection
    findRects(img, blacklow, blackhigh, whitelow, whitehigh, COLOR::BLACK, COLOR::WHITE, board);
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

    ret.first = std::pair<int, int>(board.left, board.top);
    ret.second = std::pair<int, int>(board.right, board.bottom);

    return ret;
}

/*
int main() {
    Board board;
    //Takes the input image
    cv::Mat img = cv::imread("C:\\Users\\dimit\\Desktop\\scr3.png");
    cv::Mat orig;
    //Switches to HSV
    img.copyTo(orig);
    //cv::imshow("test", img);
    cv::cvtColor(img, img, cv::COLOR_BGR2HSV);
    //White field detection
    findRects(img, blacklow, blackhigh, whitelow, whitehigh, COLOR::BLACK, COLOR::WHITE, board);
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


    board.width = board.right - board.left;
    board.height = board.bottom - board.top;

    vector<Tile> tile_vect = addTiles(board.width, true);//true for now until i get player color
    board.Tiles = tile_vect;
    cv::Point p1(board.left, board.top + 50);
    cv::Point p2(board.left + 50, board.top);
    cv::Point p3(board.right, board.bottom + 20);
    cv::Point p4(board.right + 20, board.bottom);
    rectangle(orig, p1, p2, cv::Scalar(255, 0, 0), 2, cv::LINE_8);
    rectangle(orig, p3, p4, cv::Scalar(255, 0, 0), 2, cv::LINE_8);
    cv::imshow("conts", orig);
    cv::waitKey(0);
}

*/
