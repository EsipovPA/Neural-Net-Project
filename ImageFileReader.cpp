#include "pch.h"
#include "ImageFileReader.h"

#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

std::vector<int> ReadFileToIntArray(std::string fName) {
	vector<int> outVect;
	
	ifstream inFile;
	inFile.open(fName);

	if (inFile.is_open()) {
		string valStr = "";
		do {
			char inCh = inFile.get();
			if (inCh != ' ' && inCh != '\n' && !inFile.eof()) {
				valStr += inCh;
			}
			else {
				outVect.push_back((int)atof(valStr.c_str()));
				valStr = "";
			}
		} while (!inFile.eof());
	}
	else {
		cout << "No file \"" << fName << "\" found\n";
	}

	inFile.close();
	return outVect;
}

std::vector<double> ReadFileToDoubleArray(std::string fName) {
	vector<double> outVect;

	ifstream inFile;
	inFile.open(fName);

	if (inFile.is_open()) {
		string valStr = "";
		do {
			char inCh = inFile.get();
			if (inCh != ' ' && inCh != '\n' && !inFile.eof()) {
				valStr += inCh;
			}
			else {
				outVect.push_back(atof(valStr.c_str()));
				valStr = "";
			}
		} while (!inFile.eof());
	}
	else {
		cout << "No file \"" << fName << "\" found\n";
	}

	inFile.close();
	return outVect;
}

void PrintImageToConsole(string fName) {
	ifstream inFile;
	inFile.open(fName);

	cout << "Number in the file " << fName << " is:\n";

	if (inFile.is_open()) {
		do {
			char inCh = inFile.get();
			if (inCh == '1') {
				cout << char(219);
			}
			else if (inCh == '0') {
				cout << " ";
			}
			else if (inCh != ' '){
				cout << inCh;
			}
		} while (!inFile.eof());
	}
	else {
		cout << "Can not open the file: " << fName << "\n";
	}
}

vector<double>* ImgToDoubleArrayPtr(string fName)
{
	vector<double> *outVect = new vector<double>();
	Mat img = imread(fName);

	if (!img.data) {
		cout << "Cannot open the image: " << fName << "\n";
	}
	else {
		for (int i = 0; i < img.rows; i++) {
			for (int j = 0; j < img.cols; j++) {
				outVect->push_back(img.at<double>(i, j));
				cout << " val = " << outVect->back() << "\n";
			}
		}
	}

	return outVect;
}

#include <filesystem>
namespace fs = std::filesystem;

void ScanTheFolder(string folder, vector<string> *fVectPtr) {
	for (const auto & entry : fs::directory_iterator(folder)) {
		fVectPtr->push_back(entry.path().string());
	}
}
