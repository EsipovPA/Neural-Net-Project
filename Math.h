#include <cmath>
#include <vector>
#include <iostream>
#include <math.h>

using namespace std;

struct NetStructure;
class Connection;
class Neuron;
class Net;

// Types of the activation function
#define LINEAR_FUNCTION 1
#define SIGMOID_FUNCTION 2
#define HYPERBOLIC_TAN_FUNCTION 3
#define BIAS_NEURON_FUNCITON 4
#define INPUT_NEURON_FUNCTION 5
#define THRESHOLD_FUNCTION 5
#define SENSOR_FUNCTION 7

// Types of the neurons
#define HIDDEN_NEURON 8
#define OUTPUT_NEURON 9
#define BIAS_NEURON 10
#define INPUT_NEURON 11
#define SENSOR_NEURON 12
#define ASSOCIATIVE_NEURON 13
#define REACTING_NEURON 14

// Error calculation methods
#define MIN_SQ_METHOD 15
#define ROOT_MIN_SQ 16
#define ATAN_METHOD 17

// Types of the connections for the perceptron net
#define S_A_CONNECTION 18
#define A_R_CONNECTIN 19

#define NeuronLayer vector<Neuron*>
#define ConnectionLayer vector<Connection>

//********* Activation functions *********
double LinearActivationFunction(double inVal);

double SigmoidActivationFunction(double inVal);

double HyperbolicTanActivationFunction(double inVal);

double InputNeuronActivationFunction(double inVal);

double BiasNeuronActivationFunc(double inVal);

double ThresholdActivationFunction(double inVal);

double SensorActivationFunction(double inVal);

//********* Activation functions derivatives *********
double LinearFincDeriv(double inVal);

double SigmoidFuncDeriv(double inVal);

double HyperbolicFuncDeriv(double inVal);

//********* Error calculation functions *********
double MSE(vector<double> result, vector<double> target);

double RootMSE(vector<double> result, vector<double> target);

double Arctan(vector<double> result, vector<double> target);

