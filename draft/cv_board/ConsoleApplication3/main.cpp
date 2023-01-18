#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "class_decl.h"
#include <Windows.h>
#include <wingdi.h>
#include <experimental/filesystem>
using namespace cv;
using namespace std;
vector <Square> rects;
Scalar whitelow = Scalar(30, 24, 233);
Scalar whitehigh = Scalar(36, 255, 255);
Scalar blacklow = Scalar(40, 102, 143);
Scalar blackhigh = cv::Scalar(46, 255, 255);
//getPosition takes as an input a board object, player color and (x,y) coordinates on the board.
//It then converts the coordinates to a chess board coordinate system.
string getPosition(Board &board, int x, int y,Player col) {
    char white[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
    char black[] = { 'h','g','f','e','d','c','b','a' };
    string position;
    double squareSize = (board.right - board.left) / 8;
    if (col.color) {
        int index = (x - board.left) / squareSize;
        position = position + white[index];
        int index1 = (y - board.top) / squareSize;
        int num = 8 - index1;
        position = position + to_string(num);
    }
    else {
        int index = (x - board.left) / squareSize;
        position = position + black[index];
        int num  = (y - board.top) / squareSize;
        position = position + to_string(num);
    };

    return position;
}
string indexToCords(int i,Player col) {
    char white[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
    char black[] = { 'h','g','f','e','d','c','b','a' };
    int slovo = i % 8;
    int broj = i - slovo;
    string position;
    if (slovo == 0) {
        return 'h' + to_string(i / 8);
    }else{
        string position;
        if (col.color) {
            position = position + white[slovo-1] + to_string(broj+1);
        }
        else {
            position = position + black[slovo-1] + to_string(broj+1);
        };
    };
    return position;
}
//findRects takes as imput the image and color boundaries for the chessboard square colors, and then it proceeds
//to find the squares and return the overall board position on the screen.
void findRects(cv::Mat &img, cv::Scalar low, cv::Scalar high, cv::Scalar low1, cv::Scalar high1, COLOR color, COLOR color1,Board &board) {
    Mat mask;
    inRange(img, low, high, mask);
    vector<std::vector<cv::Point>> contours;
    findContours(mask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    //cv::imshow("maska", mask);
    for (size_t i = 0; i < contours.size(); ++i) {
        Rect boundRect = cv::boundingRect(contours[i]);
        if (abs(boundRect.width - boundRect.height) <= 2 && boundRect.width > 35) {//adds only squares
            rects.emplace_back(color, boundRect, boundRect.x + boundRect.width / 2, boundRect.y + boundRect.height / 2);
        
       }
    }
    
    inRange(img, low1, high1, mask);
    findContours(mask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    //cv::imshow("maska", mask);
    for (size_t i = 0; i < contours.size(); ++i) {
        cv::Rect boundRect = cv::boundingRect(contours[i]);
        if (abs(boundRect.width - boundRect.height) <= 2 && boundRect.width > 35) {//adds only squares
            rects.emplace_back(color1, boundRect, boundRect.x + boundRect.width / 2, boundRect.y + boundRect.height / 2);
            

        }
    }

}
void addTiles(bool col,Board &board) { //adds tiles to a vector
    int size = board.width;
    char white[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
    char black[] = { 'h','g','f','e','d','c','b','a' };
    int iter = size / 8;
    for (int i = 1; i <= 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (col) {
                Tile t(white[j], i);
                board.Tiles.push_back(t);
            }
            else {
                Tile t(black[j], i);
                board.Tiles.push_back(t);
            }
        }
    }
}
bool use_mask;
Mat img; Mat templ; Mat mask; Mat result;
const char* image_window = "Source Image";
const char* result_window = "Result window";

int max_Trackbar = 5;

Mat conv(Mat& img){
    Mat hsvImg;
    cvtColor(img, hsvImg, COLOR_BGR2HSV);
    return hsvImg;
};
float detectPieces(Board &board, Mat &img1, Mat &templ) {
    Mat img_orig;
    img1.copyTo(img_orig);
    imshow("test", templ);
    Mat result;
    matchTemplate(img1, templ, result, TM_CCOEFF_NORMED);
    //normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    Point matchLoc;
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    if (maxVal > 0.5) {
        matchLoc = maxLoc;
        rectangle(img1, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
        rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
        imshow(image_window, result);
    }
    return maxVal;
}
//Returns the highest matched Piece object to the given filename, with piece name and position on chessboard
int matchPiece(Board& board,char* filename,Point coord, vector<pair<char, Mat>> templates,Player col) {
    int max = 0;
    Mat slika;
    float maxval = 0;
    for (int i = 0; i < templates.size(); i++) {
        float j=detectPieces(board, slika, templates[i].second);
        if (j > maxval) {
            maxval = j;
            max = i;
        }
    };
    return max;
}
Mat getTemplates(string file) {
    return imread("C:\\Users\\dimit\\Desktop\\cps\\" + file+".png");
}
int  main() {  //when merging into the algo code change the name for the call
    //this func finds the board and initializes the main board class accordingly, storing all the data
    bool playerColor;
    Board board;
    vector<pair<char, Mat>> templates;
    /*to do
    * for (i in razvans folder of squares){
    * detect piece for each in filename
    * create piece object with converting razvan's square index to chess tile cords
    * add piece to board.pieces
    */
    pair<char, Mat> bk{ 'k',getTemplates("black_king") };
    templates.push_back(bk);
    pair<char, Mat> bq{ 'q', getTemplates("black_queen") };
    templates.push_back(bk);
    pair<char, Mat> bn{ 'n', getTemplates("black_knight") };
    templates.push_back(bk);
    pair<char, Mat> bbk{ 'n' , getTemplates("b_black_knight") };
    templates.push_back(bk);
    pair<char, Mat> bb = { 'b',getTemplates("black_bishop") };
    templates.push_back(bk);
    pair<char, Mat> bbb = { 'b',getTemplates("b_black_bishop") };
    templates.push_back(bk);
    pair<char, Mat> br = {'r', getTemplates("black_rook")};
    templates.push_back(bk);
    pair<char, Mat>bbr = {'r',getTemplates("b_black_rook")};
    templates.push_back(bk);
    pair<char, Mat> bp= {'p',getTemplates("black_pawn")};
    templates.push_back(bk);
    pair<char, Mat> bbp= {'p',getTemplates("b_black_pawn")};
    templates.push_back(bk);
    pair<char, Mat> wk= {'W',getTemplates("white_king")};
    templates.push_back(bk);
    pair<char, Mat> wq= {'Q',getTemplates("white_queen")};
    templates.push_back(bk);
    pair<char, Mat>wn = {'N',getTemplates("white_knight")};
    templates.push_back(bk);
    pair<char, Mat> bwn= {'N',getTemplates("b_white_knight")};
    templates.push_back(bk);
    pair<char, Mat>wb = {'B',getTemplates("white_bishop")};
    templates.push_back(bk);
    pair<char, Mat> bwb= {'B',getTemplates("b_white_bishop")};
    templates.push_back(bk);
    pair<char, Mat> wr= {'R',getTemplates("white_rook")};
    templates.push_back(bk);
    pair<char, Mat>bwr = {'R',getTemplates("b_white_rook")};
    templates.push_back(bk);
    pair<char, Mat> wp= {'P',getTemplates("white_pawn")};
    templates.push_back(bk);
    pair<char, Mat> bwp= {'P',getTemplates("b_white_pawn")};
    templates.push_back(bk);
    //Takes the input image
    Mat img = imread("C:\\Users\\dimit\\Desktop\\scr3.png");
    Mat testic = imread("C:\\Users\\dimit\\Desktop\\zutikonj.png");
    Mat orig;
    img.copyTo(orig);
    imshow("testaaaaaaa", testic);
    cv::cvtColor(img, img, cv::COLOR_BGR2HSV);
    //White field detection
    string test1 = indexToCords(2, Player(true));
    cout << test1 << endl;
    findRects(img, blacklow, blackhigh, whitelow, whitehigh, COLOR::BLACK, COLOR::WHITE, board);
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
    //
    if (col2[0] > col1[0]) { // 1 or true is white player
        playerColor = true;
    }
    else {
        playerColor = false;
    }
    //export the board from the pieces section in fen format

    Point p2(board.left + 50, board.top+board.height/16);
    Point p4(board.left + 20, board.bottom - board.width / 16);
    rectangle(orig,p1,p2, Scalar(255, 0, 0) ,2,LINE_8); 
    rectangle(orig, p3, p4, Scalar(255, 0, 0), 2, LINE_8);
    imshow("orig", orig);
    cv::waitKey(0);


}