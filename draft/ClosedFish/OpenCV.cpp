#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "class_decl.h"
#include "ChessBotUI.h"

#pragma region values

#define ORIGSQ 104

namespace templateVals {
    char* templateNames[24] = { "\\black_king",
        "\\b_black_king",
        "\\black_queen",
        "\\b_black_queen",
        "\\black_knight",
        "\\b_black_knight",
        "\\black_bishop",
        "\\b_black_bishop",
        "\\black_rook",
        "\\b_black_rook",
        "\\black_pawn",
        "\\b_black_pawn",
        "\\white_king",
        "\\w_white_king",
        "\\white_queen",
        "\\b_white_queen",
        "\\white_knight",
        "\\b_white_knight",
        "\\white_bishop",
        "\\b_white_bishop",
        "\\white_rook",
        "\\b_white_rook",
        "\\white_pawn",
        "\\b_white_pawn"
    };

    char pieceVals[24] = { 'k', 'k', 'q', 'q', 'n', 'n', 'b', 'b', 'r', 'r', 'p', 'p',
    'K', 'K', 'Q', 'Q', 'N', 'N', 'B', 'B', 'R', 'R', 'P', 'P' };
}

namespace pieceRGB
{
    cv::Vec3b black4 = { 38, 38, 38 };
    cv::Vec3b black6 = { 29, 27, 27 };
    cv::Vec3b black1 = { 82, 83, 86 };
    cv::Vec3b black2 = { 65, 65, 68 };
    cv::Vec3b black3 = { 115, 115, 118 };
    cv::Vec3b black5 = { 117, 117, 120 };
    cv::Vec3b white1 = { 248, 248, 248 };
    cv::Vec3b white2 = { 211, 211, 221 };
    //add 255,255,255, 70,70,70
    //cv::Vec3b white3 = { 70, 73, 72 };
}

const double sqSize = 10816.0;

namespace piecePerc
{
    //Order is white rook, knight, bishop, queen, king, pawn and then black same piece order
    double percentages[12] = { 1833.0 / sqSize, 2065.0 / sqSize, 1668.0 / sqSize, 2267.0 / sqSize, 2657.0 / sqSize, 1001.0 / sqSize,
    2686.0 / sqSize, 3051 / sqSize, 2503 / sqSize, 3261 / sqSize, 3442 / sqSize, 1321 / sqSize };
    //double percentages[12] = { 1921.0 / sqSize, 2083.0 / sqSize, 1753.0 / sqSize, 2283.0 / sqSize, 2765.0 / sqSize, 1001.0 / sqSize,
    //2271.0 / sqSize, 2647 / sqSize, 2074 / sqSize, 2701 / sqSize, 2837 / sqSize, 1321 / sqSize };
}

#pragma endregion

using namespace cv;

using namespace std;

void rgb_to_hsv(double r, double g, double b, double& h, double& s, double& v)
{
    // R, G, B values are divided by 255
    // to change the range from 0..255 to 0..1
    r = r / 255.0;
    g = g / 255.0;
    b = b / 255.0;

    // h, s, v = hue, saturation, value
    double cmax = max(r, max(g, b)); // maximum of r, g, b
    double cmin = min(r, min(g, b)); // minimum of r, g, b
    double diff = cmax - cmin; // diff of cmax and cmin.
    h = -1; 
    s = -1;

    // if cmax and cmax are equal then h = 0
    if (cmax == cmin)
        h = 0;

    // if cmax equal r then compute h
    else if (cmax == r)
        h = fmod(60 * ((g - b) / diff) + 360, 360);

    // if cmax equal g then compute h
    else if (cmax == g)
        h = fmod(60 * ((b - r) / diff) + 120, 360);

    // if cmax equal b then compute h
    else if (cmax == b)
        h = fmod(60 * ((r - g) / diff) + 240, 360);

    // if cmax equal zero
    if (cmax == 0)
        s = 0;
    else
        s = (diff / cmax) * 100;

    // compute v
    v = cmax * 100;

    h = (255 - h) / 2;
    s = (s / 100) * 255;
    v = (v / 100) * 255;
}


