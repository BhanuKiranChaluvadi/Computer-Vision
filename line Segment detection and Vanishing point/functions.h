#pragma once

#include <cstdlib>
#include <vector>
#include<opencv2/core/core.hpp>
#include <tuple>
#include <iostream>

int generate_random_no(int min, int max)
{
	int rand_num;
	rand_num = min + (rand() % (int)(max - min + 1));
	return rand_num;
}

void draw_line_using_coefficents(cv::Mat img, cv::Mat coefficients)
{
	cv::Point Pt1;
	cv::Point Pt2;
	Pt1.x = -coefficients.at<float>(2) / coefficients.at<float>(0);
	Pt1.y = 0.0;
	Pt2.x = 0.0;
	Pt2.y = -coefficients.at<float>(2) / coefficients.at<float>(1);

	cv::line(img, Pt1, Pt1, cv::Scalar(0, 0, 255));
}

cv::Point points_from_coefficients_x(cv::Mat coefficients)
{
	// return point on x-axis
	cv::Point Pt1;
	Pt1.x = -coefficients.at<float>(2) / coefficients.at<float>(0);
	Pt1.y = 0.0;

	return Pt1;
}

cv::Point points_from_coefficients_y(cv::Mat coefficients)
{
	// return point on y-axis
	cv::Point Pt2;
	Pt2.x = 0.0;
	Pt2.y = -coefficients.at<float>(2) / coefficients.at<float>(1);

	return Pt2;
}



std::tuple<cv::Mat, std::vector<int>> find_dominant_Vanishing_point(std::vector<cv::Mat> norms, int threshold, int itr)
{
	int highest_count = 0;
	cv::Mat Vanishing_pt;
	std::vector<int>global_collector = {};
	for (int i = 0; i < itr; i++)
	{
		int rand_1 = generate_random_no(0, norms.size()-1);
		int rand_2 = generate_random_no(0, norms.size()-1);
		while (rand_1 == rand_2)
			rand_2 = generate_random_no(0, norms.size()-1);

		// Perform cross-product of two lines to find the intesecting point
		cv::Mat int_point;
		int_point = norms[rand_1].cross(norms[rand_2]);

		// converting  homogeneous to non-homogeneous form
		int_point.at<float>(0) = int_point.at<float>(0) / int_point.at<float>(2);
		int_point.at<float>(1) = int_point.at<float>(1) / int_point.at<float>(2);
		int_point.at<float>(2) = 1.0;

		// dot product
		int counter = 0;
		std::vector<int>local_collector = {};
		for (int i = 0; i < norms.size(); i++)
		{
			double dot = norms[i].dot(int_point);
			if (dot >= -(threshold) && dot <= (threshold))
			{
				local_collector.push_back(i);
				counter++;
			}
		}
		// Setting the Vanishing point
		if (counter > highest_count)
		{
			highest_count = counter;
			Vanishing_pt = int_point.clone();
			global_collector = local_collector;
		}
	}
	return  std::make_tuple(Vanishing_pt, global_collector);
}


// vector of matrices 
std::tuple<std::vector <cv::Mat>, std::vector<cv::Point>, std::vector<cv::Point>> find_vanishing_pts(
	cv::Mat img, std::vector<cv::Mat> norms, int no_of_vanishing_pt, int threshold, int itr)
{
	std::vector <cv::Mat> Vanishing_points;
	cv::Mat Vanishing_pt;
	std::vector<int>lines_passing = {};
	std::vector<cv::Point> line_passing_points_x = {};
	std::vector<cv::Point> line_passing_points_y = {};
	for (int i = 0; i < no_of_vanishing_pt; i++)
	{
		tie(Vanishing_pt, lines_passing) = find_dominant_Vanishing_point(norms, threshold, itr);
		Vanishing_points.push_back(Vanishing_pt.clone());

		// Draw lines on the image
		// Erase from the out
		for (int k = 0; k < lines_passing.size(); k++)
		{
			line_passing_points_x.push_back(points_from_coefficients_x(norms[lines_passing[k]]));
			line_passing_points_y.push_back(points_from_coefficients_y(norms[lines_passing[k]]));
		}

		for (int j = lines_passing.size(); j > 0; j--)
		{
			norms.erase(norms.begin() + lines_passing[j-1]);
		}

	}

	return std::make_tuple(Vanishing_points, line_passing_points_x, line_passing_points_y);
}


void draw_vanishing_points(cv::Mat img, std::vector <cv::Mat> Vanishing_points, double radius)
{
	int no_of_points = Vanishing_points.size();
	cv::Point center;
	int thickness = -1;
	int lineType = 5;
	for (int i = 0; i < no_of_points; i++)
	{
		center.x = Vanishing_points[i].at<float>(0);
		center.y = Vanishing_points[i].at<float>(1);
		cv::circle(img, center, radius, cv::Scalar(255, 0, 0), thickness, lineType);
	}
}




/*
// Display a vector
for (cv::Mat n : Vanishing_pts)
	std::cout << n << std::endl;
*/