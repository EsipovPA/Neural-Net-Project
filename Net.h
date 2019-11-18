#include "Neuron.h"
#include "Connection.h"
#include "LogFile.h"
#include "ImageFileReader.h"
#include "Preprocessing.h"
#include "NetStructures.h"

#include <string>

//*********** Net Class ***********
class Net {
public:
	Net(NetStructure *netStr);
	Net(ConvStructure *netStr);

	bool TakeInput(vector<double> *inVals);
	bool TakeInputFile(string *fName);
	bool TakeInputImage(string imgName);
	void PushForward();
	void PushForwardCount(int layerNum);		// Testing neeeded
	vector<double> *GetOutVals();
	void ClearNeurons();

	// Study methods
	void Train(vector<vector<double>*> *inVects, vector<vector<double>*> *targetVals, unsigned long iterations, int errCalc = MIN_SQ_METHOD);
	void AutoencoderStudy(vector<vector<double>*> *inVects, unsigned long iterations, int errCalc = MIN_SQ_METHOD);

	void SaveStateToFile(string fName = "NNS_State.txt");

private:
	// values
	vector<NeuronLayer> neurons;
	vector<ConnectionLayer> connections;
	int m_funcType;
	double m_studySpeed = 0.5;
	double m_studyMoment = 0.0;

	// methods
	void AddNeuronLayer(int size, int funcType, int neuronType, bool AddBias);
	void AddConnLayer(NeuronLayer *from, NeuronLayer *to, vector<double> weights);
	void AddConnLayer(NeuronLayer *from, NeuronLayer *to);
	void BackPropagation(vector<double> *errors);

	void Study(vector<double> *inVals, vector<double> *targetVals, unsigned long iterations, int errCalc = MIN_SQ_METHOD);

	vector<vector<double>*> *GetConnectionsWeights();				// Testing needed
	void SetConnWeights(int layerNum, vector<double> weights);		// Testing needed
	vector<double> *GetLayerInput(int layerNum);					// Testing
};

