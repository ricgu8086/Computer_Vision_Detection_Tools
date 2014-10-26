//============================================================================
// Name        : main.cpp
// Author      : Ricardo Guerrero Gómez-Olmedo
// Version     : 1.0
// Copyright   : LGPL v.3
// Description : Example of how to use Matlab-ported functions and other tools
//				 designed for dealing with detections in C++
//============================================================================

#include <opencv2/opencv.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv/cv.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

#include "functions.h"

#define widthFrame 2000
#define heightFrame 1200

using namespace std;
using namespace DetectionSpace;

int main(int argc, char *argv[])
{
	int frame = -1;
	ostringstream out; //Output String Stream
	vector <struct detection> list_detections;
	string string_detection;
	cvNamedWindow ("Before NMS", CV_WINDOW_AUTOSIZE);
	cvNamedWindow ("After NMS", CV_WINDOW_AUTOSIZE);
	IplImage *before = cvCreateImage(cvSize(widthFrame, heightFrame), IPL_DEPTH_8U, 3);
	IplImage *before_small = cvCreateImage(cvSize(widthFrame/4, heightFrame/4), IPL_DEPTH_8U, 3);
	cvZero(before);
	IplImage *after = cvCreateImage(cvSize(widthFrame, heightFrame), IPL_DEPTH_8U, 3);
	IplImage *after_small = cvCreateImage(cvSize(widthFrame/4, heightFrame/4), IPL_DEPTH_8U, 3);
	cvZero(after);
	CvPoint top_left, bottom_right;
	ifstream fs; //Input File Stream
	struct detection detection;

	//Suggested frames for test the algorithms: 3, 14, 104, 105

	cout << "Introduce the number of frame you want to read its detections:  ";
	cin >> frame;

	while ( !cin.eof() && !cin.fail() && frame >0)
	{
		/** Opening the file with detections **/

		//Hard way -> C++ style
		out.str(""); //Clean the stream every iteration
		out << "./test/image";
		out.width(6);
		out.fill('0');
		out << frame;
		out << ".jpg.detections";

		//Very easy way -> C style
		//sprintf (rute, "./test/image%.6d.jpg.detections", frame);

		fs.open (out.str().c_str(), ios::in);

		if (!fs)
		{
			cout << "Error. Cannot read the file" << endl;
			return 0;
		}

		/** Reading all detections in the file **/

		list_detections.clear();

		do
		{
			getline(fs, string_detection);

			if (fs.eof())
			{
				break;
			}

			processString (string_detection, &detection);

			string_detection.clear();


			list_detections.push_back(detection);
		}
		while (true);

		/** We don't need the file anymore **/
		fs.close();

		/** Ensure data readed from file is correct:
		    no negative coordinates, left value is no bigger than right's, etc **/

		list_detections = detectionsFiltering(list_detections, widthFrame, heightFrame);

		/** Show detections before NMS **/

		for (unsigned int i=0;i<list_detections.size();i++)
		{
			top_left = cvPoint(list_detections.at(i).x1, list_detections.at(i).y1);
			bottom_right = cvPoint(list_detections.at(i).x2, list_detections.at(i).y2);
			cvRectangle(before, top_left, bottom_right, CV_RGB(0xff,0x00,0x00), 4);
		}

		//The original detections were computed in a 2000x1200 image (in order to improve the detector accuracy)
		//but that size is too big for the screen so after drawing the bounding boxes
		//the images will be scaled to fit better.
		cvResize(before, before_small);
		cvShowImage("Before NMS", before_small);

		/** Calling fastNms to erase overlapped detections, but first we detect if there is any overlap **/

		if (collisionsAll(list_detections))
		{
			list_detections = fastNms (list_detections);
		}

		/** Show detections after NMS **/

		for (unsigned int i=0;i<list_detections.size();i++)
		{
			top_left = cvPoint(list_detections.at(i).x1, list_detections.at(i).y1);
			bottom_right = cvPoint(list_detections.at(i).x2, list_detections.at(i).y2);
			cvRectangle(after, top_left, bottom_right, CV_RGB(0x00,0xff,0x00), 4);
		}

		cvResize(after, after_small);
		cvShowImage("After NMS", after_small);

		cvWaitKey(0);

		cout << "Introduce the number of the frame you want to read its detections:  ";
		cin >> frame;

		cvZero(before);
		cvZero(after);
	}



	cvReleaseImage(&before);
	cvReleaseImage(&before_small);
	cvReleaseImage(&after);
	cvReleaseImage(&after_small);
	cvDestroyWindow("Antes NMS");
	cvDestroyWindow("Despues NMS");
}


/*
 * *** Performance comparative *** *

without collisionsAll()
Introduce the number of frame you want to read its detections:  3
Done in 6.379e-06sec.
Introduce the number of frame you want to read its detections:  14
Done in 3.889e-05sec.
Introduce the number of frame you want to read its detections:  104
Done in 4.4472e-05sec.
Introduce the number of frame you want to read its detections:  105
Done in 3.4988e-05sec.

with collisionsAll()
Introduce the number of frame you want to read its detections:  3
Done in 5.6e-07sec.
Introduce the number of frame you want to read its detections:  14
Done in 1.556e-06sec.
Introduce the number of frame you want to read its detections:  104
Done in 6.3775e-05sec.
Introduce the number of frame you want to read its detections:  105
Done in 4.6393e-05sec.

only collisionsAll(), no fastNms()
Introduce the number of frame you want to read its detections:  3
Done in 5.38e-07sec.
Introduce the number of frame you want to read its detections:  14
Done in 1.331e-06sec.
Introduce the number of frame you want to read its detections:  104
Done in 1.518e-06sec.
Introduce the number of frame you want to read its detections:  105
Done in 1.098e-06sec.


Note: test was used using:

	double t = (double)cv::getTickCount();
	//code to evaluate
	t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
	cout << "Done in " << t << "sec." << endl;
 */


