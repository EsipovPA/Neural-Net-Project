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
}
