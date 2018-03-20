#include<opencv2\opencv.hpp>

//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include <iostream>
//using namespace cv;
//using namespace std;
//int main()
//{
//	Mat img = imread("C:/Users/alpha/Dropbox/_Port/Y5/comvi/database/ba.jpg", 0);
//	Mat grayHist;
//	int histSize = 256;
//	float range[] = { 0, 256 };
//	const float* histRange = { range };
//	calcHist(&img, 1, 0, Mat(), grayHist, 1, &histSize, &histRange); //calculate gray-scale histogram
//	cout << grayHist << endl; //display histogram 
//	return 0;
//}

//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/objdetect/objdetect.hpp"
//#include <iostream>
//using namespace cv;
//using namespace std;
//int main()
//{
//	Mat img = imread("C:/Users/alpha/Dropbox/_Port/Y5/comvi/database/ba.jpg", 0);
//	Mat resized_img;
//	resize(img, resized_img, Size(64, 128)); //resize image
//	HOGDescriptor hog;
//	vector<float>descriptors;
//	hog.compute(resized_img, descriptors, Size(0, 0), Size(0, 0)); //compute HoG histogram
//	cout << descriptors.size() << endl;
//	return 0;
//}


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/legacy/legacy.hpp" 
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;
int main()
{
	namedWindow("SIFT", CV_WINDOW_AUTOSIZE);
	namedWindow("SIFTmatching", CV_WINDOW_AUTOSIZE);
	Mat img1 = imread("C:/Users/alpha/Dropbox/_Port/Y5/comvi/database/pb.png", 0);
	Mat img2 = imread("C:/Users/alpha/Dropbox/_Port/Y5/comvi/database/pb2.jpg", 0);
	SIFT sift1, sift2;
	vector<KeyPoint> key_points1, key_points2;
	Mat descriptors1, descriptors2, maskmat;
	sift1(img1, maskmat, key_points1, descriptors1); //detect key points and their corresponding SIFT descriptors
	sift2(img2, maskmat, key_points2, descriptors2);
	BruteForceMatcher<L2<float>> matcher;
	vector<DMatch> matches;
	matcher.match(descriptors1, descriptors2, matches); //keypoints matching
	std::nth_element(matches.begin(), matches.begin() + 99, matches.end()); //extract ~100~ 25 best matches
	matches.erase(matches.begin() + 100, matches.end());  // delete the rest matches
	Mat keypoint_img, matching_img;
	drawKeypoints(img1, key_points1, keypoint_img, Scalar(255, 255, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	imshow("SIFT", keypoint_img);
	drawMatches(img1, key_points1, img2, key_points2, matches, matching_img, Scalar(255, 255, 255), Scalar::all(-1), maskmat, DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	imshow("SIFTmatching", matching_img);
	waitKey(0);
	return 0;
}