////#include "opencv2/highgui/highgui.hpp"
////#include "opencv2/contrib/contrib.hpp"
////#include <iostream>
////#include <vector>
////using namespace cv;
////using namespace std;
////int main()
////{
////	Directory dir;
////	string dir_path = "C:/Users/alpha/Dropbox/_Port/Y5/comvi/database/";
////	vector<string> filename = dir.GetListFiles(dir_path, "*.jpg", false);
////	string fullfile;
////	namedWindow("image", CV_WINDOW_AUTOSIZE);
////	int filesize = filename.size();
////	for (int i = 0; i<filesize; i++)
////	{
////		fullfile = dir_path + filename[i];
////		cout << fullfile << endl;
////		Mat img = imread(fullfile);
////		imshow("image", img);
////		waitKey(0);
////	}
////	return 0;
////}
//
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/objdetect/objdetect.hpp"
//#include <iostream>
//#include <vector>
//using namespace cv;
//using namespace std;
//int main()
//{
//	string face_cascade_name = "C:/Users/alpha/Downloads/opencv/sources/data/haarcascades/haarcascade_frontalface_default.xml";
//	CascadeClassifier face_cascade;
//	if (!face_cascade.load(face_cascade_name))
//	{
//		cerr << "Error loading face detection model." << endl;
//		exit(0);
//	}
//	vector<Rect> faces;
//
//	VideoCapture capture(0);
//	if (!capture.isOpened())  // if not success, exit program
//	{
//		cout << "Cannot open the video file" << endl;
//		return -1;
//	}
//	Mat frame;
//	int key = 0;
//	namedWindow("face", CV_WINDOW_AUTOSIZE);
//	while (key != 27) //press "Esc" to stop
//	{
//		capture >> frame;
//		face_cascade.detectMultiScale(frame, faces, 1.2, 2, 0, Size(50, 50)); //detect faces
//		if (faces.empty()) { //no faces are detected
//			cv::imshow("face", frame);
//			key = waitKey(3);
//			continue;
//		}
//		int faces_length = faces.size();
//		for (int i = 0; i < faces_length; i++)
//			rectangle(frame, faces[i], Scalar(0, 0, 255)); //draw rectangle
//		imshow("face", frame);
//		key = waitKey(3);
//	}
//}


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;
bool compareRect(cv::Rect r1, cv::Rect r2) { return r1.height < r2.height; }
int main()
{
	string face_cascade_name = "C:/Users/alpha/Downloads/opencv/sources/data/haarcascades/haarcascade_frontalface_default.xml";
	CascadeClassifier face_cascade;
	if (!face_cascade.load(face_cascade_name))
	{
		cerr << "Error loading face detection model." << endl;
		exit(0);
	}
	vector<Rect> faces;

	VideoCapture capture(0);
	if (!capture.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the video file" << endl;
		return -1;
	}
	Mat frame;
	string name, fullfile;
	cout << "Please input your emotion: ";
	cin >> name;
	string path = "C:/Users/alpha/Dropbox/_Port/Y5/comvi/database/emotions/";
	namedWindow("emotion", CV_WINDOW_AUTOSIZE);
	namedWindow("collection", CV_WINDOW_AUTOSIZE);
	char key = 0;
	int count = 0;
	while (key != 27) //press "Esc" to stop
	{
		capture >> frame;
		face_cascade.detectMultiScale(frame, faces, 1.2, 2, 0, Size(50, 50)); //detect faces
		if (faces.empty()) { //no faces are detected
			imshow("face", frame);
			key = waitKey(30);
			continue;
		}
		if (key == 'c') //press "c" to take a picture
		{
			stringstream name_count;
			name_count << ++count;
			fullfile = path + name + name_count.str() + ".jpg";
			cout << "Should be saving now";
			imwrite(fullfile, frame); //save the picture
			imshow("collection", frame);
			key = waitKey(30);
		}
		Rect facerect = *max_element(faces.begin(), faces.end(), compareRect); //only the largest face bounding box are maintained
		rectangle(frame, facerect, Scalar(0, 0, 255)); //draw rectangle
		imshow("face", frame);
		key = waitKey(30);
	}
}