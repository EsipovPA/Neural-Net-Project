#include "pch.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct NetStructure;
class Connection;
class Neuron;
class Net;

#define LINEAR_FUNCTION 1
#define SIGMOID_FUNCTION 2
#define HYPERBOLIC_TAN_FUNCTION 3
#define BIAS_NEURON_FUNCITON 4
#define INPUT_NEURON_FUNCTION 5

#define NeuronLayer vector<Neuron>
#define ConnectionLayer vector<Connection>


//********* Activation functions *********
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
	return 0;		// Change later to real funciton
}

//********* Error calculation functions *********

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

//********* Neuron Class **********
class Neuron {
public:
	Neuron(int funcType = LINEAR_FUNCTION);

	double GetOutVal();
	void SetInVal(double inVal);
	void SetOutVal(double val);		//Only applyed for input neurons
	void AddToInVal(double inValAdd);

private:
	//Указатель на функцию активации может быть?
	vector<Connection*> connections;
	double m_inWal = 0.0;
	double m_outWal = 0.0;

	double(*ActivationFunction)(double inVal);
};

Neuron::Neuron(int funcType) {
	if (funcType == LINEAR_FUNCTION) {
		ActivationFunction = &LinearActivationFunction;
	}
	else if (funcType == SIGMOID_FUNCTION) {
		ActivationFunction = &SigmoidActivationFunction;
	}
	else if (funcType == HYPERBOLIC_TAN_FUNCTION) {
		ActivationFunction = &HyperbolicTanActivationFunction;
	}
	else if (funcType == INPUT_NEURON_FUNCTION) {
		ActivationFunction = &InputNeuronActivationFunction;
	}
	else if (funcType == BIAS_NEURON_FUNCITON) {
		ActivationFunction = &BiasNeuronActivationFunc;
	}
	else {
		cout << "Neuron constructor error: " << funcType << " is unknown function type!\n";
	}
}

double Neuron::GetOutVal() {
	m_outWal = ActivationFunction(m_inWal);
	return m_outWal;
}

void Neuron::SetInVal(double inVal) {
	m_inWal = inVal;
}

void Neuron::AddToInVal(double inValAdd) {
	m_inWal += inValAdd;
}

void Neuron::SetOutVal(double val) {
	this->m_outWal = val;
}

//********* Connection Class *********
class Connection {
public:
	Connection() {};
	Connection(Neuron *from, Neuron *to) {
		this->m_from = from;
		this->m_to = to;
		this->m_weight = GetRandomWeight();
	}
	Connection(Neuron *from, Neuron *to, double weight) : Connection(from, to) {
		this->m_weight = weight;
	}

	void FeedForward();
	void UpdateWeight(double weight) { m_weight = weight; }

private:
	Neuron *m_from;
	Neuron *m_to;
	double m_weight;

	double GetRandomWeight() { return (double)rand() / RAND_MAX; }
};

void Connection::FeedForward() {
	double sourceVal = m_from->GetOutVal();
	cout << "\nsource = " << sourceVal << "\n";
	cout << "weight = " << m_weight << "\n";
	cout << "result = " << sourceVal * m_weight << "\n";
	m_to->AddToInVal(sourceVal * m_weight);
}


//*********** Net structure ***********
struct NetStructure {
	NetStructure() {
		addBias = false;
		activationFunction = LINEAR_FUNCTION;
	};

	bool addBias;
	int activationFunction;

	vector<unsigned> neurons;
	vector<vector<double>> conWeights;
};

//*********** Net Class ***********
class Net {
public:
	Net() { aFunctionType = LINEAR_FUNCTION; };
	Net(int aFType) : Net() { aFunctionType = aFType; };
	Net(NetStructure netStructure);	//Change input to pointer type

	void AddNeuronLayer(int size, bool AddBias = false);
	void AddNeuronLayer(int size, int funcType, bool AddBias = false);
	void AddConnLayer(NeuronLayer *from, NeuronLayer *to);
	void AddConnLayer(NeuronLayer *from, NeuronLayer *to, vector<double> weights);
	void SetActivationFunction(int aFType) { this->aFunctionType = aFType; };

