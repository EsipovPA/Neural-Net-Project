#include "Math.h"
#include <string>

//********* Connection Class *********
class Connection {
public:
	Connection() {};
	Connection(Neuron *from, Neuron *to) {
		this->m_from = from;
		this->m_to = to;
		this->m_weight = GetRandomWeight();
		//m_from->AddConnectionPtr(this);	// Maybe better to create pointers after constructing the net
	}
	Connection(Neuron *from, Neuron *to, double weight) : Connection(from, to) {
		this->m_weight = weight;
	}

	void FeedForward();
	void PropagateBack();
	void UpdateWeight(double gradient) {

		if (m_studySpeedPtr == NULL) {
			cout << "Can't solve\n";
			return;
		}

		double delta = *m_studySpeedPtr * gradient  + *m_studyMomentPtr * m_lastDelta;
		m_lastDelta = delta;
		m_weight += delta;
	}

	double GetWeightTimesDelta() { return (m_weight * m_to->GetDelta()); }
	double GetDelta();
	//double GetDelta() { return m_currentDelta; }

	// Pointers to the network study parameters
	double *m_studySpeedPtr;
	double *m_studyMomentPtr;

	double m_lastDelta;
	double m_currentDelta;

	//debugging val
	bool doneBackProp = false;

//private:
	Neuron *m_from;
	Neuron *m_to;
	double m_weight;

	double GetRandomWeight() { return (double)rand() / RAND_MAX; }
};

void Connection::FeedForward() {
	double sourceVal = m_from->GetOutVal();
	m_to->AddToInVal(sourceVal * m_weight);
}

double Connection::GetDelta() {
	return this->m_to->GetDelta();
}

void Connection::PropagateBack() {
	double sourceVal = m_to->GetDelta();
	m_from->AddToDeltaSum(sourceVal * m_weight);
	m_currentDelta = m_to->GetDelta();
}