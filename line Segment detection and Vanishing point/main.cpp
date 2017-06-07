#include <stdio.h>
#include <stdlib.h>
#include "lsd.h"

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include <iomanip>
#include<conio.h>           // may have to modify this line if not using Windows
#include <vector>
#include "functions.h"

int main()
{
	// ***** Tweak these ******////
	int threshold = 50;		//40	// Max allowable distance bt vanishing point and the line segment
	int no_vanising_points = 3;		// No of vanishing points needed to compute
	int iterations = 10000;			// No of iterations, finding the intersection of 2 random lines.
	// ***** Tweak these ******////

	srand(time(0));
	cv::Mat imgOriginal;        // input image
	std::cout << "Size of image" << std::endl;
	imgOriginal = cv::imread("image_1.jpg", 1);					// open image

	if (imgOriginal.empty()) {                                  // if unable to open image
		std::cout << "error: image not read from file\n\n";     // show error message on command line
		_getch();                                               // may have to modify this line if not using Windows
		return(0);                                              // and exit program
	}

	std::vector<int> compression_params_PGM;
	compression_params_PGM.push_back(CV_IMWRITE_PXM_BINARY);
	compression_params_PGM.push_back(0);
	cv::imwrite("image.pgm", imgOriginal, compression_params_PGM);

	// Read the saved .pgm image
	cv::Mat pgm_image;
	pgm_image = cv::imread("image.pgm", 0);

	// reading the image into double *
	int n;
	int rows = pgm_image.rows;
	int cols = pgm_image.cols;
	std::cout << cols << " " << rows << std::endl;
	double * img;
	double * out;
	img = (double *)malloc(rows * cols * sizeof(double));

	cv::MatIterator_<uchar> it, end;
		
	for(int i=0; i<rows; i++)
		for (int j = 0; j < cols; j++)
			img[i+j*rows] = pgm_image.at<uchar>(i, j);

	// LSD call
	out = lsd(&n, img, rows, cols);

	std::vector<cv::Mat> norms = {};
	for (int i = 0; i < n; i++)
	{
		double read[4]; // 4 co-ordinates of 2 points
		for (int j = 0; j < 4; j++)
			read[j] = float(out[7 * i + j]);
			
		float va[3] = { read[1], read[0], 1 };  // Point1 co-ordinates in homogeneous form
		float vb[3] = { read[3], read[2], 1 };	// Point2 co-ordinates in homogeneous form
		float vc[3];
		cv::Mat Va(1, 3, CV_32FC1, va);
		cv::Mat Vb(1, 3, CV_32FC1, vb);
		norms.push_back(Va.cross(Vb));			// cross-product in homogeneous gives equation of line a,b,c coefficants
	}

	/* free memory */
	free((void *)img);
	free((void *)out);

	// Find Vector of vanishing points
	std::vector <cv::Mat> Vanishing_pts;
	std::vector<cv::Point> X_s, Y_s;
	tie(Vanishing_pts, X_s, Y_s)= find_vanishing_pts(imgOriginal, norms, no_vanising_points, threshold, iterations);
	std::cout << " " << std::endl;
	std::cout << "Vanishinig Points" << std::endl;
	for (cv::Mat n : Vanishing_pts)
		std::cout << n << std::endl;

	// Draw lines and vanishing points on the image
	for (int i = 0; i < X_s.size(); i++)
		cv::line(imgOriginal, X_s[i], Y_s[i], cv::Scalar(0, 255, 0));

	double radius = 7.0;
	draw_vanishing_points(imgOriginal, Vanishing_pts, radius);

	// Display image
	cv::namedWindow("imgOriginal", CV_WINDOW_NORMAL);     // note: you can use CV_WINDOW_NORMAL which allows resizing the window
	cv::imshow("imgOriginal", imgOriginal);     // show windows

	cv::waitKey(0);                 // hold windows open until user presses a key

	return(0);

	}
