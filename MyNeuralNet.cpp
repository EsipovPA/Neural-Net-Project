#include "pch.h"
#include "MyNeuralNet.h"

//********* Main Part *************
int main() {
	cout << "Test basic net\n";

	vector<vector<double>> conWeights;
	vector<unsigned> neurons = { 2, 2, 1 };

	conWeights.push_back(vector<double>());
	conWeights.back() = { 0.45, 0.78, -0.12, 0.13 };
	conWeights.push_back(vector<double>());
	conWeights.back() = { 1.5, -2.3 };

	NetStructure str;
	str.addBias = false;
	str.activationFunction = SIGMOID_FUNCTION;
	str.neurons = neurons;
	str.conWeights = conWeights;
	str.studySpeed = 0.5;
	str.gradientMoment = 0.3;

	Net net(&str);

	// Uncomment when net constructor tested
	vector<double> input = { 1.0, 0.0 };
	vector<double> target = { 1 };

	net.Study(&input, &target, 2000);

	cout << "Now it's time to test the net!\n";

	// Old algorythm/ Just two iterations
	/*
	if (net.TakeInput(&input)) {
		cout << "Input given\n";
	}

	net.PushForward();
	vector<double> *outVals = net.GetOutVals();
	
	cout << "result values: \n";
	cout << "out Values size = " << outVals->size() << "\n";
	for (int v = 0; v < outVals->size(); v++) {
		cout << "	" << v << "-th = " << outVals->at(v) << "\n";
	}

	//You may change this parameter, if you want to use other function
	double(*ErrorCalculator)(vector<double> result, vector<double> target) = MSE;
	vector<double> target = { 1 };

	double resErr = ErrorCalculator(*outVals, target);

	cout << "Result MSE = " << resErr << "\n";


	cout << "\nBack propagation testing\n";
	vector<double> *errors = new vector<double>;
	
	for (int i = 0; i < target.size(); i++) {
		errors->push_back(target[i] - outVals->at(i));
		cout << "errors[" << i << "] = " << errors->at(i) << "\n";
	}

	net.BackPropagation(errors);

	net.ClearNeurons();
	outVals->clear();

	if (net.TakeInput(&input)) {
		cout << "Input given again\n";
	}

	net.PushForward();
	outVals = net.GetOutVals();
	cout << "Second result values\n";
	for (int i = 0; i < outVals->size(); i++) {
		cout << "	" << outVals->at(i) << "\n";
	}

	resErr = ErrorCalculator(*outVals, target);
	
	cout << "Second result err = " << resErr << "\n";
	cout << "Net test end\n";

	net.ClearNeurons();
	// Do not forget to clear memory
	outVals->clear();
	delete outVals;*/
}