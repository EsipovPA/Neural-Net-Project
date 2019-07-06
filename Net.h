#include "Neuron.h"
#include "Connection.h"
#include "LogFile.h"
#include "ImageFileReader.h"

#include <string>

//*********** Net structure ***********
struct NetStructure {
	NetStructure() {};

	bool addBias = false;
	int activationFunction = LINEAR_FUNCTION;

	vector<unsigned> neurons;
	vector<vector<double>> conWeights;

	double studySpeed = 1.0;
	double gradientMoment = 0.0;

	void GetWeightsFromFile(string fName = "NNS_State.txt");
};


// Not developed yet
void NetStructure::GetWeightsFromFile(string fName) {



}

//*********** Net Class ***********
class Net {
public:
	Net(NetStructure *netStr);

	bool TakeInput(vector<double> *inVals);
	bool TakeInputFile(string *fName);
	void PushForward();
	vector<double> *GetOutVals();
	void ClearNeurons();

	void Train(vector<vector<double>*> *inVects, vector<vector<double>*> *targetVals, unsigned long iterations, int errCalc = MIN_SQ_METHOD);

	void SaveStateToFile(string fName = "NNS_State.txt");

private:
	// values
	vector<NeuronLayer> neurons;
	vector<ConnectionLayer> connections;
	int m_funcType;		// Maybe not needed
	double m_studySpeed = 0.5;
	double m_studyMoment = 0.0;

	// methods
	void AddNeuronLayer(int size, int funcType, int neuronType, bool AddBias);
	void AddConnLayer(NeuronLayer *from, NeuronLayer *to, vector<double> weights);
	void AddConnLayer(NeuronLayer *from, NeuronLayer *to);
	void BackPropagation(vector<double> *errors);

	void Study(vector<double> *inVals, vector<double> *targetVals, unsigned long iterations, int errCalc = MIN_SQ_METHOD);
};

Net::Net(NetStructure *netStr) {
	m_studyMoment = netStr->gradientMoment;
	m_studySpeed = netStr->studySpeed;
	
	// Add neuron layers
	AddNeuronLayer(netStr->neurons[0], netStr->activationFunction, INPUT_NEURON, netStr->addBias);
	for (int l = 1; l < netStr->neurons.size() - 1; l++)
		AddNeuronLayer(netStr->neurons[l], netStr->activationFunction, HIDDEN_NEURON, netStr->addBias);
	AddNeuronLayer(netStr->neurons.back(), netStr->activationFunction, OUTPUT_NEURON, netStr->addBias);


	// Add connecions
	for (int i = 0; i < this->neurons.size() - 1; i++) {
		if (i < netStr->conWeights.size()) {
			AddConnLayer(&(neurons[i]), &(neurons[i + 1]), netStr->conWeights[i]);
		}
		else {
			AddConnLayer(&(neurons[i]), &(neurons[i + 1]));
		}
	}

	// Add pointers to connections to all corresponding neurons
	for (int i = 0; i < connections.size(); i++) {
		for (int j = 0; j < connections[i].size(); j++) {
			connections[i][j].m_from->AddConnectionPtr(&connections[i][j]);
		}
	}
}

void Net::AddConnLayer(NeuronLayer *from, NeuronLayer *to, vector<double> weights) {
	connections.push_back(ConnectionLayer());
	int wCount = 0;
	for (int f = 0; f < from->size(); f++) {
		for (int t = 0; t < to->size(); t++) {
			if (!to->at(t)->isBiasNeuron()) {
				if (wCount < weights.size()) {
					connections.back().push_back(Connection(from->at(f), to->at(t), weights[wCount]));
					wCount++;
				}
				else {
					connections.back().push_back(Connection(from->at(f), to->at(t)));
				}
				// Add study parametest ptrs to connections
				connections.back().back().m_studyMomentPtr = &m_studyMoment;
				connections.back().back().m_studySpeedPtr = &m_studySpeed;
			}
		}
	}
}

void Net::AddConnLayer(NeuronLayer *from, NeuronLayer *to) {
	connections.push_back(ConnectionLayer());
	for (int f = 0; f < from->size(); f++) {
		for (int t = 0; t < to->size(); t++) {
			if (!to->at(t)->isBiasNeuron()) {
				connections.back().push_back(Connection(from->at(f), to->at(t)));
			}

			// Add pointers to the study parameters
			connections.back().back().m_studyMomentPtr = &m_studyMoment;
			connections.back().back().m_studySpeedPtr = &m_studySpeed;
		}
	}
}

void Net::AddNeuronLayer(int size, int funcType, int neuronType, bool AddBias = false) {
	neurons.push_back(NeuronLayer());

	if (neuronType == HIDDEN_NEURON) {
		for (int n = 0; n < size; n++)
			neurons.back().push_back(new HiddenNeuron(funcType));
	}
	else if (neuronType == INPUT_NEURON) {
		for (int n = 0; n < size; n++)
			neurons.back().push_back(new InputNeuron(funcType));
	}
	else if (neuronType == OUTPUT_NEURON) {
		for (int n = 0; n < size; n++)
			neurons.back().push_back(new OutputNeuron(funcType));
	}
	else {
		cout << "the type of neuron: " << neuronType << " was not defined\n";
	}

	if (AddBias) {
		neurons.back().push_back(new BiasNeuron(funcType));
	}
}

bool Net::TakeInput(vector<double> *inVals) {
	if (inVals->size() != neurons[0].size()) {
		cout << "Input size incorrect\n";
		return false;
	}
	else {
		for (int i = 0; i < inVals->size(); i++) {
			neurons[0][i]->SetInVal(inVals->at(i));
		}
		return true;
	}
}

