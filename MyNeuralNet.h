#include "pch.h"
#include <iostream>
#include <vector>
#include <time.h>
#include <filesystem>

// User headers
#include "Net.h"

using namespace std;

// User defined namespaces

// Some usefull stuff

#define TraningSet pair<vector<vector<double>*>*, vector<vector<double>*>*>

vector<double> GetTargetVect(int size, int onePos) {
	vector<double> outVectPtr;
	for (int i = 0; i < size; i++) {
		if (i == onePos)
			outVectPtr.push_back(1);
		else
			outVectPtr.push_back(0);
	}
	return outVectPtr;
}

TraningSet GetLinearTraningSet(vector<string> *files, int outputSize) {
	TraningSet outSet;
	outSet.first = new vector<vector<double>*>();
	outSet.second = new vector<vector<double>*>();

	for (int i = 0; i < outputSize; i++) {
		outSet.first->push_back(new vector<double>(ReadFileToDoubleArray(files->at(i))));
		outSet.second->push_back(new vector<double>(GetTargetVect(outputSize, i)));
	}

	return outSet;
}
