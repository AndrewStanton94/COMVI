#include<opencv2\opencv.hpp>
//Gaussian filter
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
//using namespace cv;
//int main()
//{
//	namedWindow("Original Image", CV_WINDOW_AUTOSIZE);
//	namedWindow("Smoothed Image", CV_WINDOW_AUTOSIZE);
//	Mat img = imread("C:/Users/alpha/Dropbox/_Port/Y5/comvi/database/ba.jpg");
//	imshow("Original Image", img);
//	Mat smooth_img;
//	char text[35];
//	for (int i = 5; i <= 21; i = i + 4)
//	{
//		_snprintf_s(text, 35, "Kernel Size : %d x %d", i, i);
//		GaussianBlur(img, smooth_img, Size(i, i), 0, 0);
//		putText(smooth_img, text, Point(img.cols / 4, img.rows / 8), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 0), 2);
//		imshow("Smoothed Image", smooth_img);
//		waitKey(0);
//	}
//	return 0;
//}


#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
using namespace std;
int main()
{
	Mat img = imread("C:/Users/alpha/Dropbox/_Port/Y5/comvi/database/ba.jpg");
	namedWindow("Original Image", CV_WINDOW_AUTOSIZE);
	namedWindow("Sobel_X", CV_WINDOW_AUTOSIZE);
	namedWindow("Sobel_Y", CV_WINDOW_AUTOSIZE);
	namedWindow("Sobel", CV_WINDOW_AUTOSIZE);
	namedWindow("Laplacian", CV_WINDOW_AUTOSIZE);
	imshow("Original Image", img);

	Mat smooth_img, gray_img;
	//GaussianBlur(img, smooth_img, Size(3, 3), 0, 0); //Gaussian smooth
	cvtColor(img, gray_img, CV_BGR2GRAY); //convert to gray-level image

	Mat grad_x, grad_y, abs_grad_x, abs_grad_y, SobelGrad;
	Sobel(img, grad_x, CV_32FC1, 1, 0);
	convertScaleAbs(grad_x, abs_grad_x); //gradient X
	imshow("Sobel_X", abs_grad_x);

	Sobel(img, grad_y, CV_32FC1, 0, 1);
	convertScaleAbs(grad_y, abs_grad_y); //gradient Y
	imshow("Sobel_Y", abs_grad_y);

	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, SobelGrad); //total Sobel gradient
	imshow("Sobel", SobelGrad);

	Mat Lap, abs_Lap;
	Laplacian(img, Lap, CV_32FC1, 3);
	convertScaleAbs(Lap, abs_Lap); //Laplacian operator

	imshow("Laplacian", abs_Lap);
	waitKey(0);
	return 0;
}
