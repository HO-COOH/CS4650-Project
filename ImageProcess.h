#pragma once
#include <opencv2/opencv.hpp>

cv::Mat OilEffect(const cv::Mat& original, int radius, int intensity);
cv::Mat OilEffect2(const cv::Mat& original, int radius, int intensity);
cv::Mat PencilEffect(const cv::Mat& img, int K_SIZE, int S_SIZE);
