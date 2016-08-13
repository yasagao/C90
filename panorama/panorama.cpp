#include <iostream>
#include "opencv2/opencv.hpp"
#include <string>
#include <vector>

using namespace std;
using namespace cv;

int main()
{
	vector<Mat> input;
	Mat panorama;

	Mat image1 = imread("1.JPG",IMREAD_UNCHANGED );
	Mat image2 = imread("2.JPG",IMREAD_UNCHANGED );
	//Mat image3 = imread("6.JPG",IMREAD_UNCHANGED );
/*
	input.push_back(image1);
	input.push_back(image2);
	input.push_back(image3);
*/
	Ptr<AKAZE> detector = AKAZE::create();

  vector<KeyPoint> keys1 , keys2;
	Mat descriptors1, descriptors2;

	detector->detectAndCompute(image1, noArray(), keys1, descriptors1);
  detector->detectAndCompute(image2, noArray(), keys2, descriptors2);
	//cout << Homography << endl;

	//Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
	vector<DMatch> matches;
	BFMatcher matcher(NORM_HAMMING, true);
	matcher.match(descriptors1,descriptors2,matches);
	vector<DMatch> Good_Matches;
	vector<KeyPoint> good_keys1, good_keys2;

	int i;
	for(i = 0; i < matches.size(); i++)
	{
		if(matches[i].distance < 80)
		{
			Good_Matches.push_back(matches[i]);
			good_keys1.push_back(keys1[matches[i].queryIdx]);
		 	good_keys2.push_back(keys2[matches[i].trainIdx]);
		}
	}

	vector<Point2f> points1, points2;
	KeyPoint::convert(good_keys1,points1);
	KeyPoint::convert(good_keys2,points2);

	Mat Homography = findHomography(points1,points2,CV_RANSAC,10);

	cout << Homography << endl;

	Mat dst_image;      
	warpPerspective(image1,dst_image, Homography, Size(image1.cols * 1.8, image1.rows * 1.3));
	//image2.copyTo(Mat(dst_image, Rect(0, 0, image2.cols,image2.rows)));

	imwrite("panorama.jpg", dst_image);
	namedWindow("panorama",WINDOW_NORMAL);
	imshow("panorama", dst_image);
	waitKey(0);
}
