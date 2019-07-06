#include "pch.h"
#include "Math.h"


double LinearActivationFunction(double inVal) {
	return inVal;
}

double SigmoidActivationFunction(double inVal) {
	return 1 / (1 + exp(-inVal));
}

double HyperbolicTanActivationFunction(double inVal) {
	return (exp(2 * inVal) - 1) / (exp(2 * inVal) + 1);
}

double InputNeuronActivationFunction(double inVal) {
	return inVal;
}

double BiasNeuronActivationFunc(double inVal) {
	return 1.0;
}

double ThresholdActivationFunction(double inVal) {
	return (inVal >= 0) ? 1.0 : 0.0;
}

double SensorActivationFunction(double inVal) {
	return (inVal >= 0.5) ? 1 : 0;
}

double LinearFincDeriv(double inVal) {
	return inVal;
}

double SigmoidFuncDeriv(double inVal) {
	return (1.0 - inVal) * inVal;
}

double HyperbolicFuncDeriv(double inVal) {
	return 1.0 - pow(inVal, 2.0);
}

double MSE(vector<double> result, vector<double> target) {
	if (result.size() != target.size()) {
		return -1;
	}
	else {
		double retVal = 0;
		for (int i = 0; i < result.size(); i++) {
			retVal += pow((result[i] - target[i]), 2);
		}

		return (retVal / result.size());
	}

}

double RootMSE(vector<double> result, vector<double> target) {
	if (result.size() != target.size()) {
		return -1;
	}
	else {
		double retVal = 0;
		for (int i = 0; i < result.size(); i++) {
			retVal += pow((result[i] - target[i]), 2);
		}

		return sqrt(retVal / result.size());
	}
}

double Arctan(vector<double> result, vector<double> target) {
	if (result.size() != target.size()) {
		return -1;
	}
	else {
		double retVal = 0;
		for (int i = 0; i < result.size(); i++) {
			retVal += pow(atan(result[i] - target[i]), 2);
		}

		return (retVal / result.size());
	}
}