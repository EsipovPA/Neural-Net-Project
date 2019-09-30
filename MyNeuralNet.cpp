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

	return 0;
}


//********* Autoencoder study *************
// Does not teach the hidden layer
// if the output layer contains only one neuron
/*
int main() {
	NetStructure *sPtr = new NetStructure();
	sPtr->activationFunction = SIGMOID_FUNCTION;
	sPtr->neurons.push_back(15);
	sPtr->neurons.push_back(5);
	sPtr->neurons.push_back(1);
	sPtr->studySpeed = 0.5;
	sPtr->gradientMoment = 0.3;
	sPtr->addBias = false;

	// Create the net
	Net *net = new Net(sPtr);

	vector<vector<double>*> testVals;
	vector<double>* target = new vector<double>();

	// Test net input
	testVals.push_back(new vector<double>());
	testVals.push_back(new vector<double>());
	testVals.push_back(new vector<double>());

	for (int i = 0; i < 15; i++) {
		testVals[0]->push_back(double(i));
		testVals[1]->push_back(double(i));
		testVals[2]->push_back(double(i));
		target->push_back(15.0 - double(i));
	}

	// Normalize input data
	for (int i = 0; i < testVals.size(); i++) {
		NormalizeArray(testVals[i]);
	}

	// Train the net by autoencoder
	net->AutoencoderStudy(&testVals, 1000);

	// Test classification
	// First input
	net->ClearNeurons();
	net->TakeInput(testVals[0]);
	net->PushForward();
	vector<double> *outVals = net->GetOutVals();

	cout << "First output = " << outVals->at(0) << "\n";

	// Clear the memory
	outVals->clear();
	delete outVals;

	// Second input
	net->ClearNeurons();
	net->TakeInput(target);
	net->PushForward();
	outVals = net->GetOutVals();

	cout << "Second input = " << outVals->at(0) << "\n";

	cout << "End of the autoencoder study test\n";

	return 0;
}
*/

// Can't remember this part of code. Need to figure it out later
/*
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

	return 0;
}
*/


// Working algorithm for alphebet reading study
/*
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
*/

// Learn the net to find certain image in the set
// Set the net structure
/*
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

// Wirking on reading chineese characters
// Reading images goes incorrect
// Need to debug it later
/*

int main() {
	cout << "Test basic net\n";

	// Try to teach the net to read characters from images
	NetStructure *sPtr = new NetStructure();
	sPtr->activationFunction = SIGMOID_FUNCTION;
	sPtr->neurons.push_back(90 * 90);
	sPtr->neurons.push_back(100);
	sPtr->neurons.push_back(10);
	sPtr->studySpeed = 0.5;
	sPtr->gradientMoment = 0.3;
	sPtr->addBias = false;

	// Set names of the traning files
	vector<string> files;
	ScanTheFolder("E:\\CodingStudy\\ChineeseIMG", &files);

	cout << "Test files output:\n";



	cout << "\nThe end\nPress any key\n";
	cin.get();
}
*/