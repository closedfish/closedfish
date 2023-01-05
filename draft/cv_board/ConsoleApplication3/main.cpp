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

void addTiles(bool col,Board &board) { //adds tiles to a vector
    int size = board.width;
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
}

//board initialization only at the start of the game
void addPieces(Board &board){
    for (int i=1; i <= 8; i++) {
        board.Pieces.push_back(Piece('P', COLOR::WHITE, Tile(char(96 + i), 2)));
        board.Pieces.push_back(Piece('P', COLOR::BLACK, Tile(char(96 + i), 7)));
    };
    board.Pieces.push_back(Piece('K', COLOR::WHITE, Tile('e', 1)));
    board.Pieces.push_back(Piece('K', COLOR::BLACK, Tile('e', 8)));
    board.Pieces.push_back(Piece('Q', COLOR::WHITE, Tile('d', 1)));
    board.Pieces.push_back(Piece('Q', COLOR::BLACK, Tile('d', 8)));
    board.Pieces.push_back(Piece('R', COLOR::WHITE, Tile('a', 1)));
    board.Pieces.push_back(Piece('R', COLOR::WHITE, Tile('h', 1)));
    board.Pieces.push_back(Piece('R', COLOR::BLACK, Tile('a', 8)));
    board.Pieces.push_back(Piece('R', COLOR::BLACK, Tile('h', 8)));
    board.Pieces.push_back(Piece('N', COLOR::WHITE, Tile('b', 1)));
    board.Pieces.push_back(Piece('N', COLOR::WHITE, Tile('g', 1)));
    board.Pieces.push_back(Piece('N', COLOR::BLACK, Tile('b', 8)));
    board.Pieces.push_back(Piece('N', COLOR::BLACK, Tile('g', 8)));
    board.Pieces.push_back(Piece('B', COLOR::WHITE, Tile('c', 1)));
    board.Pieces.push_back(Piece('B', COLOR::WHITE, Tile('f', 1)));
    board.Pieces.push_back(Piece('B', COLOR::BLACK, Tile('c', 8)));
    board.Pieces.push_back(Piece('B', COLOR::BLACK, Tile('f', 8)));
}
void movePiece(string move, Board &board) {//executes the move, updates the board class
    //to do: Short and long castles, queen conversion
    //for some reason the main if isn't executed, check that
    char& piece = move[0];
    char& let = move[1];
    char& num = move[2];
    char& let1 = move[3];
    char& num1 = move[4];
    vector<Piece> piecescopy = board.Pieces;
    for (int i = 0; i < board.Pieces.size(); i++) {
        //remove taken piece
        if (let1 == board.Pieces[i].position.let && num1 == board.Pieces[i].position.cord) {
            piecescopy.erase(piecescopy.begin()+i-1);
        }
        // change coords of the moved piece
        cout << board.Pieces[i].position.let<<let <<num<<board.Pieces[i].position.cord<< endl;
        if (let==board.Pieces[i].position.let && num==board.Pieces[i].position.cord){
            cout << piecescopy[i].position.cord << endl;
            piecescopy[i].position.let = let1;
            piecescopy[i].position.cord = num1;
            cout << piecescopy[i].position.cord << endl;
        }
    }
    board.Pieces = piecescopy;
}
int  main() {  //when merging into the algo code change the name for the call
    //this func finds the board and initializes the main board class accordingly, storing all the data
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
    ///storin the pieces
    board.width = board.right - board.left;
    board.height = board.bottom - board.top;
    Point p1(board.left + board.width / 16, board.top + 50);
    Point p3(board.left + board.width / 16, board.bottom + 20);
    Vec3b col1 = orig.at<Vec3b>(p1);
    Vec3b col2 = orig.at<Vec3b>(p3);
    cout << col1 << endl;
    cout << col2 << endl;
    //
    if (col2[0] > col1[0]) {
        playerColor = true;
    }
    else {
        playerColor = false;
    }
    //adding tiles and pieces
    addTiles(playerColor, board);
    addPieces(board);
    movePiece("Pa2a3", board);
    //
    Point p2(board.left + 50, board.top+board.height/16);
    Point p4(board.left + 20, board.bottom - board.width / 16);
    rectangle(orig,p1,p2, Scalar(255, 0, 0) ,2,LINE_8); 
    rectangle(orig, p3, p4, Scalar(255, 0, 0), 2, LINE_8);
    cv::imshow("conts", orig);
    cv::waitKey(0);
}