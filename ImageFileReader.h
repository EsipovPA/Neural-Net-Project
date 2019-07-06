#pragma once

#include <string>
#include <vector>


std::vector<int> ReadFileToIntArray(std::string fName);

std::vector<double> ReadFileToDoubleArray(std::string fName);

void PrintImageToConsole(std::string fName);