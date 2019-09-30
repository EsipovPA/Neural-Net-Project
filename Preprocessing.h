#pragma once

#include <vector>
#include <cmath>

using namespace std;

double CalculateMidValue(vector<double> *values) {
	double mid = 0.0;

	for (int i = 0; i < values->size(); i++) {
		mid += values->at(i);
	}

	return mid / (double(values->size()));
}

double GetVariance(vector<double> *values, double midValue) {
	double vari = 0;
	for (int i = 0; i < values->size(); i++) {
		vari += pow(values->at(i) - 
			midValue, 2.0);
	}

	vari = vari / double(values->size());
	vari = pow(vari, 0.5);

	return vari;
}

void NormalizeArray(vector<double> *values) {
	double midValue = CalculateMidValue(values);
	double vari = GetVariance(values, midValue);
	
	for (int i = 0; i < values->size(); i++) {
		values->at(i) = (values->at(i) - midValue) / (vari * 2.0);
	}
}
