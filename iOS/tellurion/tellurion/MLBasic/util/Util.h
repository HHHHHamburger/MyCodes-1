#pragma once
#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

#ifdef _DEBUG
#pragma comment(lib, "opencv_core2413d")
#pragma comment(lib, "opencv_highgui2413d")
#pragma comment(lib, "opencv_features2d2413d")
#pragma comment(lib, "opencv_ml2413d")
#pragma comment(lib, "opencv_nonfree2413d")
#pragma comment(lib, "opencv_imgproc2413d")
#else
#pragma comment(lib, "opencv_core2413")
#pragma comment(lib, "opencv_highgui2413")
#pragma comment(lib, "opencv_features2d2413")
#pragma comment(lib, "opencv_ml2413")
#pragma comment(lib, "opencv_nonfree2413")
#pragma comment(lib, "opencv_imgproc2413")
#endif

#include <stdio.h> 
#include <sys/uio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <string>

class ImageUtil {
public:
	static cv::Mat ImageReSize(const cv::Mat& image, int weight = 255, int height = 255, bool scale = false)
	{
		cv::Mat unified_image;
		cv::Size size;
		if (scale) {
			size = cv::Size(weight, height);
		} else {
			int s = cv::min(image.rows, image.cols);
			float scale = 1.0*weight / s;
			size = cv::Size(image.cols  * scale, image.rows * scale);
		}
		cv::resize(image, unified_image, size);
		cv::medianBlur(unified_image, unified_image, 3);
		return unified_image;
	}
    static Mat ImageCut(const Mat &origin) {
        Mat img = ImageUtil::ImageReSize(origin, 400, 400, false);
        int row_start = (int)((float)img.rows / 2 - 120);
        int row_end = (int)((float)img.rows / 2  + 120);
        int col_start = (int)((float)img.cols / 2 - 120);
        int col_end = (int)((float)img.cols / 2 + 120);
        Range row_range(row_start, row_end);
        Range col_range(col_start, col_end);
        return Mat(img, row_range, col_range);
    }
	static cv::Mat load(std::string image_file_path) {
		cv::Mat image;
		if (image_file_path.find(".jpg")) 
			image = cv::imread(image_file_path);
		return image;
	}
    static Mat ImageSmooth(const Mat &src, const int height, const int width) {
        Mat smooth_image;
        GaussianBlur(src, smooth_image, Size(height, width), 0, 0);
        return smooth_image;
    }
};

class Util {
public:
    static double FindMax(const double array[], const int lenght) {
        double max_element = -1;
        for (int i = 0; i < lenght; i++) {
            if (array[i] > max_element) max_element = array[i];
        }
        return max_element;
    }
	static void writeMat(const Mat &mat, const string &fileName)
	{
		int n = mat.rows;
		int m = mat.cols;
		// Declare what you need
		cv::FileStorage file(fileName, cv::FileStorage::WRITE);
		// Write to file!
		try {
			file << "vocab" << mat;
		}
		catch (Exception e)
		{
			cout << e.what() << endl;
			getchar();
		}
	}
	static Mat readMat(const string &fileName) {

		// Declare what you need
		cv::FileStorage file(fileName, cv::FileStorage::READ);

		Mat mat;
		string mat_name;
		// Write to file!
		try {
			file["vocab"] >> mat;
		}
		catch (Exception e)
		{
			cout << e.what() << endl;
			getchar();
		}
		return mat;

	}
};

#endif