Net::Net(NetStructure *netStr) {
	m_studyMoment = netStr->gradientMoment;
	m_studySpeed = netStr->studySpeed;
	m_funcType = netStr->activationFunction;

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

Net::Net(ConvStructure *netStr) {

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

bool Net::TakeInputImage(string imgName) {
	vector<double> *inVals = ImgToDoubleArrayPtr(imgName);

	if (inVals->size() != neurons[0].size()) {
		cout << "Input image " << imgName << " size unsupported.\n";
		cout << "Please input the image of size 90x90\n";
		return false;
	}
	else {
		return TakeInput(inVals);
	}
}

void Net::PushForward() {
	for (int l = 0; l < connections.size(); l++) {
		for (int c = 0; c < connections[l].size(); c++) {
			connections[l][c].FeedForward();
		}
	}
}

// Maybe not needed
void Net::PushForwardCount(int layerNum) {
	if (layerNum != 0)
		cout << "In the PushForwardCount with layer num = " << layerNum << "\n";

	
	for (int i = 0; i < layerNum; i++) {
		cout << "connections layer[" << i << "].size() = " << connections[i - 1].size() << "\n";
//		for (int c = 0; c < connections[i - 1].size(); c++) {
//		connections[i - 1][c].FeedForward();
//}
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
	//LogFile logFile("E:\\NetStudyLog.txt");

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
	
	cout << "Teach start\n";
	int inputTurn = 0;
	for (int i = 0; i < iterations; i++) {
		if (inputTurn >= inVects->size())
			inputTurn = 0;

		cout << "iteration = " << i;
		Study(inVects->at(inputTurn), targetVals->at(inputTurn), 1, errCalc);

		inputTurn++;
	}
	cout << "Teach end\n";
}

void Net::Study(vector<double> *inVals, vector<double> *targetVals, unsigned long iterations, int errCalc) {
	//LogFile logFile("E:\\NetStudyLog.txt");
	
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

			// Disable logging for better performance
			//logFile.ToLog(std::to_string(i) + " " + std::to_string(resErr));

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

vector<vector<double>*> *Net::GetConnectionsWeights() {
	vector<vector<double>*> *outVals = new vector<vector<double>*>();
	for (int l = 0; l < connections.size(); l++) {
		outVals->push_back(new vector<double>());
		for (int c = 0; c < connections[l].size(); c++) {
			outVals->back()->push_back(connections[l][c].GetConWeight());
		}
	}

	return outVals;
}

vector<double>* Net::GetLayerInput(int layerNum) {
	vector<double> *inVect = new vector<double>();

	for (Neuron *nPtr : neurons[layerNum]) {
		inVect->push_back(nPtr->GetInputValue());
	}

	return inVect;
}

void Net::SetConnWeights(int layerNum, vector<double> weights) {
	if (weights.size() == connections[layerNum].size()) {
		for (int i = 0; i < connections[layerNum].size(); i++) {
			connections[layerNum][i].SetWeight(weights[i]);
		}
	}
	else {
		cout << "\n\nERROR!!\nConnections layer is not the same size as the new weights array\n";
		cout << "Layer number = " << layerNum << "\nERROR!!\n\n";
	}
}

void Net::AutoencoderStudy(vector<vector<double>*> *inVects, unsigned long iterations, int errCalc) {
	NetStructure *sPtr = new NetStructure();
	sPtr->activationFunction = m_funcType;
	sPtr->studySpeed = 0.5;
	sPtr->gradientMoment = 0.3;
	sPtr->addBias = false;

	// Teach the net using autoencoder
	for (int cl = 0; cl < connections.size(); cl++) {
		// Build the autoencoder
		sPtr->neurons.clear();
		sPtr->neurons.push_back(this->neurons[cl].size());
		sPtr->neurons.push_back(this->neurons[cl + 1].size());
		sPtr->neurons.push_back(this->neurons[cl].size());
		Net *aePtr = new Net(sPtr);

		// Get the layer input data
		vector<vector<double>*> studyInput;
		for (int i = 0; i < inVects->size(); i++) {
			this->ClearNeurons();
			this->TakeInput(inVects->at(i));
			this->PushForward();
			studyInput.push_back(this->GetLayerInput(cl));
		}

		// Study the autoencoder
		aePtr->Train(&studyInput, &studyInput, iterations * inVects->size(), errCalc);

		// Move connections weights to the main network
		vector<vector<double>*> *conVecights = aePtr->GetConnectionsWeights();
		for (int con = 0; con < this->connections[cl].size(); con++) {
			this->connections[cl][con].SetWeight(conVecights->at(0)->at(con));
		}

		// Delete the traning net
		delete aePtr;
		
		// Clear the memory
		for (int i = 0; i < studyInput.size(); i++) {
			studyInput[i]->clear();
			delete studyInput[i];
		}
		
	}

	/*
	// Teach the net by the autoencoder
	// Works for every connections layer
	for (int cl = 0; cl < connections.size(); cl++) {
		// Build the autoencoder
		sPtr->neurons.clear();
		sPtr->neurons.push_back(this->neurons[cl].size());
		sPtr->neurons.push_back(this->neurons[cl + 1].size());
		sPtr->neurons.push_back(this->neurons[cl].size());
		Net *nPtr = new Net(sPtr);

		// Get input data
		vector<vector<double>*> studyInput;
		for (int i = 0; i < inVects->size(); i++) {
			this->ClearNeurons();
			this->TakeInput(inVects->at(i));
			this->PushForward(cl);
			studyInput.push_back(&(this->GetLayerInput(cl)));
		}
		
		
		// Train the autoencoder
		nPtr->Train(inVects, inVects, iterations, errCalc);

		cout << "Autoencoder study iteration ¹" << cl << " is done\n";

		
		// Set connections from autoencoder to the main net
		//for (int i = 0; i < nPtr->connections[0].size(); i++) {
		//	this->connections[cl][i].SetWeight(nPtr->connections[0][i].GetConWeight());
		//}

		// Clear study array
		// Maybe not needed. Test later
		

		// Delete autoencoder network
		delete nPtr;
	}*/
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

// Evolution study funcions

// Mutate the weights in the parent neural network
//  Mutate weights by redefinign part of them
void Mumate(Net *nPtr, int weightsToMutate) {
	cout << "In the main mutate functon\n";
}

vector<Net*> Mutate(vector<Net*> parents, int clonesCount = 3) {
	vector<Net*> children;

	// Mutate here

	parents.clear();		// Clear population before return
	return children;
}

vector<Net*> GenerateParents(NetStructure *strPtr, int parentsCount) {
	vector<Net*> parents;

	for (int p = 0; p < parentsCount; p++) {
		parents.push_back(new Net(strPtr));		// Need to check if the nets are unic
	}

	return parents;
}

void SortChildren(vector<Net*> children /*training data here*/ /*number of survivals here*/) {
	cout << "Sort the children on how do they solve the challendge\n";
}

// The general Evolution study method
Net *EvolutionStudy(NetStructure *strPtr, int generations, int parentsCount /*training data here*/) {
	cout << "Evolution study test\n";



	vector<Net*> population = GenerateParents(strPtr, parentsCount);
/*
	for (int g = 0; g < generations; g++) {
		population = Mutate(population);
		SortChildren(population);
	}

	// Need to test this part
*/
	Net *bestChild = population[0];
	population.clear();


	return bestChild;
}