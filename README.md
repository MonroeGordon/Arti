# Arti

This repository contains iterations of Arti, a spiking neural network AI.

Arti Beta

I have currently created a Tic-Tac-Toe game that you can play against an AI I call Arti. In the current iteration, Arti plays random moves on empty spaces. He does not learn. His neural network consists of 19 sensory neurons to provide input to a single neuron that makes a random move based on the available empty spaces. Arti runs on a separate thread and is always on while the program is running. 

During the entire runtime, Arti is logging his data that gets written to an Excel file when you exit the program. The current log settings have him logging the game state inputs and the data from the somatic neurode of his single neuron. The Arti_1_0_Datalog class contains several functions to control what neural data Arti logs. The setup for his data logging is set at the end of his init function.

Arti's neural network is modeled after human neurons. A neuron inside his neural net is made up of neurodes, which are subunits that can process synaptic input in various ways. A neuron would, therefore, be made up of at least one neurode performing the function of the soma. It may also contain several dendritic neurodes and axonal neurodes. Arti's 19 sensory neurons only contain a somatic neurode. His single neuron contains 91 dendritic neurodes, 1 somatic neurode and 9 axonal neurodes. Arti has a total of 273 synapses, which are modeled after human synapses.

Artilab

**Important Note: files for this project include source and header files for the Dear ImGui library. This library is used for creating the GUI of the program. My program uses the OpenGL 3 backend for ImGui. OpenGL files are not included in this repository, nor are any static or dynamic libraries. To setup this project for yourself, you will need to know how to setup and incorporate ImGui and OpenGL into the project.**

Artilab is a software program I am developing in C++ to allow me to design, test and work with Arti. Arti will always be running in the background of this program. Ideally, when Arti is built with the capabilities, I'll be able to chat with Arti and have him help me work the program. Currently, I've developed several Arti Viewers that allow me to see his neural matrices in several different ways (tree, matrix, and table views currently). I will be developing editors that I will use for designing and constructing Arti's neural matrices. Also, there will be various virtual environments that I will use for testing Arti. Currently, Tic-Tac-Toe is being constructed for use with Arti. Afterwards, I will have Arti learn to play Chess.

This current version allows for viewing of Arti and playing Arti in Tic-Tac-Toe. Arti's current neural matrix is a C++ recreation of the neural matrix I designed in Python for Arti Beta, but with modifications to the processing of the neural matrix. The recreation still has design issues to work out, but Arti can successfully play Tic-Tac-Toe against a human player. The table and tree viewers are fully operational. The matrix viewer is mostly operational, but has some bugs to work out.

This version of Arti is the first official, non-prototype version. He is programmed to run on an NVIDIA GPU via the CUDA library. Arti's core code is written in pure C. All overlay elements (neural tree and neural matrix viewer) for Arti and the Artilab program are in C++.
