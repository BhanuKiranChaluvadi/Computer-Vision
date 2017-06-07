// CannyStill.cpp

#include<opencv2/calib3d.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2\xfeatures2d.hpp>

#include<iostream>
#include<vector>
#include<ctime>
#include "function.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
int main() {

	srand(time(NULL));
	cv::Mat img_1;        // input image
	cv::Mat img_2;        // input image

	img_1 = cv::imread("2a.jpg");          // open image
	img_2 = cv::imread("2b.jpg");          // open image

	if (!img_1.data || !img_2.data) {                                  // if unable to open image
		std::cout << "error: image not read from file \n";     // show error message on command line
		return -1;                                             // and exit program
	}

	// create a SIFT pointer
	cv::Ptr<cv::Feature2D> f2d = cv::xfeatures2d::SIFT::create();

	// Detect the key points
	std::vector<cv::KeyPoint> keypoints_1, keypoints_2;

	// Detect keypoints
	f2d->detect(img_1, keypoints_1);
	f2d->detect(img_2, keypoints_2);

	// Calcualte decriptors( feature vectors )
	cv::Mat descriptors_1, descriptors_2;
	f2d->compute(img_1, keypoints_1, descriptors_1);
	f2d->compute(img_2, keypoints_2, descriptors_2);

	// Add results to image and save
	cv::Mat output_1, output_2;
	cv::drawKeypoints(img_1, keypoints_1, output_1);
	cv::drawKeypoints(img_2, keypoints_2, output_2);

	// Consruction of matcher BFMatcher - Brute Force Matcher 
	//cv::BFMatcher matcher;
	cv::FlannBasedMatcher matcher;
	std::vector <cv::DMatch> matches;
	matcher.match(descriptors_1, descriptors_2, matches);

	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distance between keypoints
	for (int i = 0; i < descriptors_1.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n", min_dist);

	//-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
	//-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
	//-- small)
	//-- PS.- radiusMatch can also be used here.
	std::vector< cv::DMatch > good_matches;
	for (int i = 0; i < descriptors_1.rows; i++)
	{
		if (matches[i].distance <= std::max(2 * min_dist, 0.02))
		{
			good_matches.push_back(matches[i]);
		}
	}

	//-- Draw only good matches
	cv::Mat img_matches;
	cv::drawMatches(img_1, keypoints_1, img_2, keypoints_2,
		good_matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
		std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//-- Show detected matches
	cv::namedWindow("Good Matches", CV_WINDOW_NORMAL);     // note: you can use CV_WINDOW_NORMAL which allows resizing the window, CV_WINDOW_AUTOSIZE
	imshow("Good Matches", img_matches);

	/*
	for (int i = 0; i < (int)good_matches.size(); i++)
	{
		printf("-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx);
	}*/

	std::vector<cv::Point2f> src_pts, dst_pts;
	for (std::vector <cv::DMatch>::iterator it = good_matches.begin(); it != good_matches.end(); ++it)
	{
		src_pts.push_back(keypoints_1[it->queryIdx].pt);
		dst_pts.push_back(keypoints_2[it->trainIdx].pt);
	}

	// Fundamental matrix 
	cv::Mat fundamental_matrix = cv::findFundamentalMat(src_pts, dst_pts, CV_FM_RANSAC);
	std::cout << " " << std::endl;
	std::cout << "First fundamental matrix - Direct code - OpenCV" << std::endl;
	std::cout << fundamental_matrix << std::endl;

	// find_A_matrix
	cv::Mat findFundamental_Mat = find_fundamental_matrix(src_pts, dst_pts, 20000, 0.25);
	std::cout << " " << std::endl;
	std::cout << "Final fundamental matrix - Ransac implementation" << std::endl;
	std::cout << findFundamental_Mat << std::endl;
	
	cv::waitKey(0);

	return 0;
}



