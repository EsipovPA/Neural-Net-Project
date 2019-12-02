# Neural_Net_Study

Usefull papers on Neural networks
1. https://habr.com/ru/post/312450/
2. https://habr.com/ru/post/313216/

# My papers on LinkedIn. Kind of personnal blog about the journy inside Machene learning

1. https://www.linkedin.com/pulse/my-first-neural-network-solving-xor-problem-teacher-paul-esipov/
2. https://www.linkedin.com/pulse/traning-perceptron-read-symbols-from-images-neural-networks-esipov/

# The description of the classes in this project is given in the first paper.

What one may find in this project:
1. Net.h - the header file containing the description of the neural network class itself. In this file one may find the Net class and the methods of this class. Also there are the NetStructure structure (sounds kinda funny), that is used by the Net class constructor as an input. This is the main header file, required to get the net up and running. Right now I'm working on adding the evolution training algorithm to this file. To use this header in your project you are going to need to add the following list of files: Neuron.h, Connection.h, LogFile.h, ImageFileReader.h, ImageFileReader.cpp, Preprocessing.h, Math.h, Math.cpp

2. Neuron.h - the header file containing a definition and a description of the Neuron class. Also there are definitions for some classes derived from the main Neuron class, but right now they did not find any application. This header is mostly stand-alone file. a project with this file, one is going to need to add just the "Math.h" and the "Math.cpp" files to the same project.

3. Connection.h - the header file containing a definition and a description of the Connection class, used to connect neurons into the complete neural network. This header also requires just the "Math.h" header file to run correctly.

4. Math.h and Math.cpp - source files containing some math. In these files the definitions of the types of the activation functions, the neurons, the error calculation methods and the types of connections are given. This files are required by all other components of the project. Also there are activation functions, their derivatives and error calculation functions in these files.

5. Preprocessing.h - this file is in work in progress state. In the future it will contain a preprocessing functions for a network input data.

6. LogFile.h - my personal class for logging.

7. ImageFileReader.h and ImageFileReader.cpp - these files are also in work in progress state. I'm aiming to develop and put there some methods for image files reading and converting into arrays of input data for the net. With these methods, the net will be able to read, recognize and do other funny stuff with the images and videos. Mainly, it is the basis for the interface to the OpenCV library.

# How to run and test the project:

1. Create the new project in the VisualStudio. This project may be built with other IDEs, but is not tested this way.

2. Put into the project the files described in the list above.

3. #include "Net.h"

4. In the main part of the program, create an instance for NetStructure. This structure contains basic parameters for the net constructor. For example:

		// Create the structure instance
		NetStructure *sPtr = new NetStructure();
		
		// Slect the type of the activation function
		sPtr->activationFunction = SIGMOID_FUNCTION;
		
		// Three neurn layers
		sPtr->neurons.push_back(15);
		sPtr->neurons.push_back(15);
		sPtr->neurons.push_back(10);
		
		// Study parameters
		sPtr->studySpeed = 0.5;
		sPtr->gradientMoment = 0.3;
		sPtr->addBias = false;

5. Create The Net:

		Net *nPtr = new Net(sPtr);

6. Give the net some training data and teach it.

		TraningSet trSet = GetLinearTraningSet(&files, 10);
		nPtr->Train(trSet.first, trSet.second, 10000);
		
The training set is as follows:

		#define TraningSet pair<vector<vector<double>*>*, vector<vector<double>*>*>

7. Test the net.