OpenCV::OpenCV(bmpClass* bm)
{
    this->init(bm);
}

void OpenCV::loadTemplates()
{
    for (int i = 0; i < 24; ++i)
    {
        templates.push_back({ templateVals::pieceVals[i], getTemplates(templateVals::templateNames[i]) });
    }
}

void OpenCV::init(bmpClass* bm) {
    GetFullPathNameA("templates", 100, path, nullptr);

    img = imread(bm->_saveScreenToFileWithType("test\\screen2.png", 1));

    img.copyTo(orig);
}

void OpenCV::updateScreen(bmpClass* bm)
{
    img = imread(bm->_saveScreenToFileWithType("test\\screen.png", 1));

    img.copyTo(orig);
}

void _createScaledTemplate(std::string path, std::string name, std::string pathNew, double scale)
{
    cv::Mat temp = imread(path + name + ".png");
    cv::Mat resz;
    if (scale < 1.0)
    {
        cv::resize(temp, resz, cv::Size(), scale, scale, cv::INTER_AREA);
    }
    else
    {
        cv::resize(temp, resz, cv::Size(), scale, scale, cv::INTER_CUBIC);
    }
    imwrite(pathNew + name + ".png", resz);
}

void OpenCV::createScaledTemplates(int newSize)
{
    GetFullPathNameA("templatesNew", 100, pathNew, nullptr);
    double scale = (newSize + 0.0) / (ORIGSQ + 0.0);
    std::cout << scale << "\n";
    for (int i = 0; i < 24; ++i)
    {
        _createScaledTemplate(path, templateVals::templateNames[i], pathNew, scale);
    }
}

void OpenCV::updTemplates(int bg1Old, int bg2Old, int bg1, int bg2)
{
    for (int i = 0; i < 24; ++i)
    {
        std::string _path = pathNew + (string)templateVals::templateNames[i] + (string)".png";
        std::string _pathNew = pathNew + (string)templateVals::templateNames[i] + (string)"7.png";
        cv::Mat img = imread(_path);
        unsigned char r, g, b;
        r = (bg1Old & 255);
        g = ((bg1Old >> 8) & 255);
        b = ((bg1Old >> 16) & 255);
        cv::Vec3b col1Old = { r, g, b };
        r = (bg2Old & 255);
        g = ((bg2Old >> 8) & 255);
        b = ((bg2Old >> 16) & 255);
        cv::Vec3b col2Old = { r, g, b };
        r = (bg1 & 255);
        g = ((bg1 >> 8) & 255);
        b = ((bg1 >> 16) & 255);
        cv::Vec3b col1 = { r, g, b };
        r = (bg2 & 255);
        g = ((bg2 >> 8) & 255);
        b = ((bg2 >> 16) & 255);
        cv::Vec3b col2 = { r, g, b };

        int tolerance = 25;

        std::transform(img.begin<Vec3b>(), img.end<Vec3b>(), img.begin<Vec3b>(), [&](auto& pixel) {
            if (norm(pixel, col1Old, NORM_L2SQR) < tolerance * tolerance) {
                return col1;
            }
            else {
                return pixel;
            }
        });
        std::transform(img.begin<Vec3b>(), img.end<Vec3b>(), img.begin<Vec3b>(), [&](auto& pixel) {
            if (norm(pixel, col2Old, NORM_L2SQR) < tolerance * tolerance) {
                return col2;
            }
            else {
                return pixel;
            }
        });
        imwrite(pathNew + (string)templateVals::templateNames[i] + (std::string)"7.png", img);
    }
}

cv::Mat OpenCV::getTemplates(string file) {
    return imread(pathNew + file + "7.png");
}

cv::Mat OpenCV::conv(Mat& img) {
    Mat hsvImg;
    cvtColor(img, hsvImg, COLOR_BGR2HSV);
    return hsvImg;
};

