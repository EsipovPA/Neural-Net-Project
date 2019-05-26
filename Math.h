#include <cmath>
#include <vector>
#include <iostream>

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

#define HIDDEN_NEURON 6
#define OUTPUT_NEURON 7
#define BIAS_NEURON 8
#define INPUT_NEURON 9

#define MIN_SQ_METHOD 10
#define ROOT_MIN_SQ 11
#define ATAN_METHOD 12

#define NeuronLayer vector<Neuron*>
#define ConnectionLayer vector<Connection>

//********* Activation functions *********
double LinearActivationFunction(double inVal);

double SigmoidActivationFunction(double inVal);

double HyperbolicTanActivationFunction(double inVal);

double InputNeuronActivationFunction(double inVal);

double BiasNeuronActivationFunc(double inVal);

//********* Activation functions derivatives *********
double LinearFincDeriv(double inVal);

double SigmoidFuncDeriv(double inVal);

double HyperbolicFuncDeriv(double inVal);

//********* Error calculation functions *********
double MSE(vector<double> result, vector<double> target);

double RootMSE(vector<double> result, vector<double> target);

double Arctan(vector<double> result, vector<double> target);