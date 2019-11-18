#include "pch.h"
#include "MyNeuralNet.h"

int main() {
	NetStructure *sPtr = new NetStructure();
	sPtr->activationFunction = SIGMOID_FUNCTION;
	sPtr->neurons.push_back(15);
	sPtr->neurons.push_back(15);
	sPtr->neurons.push_back(10);
	sPtr->studySpeed = 0.5;
	sPtr->gradientMoment = 0.3;
	sPtr->addBias = false;

	// Set names of the traningset files
	vector<string> files;
	for (int i = 0; i < 10; i++) {
		files.push_back("C:\\Users\\PaulPC\\source\\repos\\MyNeuralNet\\Debug\\imgs\\TestFile" + to_string(i) + ".txt");
	}

	// Create the net
	Net *nPtr = new Net(sPtr);

	// Get linear traning set
	TraningSet trSet = GetLinearTraningSet(&files, 10);

	// Train the net
	nPtr->Train(trSet.first, trSet.second, 10000);

	// Test the net
	cout << "Test the net\n";
	for (int i = 0; i < 10; i++) {
		cout << "Image:\n";
		PrintImageToConsole(files[i]);
		nPtr->TakeInputFile(&files[i]);

		nPtr->PushForward();

		vector<double> *outVals = nPtr->GetOutVals();
		for (int j = 0; j < outVals->size(); j++) {
			if (outVals->at(j) >= 0.5) {
				if (j != 9)
					cout << "the number on the image is: " << j + 1 << "\n";
				else
					cout << "the number on the image is: 0\n";
			}
		}
		cout << "\n\n";

		//Clear memory
		delete outVals;
		nPtr->ClearNeurons();
	}
}