double OpenCV::computePercentage(std::string filename, int side)
{
    cv::Mat img = imread((std::string)path + "\\" + filename + ".png");

    double matchedB = 0;
    double matchedW = 0;

    int tolerance = 2;

    cv::Mat_<cv::Vec3b>::iterator it = img.begin<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itend = img.end<cv::Vec3b>();

    for(; it != itend; ++it)
    {
        cv::Vec3b pixel = *it;
        if (norm(pixel, pieceRGB::black1, NORM_L2SQR) < tolerance * tolerance) {
            matchedB++;
        }
        else {
            if (norm(pixel, pieceRGB::black2, NORM_L2SQR) < tolerance * tolerance)
            {
                matchedB++;
            }
            else
                if (norm(pixel, pieceRGB::black3, NORM_L2SQR) < tolerance * tolerance)
                {
                    matchedB++;
                }
                else
                    if (norm(pixel, pieceRGB::black4, NORM_L2SQR) < tolerance * tolerance)
                    {
                        matchedB++;
                    }
                    else
                        if (norm(pixel, pieceRGB::black5, NORM_L2SQR) < tolerance * tolerance)
                        {
                            matchedB++;
                        }
                        else
                            if (norm(pixel, pieceRGB::black6, NORM_L2SQR) < tolerance * tolerance)
                            {
                                matchedB++;
                            }
        }
        if (norm(pixel, pieceRGB::white1, NORM_L2SQR) < tolerance * tolerance) {
            matchedW++;
        }
        else {
            if (norm(pixel, pieceRGB::white2, NORM_L2SQR) < tolerance * tolerance)
            {
                matchedW++;
            }
            //else
                //if (norm(pixel, pieceRGB::white3, NORM_L2SQR) < tolerance * tolerance)
                //{
                    //matchedW++;
                //}
        }
    }

    std::cout << "black and white patched for file named " << filename << " " << matchedB << " " << matchedB/sqSize << " <> " << matchedW << " " << matchedW / sqSize  << "\n";
    return max(matchedW, matchedB);
}

int OpenCV::matchPiecePerc(std::string filename, int side)
{
    cv::Mat imgO = imread(filename);

    cv::Mat img;

    double matchedB = 0;
    double matchedW = 0;

    int tolerance = 3;

    if (side != ORIGSQ)
    {
        tolerance = 7;
        double scale = (ORIGSQ + 0.0) / (side + 0.0);
        if (scale < 1.0)
        {
            cv::resize(imgO, img, cv::Size(ORIGSQ, ORIGSQ), 0, 0, cv::INTER_AREA);
        }
        else
        {
            cv::resize(imgO, img, cv::Size(ORIGSQ, ORIGSQ), 0, 0, cv::INTER_CUBIC);
        }
    }

    else
    {
        img = imread(filename);
    }

    imwrite(filename + ".png", img);

    cv::Mat_<cv::Vec3b>::iterator it = img.begin<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itend = img.end<cv::Vec3b>();

    for (; it != itend; ++it)
    {
        cv::Vec3b pixel = *it;
        if (norm(pixel, pieceRGB::black1, NORM_L2SQR) < tolerance * tolerance) {
            matchedB++;
        }
        else {
            if (norm(pixel, pieceRGB::black2, NORM_L2SQR) < tolerance * tolerance)
            {
                matchedB++;
            }
            else
                if (norm(pixel, pieceRGB::black3, NORM_L2SQR) < tolerance * tolerance)
                {
                    matchedB++;
                }
                else
                    if (norm(pixel, pieceRGB::black4, NORM_L2SQR) < tolerance * tolerance)
                    {
                        matchedB++;
                    }
                    else
                        if (norm(pixel, pieceRGB::black5, NORM_L2SQR) < tolerance * tolerance)
                        {
                            matchedB++;
                        }
                        else
                            if (norm(pixel, pieceRGB::black6, NORM_L2SQR) < tolerance * tolerance)
                            {
                                matchedB++;
                            }
        }
        if (norm(pixel, pieceRGB::white1, NORM_L2SQR) < tolerance * tolerance) {
            matchedW++;
        }
        else {
            if (norm(pixel, pieceRGB::white2, NORM_L2SQR) < tolerance * tolerance)
            {
                matchedW++;
            }
            //else
                //if (norm(pixel, pieceRGB::white3, NORM_L2SQR) < tolerance * tolerance)
                //{
                    //matchedW++;
                //}
        }
    }
    //std::cout << matchedB << " <--> " << matchedW << "\n";

    if (matchedB > matchedW)
    {
        //std::cout << "E negru bro \n";
        double perc = matchedB / sqSize;
        double diff = 99999;
        int match = -1;
        for (int i = 6; i < 12; ++i)
        {
            double tempDiff = abs(perc - piecePerc::percentages[i]);
            if (tempDiff < diff)
            {
                diff = tempDiff;
                match = i;
            }
        }
        std::cout << perc << "<-\n";
        return match;
    }
    double perc = matchedW / sqSize;
    double diff = 99999;
    int match = -1;
    for (int i = 0; i < 6; ++i)
    {
        double tempDiff = abs(perc - piecePerc::percentages[i]);
        if (tempDiff < diff)
        {
            diff = tempDiff;
            match = i;
        }
    }
    std::cout << perc << "<-\n";
    return match;
}

