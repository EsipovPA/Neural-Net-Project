#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class LogFile {
public:
	LogFile() {};
	LogFile(string name) {
		m_name = name;
	}
	~LogFile() {
		std::ofstream logFile(m_name, std::ofstream::out);

		logFile << m_data;

		logFile.close();
	};

	void ToLog(string message) {
		m_data += message + "\n";
	};
	void SetName(string name) {
		m_name = name;
	};

	
private:
	string m_name = "";
	string m_data = "";
};