bool Net::TakeInputFile(string *fName) {
	vector<double> inVals = ReadFileToDoubleArray(*fName);
	
	if (inVals.size() != neurons[0].size()) {
		cout << "Input file \"" << *fName << "\" has incorrect data format\n";
		return false;
	}
	else {
		return TakeInput(&inVals);
	}
}

void Net::PushForward() {
	for (int l = 0; l < connections.size(); l++) {
		for (int c = 0; c < connections[l].size(); c++) {
			connections[l][c].FeedForward();
		}
	}
}

void Net::BackPropagation(vector<double> *errors)
{
	if (errors->size() != neurons.back().size()) {
		cout << "Sizes of the output layer and errors vector are different\n";
		cout << "To do back prop, please input correct size array\n";
		cout << "Output later size = " << neurons.size() << "\n";
		return;
	}
	else {
		for (int i = 0; i < errors->size(); i++) {
			neurons.back()[i]->AddToDeltaSum(errors->at(i));
			neurons.back()[i]->CalculateDelta();
		}
		
		int nLayer = neurons.size() - 2;

		// Back propagation itself
		for (int i = connections.size() - 1; i >= 0; i--) {
			// Propagate delta back through connections in the connection layer
			for (int j = 0; j < connections[i].size(); j++) {
				connections[i][j].PropagateBack();
			}

			// Calculate delta for every neuron in layer
			for (int j = 0; j < neurons[nLayer].size(); j++) {
				neurons[nLayer][j]->CalculateDelta();

				// Calculate gradient and update outcoming connections weights
				for (int con = 0; con < neurons[nLayer][j]->m_outConnections.size(); con++) {
					double gradient = neurons[nLayer][j]->GetOutVal() * neurons[nLayer][j]->m_outConnections[con]->GetDelta();
					neurons[nLayer][j]->m_outConnections[con]->UpdateWeight(gradient);
				}
			}

			// Check if layer calculation has gone wrong
			nLayer -= 1;
			if (nLayer < 0) {
				return;
			}

		}
	}

}

vector<double> *Net::GetOutVals() {
	vector<double> *outVals = new vector<double>();

	for (int i = 0; i < neurons.back().size(); i++) {
		outVals->push_back(neurons.back()[i]->GetOutVal());
	}

	return outVals;
}

void Net::ClearNeurons() {
	for (int i = 0; i < neurons.size(); i++) {
		for (int j = 0; j < neurons[i].size(); j++) {
			neurons[i][j]->Clear();
		}
	}
}

void Net::Train(vector<vector<double>*> *inVects, vector<vector<double>*> *targetVals, unsigned long iterations, int errCalc) {
	LogFile logFile("E:\\NetStudyLog.txt");

	double(*ErrorCalculator)(vector<double> result, vector<double> target) = NULL;
	if (errCalc == MIN_SQ_METHOD)
		ErrorCalculator = MSE;
	else if (errCalc == ROOT_MIN_SQ)
		ErrorCalculator = RootMSE;
	else if (errCalc == ATAN_METHOD)
		ErrorCalculator = Arctan;

	for (int i = 0; i < inVects->size(); i++) {
		if (inVects->at(i)->size() != neurons[0].size()) {
			cout << "input at " << i << " has wrong size\n";
			return;
		}
	}

	for (int i = 0; i < targetVals->size(); i++) {
		if (targetVals->at(i)->size() != neurons.back().size()) {
			cout << "target vals size != neurons back size\n";
			return;
		}
	}

	cout << "Ready to study\n";
	
	int inputTurn = 0;
	for (int i = 0; i < iterations; i++) {
		if (inputTurn >= inVects->size())
			inputTurn = 0;

		cout << "iteration = " << i;
		Study(inVects->at(inputTurn), targetVals->at(inputTurn), 1, errCalc);

		inputTurn++;
	}
}

void Net::Study(vector<double> *inVals, vector<double> *targetVals, unsigned long iterations, int errCalc) {
	LogFile logFile("E:\\NetStudyLog.txt");
	
	double(*ErrorCalculator)(vector<double> result, vector<double> target) = NULL;
	if (errCalc == MIN_SQ_METHOD)
		ErrorCalculator = MSE;
	else if (errCalc == ROOT_MIN_SQ)
		ErrorCalculator = RootMSE;
	else if (errCalc == ATAN_METHOD)
		ErrorCalculator = Arctan;
	
	//double(*ErrorCalculator)(vector<double> result, vector<double> target) = MSE;

	if (inVals->size() != neurons[0].size() || targetVals->size() != neurons.back().size()) {
		cout << "target vals size or input vals size incorrect\n";
		return;
	}

	for (int i = 0; i < iterations; i++) {
		if (this->TakeInput(inVals)) {
			// Take input and push forward
			this->PushForward();
			vector<double> *outVals = this->GetOutVals();
			double resErr = ErrorCalculator(*outVals, *targetVals);

			// Debugging output
			cout << "   Error = " << resErr << "\n\n";
			// Delete later

			logFile.ToLog(std::to_string(i) + " " + std::to_string(resErr));

			// Take errors and propagate back
			vector<double> *errors = new vector<double>;
			for (int e = 0; e < targetVals->size(); e++) {
				errors->push_back(targetVals->at(e) - outVals->at(e));
			}

			BackPropagation(errors);
			
			this->ClearNeurons();

			errors->clear();
			outVals->clear();
			delete errors;
			delete outVals;
		}
	}
}

// Debugging
void Net::SaveStateToFile(string fName) {
	ofstream oFile(fName, ofstream::out);

	for (int l = 0; l < connections.size(); l++) {
		for (int con = 0; con < connections[l].size(); con++) {
			oFile << l << " " << con << " " << connections[l][con].m_weight << "\n";
		}
	}

	oFile.close();
}