std::string covertRankToFen(std::string rank)
{
    std::string rev;
    bool pieceQ = false;
    for (int i = 0; i < 8; ++i)
    {
        rev += rank[i];
        if (rank[i] != '0')
        {
            pieceQ = true;
        }
    }
    if (pieceQ == false)
    {
        return (string)"8/";
    }
    int curEmpty = 0;
    std::string ret;
    for (int i = 0; i < 8; ++i)
    {
        if (rev[i] != '0')
        {
            if (curEmpty != 0)
            {
                ret += (curEmpty + '0');
            }
            ret += rev[i];
            int nextEmpty = 0;
            bool add = false;
            for (int j = i + 1; j < 8; ++j)
            {
                if (j == 7 && rev[j] == '0')
                {
                    add = true;
                }
                if (rev[j] != '0')
                {
                    break;
                }
                nextEmpty++;
            }
            if (nextEmpty != 0 && add)
            {
                ret += (nextEmpty + '0');
            }
            curEmpty = 0;
        }
        else
        {
            curEmpty++;
        }
    }
    return ret + '/';
}

std::string OpenCV::returnFen() {//naive
    string ret[8];
    for (int i = 1; i < 9; ++i)
    {
        for (int j = 1; j < 9; ++j)
        {
            ret[i - 1] += '0';
        }
    }
    int sep = 0;
    for (auto it : board.Pieces)
    {
        auto pc = it.type;
        auto ti = it.position;
        ret[ti.cord - 1][ti.let - 'a'] = pc;
        sep++;
    }
    string fen[8];
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            fen[i] += ret[i][j];
        }
    }
    string _fen;
    for (int i = 0; i < 8; ++i)
    {
        _fen += covertRankToFen(ret[i]);
    }
    return _fen;
};

pair<char,int> OpenCV::indexToCords(int i) {
    char white[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
    char black[] = { 'h', 'g', 'f', 'e', 'd', 'c', 'b', 'a' };
    int slovo = i % 8;
    int broj = i - slovo;
    pair<char, int>position;
    if (playerColor.color)
    {
        //its white
        position.first = white[slovo];
        position.second = 1 + (i>>3);
    }
    else
    {
        position.first = black[slovo];
        position.second = 8 - (i >> 3);
    }
    return position;
}

double OpenCV::detectPieces(Mat& img1, Mat& templ) {
    Mat img_orig;
    img1.copyTo(img_orig);
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
        //imshow(image_window, result);
    }
    return maxVal;
}

    //Returns the highest matched Piece object to the given filename, with piece name and position on chessboard
