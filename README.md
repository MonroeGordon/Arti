# Arti

This repository contains iterations of Arti, a spiking neural network AI.

Arti 1.0

I have currently created a Tic-Tac-Toe game that you can play against an AI I call Arti. In the current iteration, Arti plays random moves on empty spaces. He does not learn. His neural network consists of 19 sensory neurons to provide input to a single neuron that makes a random move based on the available empty spaces. Arti runs on a separate thread and is always on while the program is running. 

During the entire runtime, Arti is logging his data that gets written to an Excel file when you exit the program. The current log settings have him logging the game state inputs and the data from the somatic neurode of his single neuron. The Arti_1_0_Datalog class contains several functions to control what neural data Arti logs. The setup for his data logging is set at the end of his init function.

Arti's neural network is modeled after human neurons. A neuron inside his neural net is made up of neurodes, which are subunits that can process synaptic input in various ways. A neuron would, therefore, be made up of at least one neurode performing the function of the soma. It may also contain several dendritic neurodes and axonal neurodes. Arti's 19 sensory neurons only contain a somatic neurode. His single neuron contains 91 dendritic neurodes, 1 somatic neurode and 9 axonal neurodes. Arti has a total of 273 synapses, which are modeled after human synapses.
