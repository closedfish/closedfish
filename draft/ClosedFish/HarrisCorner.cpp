#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <Windows.h>
#include <wingdi.h>
#include <utility>
#include <queue>
#include <algorithm>
#include "HarrisCorner.h"

void HarrisCorner::addCorners()
{
    // Load the input image and convert to grayscale
    HBITMAP hbitmap = (HBITMAP)LoadImage(NULL, (LPCSTR)L"image.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    BITMAP bitmap;
    GetObject(hbitmap, sizeof(BITMAP), &bitmap);
    BYTE* pixels = (BYTE*)bitmap.bmBits;
    int width = bitmap.bmWidth;
    int height = bitmap.bmHeight;
    int stride = bitmap.bmWidthBytes;

    // Compute the gradient of the image using finite differences
    float* Ix = new float[width * height];
    float* Iy = new float[width * height];
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int p = y * stride + x;
            int px = (x < width - 1) ? p + 1 : p;
            int py = (y < height - 1) ? p + stride : p;
            Ix[p] = pixels[px] - pixels[p];
            Iy[p] = pixels[py] - pixels[p];
        }
    }

    // Compute the Harris corner response function
    float* R = new float[width * height];
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int p = y * stride + x;
                    R[p] = Ix[p] * Iy[p] - Ix[p] * Iy[p];
        }
    }

    // Threshold the response function to find corner points
    BYTE* corners = new BYTE[width * height];
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int p = y * stride + x;
            corners[p] = (R[p] > 0.1) ? 255 : 0;
        }
    }

    // Extract the corner points
    std::vector<POINT> points;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int p = y * stride + x;
            if (corners[p] > 0) {
                points.push_back({x, y});
                std::cout << x << " "  << y << " \n";
            }
        }
    }

    // Draw the corner points on the image
    /*HDC hdc = GetDC(hwnd);
    HDC hdc_mem = CreateCompatibleDC(hdc);
    HBITMAP hbitmap_mem = CreateCompatibleBitmap(hdc, width, height);
    HBITMAP hbitmap_old = (HBITMAP)SelectObject(hdc_mem, hbitmap_mem);
    BitBlt(hdc_mem, 0, 0, width, height, hdc, 0, 0, SRCCOPY);

    // Set up the pens and brushes for drawing
    HPEN hpen_red = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    HPEN hpen_old = (HPEN)SelectObject(hdc_mem, hpen_red);
    HBRUSH hbrush_old = (HBRUSH)SelectObject(hdc_mem, GetStockObject(NULL_BRUSH));

    // Draw the corner points on the image
    for (POINT point : points) {
        Ellipse(hdc_mem, point.x - 5, point.y - 5, point.x + 5, point.y + 5);
    }

    // Clean up
    SelectObject(hdc_mem, hpen_old);
    SelectObject(hdc_mem, hbrush_old);
    DeleteObject(hpen_red);

    // Copy the image with the corner points back to the screen
    BitBlt(hdc, 0, 0, width, height, hdc_mem, 0, 0, SRCCOPY);

    // Clean up
    SelectObject(hdc_mem, hbitmap_old);
    DeleteObject(hbitmap_mem);
    DeleteDC(hdc_mem);
    ReleaseDC(hwnd, hdc);

    */
}
