#include "tttcv.h"

#include <iostream>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

VideoCapture stream;

int coords[9][2] = {
	{331,118}, //TR
	{359,234}, //MR
	{376,341}, //BR
	{216,136}, //TM
	{243,249}, //MM
	{263,356}, //BM
	{ 91,151}, //TL
	{124,264}, //ML
	{158,370}, //BL
};

void initCV()
{
	stream.open(0);
	if (!stream.isOpened())
	{
		std::cerr << "Cannot open camera!" << std::endl;
		exit(1);
	}
}

void checkBoxes(bool filled[9])
{
	Mat capture;
	Mat src;
	Mat boardImage;
	
	for (uint8_t i = 0; i < 10; i++)
	{
		stream.grab();
	}
	stream.read(capture);
	
	boardImage = Mat(capture.size(), CV_8UC1);
	cvtColor(capture, boardImage, COLOR_BGR2GRAY);
	GaussianBlur(boardImage, boardImage, Size(11,11), 0);
	adaptiveThreshold(boardImage, boardImage, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);
	bitwise_not(boardImage, boardImage);
	
	Mat kernel = (Mat_<uchar>(3,3) << 0,1,0,1,1,1,0,1,0);
	dilate(boardImage, boardImage, kernel);
	
	imwrite("test.jpg", boardImage);
	
	for (uint8_t i = 0; i < 9; i++)
	{
		//std::cout << "tile: " << i << std::endl;
		int whitePixels = 0;
		int x = coords[i][0];
		int y = coords[i][1];
		for (int curX = x - 30; curX < x + 30; curX++)
		{
			for (int curY = y - 30; curY < y + 30; curY++)
			{
				if (boardImage.at<uint8_t>(curY, curX) > 127)
				{
					whitePixels++;
				}
			}
		}
		//std::cout << "num white pixels in tile " << i << ": " << whitePixels << std::endl;
		if (whitePixels > (60 * 60 * 5 / 100)) // >5%
		{
			filled[i] = true;
		}
		else
		{
			filled[i] = false;
		}
		//std::cout << "tile " << i << " filled: " << filled[i] << std::endl;
	}
	
	std::cout << "Done with image" << std::endl;
}
