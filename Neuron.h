#include "Math.h"

//********* Neuron Class **********
class Neuron {
public:

	Neuron(int funcType = LINEAR_FUNCTION) {
		if (funcType == LINEAR_FUNCTION) {
			ActivationFunction = &LinearActivationFunction;
			ActivationFunctionDerivative = &LinearFincDeriv;
		}
		else if (funcType == SIGMOID_FUNCTION) {
			ActivationFunction = &SigmoidActivationFunction;
			ActivationFunctionDerivative = &SigmoidFuncDeriv;
		}
		else if (funcType == HYPERBOLIC_TAN_FUNCTION) {
			ActivationFunction = &HyperbolicTanActivationFunction;
			ActivationFunctionDerivative = &HyperbolicFuncDeriv;
		}
		else {
			cout << "Neuron constructor error: " << funcType << " is unknown function type!\n";
		}
	}

	// Public neuron class methods
	double GetOutVal() { 
		m_outVal = ActivationFunction(m_inVal);
		return m_outVal;
	}
	void CalculateOutVal() { m_outVal = ActivationFunction(m_inVal); }
	//double GetDelta() { return m_delta; }
	double GetDelta() { return this->m_delta; }

	void CalculateDelta() { 
		m_delta = m_deltaSum * ActivationFunctionDerivative(m_outVal);
	}

	void SetInVal(double inVal) { m_inVal = inVal; }
	void AddToInVal(double inValAdd) { m_inVal += inValAdd; }
	void AddToDeltaSum(double inValAdd) { m_deltaSum += inValAdd; }

	void AddConnectionPtr(Connection *conPtr) {
		m_outConnections.push_back(conPtr);
	}
	void Clear() {
		this->m_inVal = 0.0;
		this->m_outVal = 0.0;
		this->m_delta = 0.0;
		this->m_deltaSum = 0.0;
	}

	virtual bool isBiasNeuron() { return false; };

	//Debugging function. Delete later
	void DebugDelta() {
		cout << "delta = " << m_delta << "\n";
		cout << "m_deltaSum = " << m_deltaSum << "\n";
	}



	vector<Connection*> m_outConnections;
	double m_inVal = 0.0;
	double m_outVal = 0.0;
	double m_delta = 0.0;
	double m_deltaSum = 0.0;
	int m_neuronType = HIDDEN_NEURON;

	double(*ActivationFunction)(double inVal);
	double(*ActivationFunctionDerivative)(double inVal);
};

class HiddenNeuron : public Neuron {
public:
	HiddenNeuron(int funcType = LINEAR_FUNCTION) : Neuron(funcType) {}

private:
	// To make these paraeters private
	using Neuron::m_inVal;
	using Neuron::m_outConnections;
	using Neuron::m_neuronType;
	using Neuron::m_outVal;
	using Neuron::m_delta;

	// To make these methods private
	using Neuron::ActivationFunction;
	using Neuron::ActivationFunctionDerivative;

};

class OutputNeuron : public Neuron {
public:
	OutputNeuron(int funcType = LINEAR_FUNCTION) : Neuron(funcType) {}

private:
	// To make these paraeters private
	using Neuron::m_inVal;
	using Neuron::m_outConnections;
	using Neuron::m_neuronType;
	using Neuron::m_outVal;
	using Neuron::m_delta;

	// To make these methods private
	using Neuron::ActivationFunction;
	using Neuron::ActivationFunctionDerivative;
};

class InputNeuron : public Neuron {
public:
	InputNeuron(int funcType = LINEAR_FUNCTION) : Neuron(funcType) {
		ActivationFunction = &InputNeuronActivationFunction;
	}

private:
	// To make these paraeters private
	using Neuron::m_inVal;
	using Neuron::m_outConnections;
	using Neuron::m_neuronType;
	using Neuron::m_outVal;
	using Neuron::m_delta;

	// To make these methods private
	using Neuron::ActivationFunction;
	using Neuron::ActivationFunctionDerivative;
};

class BiasNeuron : public Neuron {
public:
	BiasNeuron(int funcType = LINEAR_FUNCTION) : Neuron(funcType) {
		ActivationFunction = &BiasNeuronActivationFunc;
	}
	
	bool isBiasNeuron() { return true; }
private:
	// To make these paraeters private
	using Neuron::m_inVal;
	using Neuron::m_outConnections;
	using Neuron::m_neuronType;
	using Neuron::m_outVal;
	using Neuron::m_delta;

	// To make these methods private
	using Neuron::ActivationFunction;
	using Neuron::ActivationFunctionDerivative;
};

// Old neuron methods definitions
/*
Neuron::Neuron(int funcType) {
	if (funcType == LINEAR_FUNCTION) {
		ActivationFunction = &LinearActivationFunction;
		ActivationFunctionDerivative = &LinearFincDeriv;
	}

	else if (funcType == SIGMOID_FUNCTION) {
		ActivationFunction = &SigmoidActivationFunction;
		ActivationFunctionDerivative = &SigmoidFuncDeriv;
	}
	else if (funcType == HYPERBOLIC_TAN_FUNCTION) {
		ActivationFunction = &HyperbolicTanActivationFunction;
		ActivationFunctionDerivative = &HyperbolicFuncDeriv;
	}
	else {
		cout << "Neuron constructor error: " << funcType << " is unknown function type!\n";
	}

//	CalculateDelta = &HiddenNeuronDelta;
}

Neuron::Neuron(int funcType, int neuronType) : Neuron(funcType) {
	if (neuronType == BIAS_NEURON) {
		ActivationFunction = &BiasNeuronActivationFunc;
	}
	else if (neuronType == OUTPUT_NEURON) {
		//CalculateDelta = &OutputNeuronDelta;
	}
	else if (neuronType == INPUT_NEURON) {
		ActivationFunction = &InputNeuronActivationFunction;
	}
}

void Neuron::SetNeuronType(int nType) {
	if (nType == HIDDEN_NEURON || nType == BIAS_NEURON) {
		//CalculateDelta = &HiddenNeuronDelta;
	}
	else if (nType == OUTPUT_NEURON) {
		//CalculateDelta = &OutputNeuronDelta;
	}

	m_neuronType = nType;
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

void Neuron::PrintConnectionNumber() {
	cout << "This neuron has " << m_outConnections.size() << " outcoming connections\n";
}

bool Neuron::isBiasNeuron() {
	return m_neuronType == BIAS_NEURON;
}

double Neuron::GetOutValDerivative()
{
	return ActivationFunctionDerivative(m_inWal);
}

//********* Delta calculation functions *********
double OutputNeuronDelta(Neuron *neuron, const vector<double> *calcInput) {
	return (calcInput->at(0) - neuron->GetOutVal()) * neuron->GetOutValDerivative();
}

double HiddenNeuronDelta(Neuron *neuron, const vector<double> *calcInput) {
	double outVal = 0;

	


	return outVal;
}
*/