//#include "opencv2/highgui/highgui.hpp"
//#include <iostream>
//using namespace cv;
//using namespace std;
//int main()
//{
//	Mat A(2, 4, CV_32FC1, 2);
//	cout << "A = " << endl << " " << A << endl << endl;
//	Mat B = Mat::eye(4, 4, CV_32FC1)*5;
//	cout << "B = " << endl << " " << B << endl << endl;
//	Mat F = A.clone();
//	Mat G;
//	B.copyTo(G);
//	G.at<float>(2, 3) = 5;
//	cout << "F = " << endl << " " << F << endl << endl << "G = " << endl << " " << G << endl;
//	Mat M = F*G; //Matrix Multiplication
//	cout << "M = " << endl << " " << M << endl << endl;
//	Mat F_row = F.row(1);
//	G.row(2).copyTo(F_row); //replace the 2nd row of F by the 3rd row of G
//	cout << "F = " << endl << " " << F << endl << endl;
//	Mat N;
//	M.reshape(1, 1).copyTo(N);
//	cout << "N = " << endl << " " << N << endl << endl;
//	return 0;
//}



//#include "opencv2/highgui/highgui.hpp"
//#include <iostream>
//using namespace cv;
//using namespace std;
//void onMouse(int event, int x, int y, int flags, void* img)
//{
//	switch (event)
//	{
//	case CV_EVENT_LBUTTONDOWN: //click left button of mouse
//		cout << "Coordinate: (" << x << ',' << y << ')' << endl;
//		IplImage *timg = cvCloneImage((IplImage*)img);
//		cvCircle(timg, cvPoint(x, y), 3, Scalar(0, 0, 255), CV_FILLED);
//		cvShowImage("image", timg);
//		cvReleaseImage(&timg);
//		break;
//	}
//}
//int main()
//{
//	IplImage *img = cvLoadImage("C:/Users/alpha/Dropbox/_Port/Y5/comvi/database/ba.jpg");
//	cvNamedWindow("image", CV_WINDOW_AUTOSIZE);
//	setMouseCallback("image", onMouse, img);
//	cvShowImage("image", img);
//	waitKey(0);
//	return 0;
//}

#include<opencv2\opencv.hpp>
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;
vector<Point> capturePoint;
void onMouse(int event, int x, int y, int flags, void* param)
{
	int n = 0;
	switch (event)
	{
		case CV_EVENT_LBUTTONDOWN: //click left button of mouse
			cout << "Coordinate: (" << x << ',' << y << ')' << endl;
			n++;
			if (n<2)
				capturePoint.push_back(Point(x, y));
			break;
	}
}
int main()
{
	string text;
	cin >> text; //input any characters and finish by press "Enter"
	Mat img = imread("C:/Users/alpha/Dropbox/_Port/Y5/comvi/database/ba.jpg");
	namedWindow("image", CV_WINDOW_AUTOSIZE);
	setMouseCallback("image", onMouse, NULL);
	imshow("image", img);
	waitKey(0);
	rectangle(img, capturePoint[0], capturePoint[1], Scalar(0, 0, 255)); //draw rectangle
	putText(img, text, Point(capturePoint[0].x + 2, capturePoint[0].y - 10), CV_FONT_HERSHEY_COMPLEX, 0.5, cvScalar(255, 0, 0)); // overlay text
	imshow("image", img);
	waitKey(0);
	return 0;
}
