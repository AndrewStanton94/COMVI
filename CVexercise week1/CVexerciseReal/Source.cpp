//#include "opencv2/highgui/highgui.hpp"
//#include <iostream>
//using namespace cv;
//using namespace std;
//int main()
//{
//	Mat img = imread("C:/Users/alpha/Dropbox/_Port/Y5/comvi/database/ba.jpg");
//	if (img.empty()) //check whether the image is loaded or not
//	{
//		cout << "Error : Image cannot be loaded..!!" << endl;
//		return -1;
//	}
//	namedWindow("image", CV_WINDOW_AUTOSIZE); //create a window with the name "image"
//	imshow("image", img); //display the image which is stored in the 'img' in the "image" window
//	waitKey(0); //wait infinite time for a keypress
//	return 0;
//}


#include "opencv2/highgui/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;
int main()
{
	VideoCapture capture(0); // open the video file for reading
	if (!capture.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the video file" << endl;
		return -1;
	}
	//double fps = capture.get(CV_CAP_PROP_FPS); //get the frames rate of the video
	//int delay = (int)(1000 / fps); //delay between two frames
	Mat frame;
	namedWindow("video", CV_WINDOW_AUTOSIZE);
	int key = 0;
	while (key != 27) // press "Esc" to stop
	{
		if (!capture.read(frame))
		{
			break;
		}
		flip(frame,frame,-1);
		imshow("video", frame);
		key = waitKey(1);
	}
	return 0;
}
