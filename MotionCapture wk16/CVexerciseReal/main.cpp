#include <time.h>  
#include <math.h>  
#include <ctype.h>  
#include <stdio.h>  
#include <iostream>
#include <stdlib.h>
#include <opencv/cv.h>  
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/highgui.h> 
#include  <opencv/ml.h>	
#include <fstream>
#include<sstream>


using namespace cv;
using namespace std;



//--------Using SIFT as feature extractor and FlannBased for assigning a new point to the nearest one in the dictionary
Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("FlannBased");
Ptr<DescriptorExtractor> extractor = new SiftDescriptorExtractor();
SiftFeatureDetector detector(100);
//---dictionary size=number of cluster's centroids
int clusterSize = 10;
TermCriteria tc(CV_TERMCRIT_ITER, 10, 0.001);
int retries = 3;
int flags = KMEANS_PP_CENTERS;
BOWKMeansTrainer bowTrainer(clusterSize, tc, retries, flags);
BOWImgDescriptorExtractor bowDE(extractor, matcher);
//=====================================FAST========================================================================================
//Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");//BruteForce //FlannBased
//Ptr<DescriptorExtractor> extractor = new OrbDescriptorExtractor();
//Ptr<FeatureDetector> detector = FeatureDetector::create( "FAST" );  
//Ptr<DescriptorExtractor> extractor = DescriptorExtractor::create( "BRIEF" ); 
//Ptr<DescriptorExtractor> extractor = new SiftDescriptorExtractor();
//FastFeatureDetector detector(10);
//SiftFeatureDetector detector(200);
//ORB detector(100);
//int clusterSize = 500;
//TermCriteria tc(CV_TERMCRIT_ITER, 10, 0.001);
//int retries = 1;
//int flags = KMEANS_PP_CENTERS;
//BOWKMeansTrainer bowTrainer(clusterSize, tc, retries, flags);
//BOWImgDescriptorExtractor bowDE(extractor, matcher);

// various tracking parameters (in seconds)
const double MHI_DURATION = 1;
const double MAX_TIME_DELTA = 0.5;
const double MIN_TIME_DELTA = 0.05;
const double threshold_value = 25;
Mat SAMHI_10 = Mat(480, 640, CV_32FC1, Scalar(0, 0, 0));;
int CountLines(char *filename)
{
	ifstream ReadFile;
	int n = 0;
	char line[512];
	string temp;
	ReadFile.open(filename, ios::in);//
	if (ReadFile.fail())//
	{
		return 0;
	}
	else//
	{
		while (getline(ReadFile, temp))
		{
			n++;
		}
		return n;
	}

	ReadFile.close();
}
int main(int argc, char** argv)
{
	//cv::initModule_nonfree();
	IplImage* motionImage = 0;
	VideoCapture cap = 0;
	fstream in("train.txt");
	int LineNumbers;
	LineNumbers = CountLines("train.txt");
	for (int fnum = 1; fnum <= LineNumbers; fnum++)
	{
		string fname;
		string path1;
		string path2;
		string filename;
		int lab;
		in >> lab;
		in >> filename;

		fname = "D:\\Port\\COMVI\\DB\\train\\" + filename;
		cap = VideoCapture(fname);

		Mat image_binary_prev_5, image_binary, image_binary_diff_5;
		int nframes = cap.get(CV_CAP_PROP_FRAME_COUNT); // Getting number of frames
		for (int i = 1; i < nframes; i++)
		{
			Mat frame;
			// Retrieve a single frame from the capture
			cap.read(frame);
			//Mat imageROI;  
			//
			//frame=frame(Rect(90,10,500,450));  //
			cvtColor(frame, image_binary, CV_BGR2GRAY);
			//	imshow("img1",frame); 
			int num = 5;

			if (i == 1) {
				image_binary_prev_5 = image_binary.clone();
			}
			//to perform differences between frames adjacent by 5 frames.
			if (i % num == 0) {
				absdiff(image_binary_prev_5, image_binary, image_binary_diff_5);
				image_binary_prev_5 = image_binary.clone();
			}

			//to create an initial samhi image of 5;
			if (i == num + 1) {
				threshold(image_binary_diff_5, image_binary_diff_5, threshold_value, 255, THRESH_BINARY);
				Size framesize = image_binary_diff_5.size();
				int h = framesize.height;
				int w = framesize.width;
				SAMHI_10 = Mat(h, w, CV_32FC1, Scalar(0, 0, 0));
				updateMotionHistory(image_binary_diff_5, SAMHI_10, (double)i / nframes, MHI_DURATION);
			}
			//creating SAMHI image of 5
			if (i > num + 1 && i % num == 0) {
				threshold(image_binary_diff_5, image_binary_diff_5, threshold_value, 255, THRESH_BINARY);
				updateMotionHistory(image_binary_diff_5, SAMHI_10, (double)i / nframes, MHI_DURATION);
			}
		}
		cout << "File " << fname << " MHI done.." << endl;

		vector<KeyPoint> keypoint;
		SAMHI_10.convertTo(SAMHI_10, CV_8UC1, 255, 0);
		detector.detect(SAMHI_10, keypoint);
		Mat features;
		Mat keyP1, keyP2;
		Mat allDescriptors;
		extractor->compute(SAMHI_10, keypoint, features);
		//features .convertTo(features, CV_32F);
		allDescriptors.push_back(features);
		bowTrainer.add(features);
		//drawKeypoints(SAMHI_10,keypoint,keyP1,Scalar::all(-1),0);  
		//imshow("MHI",SAMHI_10); 
		//imshow("keypoint",keyP1);
		//imshow("features",features);
		// imshow("Image",image_binary_diff_5);
		//cvWaitKey();

	}

	in.close();
	FileStorage fs("Vocabulary.xml", FileStorage::WRITE);
	Mat dictionary = bowTrainer.cluster();

	//int i =0;

	fs << "vocabulary"<<dictionary;
	fs.release();

	//int i =0;
	////////////////////////////////////////

	return 0;
}
