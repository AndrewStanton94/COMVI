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
int Motion_num = 0;
int Motion_flag = 0;
const double MHI_DURATION = 1;
const double MAX_TIME_DELTA = 0.5;
const double MIN_TIME_DELTA = 0.05;
const double threshold_value = 25;
Mat image_binary_prev_5, image_binary, image_binary_diff_5;
Mat SAMHI_10 = Mat(480, 640, CV_32FC1, Scalar(0, 0, 0));;
int results = 0;
Mat labels;
Mat trainingData;
Mat testData;
Mat Testlabels;

int main(int argc, char** argv)
{
	cv::FileStorage fs("data.xml", cv::FileStorage::READ);
	if (!fs.isOpened())
	{
		cerr << "failed to open " << endl;
	}
	fs["trainData"] >> trainingData;
	fs["label"] >> labels;
	int filesize = trainingData.rows;
	int folders = 5;// you can change the folders to 10 
	int validateVol = (int)(filesize / folders);
	int labelline = labels.rows;

	srand(time(0));


	Mat conmat = Mat::zeros(10, 10, CV_32SC1);
	for (int i = 0; i<folders; i++)
	{

		Mat trainS(filesize - validateVol, clusterSize, CV_32FC1);
		Mat validS(validateVol, clusterSize, CV_32FC1);
		Mat Tlabel(filesize - validateVol, 1, CV_32SC1);
		Mat Vlabel(validateVol, 1, CV_32SC1);
		int *markIdx = new int[filesize];
		for (int j = 0; j<filesize; j++)
		{
			markIdx[j] = 0;
		}
		for (int j = 0; j<validateVol; j++)
		{
			int a = rand() % filesize;
			while (markIdx[a] == 1)
			{
				a = rand() % filesize;
			}
			markIdx[a] = 1;
		}
		int countT = 0;
		int countV = 0;
		for (int j = 0; j<filesize; j++)
		{
			if (markIdx[j] == 0)
			{
				Mat trainS_row = trainS.row(countT);
				trainingData.row(j).copyTo(trainS_row);
				Tlabel.at<int>(countT) = labels.at<int>(j);

				/*Mat Tlabel_row = Tlabel.row(countT);
				labels.row(j).copyTo(Tlabel_row);*/
				int a = labels.at<int>(j);
				countT++;
			}
			else
			{
				Mat validS_row = validS.row(countV);
				trainingData.row(j).copyTo(validS_row);

				/*	Mat Vlabel_row = Vlabel.row(countV);
				labels.row(j).copyTo(Vlabel_row);*/
				Vlabel.at<int>(countV) = labels.at<int>(j);
				countV++;
			}
		}
		CvSVM SVM;
		CvSVMParams paras;
		paras.svm_type = CvSVM::C_SVC;
		paras.kernel_type = CvSVM::RBF;
		paras.gamma = 0.50625000000000009;
		paras.C = 312.50000000000000;
		paras.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 1000, 0.000001);

		//paras.gamma = 0.01;
		//paras.C = 1;
		//paras.p = 5e-3;
		//paras.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100000, FLT_EPSILON );


		bool res = SVM.train_auto(trainS, Tlabel, Mat(), Mat(), paras);

		CvSVMParams params_re = SVM.get_params();

		printf("\nParms: C = %f,gamma = %f \n", params_re.C, params_re.gamma);
		//bool res = SVM.train(trainingData,labels,Mat(),Mat(),paras);
		//bool res = SVM.train_auto(validS,Vlabel,Mat(),Mat(),paras);
		SVM.save("svm2.xml");
		//CvSVMParams params_re = SVM.get_params();
		//float C = params_re.C;   
		//float P = params_re.p;   
		//float gamma = params_re.gamma;  
		//   SVM.train(trainS,Tlabel,Mat(),Mat(),params_re);
		for (int j = 0; j<validateVol; j++)
		{
			//float motion = SVM.predict(validS.row(j));

			//Mat a = validS.row(j);

			int motion = SVM.predict(validS.row(j));
			cout << Vlabel.at<int>(j) << ' ' << (int)motion << endl;
			conmat.at<int>(Vlabel.at<int>(j) - 1, (int)(motion)-1)++;
			//
			/*		double err = (double)countNonZero(Vlabel.at<float>(j) - motion) / validS.rows;
			printf("%s%f", "Accuracy : ", (1-err)*100);
			printf("%");*/
		}
	}
	cout << conmat << endl;
	system("pause");
	return 0;
}
