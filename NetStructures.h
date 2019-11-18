#pragma once
#include <vector>
#include "Math.h"


using namespace std;


//*******Basic net structure ***********
struct NetStructure {
	NetStructure() {};
	NetStructure(vector<unsigned> neurons_, vector<vector<double>> conWeights_, double sSpeed, double sGrad, bool needBias, int activationFunction_);

	bool addBias = false;
	int activationFunction = LINEAR_FUNCTION;

	vector<unsigned> neurons;
	vector<vector<double>> conWeights;

	double studySpeed = 1.0;
	double gradientMoment = 0.0;

	void GetWeightsFromFile(string fName = "NNS_State.txt");
};

NetStructure::NetStructure(vector<unsigned> neurons_, vector<vector<double>> conWeights_, double sSpeed, double sGrad, bool needBias, int activationFunction_) {
	this->activationFunction = activationFunction_;
	this->addBias = needBias;
	this->conWeights = conWeights_;
	this->gradientMoment = sGrad;
	this->neurons = neurons_;
	this->studySpeed = sSpeed;
}

// Not developed yet
void NetStructure::GetWeightsFromFile(string fName) {

	cout << "Testing NetStructure::GetWeightsFromFile\n";

}

//*****Convolutional net structure******
struct ConvStructure {
	ConvStructure() {}


};
