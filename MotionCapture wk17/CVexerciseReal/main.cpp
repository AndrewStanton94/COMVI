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
	//FileStorage fs("Vocabulary.xml", FileStorage::WRITE);
	//Mat dictionary = bowTrainer.cluster();
	//int a;
	////Mat vocabulary = bow.cluster(); 
	//int i =0;
	//fs << "Vocabulary"<<dictionary; 
	//fs.release();

	//int i =0;
	////////////////////////////////////////

	FileStorage fs2;
	Mat dictionary1;
	//Mat dictionary =bowTrainer.cluster();
	fs2.open("Vocabulary.xml", FileStorage::READ);
	fs2["vocabulary"] >> dictionary1;
	//dictionary1.convertTo(dictionary1, CV_8UC1);
	//dextract.setVocabulary(vocabulary);
	//Mat training_mat(num_img , dictionarySize,CV_32FC1);
	//Mat labels(num_img,1,CV_32FC1);
	//dictionary.save("vocabulary.xml");
	bowDE.setVocabulary(dictionary1);
	//cout << "extracting histograms in the form of BOW for each image " << endl;
	Mat labels(0, 1, CV_32SC1);
	Mat trainingData(0, clusterSize, CV_32FC1);
	int k = 0;
	vector<KeyPoint> keypoint1;
	Mat bowDescriptor1;
	//in.open("train.txt");
	//Mat trainSample(LineNumbers, clusterSize, CV_32FC1);
	LineNumbers = CountLines("train.txt");
	for (int fnum = 1; fnum <= LineNumbers; fnum++)
	{
		string fname;
		int l;
		in >> l;
		in >> fname;
		string filename1;
		filename1 = "D:\\Port\\COMVI\\DB\\train\\" + fname;

		cap = VideoCapture(filename1);
		//IplImage*  frame;
		Mat image_binary_prev_5, image_binary, image_binary_diff_5;
		int nframes = cap.get(CV_CAP_PROP_FRAME_COUNT);
		for (int i = 1; i < nframes; i++)
		{
			Mat frame;
			// Retrieve a single frame from the capture
			cap.read(frame);
			//	frame=frame(Rect(90,10,500,450));  //
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
		//extracting histogram in the form of bow for each image 
		SAMHI_10.convertTo(SAMHI_10, CV_8UC1, 255, 0);
		detector.detect(SAMHI_10, keypoint1);


		bowDE.compute(SAMHI_10, keypoint1, bowDescriptor1);
		labels.push_back(l);
		//bowDescriptor1 .convertTo(bowDescriptor1, CV_32F);
		trainingData.push_back(bowDescriptor1);
		//Mat trainSample_row = trainSample.row(i);
		//bowDescriptor1.copyTo(trainSample_row);

	}

	in.close();
	//Setting up SVM parameters
	CvSVMParams params;
	params.kernel_type = CvSVM::RBF;
	params.svm_type = CvSVM::C_SVC;
	params.gamma = 0.50625000000000009;
	params.C = 312.50000000000000;
	params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, 0.000001);
	CvSVM svm_classif;

	FileStorage tra("train.xml", FileStorage::WRITE);
	if (!tra.isOpened())
	{
		cerr << "failed to open " << endl;
	}
	tra << "trainData" << trainingData << "label" << labels;
	tra.release();

	bool res = svm_classif.train_auto(trainingData, labels, cv::Mat(), cv::Mat(), params);
	svm_classif.save("svmidch.xml");
	CvSVMParams params_re = svm_classif.get_params();

	printf("\nParms: C = %f,gamma = %f \n", params_re.C, params_re.gamma);

	//system("pause");
	return 0;
}
