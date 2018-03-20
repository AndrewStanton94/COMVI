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
#include <opencv/ml.h>	
#include <fstream>
#include <sstream>
using namespace cv;
using namespace std;
//SIFT feature

//Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("FlannBased");
//Ptr<DescriptorExtractor> extractor = new SiftDescriptorExtractor();
//SiftFeatureDetector detector(100);
//int clusterSize = 4;
//TermCriteria tc(CV_TERMCRIT_ITER, 10, 0.001);
//int retries = 1;
//int flags = KMEANS_PP_CENTERS;
//BOWKMeansTrainer bowTrainer(clusterSize, tc, retries, flags);
//BOWImgDescriptorExtractor bowDE(extractor, matcher);
//---------------------------------ORB Feature----------------------------------------------------------
Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("FlannBased");
Ptr<DescriptorExtractor> extractor = new SiftDescriptorExtractor();
//Ptr<DescriptorExtractor> extractor = new OrbDescriptorExtractor();
//cv::DescriptorExtractor * extractor = new cv::OrbDescriptorExtractor();
SiftFeatureDetector detector(100);
//FeatureDetector * detector = new OrbFeatureDetector(100);//(100);
//FastFeatureDetector detector(100);
int clusterSize = 10;
TermCriteria tc(CV_TERMCRIT_ITER, 10, 0.001);
int retries = 3;
int flags = KMEANS_PP_CENTERS;
BOWKMeansTrainer bowTrainer(clusterSize, tc, retries, flags);
BOWImgDescriptorExtractor bowDE(extractor, matcher);

//----------------------------------------------------------------------------------------------

// various tracking parameters (in seconds)
const double MHI_DURATION = 1;
const double MAX_TIME_DELTA = 0.5;
const double MIN_TIME_DELTA = 0.05;
const double threshold_value = 25;
Mat SAMHI_10 = Mat(480, 640, CV_32FC1, Scalar(0, 0, 0));
int CountLines(char *filename){
	ifstream ReadFile;
	int n = 0;
	char line[512];
	string temp;
	ReadFile.open(filename, ios::in);
	if (ReadFile.fail())	{
		return 0;
	}
	else{
		while (getline(ReadFile, temp)){
			n++;
		}
		return n;
	}
	ReadFile.close();
}

int main(int argc, char** argv){
	IplImage* motionImage = 0;
	VideoCapture cap = 0;
	fstream in("test.txt");
	int LineNumbers;
	LineNumbers = CountLines("test.txt");

	Mat dictionary;
	FileStorage fs2("Vocabulary.xml", FileStorage::READ);

	if (fs2.isOpened()){
		fs2["vocabulary"] >> dictionary;
	}
	bowDE.setVocabulary(dictionary);

	CvSVM MotionRecognition;
	MotionRecognition.load("svmidch.xml");

	Mat groundTruth(0, 1, CV_32FC1);
	Mat evalData(0, clusterSize, CV_32FC1);
	vector<KeyPoint> keypoint2;
	Mat bowDescriptor2;

	Mat results(0, 1, CV_32FC1);

	string fname;
	//float l;
	//in>>l;

	float label;
	for (int fnum = 1; fnum <= LineNumbers; fnum++){
		string fname;
		string path1;
		string path2;
		string filename;
		float lab;
		in >> lab;
		in >> filename;
		string filename1 = "D:\\Port\\COMVI\\DB\\train\\" + filename;
		cap = VideoCapture(filename1);

		Mat image_binary_prev_5, image_binary, image_binary_diff_5;
		int nframes = cap.get(CV_CAP_PROP_FRAME_COUNT);
		for (int i = 1; i < nframes; i++)		{
			Mat frame;
			// Retrieve a single frame from the capture
			cap.read(frame);

			cvtColor(frame, image_binary, CV_BGR2GRAY);

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
		vector<KeyPoint> keypoint;
		SAMHI_10.convertTo(SAMHI_10, CV_8UC1, 255, 0);
		//detector.detect(SAMHI_10,keypoint);
		Mat features;
		detector.detect(SAMHI_10, keypoint);
		bowDE.compute(SAMHI_10, keypoint, bowDescriptor2);
		evalData.push_back(bowDescriptor2);
		groundTruth.push_back(lab);

		float result = MotionRecognition.predict(bowDescriptor2);
		cout << "File " << fnum << " tested:: " << result << endl;
		results.push_back(result);
	}

	FileStorage tra1("test.xml",FileStorage::WRITE);
	if (!tra1.isOpened()){  
		cerr<<"failed to open "<<endl;  
	}
	tra1<<"trainData"<<evalData<<"label"<<groundTruth;
	tra1.release();
	double errorRate = (double)countNonZero(groundTruth - results) / evalData.rows;
	double accuracy = (double) 1 - errorRate;
	printf("%s%f%s", "Accuracy is ", accuracy * 100 , "%");
	system("pause");
	return 0;
}
