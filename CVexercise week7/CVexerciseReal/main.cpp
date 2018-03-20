#include "cvtutorial.h"
int main()
{
	cvtutorial a;
	char yn;
	bool loadSVM = true;
	cout << "Do you want to train a new Face Recognition model?" << endl;
	cout << "If you update the training data, please input 'y'. y/n?";
	cin >> yn;
	if (yn == 'y')
	{
		a.TrainingData("C:/Users/alpha/Dropbox/_Port/Y5/comvi/database/face/FaceRecognition.txt");
		a.FRTrain(true);
		loadSVM = false;
	}
	a.FR(loadSVM);
	return 0;
}
