#pragma once

#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv\cv.h>
//#include<vector>

void draw_point(cv::Mat imag, cv::Point2f point, double radius)
{
	int thickness = -1;
	int lineType = 8;
	cv::circle(imag, point, radius, cv::Scalar(0, 0, 255), thickness, lineType);
}

void diplay_image(cv::Mat img, std::string name)
{
	cv::namedWindow(name, CV_WINDOW_NORMAL);     // note: you can use CV_WINDOW_NORMAL which allows resizing the window, CV_WINDOW_AUTOSIZE
	imshow(name, img);
}

cv::Mat convert_cv_point_to_Mat_size_3_1(cv::Point2f point)
{
	float pt[3] = { point.x, point.y, 1};
	cv::Mat Pt(3, 1, CV_32FC1, pt);
	return Pt;
}

cv::Mat convert_cv_point_to_Mat_size_1_3(cv::Point2f point)
{
	float pt[3] = { point.x, point.y, 1 };
	cv::Mat Pt(1, 3, CV_64FC1, pt);
	return Pt;
}

int generate_single_random_no(int min, int max)
{
	int rand_num;
	rand_num = min + (rand() % (int)(max - min + 1));
	return rand_num;
}

std::vector<int> generate_random_numbers(int number, int min, int max)
{
	std::vector<int> v;

	while(v.size() < 9)
	{
		int rand_num = generate_single_random_no(min, max);
		if (std::find(v.begin(), v.end(), rand_num) == v.end())
			v.push_back(rand_num);
	}
	/*
	std::cout << "Display" << std::endl;
	for (int n : v)
		std::cout << n << std::endl; */

	return v;
}
cv::Mat find_F_matrix(std::vector<cv::Point2f> src_pts, std::vector<cv::Point2f> dst_pts)
{
	std::vector<int> random_numbers = generate_random_numbers(9, 0, src_pts.size()-1);

	cv::Mat A;
	
	for (auto itr = random_numbers.begin(); itr != random_numbers.end(); ++itr)
	{
		// converting into homogeneous form
		cv::Mat P = (cv::Mat_<float>(3, 1) << src_pts[*itr].x, src_pts[*itr].y, 1);
		cv::Mat P_not = (cv::Mat_<float>(3, 1) << dst_pts[*itr].x, dst_pts[*itr].y, 1);
		cv::Mat mul = P * P_not.t(); //3X3 Matrix

		A.push_back(mul.reshape(1, 1));
	}

	cv::Mat S, U, V;
	cv::SVD::compute(A, S, U, V);

	cv::Mat output = V.col(8).clone();

	return output.reshape(1, 3);
}

cv::Mat find_fundamental_matrix(std::vector<cv::Point2f> src_pts, std::vector<cv::Point2f> dst_pts, int iterations, double threshold)
{
	cv::Mat Global_Fundamental_Mat;
	int max_count = 0;
	for (int i = 0; i < iterations; i++)
	{
		cv::Mat Local_F_Mat = find_F_matrix(src_pts, dst_pts);
		int local_count = 0;
		// check
		for (int j = 0; j < src_pts.size(); j++)
		{
			cv::Mat P = (cv::Mat_<float>(3, 1) << src_pts[j].x, src_pts[j].y, 1);
			cv::Mat P_dash = (cv::Mat_<float>(3, 1) << dst_pts[j].x, dst_pts[j].y, 1);
			cv::Mat product = P.t() * Local_F_Mat * P_dash;  // 1X1 Matrix - only one element
			double scalar = product.at<float>(0);
			if (abs(scalar) <= threshold)
				local_count++;
		}
		//std::cout << "local_local_Count = " << local_count << std::endl;
		if (max_count < local_count)
		{
			max_count = local_count;
			Global_Fundamental_Mat = Local_F_Mat.clone();
			if (max_count == src_pts.size())
				break;
		}
	}
	return Global_Fundamental_Mat;
}