	bool TakeInput(vector<double> *inVals);
	void PushForward();
	vector<double> *GetOutWals();

private:
	vector<NeuronLayer> neurons;
	vector<ConnectionLayer> connections;

	int aFunctionType;

};

Net::Net(NetStructure netStr) {
	for (int i = 0; i < netStr.neurons.size(); i++) {
		AddNeuronLayer(netStr.neurons[i], netStr.activationFunction);
	}

	for (int i = 0; i < this->neurons.size() - 1; i++) {
		if (i < netStr.conWeights.size()) {
			AddConnLayer(&(neurons[i]), &(neurons[i + 1]), netStr.conWeights[i]);
		}
		else {
			AddConnLayer(&(neurons[i]), &(neurons[i + 1]));
		}
	}
}


void Net::AddNeuronLayer(int size, bool AddBias) {
	neurons.push_back(NeuronLayer());
	for (int i = 0; i < size; i++) {
		neurons.back().push_back(Neuron());
	}

	if (AddBias) {
		//Figure out how to add bias neuron
	}
}

void Net::AddNeuronLayer(int size, int funcType, bool AddBias) {
	neurons.push_back(NeuronLayer());
	for (int i = 0; i < size; i++) {
		if (neurons.size() == 1) {
			neurons.back().push_back(Neuron(INPUT_NEURON_FUNCTION));
		}
		else {
			neurons.back().push_back(Neuron(funcType));
		}
	}

	if (AddBias) {
		//Figure out how to add bias neuron
	}
}

void Net::AddConnLayer(NeuronLayer *from, NeuronLayer *to, vector<double> weights) {
	connections.push_back(ConnectionLayer());
	int wCount = 0;
	for (int f = 0; f < from->size(); f++) {
		for (int t = 0; t < to->size(); t++) {
			if (wCount < weights.size()) {
				cout << "f = " << f << " t = " << t << " w = " << weights[wCount] << "\n";
				connections.back().push_back(Connection(&(from->at(f)), &(to->at(t)), weights[wCount]));
				wCount++;
			}
			else {
				connections.back().push_back(Connection(&(from->at(f)), &(to->at(t))));
			}
		}
	}
}

void Net::AddConnLayer(NeuronLayer *from, NeuronLayer *to) {
	connections.push_back(ConnectionLayer());
	for (int f = 0; f < from->size(); f++) {
		for (int t = 0; t < to->size(); t++) {
			connections.back().push_back(Connection(&(from->at(f)), &(to->at(t))));
		}
	}
}

bool Net::TakeInput(vector<double> *inVals) {
	if (inVals->size() != neurons[0].size()) {
		cout << "Input size incorrect\n";
		return false;
	}
	else {
		for (int i = 0; i < inVals->size(); i++) {
			neurons[0][i].SetInVal(inVals->at(i));
		}
		return true;
	}
}

void Net::PushForward() {
	for (int l = 0; l < connections.size(); l++) {
		for (int c = 0; c < connections[l].size(); c++) {
			connections[l][c].FeedForward();
		}
	}
}

vector<double>* Net::GetOutWals() {
	vector<double> *outVals = new vector<double>();

	for (int i = 0; i < neurons.back().size(); i++) {
		outVals->push_back(neurons.back()[i].GetOutVal());
	}

	return outVals;
}

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

	Net net(str);

	vector<double> input = { 1.0, 0.0 };
	
	if (net.TakeInput(&input)) {
		cout << "Input given\n";
	}

	net.PushForward();

	vector<double> *outVals = net.GetOutWals();

	cout << "result values: \n";
	cout << "out Values size = " << outVals->size() << "\n";
	for (int v = 0; v < outVals->size(); v++) {
		cout << "	" << v << "-th = " << outVals->at(v) << "\n";
	}

	// Do not forget to clear memory
	outVals->clear();
	delete outVals;

	cout << "End of basic net test\n";
}