#include "pch.h"
#include "MyNeuralNet.h"

//********* Main Part *************
int main() {
	cout << "Test basic net\n";

	// Mybe build the net with the hidden layer?
	// Habr doesrecommend this approach

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
		files.push_back("C:\\Users\\PaulPC\\source\\repos\\MyNeuralNet\\Debug\\TestFile" + to_string(i) + ".txt");
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

	//nPtr->SaveStateToFile();

	cout << "\nThe end\nPress any key\n";
	cin.get();
}

/*
// Learn the net to find certain image in the set
// Set the net structure
NetStructure *sPtr = new NetStructure();
sPtr->activationFunction = SIGMOID_FUNCTION;
sPtr->neurons.push_back(15);
sPtr->neurons.push_back(1);
sPtr->studySpeed = 0.5;
sPtr->gradientMoment = 0.3;
sPtr->addBias = false;

// Set traning set files
vector<string> files;
for (int i = 0; i < 10; i++) {
	files.push_back("C:\\Users\\PaulPC\\source\\repos\\MyNeuralNet\\Debug\\TestFile" + to_string(i) + ".txt");
}

// Create the net
Net *nPtr = new Net(sPtr);

cout << "The net is constructed\n";

// Create traning set
int setCount = 0;
vector<vector<double>*> fVect;
vector<vector<double>*> target;

for (int i = 0; i < 10; i++) {
	fVect.push_back(new vector<double>(ReadFileToDoubleArray(files[i])));
	if (i == 2) {
		target.push_back(new vector<double>({ 1 }));
	}
	else {
		target.push_back(new vector<double>({ 0 }));
	}
}

nPtr->Train(&fVect, &target, 10000);
cout << "Study is done\n";
cout << "Time to test the net\n";

for (int i = 0; i < 10; i++) {
	cout << "Image:\n";
	PrintImageToConsole(files[i]);
	nPtr->TakeInputFile(&files[i]);

	nPtr->PushForward();

	vector<double> *outVals = nPtr->GetOutVals();
	if (outVals->back() >= 0.5) {
		cout << "This is the target \"3\" image\n";
	}
	else {
		cout << "This is !!!NOT!!! the target \"3\" image\n";
	}

	//Clear memory
	delete outVals;
	nPtr->ClearNeurons();
}

cout << "Now let's deformate the target image\n";
string fName = "C:\\Users\\PaulPC\\source\\repos\\MyNeuralNet\\Debug\\TestFile10.txt";
PrintImageToConsole(fName);
nPtr->TakeInputFile(&fName);
nPtr->PushForward();
vector<double> *outVals = nPtr->GetOutVals();

if (outVals->back() >= 0.5) {
	cout << "This looks like the target image\n";
}
else {
	cout << "I don't think that this is the target image\n";
}

// Clear the memory
delete sPtr;
delete outVals;

*/