int OpenCV::matchPiece(char* filename) {
    int max = 0;
    Mat slika = imread(filename);
    double maxval = 0;
    for (int i = 0; i < this->templates.size(); i++) {
        double j = detectPieces(slika, this->templates[i].second);
        std::cout << j << " " << templates[i].first << "\n";
        if (j > maxval) {
            maxval = j;
            max = i;
        }
    };
    std::cout << "The piece that we done found is " << this->templates[max].first << "\n\n\n";
    return max;
}

std::string OpenCV::getPosition(Board board, int x, int y, Player col) {
    char white[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
    char black[] = { 'h', 'g', 'f', 'e', 'd', 'c', 'b', 'a' };
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

void OpenCV::findRects(cv::Mat img, cv::Scalar low, cv::Scalar high, cv::Scalar low1, cv::Scalar high1, COLOR color, COLOR color1, Board board) {
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

void OpenCV::drawRects(cv::Mat& img) { //draws highlight squares on the image of the board
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
}

vector<Tile> OpenCV::addTiles(int size, bool col) {
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

void OpenCV::initColors(bmpClass* bm)
{
    std::pair<int, int> cols = bm->giveBoardColours();
    double r, g, b;
    double h, s, v;
    r = (cols.first & 255) + 0.0;
    g = ((cols.first >> 8) & 255) + 0.0;
    b = ((cols.first >> 16) & 255) + 0.0;
    rgb_to_hsv(r, g, b, h, s, v);
    blacklow = cv::Scalar(h - 8, s - 5, v - 5);
    blackhigh = cv::Scalar(h + 5, 255, 255);
    //std::cout << r << " " << g << " " << b << "<-\n";
    //std::cout << h << " " << s << " " << v << "\n";
    r = (cols.second & 255) + 0.0;
    g = ((cols.second >> 8) & 255) + 0.0;
    b = ((cols.second >> 16) & 255) + 0.0;
    rgb_to_hsv(r, g, b, h, s, v);
    whitelow = cv::Scalar(h - 8, s - 5, v - 5);
    whitehigh = cv::Scalar(h + 5, 255, 255);
    //std::cout << h << " " << s << " " << v << "\n";
}

/// <summary>
/// Gives the coordinates of the board
/// </summary>
/// <param name="filepath">path to the photo</param>
/// <returns>board left, top, right, bottom coordinates</returns>
std::pair < std::pair<int, int>, std::pair<int, int> > OpenCV::giveBoard(bmpClass* bm)
{
    initColors(bm);
    char* filepath = bm->_saveScreenToFileWithType("test\\screen.png", 1);

    std::pair < std::pair<int, int>, std::pair<int, int> > ret;

    Board board;
    cv::Mat img = cv::imread(filepath);

    cv::Mat orig;
    //Switches to HSV
    img.copyTo(orig);
    //cv::imshow("test", img);
    cv::cvtColor(img, img, cv::COLOR_BGR2HSV);

    //White field detection
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

    drawRects(orig);

    std::string test = "\\tesm.png";

    cv::imwrite(path + test, orig);

    ///storin the pieces
    board.width = board.right - board.left;
    board.height = board.bottom - board.top;
    Point p1(board.left + board.width / 16, board.top + 50);
    Point p3(board.left + board.width / 16, board.bottom + 20);
    Vec3b col1 = orig.at<Vec3b>(p1);
    Vec3b col2 = orig.at<Vec3b>(p3);

    if (col2[0] > col1[0]) { // 1 or true is white player
        playerColor = true;
    }
    else {
        playerColor = false;
    }

    Point p2(board.left + 50, board.top + board.height / 16);
    Point p4(board.left + 20, board.bottom - board.width / 16);
    rectangle(orig, p1, p2, Scalar(255, 0, 0), 2, LINE_8);
    rectangle(orig, p3, p4, Scalar(255, 0, 0), 2, LINE_8);

    ret.first = std::pair<int, int>(board.left, board.top);
    ret.second = std::pair<int, int>(board.right, board.bottom);

    return ret;
}
