from tkinter.tix import TCL_TIMER_EVENTS
from datalog import *
import mysql.connector
from neuralmath import *
import random
from threading import *

MIN_VALUE = 0.001 # global minimum value

"""
Arti_1_0: This class contains all data and functions that create and control the AI neural network called Arti. This class is for Arti 1.0.
This is the first iteration of Arti, which is designed to play Tic-Tac-Toe by selecting a random open square for its next move on its turn,
while avoiding occupied squares. This iteration has no concept of winning or losing and does not learn. Arti's neural network, in this
iteration, contains 19 sensory neurons and 1 neuron. The singular neuron is responsible for generating Arti's next random move to an open
square. Arti_1_0 is a subclass of Thread so that it can run separately from all other program threads.
"""
class Arti_1_0(Thread):
    """
    Initializes Arti's neural network, which is comprised of 120 neurodes and 273 synapses. It also initializes game state variables that are
    used as inputs to Arti's sensory neurons.
    self: object reference
    """
    def __init__(self):
        super().__init__()
        
        self.time = 0.0
        self.turn = 'X'
        self.player = 'O'
        self.moveNum = 0
        self.board = [' ', ' ', ' ',
                      ' ', ' ', ' ',
                      ' ', ' ', ' ']
        self.xNeuron = []
        self.oNeuron = []
        self.mNeuron = []
        self.tNeuron = SensoryNeuron(soma=SomaticNeurode(activation=SigmoidActivation(1, 10)))
        self.neuron = Neuron(soma=SomaticNeurode(activation=HardSigmoidActivation(1.5, 2), oscillator=ReverseSawtoothOscillator(0.82, 0.502, -0.02), threshold=0.1))
        self.output = 0

        self.tNeuron.soma.synInput[0].config(sign=-1)
        self.tNeuron.soma.addSynapticInput(SensorySynapse(sign=1))

        for i in range(9):
            self.xNeuron.append(SensoryNeuron(soma=SomaticNeurode(activation=SigmoidActivation(2.3, 10), oscillator=SawtoothOscillator(1, 0.5, i / 4.5))))
            self.oNeuron.append(SensoryNeuron(soma=SomaticNeurode(activation=SigmoidActivation(2.3, 10), oscillator=SawtoothOscillator(1, 0.5, i / 4.5))))
            self.mNeuron.append(SensoryNeuron(soma=SomaticNeurode(activation=SigmoidActivation(2.3, 10), oscillator=SawtoothOscillator(1, 4.5, 0.53))))

        self.neuron.addDendrite(DendriticNeurode(activation=GaussianActivation(1, 1), oscillator=SawtoothOscillator(0.1, 4.5, 0.55)))
        self.neuron.connectNeurodes(self.neuron.dendrite[0], self.neuron.soma)
        self.neuron.createSynapse(self.tNeuron.soma, self.neuron.dendrite[0], InhibitorySynapse(filter=BandpassFilter(1, 10), inputDecay=0.01))

        for i in range(9):
            self.neuron.addDendrite(DendriticNeurode(activation=GaussianActivation(11, 5)))
            self.neuron.connectNeurodes(self.neuron.dendrite[i + 1], self.neuron.dendrite[0])
            self.neuron.createSynapse(self.mNeuron[i].soma, self.neuron.dendrite[i + 1], ExcitatorySynapse(filter=BandpassFilter(1, 10), inputDecay=0.01))

            for j in range(9):
                if j != i:
                    self.neuron.createSynapse(self.mNeuron[j].soma, self.neuron.dendrite[i + 1], InhibitorySynapse(weight=9))

            self.neuron.addAxon(AxonalNeurode(activation=HardSigmoidActivation(4.5, 2 / 9), threshold=1 / 9))
            self.neuron.connectNeurodes(self.neuron.soma, self.neuron.axon[i])
            self.neuron.axon[i].addSynapticOutput(ExcitatorySynapse(filter=BandpassFilter(i + 1, 1)))

        for i in range(9):
            for j in range(9):
                self.neuron.addDendrite(DendriticNeurode(activation=GaussianActivation(1.175, 0.05), decay=0.1, oscillator=SawtoothOscillator(1, 0.5, j / 4.5)))
                self.neuron.connectNeurodes(self.neuron.dendrite[1 + j + ((i + 1) * 9)], self.neuron.dendrite[i + 1], weight=10)
                self.neuron.createSynapse(self.xNeuron[j].soma, self.neuron.dendrite[1 + j + ((i + 1) * 9)], InhibitorySynapse(weight=1))
                self.neuron.createSynapse(self.oNeuron[j].soma, self.neuron.dendrite[1 + j + ((i + 1) * 9)], InhibitorySynapse(weight=1))

        self.dataLog = Arti_1_0_DataLog()
        self.dataLog.logInputs()
        self.dataLog.logSoma()

        self.lock = Lock()
        self.stopped = True

        self.listeners = []

    """
    This function adds the specified listener to Arti's list of listeners that will receive messages from Arti.
    self: object reference
    listener: the listener object to add
    """
    def addListener(self, listener):
        if listener is not None:
            self.listeners.append(listener)
            return True

        return False

    """
    This function removes the specified listener from Arti's list of listeners.
    self: object reference
    listener: the listener object to remove
    """
    def removeListener(self, listener):
        if self.listeners.__contains__(listener):
            self.listeners.remove(listener)
            return True

        return False

    """
    This function runs Arti's thread by calling the update function.
    self: object reference
    """
    def run(self):
        self.update()

    """
    This function updates the game state variables that are used as input to Arti's sensory neurons.
    self: object reference
    turn: turn X or O
    moveNum: current move number
    board: current gameboard state (list of X, O, or empty spaces)
    """
    def setInput(self, turn, moveNum, board):
        with self.lock:
            self.turn = turn
            self.moveNum = moveNum
            self.board = board

    """
    This function sets which player Arti will be playing as.
    self: object reference
    p: player X or O
    """
    def setPlayer(self, p):
        with self.lock:
            self.player = 'X' if p == 'X' else 'O'

    """
    This function sets the stopped flag to stop Arti's thread.
    self: object reference
    """
    def stop(self):
        with self.lock:
            self.stopped = True

    """
    This function processes Arti's neural network on a repeated cycle until Arti is told to stop. Arti also updates his datalog every cycle.
    self: object reference
    """
    def update(self):
        with self.lock:
            self.stopped = False # Reset stopped
            
        # Arti's processing loop
        while not self.stopped:
            # Set turn neuron inputs
            if self.turn == self.player:
                self.tNeuron.soma.synInput[0].currentInput = 1
            else:
                self.tNeuron.soma.synInput[1].currentInput = 1

            self.tNeuron.update(self.time) # Process turn neuron

            # For each square
            for i in range(9):
                # Set move neuron inputs
                if i == self.moveNum - 1:
                    self.mNeuron[i].soma.synInput[0].currentInput = 1

                # Set X and O neuron inputs
                if self.board[i] == 'X':
                    self.xNeuron[i].soma.synInput[0].currentInput = 1
                elif self.board[i] == 'O':
                    self.oNeuron[i].soma.synInput[0].currentInput = 1

                # Process move, X and O neurons
                self.mNeuron[i].update(self.time)
                self.xNeuron[i].update(self.time)
                self.oNeuron[i].update(self.time)

            # Process Arti's main neuron
            self.neuron.update(self.time)

            # Process the axons' synaptic outputs
            for i in range(9):
                self.neuron.axon[i].synOutput[0].update(self.time, 0)

            # Calculate Arti's final output based on his axons' synaptic outputs
            self.output = 0

            if self.turn == self.player:
                for i in range(9):
                    if self.neuron.axon[i].synOutput[0].sum == i + 1:
                        self.output = i + 1

            # Update Arti's clock
            self.time += 0.01

            # Inform Arti's listeners of his output, if he has one
            if self.turn == self.player and self.output != 0:
                for i in range(len(self.listeners)):
                    self.listeners[i].messageFromArti(ArtiMessage(self.output))

            # Update Arti's datalog
            self.dataLog.updateLog(self)

"""
ArtiMessage: This class is used to send Arti's output to all of Arti's listeners.
"""
class ArtiMessage:
    """
    Initializes Arti's message to contain his current output value.
    """
    def __init__(self, output):
        self.output = output

"""
ArtiListener: This class is used to designate objects as listeners of Arti's that will receive his messages. All subclasses must implement the
messageFromArti function.
"""
class ArtiListener:
    """
    Empty initializer function.
    self: object reference
    """
    def __init__(self):
        pass

    """
    Processes a message sent from Arti. This is to be implemented by all subclasses.
    self: object reference
    message: an ArtiMessage
    """
    def messageFromArti(self, message):
        pass

########################################################################################
# Neuron classes
########################################################################################
"""
Neuron: This class is used to create a standard neuron used in processing neural inputs from other neurons. This class is also the base
class for all neuron types.
"""
class Neuron:
    """
    Initializes the neuron with the specified somatic neurode.
    self: object reference
    soma: a somatic neurode (the neurode required for all neurons)
    """
    def __init__(self, soma=None):
        self.dendrite = []
        self.soma = SomaticNeurode() if soma is None else soma
        self.axon = []

    """
    Adds an axonal neurode to this neuron.
    self: object reference
    a: the axonal neurode to add
    """
    def addAxon(self, a):
        if a is not None: self.axon.append(a)

    """
    Adds an dendritic neurode to this neuron.
    self: object reference
    d: the dendritic neurode to add
    """
    def addDendrite(self, d):
        if d is not None: self.dendrite.append(d)

    """
    Connects the specified neurodes together with the specified input function and weight.
    self: object reference
    prevNeurode: the previous neurode
    nextNeurode: the next neurode
    func: the input function (add or multiply)
    weight: the weight value
    returns: true if the neurodes are successfully connected, otherwise false
    """
    def connectNeurodes(self, prevNeurode, nextNeurode, func=None, weight=None):
        if prevNeurode is not None and nextNeurode is not None:
            success = prevNeurode.addNext(nextNeurode)
            if not success: return False

            success = nextNeurode.addPrevious(prevNeurode, func, weight)
            if not success: 
                prevNeurode.removeNext(nextNeurode)
                return False

            return True

        return False

    """
    Creates the specified synapse between the specified neurodes.
    self: object reference
    preNeurode: the neurode creating the synaptic input
    postNeurode: the neurode receiving the synaptic input
    synapse: the type of synapse to create
    """
    def createSynapse(self, preNeurode, postNeurode, synapse=None):
        s = ExcitatorySynapse() if synapse is None else synapse
        preNeurode.addSynapticOutput(s)
        postNeurode.addSynapticInput(s)

    """
    This function processes the neuron based on the specified time value.
    self: object reference
    time: Arti's cycle clock time
    """
    def update(self, time):
        # Process all dendritic neurodes
        for i in range(len(self.dendrite)):
            self.dendrite[i].update(TCL_TIMER_EVENTS)

        # Process the somatic neurode
        self.soma.update(time)

        # Process all axonal neurodes
        for i in range(len(self.axon)):
            self.axon[i].update(time)

"""
SensoryNeuron: This class creates a basic sensory neuron that receives external sensory input and processes it into a neural output.
"""
class SensoryNeuron(Neuron):
    """
    Initializes the sensory neuron with the specified somatic neurode and number of synaptic inputs.
    self: object reference
    soma: the somatic neurode (the neurode required for all neurons)
    nInputs: the number of synaptic inputs
    """
    def __init__(self, soma=None, nInputs=1):
        super().__init__(soma)

        # Add the specified number of standard sensory synapses to the somatic neurode
        for i in range(nInputs):
            self.soma.addSynapticInput(SensorySynapse())

########################################################################################
# Neurode classes
########################################################################################
"""
Neurode: This class is the base class for all types of neurodes. Neurodes are the computational subunits that make up Arti's neural net.
"""
class Neurode:
    DEF_DECAY = 0.1 # Default sum decay rate
    DEF_WEIGHT = 1.0 # Default synaptic/neurode weight
    ADD_FUNCTION = 'add' # Add neurode input to sum
    MULTIPLY_FUNCTION = 'multiply' # Multiply sum by neurode input
    DEF_THRESHOLD = 0.9 # Default spiking threshold

    """
    Initializes the neurode with the specified activation function, sum decay rate, oscillator function, and spiking threshold.
    self: object reference
    activation: activation function
    decay: sum decay rate value
    oscillator: oscillator function
    threshold: spiking threshold value
    """
    def __init__(self, activation=None, decay=DEF_DECAY, oscillator=None, threshold=DEF_THRESHOLD):
        self.activation = SigmoidActivation() if activation is None else activation
        self.backpropagated = 0.0
        self.bpFrequency = 0.0
        self.decay = decay
        self.frequency = 0.0
        self.next = []
        self.oscillator = Oscillator() if oscillator is None else oscillator
        self.output = 0.0
        self.previous = []
        self.prevFunc = []
        self.prevWeight = []
        self.prevBpSpikeTime = 0.0
        self.prevSpikeTime = 0.0
        self.prevSum = 0.0
        self.refractory = 0.0
        self.spike = 0.0
        self.spikeCount = 0;
        self.sum = 0.0
        self.synInput = []
        self.synOutput = []
        self.threshold = threshold

    """
    Adds a neurode that is connected to receive input from this neurode.
    self: object reference
    neurode: the neurode to connect to this neurode
    returns: true if the neurode is successfully connected, otherwise false
    """
    def addNext(self, neurode):
        if neurode is not None: 
            self.next.append(neurode)
            return True

        return False

    """
    Adds a synaptic input to this neurode.
    self: object reference
    synapse: the synapse to connect to this neurode
    returns: true if the synapse is successfully connected, otherwise false
    """
    def addSynapticInput(self, synapse):
        if synapse is not None: 
            self.synInput.append(synapse)
            return True

        return False

    """
    Adds a synaptic output to this neurode.
    self: object reference
    synapse: the synapse to connect to this neurode
    returns: true if the synapse is successfully connected, otherwise false
    """
    def addSynapticOutput(self, synapse):
        if synapse is not None: 
            self.synOutput.append(synapse)
            return True

        return False

    """
    Adds a neurode that is connected to provide input to this neurode with the specified input function and weight.
    self: object reference
    neurode: the neurode to connect to this neurode
    func: the input function to use (add or multiply)
    weight: the weight value
    """
    def addPrevious(self, neurode, func=None, weight=None):
        if neurode is not None:
            self.previous.append(neurode)
            self.prevFunc.append(func if func is not None else Neurode.ADD_FUNCTION)
            self.prevWeight.append(weight if weight is not None else Neurode.DEF_WEIGHT)
            return True

        return False

    """
    This function handles the backpropagation of neurode spike outputs to previous neurodes.
    self: object reference
    spike: the spike value of a neurode after this neurode
    """
    def backpropagate(self, spike):
        bp = spike
        if bp > self.backpropagated: self.backpropagated = bp

    """
    This function sets the specified neurode values.
    self: object reference
    activation: activation function
    decay: sum decay rate value
    oscillator: oscillator function
    threshold: spiking threshold value
    """
    def config(self, activation=None, decay=None, oscillator=None, threshold=None):
        if activation is not None: self.activation = activation
        if decay is not None: self.decay = decay
        if oscillator is not None: self.oscillator = oscillator
        if threshold is not None: self.threshold = threshold

    """
    Returns the size of the list of next neurodes.
    returns: the size of the next neurode list
    """
    def nextSize(self):
        return len(self.next)

    """
    Returns the size of the list of previous neurodes.
    returns: the size of the previous neurode list
    """
    def previousSize(self):
        return len(self.previous)

    """
    Removes the specified next neurode.
    self: object reference
    neurode: the neurode to remove
    returns: true if the specified neurode is successfully removed, otherwise false
    """
    def removeNext(self, neurode):
        if self.next.__contains__(neurode): 
            self.next.remove(neurode)
            return True

        return False

    """
    Removes the specified previous neurode.
    self: object reference
    neurode: the neurode to remove
    returns: true if the specified neurode is successfully removed, otherwise false
    """
    def removePrevious(self, neurode):
        if self.previous.__contains__(neurode):
            index = self.previous.index(neurode)
            self.previous.remove(neurode)
            self.prevFunc.pop(index)
            self.prevWeight.pop(index)
            return True

        return False

    """
    Sets the next neurode at the specified index to the specified neurode.
    self: object reference
    index: the index of the next neurode to set
    neurode: the new neurode to set at index
    returns: true if the specified next neurode is set successfully, otherwise false
    """
    def setNext(self, index, neurode):
        if index >= 0 and index < len(self.next):
            if neurode is not None:
                self.next[index] = neurode
                return True

        return False

    """
    Sets the previous neurode at the specified index to the specified neurode and sets its input function and weight.
    self: object reference
    index: the index of the next neurode to set
    neurode: the new neurode to set at index
    func: the input function (add or multiply)
    weight: the weight value
    returns: true if the specified next neurode is set successfully, otherwise false
    """
    def setPrevious(self, index, neurode=None, func=None, weight=None):
        if index >= 0 and index < len(self.previous):
            if neurode is not None:
                self.previous[index] = neurode

            if func is not None:
                self.prevFunc[index] = func

            if weight is not None:
                self.prevWeight[index] = weight

            return True

        return False

    """
    This function processes the neurode based on the specified time. This function must be implemented by all subclasses.
    self: object reference
    time: Arti's cycle clock time
    """
    def update(self, time):
        return 0.0

"""
DendriticNeurode: This class creates and processes the dendritic neurode. A dendritic neurode represents a dendrite comparment of a neuron. 
This model is based off known biological dendritic processing in human neurons.
"""
class DendriticNeurode(Neurode):
    """
    Initializes the dendritic neurode based on the specified activation function, sum decay rate, oscillator function and spiking threshold.
    self: object reference
    activation: the activation function
    decay: the sum decay rate
    oscillator: the oscillator function
    threshold: the spiking threshold
    """
    def __init__(self, activation=None, decay=Neurode.DEF_DECAY, oscillator=None, threshold=Neurode.DEF_THRESHOLD):
        super().__init__(GaussianActivation() if activation is None else activation, decay, oscillator, threshold)

    """
    Adds the specified neurode that is connected to receive input from this neurode. Dendritic neurodes can only be connected to one next
    neurode.
    self: object reference
    neurode: the neurode to connect to this neurode
    returns: true if the specified neurode is successfully connected, otherwise false
    """
    def addNext(self, neurode):
        if neurode is not None and len(self.next) < 1:
            self.next.append(neurode)
            return True

        return False

    """
    This function processes the neurode based on the specified time.
    self: object reference
    time: Arti's cycle clock time
    """
    def update(self, time):
        add = 0.0 # Sum of dendritic neurode inputs that are adding to the total sum
        adders = 0 # Number of dendritic neurode inputs that are adding to the total sum
        mul = 0.0 # Sum of dendritic neurode inputs that are multiplying the total sum
        multipliers = 0 # Number of dendritic neurode inputs that are multiplying the total sum
        synIn = 0.0 # Value holder of a synaptic input

        # Decay the current sum and then add the current oscillator output and backpropagated spike
        self.sum *= self.decay
        self.sum += self.oscillator.update(time) + self.backpropagated

        # Update the refractory period value
        self.refractory = max(0, self.refractory - 0.1)

        # Process inputs from all previous neurodes
        for i in range(len(self.previous)):
            self.previous[i].backpropagate(self.spike) # Send this neurode's latest spike to all previous neurodes

            # Add or multiply neurode inputs based on their specified input function
            if self.prevFunc[i] == Neurode.ADD_FUNCTION:
                add += self.previous[i].spike * self.prevWeight[i]
                adders += 1
            else:
                mul += self.previous[i].spike * self.prevWeight[i]
                multipliers += 1

        # Add adder's sum to the total sum
        if adders > 0:
            self.sum += add

        # Mutliply the total sum by the average of the mutliplier's inputs
        if multipliers > 0:
            self.sum *= mul / multipliers

        # Process all synaptic inputs
        for i in range(len(self.synInput)):
            synIn = self.synInput[i].update(time, self.spike + self.backpropagated)

            # If the synapse is still connected to this neurode, update the total sum
            if self.synInput[i].permanence > 0.0: 
                self.sum += synIn
            
        # Calculate the neurode's output based on its activation function
        self.output = self.activation.update(self.sum)
        # Calculate the number of spikes by calculating the floor of the output divided by the combined threshold and refractory values
        self.spike = self.output // (self.threshold + self.refractory)
        # Update refractory period value based on the spike value
        self.refractory += self.spike * 0.1

        # If the neurode spiked
        if self.spike > 0.0:
            # Update the spike count, spiking frequency and previous spike time and reset the total input sum
            self.spikeCount += self.spike
            self.frequency = time - self.prevSpikeTime
            self.prevSpikeTime = time
            self.sum = 0.0
        # If the neurode did not spike
        else:
            # Reset the spike count
            self.spikeCount = 0

        # Set the input of all synaptic outputs to the spike value combined with the backpropagated spike value
        for i in range(len(self.synOutput)):
            self.synOutput[i].setCurrentInput(self.spike + self.backpropagated)
            
        # If spikes were backpropagated
        if self.backpropagated > 0.0: 
            # Update the backpropagation spike frequency and previous backpropagation spike time
            self.bpFrequency = time - self.prevBpSpikeTime
            self.prevBpSpikeTime = time

        # Update previous sum to sum and reset backpropagated
        self.prevSum = self.sum
        self.backpropagated = 0.0

        # Return the neurode's current spike output value
        return self.spike

"""
SomaticNeurode: This class creates and processes the somatic neurode. The somatic neurode represents the soma of a neuron, which is the final input
processing subunit that calculates the neuron's final output.
"""
class SomaticNeurode(Neurode):
    """
    Initializes the somatic neurode based on the specified activation function, sum decay rate, oscillator function and spiking threshold.
    self: object reference
    activation: the activation function
    decay: the sum decay rate
    oscillator: the oscillator function
    threshold: the spiking threshold
    """
    def __init__(self, activation=None, decay=Neurode.DEF_DECAY, oscillator=None, threshold=Neurode.DEF_THRESHOLD):
        super().__init__(activation=SigmoidActivation(sensitivity=np.exp(np.pi)) if activation is None else activation, decay=decay, oscillator=oscillator, threshold=threshold)

    """
    This function processes the neurode based on the specified time.
    self: object reference
    time: Arti's cycle clock time
    """
    def update(self, time):
        add = 0.0 # Sum of dendritic neurode inputs that are adding to the total sum
        adders = 0 # Number of dendritic neurode inputs that are adding to the total sum
        mul = 0.0 # Sum of dendritic neurode inputs that are multiplying the total sum
        multipliers = 0 # Number of dendritic neurode inputs that are multiplying the total sum
        synIn = 0.0 # Value holder of a synaptic input

        # Decay the current sum and then add the current oscillator output and backpropagated spike
        self.sum *= self.decay
        self.sum += self.oscillator.update(time)

        # Update the refractory period value
        self.refractory = max(0, self.refractory - 0.1)

        # Process inputs from all previous neurodes
        for i in range(len(self.previous)):
            self.previous[i].backpropagate(self.spike) # Send this neurode's latest spike to all previous neurodes

            # Add or multiply neurode inputs based on their specified input function
            if self.prevFunc[i] == Neurode.ADD_FUNCTION:
                add += self.previous[i].spike * self.prevWeight[i]
                adders += 1
            else:
                mul += self.previous[i].spike * self.prevWeight[i]
                multipliers += 1

        # Add adder's sum to the total sum
        if adders > 0:
            self.sum += add

        # Mutliply the total sum by the average of the mutliplier's inputs
        if multipliers > 0:
            self.sum *= mul / multipliers

        # Process all synaptic inputs
        for i in range(len(self.synInput)):
            synIn = self.synInput[i].update(time, self.spike + self.backpropagated)

            # If the synapse is still connected to this neurode, update the total sum
            if self.synInput[i].permanence > 0.0: 
                self.sum += synIn
            
        # Calculate the neurode's output based on its activation function
        self.output = self.activation.update(self.sum)
        # Calculate the number of spikes by calculating the floor of the output divided by the combined threshold and refractory values
        self.spike = self.output // (self.threshold + self.refractory)
        # Update refractory period value based on the spike value
        self.refractory += self.spike * 0.1

        # If the neurode spiked
        if self.spike > 0.0:
            # Update the spike count, spiking frequency and previous spike time and reset the total input sum
            self.spikeCount += self.spike
            self.frequency = time - self.prevSpikeTime
            self.prevSpikeTime = time
            self.sum = 0.0
        # If the neurode did not spike
        else:
            # Reset the spike count
            self.spikeCount = 0

        # Set the input of all synaptic outputs to the spike value
        for i in range(len(self.synOutput)):
            self.synOutput[i].setCurrentInput(self.spike)

        # Update previous sum to sum and reset backpropagated
        self.prevSum = self.sum
        self.backpropagated = 0.0

        # Return the neurode's current spike output value
        return self.spike

"""
AxonalNeurode: This class creates and processes the axonal neurode. An axonal neurode represents an axon compartment of a neuron. Axons are repsonsible for
relaying the neuron's output to other neurons, but they can have synapses that alter the output as well as membrane oscillations that can alter it as well.
"""
class AxonalNeurode(Neurode):
    """
    Initializes the axonal neurode based on the specified activation function, sum decay rate, oscillator function and spiking threshold.
    self: object reference
    activation: the activation function
    decay: the sum decay rate
    oscillator: the oscillator function
    threshold: the spiking threshold
    """
    def __init__(self, activation=None, decay=Neurode.DEF_DECAY, oscillator=None, threshold=Neurode.DEF_THRESHOLD):
        super().__init__(activation=SigmoidActivation(preference=0.9, sensitivity=np.exp(np.pi)) if activation is None else activation, decay=decay, oscillator=oscillator, 
                         threshold=threshold)

    """
    Adds the specified neurode that is connected to send input to this neurode. Axonal neurodes can only be connected to one previous
    neurode.
    self: object reference
    neurode: the neurode to connect to this neurode
    func: the input function (add or multiply)
    weight: the weight value
    returns: true if the specified neurode is successfully connected, otherwise false
    """
    def addPrevious(self, neurode, func=None, weight=None):
        if neurode is not None and len(self.previous) < 1:
            self.previous.append(neurode)
            self.prevFunc.append(func if func is not None else Neurode.ADD_FUNCTION)
            self.prevWeight.append(weight if weight is not None else Neurode.DEF_WEIGHT)
            return True

        return False

    """
    This function processes the neurode based on the specified time.
    self: object reference
    time: Arti's cycle clock time
    """
    def update(self, time):
        add = 0.0 # Sum of dendritic neurode inputs that are adding to the total sum
        adders = 0 # Number of dendritic neurode inputs that are adding to the total sum
        mul = 0.0 # Sum of dendritic neurode inputs that are multiplying the total sum
        multipliers = 0 # Number of dendritic neurode inputs that are multiplying the total sum
        synIn = 0.0 # Value holder of a synaptic input

        # Decay the current sum and then add the current oscillator output and backpropagated spike
        self.sum *= self.decay
        self.sum += self.oscillator.update(time)

        # Update the refractory period value
        self.refractory = max(0, self.refractory - 0.1)

        # Process inputs from all previous neurodes
        for i in range(len(self.previous)):
            self.previous[i].backpropagate(self.spike) # Send this neurode's latest spike to all previous neurodes

            # Add or multiply neurode inputs based on their specified input function
            if self.prevFunc[i] == Neurode.ADD_FUNCTION:
                add += self.previous[i].spike * self.prevWeight[i]
                adders += 1
            else:
                mul += self.previous[i].spike * self.prevWeight[i]
                multipliers += 1

        # Add adder's sum to the total sum
        if adders > 0:
            self.sum += add

        # Mutliply the total sum by the average of the mutliplier's inputs
        if multipliers > 0:
            self.sum *= mul / multipliers

        # Process all synaptic inputs
        for i in range(len(self.synInput)):
            synIn = self.synInput[i].update(time, self.spike + self.backpropagated)

            # If the synapse is still connected to this neurode, update the total sum
            if self.synInput[i].permanence > 0.0: 
                self.sum += synIn
            
        # Calculate the neurode's output based on its activation function
        self.output = self.activation.update(self.sum)
        # Calculate the number of spikes by calculating the floor of the output divided by the combined threshold and refractory values
        self.spike = self.output // (self.threshold + self.refractory)
        # Update refractory period value based on the spike value
        self.refractory += self.spike * 0.1

        # If the neurode spiked
        if self.spike > 0.0:
            # Update the spike count, spiking frequency and previous spike time and reset the total input sum
            self.spikeCount += self.spike
            self.frequency = time - self.prevSpikeTime
            self.prevSpikeTime = time
            self.sum = 0.0
        # If the neurode did not spike
        else:
            # Reset the spike count
            self.spikeCount = 0

        # Set the input of all synaptic outputs to the spike value
        for i in range(len(self.synOutput)):
            self.synOutput[i].setCurrentInput(self.spike)

        # Update previous sum to sum and reset backpropagated
        self.prevSum = self.sum
        self.backpropagated = 0.0

        # Return the neurode's current spike output value
        return self.spike

########################################################################################
# Activation classes
########################################################################################
"""
Activation: This class is the base class for activation function subclasses used in processing neurode activation functions.
"""
class Activation:
    DEF_PREFERENCE = 0.5 # the default preference value
    DEF_SENSITIVITY = 1.0 # the default sensitivity value

    """
    Initializes the activation function based on the specified preference and sensitivity values.
    self: object reference
    preference: the preference value (the value the produces maximal output)
    sensitivity: the sensitivity value (the value determining how wideness of the function's response to input)
    """
    def __init__(self, preference=DEF_PREFERENCE, sensitivity=DEF_SENSITIVITY):
        self.preference = preference
        self.sensitivity = sensitivity

    """
    This function sets the preference and sensitivity values to the specified values.
    self: object reference
    preference: the preference value (the value the produces maximal output)
    sensitivity: the sensitivity value (the value determining how wideness of the function's response to input)
    """
    def config(self, preference=None, sensitivity=None):
        if preference is not None: self.preference = preference
        if sensitivity is not None: self.sensitivity = sensitivity

    """
    Processes the activation function based on the specified input. This function must be implemented by all subclasses.
    self: object reference
    x: the input value
    returns: 0
    """
    def update(self, x):
        return 0.0

"""
GaussianActivation: This class processes the gaussian function.
"""
class GaussianActivation(Activation):
    """
    Initializes the activation function based on the specified preference and sensitivity values.
    self: object reference
    preference: the preference value (the value the produces maximal output)
    sensitivity: the sensitivity value (the value determining how wideness of the function's response to input)
    """
    def __init__(self, preference=Activation.DEF_PREFERENCE, sensitivity=Activation.DEF_SENSITIVITY):
        super().__init__(preference, sensitivity)

    """
    Processes the activation function based on the specified input.
    self: object reference
    x: the input value
    returns: the gaussian function of x
    """
    def update(self, x):
        return gaussian(x, 1, self.preference, self.sensitivity)

"""
HardSigmoidActivation: This class process the hard sigmoid function.
"""
class HardSigmoidActivation(Activation):
    """
    Initializes the activation function based on the specified preference and sensitivity values.
    self: object reference
    preference: the preference value (the value the produces maximal output)
    sensitivity: the sensitivity value (the value determining how wideness of the function's response to input)
    """
    def __init__(self, preference=Activation.DEF_PREFERENCE, sensitivity=Activation.DEF_SENSITIVITY):
        super().__init__(preference, sensitivity)

    """
    Processes the activation function based on the specified input.
    self: object reference
    x: the input value
    returns: the hard sigmoid function of x
    """
    def update(self, x):
        return hardsigmoid(x, 1, self.preference, self.sensitivity)

"""
SigmoidActivation: This class processes the sigmoid function
"""
class SigmoidActivation(Activation):
    """
    Initializes the activation function based on the specified preference and sensitivity values.
    self: object reference
    preference: the preference value (the value the produces maximal output)
    sensitivity: the sensitivity value (the value determining how wideness of the function's response to input)
    """
    def __init__(self, preference=Activation.DEF_PREFERENCE, sensitivity=Activation.DEF_SENSITIVITY):
        super().__init__(preference, sensitivity)

    """
    Processes the activation function based on the specified input.
    self: object reference
    x: the input value
    returns: the sigmoid function of x
    """
    def update(self, x):
        return sigmoid(x, 1, self.preference, self.sensitivity)

########################################################################################
# Oscillator classes
########################################################################################
"""
Oscillator: This is the base class for all Oscillator subclasses used in processing a neurode's oscillator. Oscillators are used to
provide intrinsic, resonating input to a neurode. This base class can be used for neurodes that do not have oscillations, since it
returns 0 as its oscillatory output.
"""
class Oscillator:
    """
    Initializes the oscillator function based on the amplitude, frequency and phase values.
    self: object reference
    amplitude: the maximal +/- height of the oscillation
    frequency: the number of oscillations per full cycle
    phase: the cycle offset value
    """
    def __init__(self, amplitude=0.0, frequency=0.0, phase=0.0):
        self.amplitude = amplitude
        self.frequency = frequency
        self.phase = phase

    """
    This function sets the amplitude, frequency and phase values to the specified values.
    self: object reference
    amplitude: the maximal +/- height of the oscillation
    frequency: the number of oscillations per full cycle
    phase: the cycle offset value
    """
    def config(self, amplitude=None, frequency=None, phase=None):
        if amplitude is not None: self.amplitude = amplitude
        if frequency is not None: self.frequency = frequency
        if phase is not None: self.phase = phase

    """
    This functions prints the amplitude, frequency and phase values of this oscillator.
    self: object reference
    """
    def print(self):
        print('Oscillator:')
        print('Amplitude = ', self.amplitude)
        print('Frequency = ', self.frequency)
        print('Phase = ', self.phase)

    """
    Updates the the output value of this oscillator based on the specified time. This function must be implemented by all
    subclasses.
    self: object reference
    time: Arti's cycle clock time
    returns: 0
    """
    def update(self, time):
        return 0.0

"""
CosineOscillator: This class processes the cosine function as the neurode's oscillatory function.
"""
class CosineOscillator(Oscillator):
    """
    Initializes the oscillator function based on the amplitude, frequency and phase values.
    self: object reference
    amplitude: the maximal +/- height of the oscillation
    frequency: the number of oscillations per full cycle
    phase: the cycle offset value
    """
    def __init__(self, amplitude=0.0, frequency=0.0, phase=0.0):
        super().__init__(amplitude, frequency, phase)

    """
    Updates the the output value of this oscillator based on the specified time.
    self: object reference
    time: Arti's cycle clock time
    returns: the cosine of time
    """
    def update(self, time):
        return cosine(time, self.amplitude, self.frequency, self.phase)

"""
ReverseSawtoothOscillator: This class processes the reverse sawtooth function as the neurode's oscillatory function.
"""
class ReverseSawtoothOscillator(Oscillator):
    """
    Initializes the oscillator function based on the amplitude, frequency and phase values.
    self: object reference
    amplitude: the maximal +/- height of the oscillation
    frequency: the number of oscillations per full cycle
    phase: the cycle offset value
    """
    def __init__(self, amplitude=0.0, frequency=0.0, phase=0.0):
        super().__init__(amplitude, frequency, phase)

    """
    Updates the the output value of this oscillator based on the specified time.
    self: object reference
    time: Arti's cycle clock time
    returns: the reverse sawtooth of time
    """
    def update(self, time):
        return sawtoothreverse(time, self.amplitude, self.frequency, self.phase)

"""
SawtoothOscillator: This class processes the sawtooth function as the neurode's oscillatory function.
"""
class SawtoothOscillator(Oscillator):
    """
    Initializes the oscillator function based on the amplitude, frequency and phase values.
    self: object reference
    amplitude: the maximal +/- height of the oscillation
    frequency: the number of oscillations per full cycle
    phase: the cycle offset value
    """
    def __init__(self, amplitude=0.0, frequency=0.0, phase=0.0):
        super().__init__(amplitude, frequency, phase)

    """
    Updates the the output value of this oscillator based on the specified time.
    self: object reference
    time: Arti's cycle clock time
    returns: the sawtooth of time
    """
    def update(self, time):
        return sawtooth(time, self.amplitude, self.frequency, self.phase)

"""
SineOscillator: This class processes the sine function as the neurode's oscillatory function.
"""
class SineOscillator(Oscillator):
    """
    Initializes the oscillator function based on the amplitude, frequency and phase values.
    self: object reference
    amplitude: the maximal +/- height of the oscillation
    frequency: the number of oscillations per full cycle
    phase: the cycle offset value
    """
    def __init__(self, amplitude=0.0, frequency=0.0, phase=0.0):
        super().__init__(amplitude, frequency, phase)

    """
    Updates the the output value of this oscillator based on the specified time.
    self: object reference
    time: Arti's cycle clock time
    returns: the sine of time
    """
    def update(self, time):
        return sine(time, self.amplitude, self.frequency, self.phase)

########################################################################################
# Synapse classes
########################################################################################
"""
NeuralInput: This class represents an input to a synapse. A neural input decays over time. A synaptic input is made up of a sum
of neural inputs. Every time a synapsing neuron spikes, a neural input is created to represent its input.
"""
class NeuralInput:
    DEF_BASE = 1.0 # the default base value of the input
    DEF_DECAY = 0.1 # the default input decay rate

    """
    Initializes a neural input based of the specified base input value, input decay rate, the start time and the sign of the input.
    self: object reference
    base: the base input value
    decay: the input decay rate
    start: the start time of the input
    sign: the sign of the input (+/-)
    """
    def __init__(self, base=DEF_BASE, decay=DEF_DECAY, start=0.0, sign=1):
        self.base = base
        self.decay = decay
        self.input = 0.0
        self.start = start
        self.sign = 1 if sign >= 0.0 else -1

    """
    Sets the base input value, input decay rate, start time and sign to the specified values.
    self: object reference
    base: the base input value
    decay: the input decay rate
    start: the start time of the input
    sign: the sign of the input (+/-)
    """
    def config(self, base=None, decay=None, start=None, sign=None):
        if base is not None: self.base = base
        if decay is not None: self.decay = decay
        if start is not None: self.start = start
        if sign is not None: self.sign = 1 if sign >= 0.0 else -1

    """
    Returns the currently calculated input value of this neural input.
    returns: the input value
    """
    def getInput(self):
        return self.input

    """
    Calculates the input value based on the specified time using the potential function.
    self: object reference
    time: Arti's clock cycle time
    returns: the input value
    """
    def update(self, time):
        self.input = potential(time, self.start, self.sign, self.decay, precision=1) * self.base
        return self.input

"""
Synapse: This class is the base class for all Synapse subclasses. It contains the data and functions used by all synapses.
"""
class Synapse:
    DEF_DECAY = NeuralInput.DEF_DECAY # the default synaptic input decay rate
    DEF_WEIGHT = 1.0 # the default weight value

    """
    Initializes the synapse based on the specified decay rate, weight value and learning function.
    self: object reference
    decay: the synaptic input decay rate
    weight: the weight value
    learnFunc: the synaptic learning function
    """
    def __init__(self, decay=DEF_DECAY, weight=DEF_WEIGHT, learnFunc=None):
        self.currentInput = 0.0
        self.decay = decay
        self.frequency = 0.0
        self.input = []
        self.learnFunc = learnFunc
        self.permanence = 1.0
        self.prevInput = 0.0
        self.prevInputTime = 0.0
        self.prevSpikeTime = 0.0
        self.sum = 0.0
        self.weight = weight

    """
    Sets the decay rate, weight value, learning function and sign of the synapse.
    self: object reference
    decay: the synaptic input decay rate
    weight: the weight value
    learnFunc: the synaptic learning function
    sign: the sign of the synapse (+/-) (used by some synapse types)
    """
    def config(self, decay=None, weight=None, learnFunc=None, sign=None):
        if decay is not None: 
            self.decay = decay

            for i in range(len(self.input)):
                self.input[i].config(decay=decay)

        if weight is not None: self.weight = weight

        if learnFunc is not None: self.learnFunc = learnFunc

    """
    Returns the neural input at the specified index.
    self: object reference
    index: the index of the neural input to return
    returns: the neural input at index
    """
    def getInput(self, index):
        if index >= 0 and index < len(self.input): return self.input[index]

    """
    Returns the list of neural inputs currently comprising the total synaptic input.
    self: object reference
    returns: list of neural inputs
    """
    def getInputs(self):
        return self.input

    """
    Returns the current number of neural inputs comprising the total synaptic input.
    self: object reference
    returns: the size of the list of neural inputs
    """
    def inputSize(self):
        return len(self.input)

    """
    Sets the current input of the synapse. The current input is the spike value of the synapsing neurode.
    self: object reference
    x: the input value
    """
    def setCurrentInput(self, x):
        self.currentInput = x

    """
    Returns the current sum of the neural inputs, which is the total synaptic input.
    self: object reference
    returns: the synaptic input sum
    """
    def sum(self):
        return self.sum

    """
    Updates the synaptic input based on the specified time and backpropagated spike value. This function must be implemented by
    all subclasses.
    self: object reference
    time: Arti's cycle clock time
    spike: the backpropagated spike value
    returns: 0
    """
    def update(self, time, spike):
        return 0.0

"""
SensorySynapse: This class represents a sensory synapse used by sensory neurons to receive external input.
"""
class SensorySynapse(Synapse):
    DEF_SENSORY_DECAY = 0.01 # the default sensory input decay rate

    """
    Initializes the sensory synapse based on the input decay rate, weight value and sign of the sensory synapse.
    self: object reference
    decay: the input decay rate
    weight: the weight value
    sign: the sign of the sensory synapse (+/-)
    """
    def __init__(self, decay=DEF_SENSORY_DECAY, weight=Synapse.DEF_WEIGHT, sign=1):
        super().__init__(decay, weight)
        self.sign = 1 if sign >= 0 else -1

    """
    Sets the input decay rate, weight value, learning function, and sign of the sensory synapse.
    self: object reference
    decay: the input decay rate
    weight: the weight value
    learnFunc: the learning function
    sign: the sign of the sensory synapse (+/-)
    """
    def config(self, decay=None, weight=None, learnFunc=None, sign=None):
        if decay is not None: 
            self.decay = decay

            for i in range(len(self.input)):
                self.input[i].config(decay=decay)

        if weight is not None: self.weight = weight

        self.learnFunc = learnFunc

        if sign is not None: self.sign = 1 if sign >= 0 else -1

    """
    Updates the synaptic input based on the specified time and backpropagated spike value.
    self: object reference
    time: Arti's cycle clock time
    spike: the backpropagated spike value
    returns: the total synaptic input
    """
    def update(self, time, spike):
        self.input.append(NeuralInput(self.currentInput, self.decay, time, self.sign))
        self.currentInput = 0.0

        self.sum = 0.0

        for i in reversed(range(len(self.input))):
            self.sum += self.input[i].update(time)
            if self.input[i].getInput() < MIN_VALUE: 
                self.input.pop(i)

        return self.sum * self.weight

"""
NeuralSynapse: This class is the base class for all NeuralSynapse subclasses. Neural synapses are synapses that process inputs from other neurodes.
"""
class NeuralSynapse(Synapse):
    """
    Initializes the neural synapse based on the input decay rate, weight value, learning function, synaptic filter, and spike frequency decay rate.
    self: object reference
    decay: the input decay rate
    weight: the weight value
    learnFunc: the learning function
    inputDecay: the spike frequency decay rate
    """
    def __init__(self, decay=Synapse.DEF_DECAY, weight=Synapse.DEF_WEIGHT, learnFunc=None, filter=None, inputDecay=Synapse.DEF_DECAY):
        super().__init__(decay, weight, learnFunc)
        self.filter = SynapticFilter() if filter is None else filter
        self.inputDecay = inputDecay

    """
    Sets the input decay rate, weight value, learning function, synaptic filter, and spike frequency decay rate to the specified values.
    self: object reference
    decay: the input decay rate
    weight: the weight value
    learnFunc: the learning function
    inputDecay: the spike frequency decay rate
    """
    def config(self, decay=None, weight=None, learnFunc=None, filter=None, inputDecay=None):
        if decay is not None:
            self.decay = decay

            for i in range(len(self.input)):
                self.input[i].config(decay=decay)

        if weight is not None: self.weight = weight
        self.learnFunc = learnFunc
        if filter is not None: self.filter = filter
        if inputDecay is not None: self.inputDecay = inputDecay

"""
ExcitatorySynapse: This class represents an excitatory neural synapse, which is a synapse that provides positive synaptic input.
"""
class ExcitatorySynapse(NeuralSynapse):
    """
    Initializes the excitatory synapse based on the input decay rate, weight value, learning function, synaptic filter, and spike frequency decay rate.
    self: object reference
    decay: the input decay rate
    weight: the weight value
    learnFunc: the learning function
    inputDecay: the spike frequency decay rate
    """
    def __init__(self, decay=Synapse.DEF_DECAY, weight=Synapse.DEF_WEIGHT, learnFunc=None, filter=None, inputDecay=Synapse.DEF_DECAY):
        super().__init__(decay, weight, learnFunc, filter, inputDecay)

    """
    Updates the synaptic input based on the specified time and backpropagated spike value.
    self: object reference
    time: Arti's cycle clock time
    spike: the backpropagated spike value
    returns: the total synaptic input
    """
    def update(self, time, spike):
        filtered = 0 # Value holder for the filtered input value

        # Decay the spike frequency value
        self.frequency *= self.inputDecay

        # If an input is received
        if self.currentInput > 0.0:
            self.frequency += self.currentInput # Add the input to the spike frequency
            self.prevInputTime = time # Update the previous input time to now
            filtered = self.currentInput * self.filter.filter(self.frequency) # filter the current input through the synaptic filter
            self.input.append(NeuralInput(filtered, self.decay, time)) # add the filtered input as a new neural input
            
        self.currentInput = 0.0 # Reset the current input

        # If backpropagated spikes are received
        if spike > 0.0:
            self.prevSpikeTime = time # Update the previous spike time to now

        self.sum = 0.0 # Reset the total synaptic input sum

        # Add all neural inputs to the total synaptic input sum and remove any neural inputs that are less than MIN_VALUE
        for i in reversed(range(len(self.input))):
            self.sum += self.input[i].update(time)
            if self.input[i].getInput() < MIN_VALUE: 
                self.input.pop(i)
        
        # If the synapse learns, update its permanence value with its STDP learning function
        if self.learnFunc is not None:
            self.permanence = max(0.0, self.permanence + ((self.learnFunc.learn(self.prevSpikeTime - self.prevInputTime)) * spike))

        self.prevInput = filtered # Update previous input to the filtered input value

        # Return the total synaptic input sum times its weight
        return self.sum * self.weight

"""
InhibitorySynapse: This class represents an inhibitory neural synapse, which is a synapse that provides negative synaptic input.
"""
class InhibitorySynapse(NeuralSynapse):
    """
    Initializes the inhibitory synapse based on the input decay rate, weight value, learning function, synaptic filter, and spike frequency decay rate.
    self: object reference
    decay: the input decay rate
    weight: the weight value
    learnFunc: the learning function
    inputDecay: the spike frequency decay rate
    """
    def __init__(self, decay=Synapse.DEF_DECAY, weight=Synapse.DEF_WEIGHT, learnFunc=None, filter=None, inputDecay=Synapse.DEF_DECAY):
        super().__init__(decay, weight, learnFunc, filter, inputDecay)

    """
    Updates the synaptic input based on the specified time and backpropagated spike value.
    self: object reference
    time: Arti's cycle clock time
    spike: the backpropagated spike value
    returns: the total synaptic input
    """
    def update(self, time, spike):
        filtered = 0 # Value holder for the filtered input value

        # Decay the spike frequency value
        self.frequency *= self.inputDecay

        # If an input is received
        if self.currentInput > 0.0:
            self.frequency += self.currentInput # Add the input to the spike frequency
            self.prevInputTime = time # Update the previous input time to now
            filtered = self.currentInput * self.filter.filter(self.frequency) # filter the current input through the synaptic filter
            self.input.append(NeuralInput(filtered, self.decay, time, -1)) # add the filtered input as a new negative neural input
            
        self.currentInput = 0.0 # Reset the current input

        # If backpropagated spikes are received
        if spike > 0.0:
            self.prevSpikeTime = time # Update the previous spike time to now

        self.sum = 0.0 # Reset the total synaptic input sum

        # Add all neural inputs to the total synaptic input sum and remove any neural inputs that are less than MIN_VALUE
        for i in reversed(range(len(self.input))):
            self.sum += self.input[i].update(time)
            if self.input[i].getInput() < MIN_VALUE: 
                self.input.pop(i)
        
        # If the synapse learns, update its permanence value with its STDP learning function
        if self.learnFunc is not None:
            self.permanence = max(0.0, self.permanence + ((self.learnFunc.learn(self.prevSpikeTime - self.prevInputTime)) * spike))

        self.prevInput = filtered # Update previous input to the filtered input value

        # Return the total synaptic input sum times its weight
        return self.sum * self.weight

########################################################################################
# SynapticFilter classes
########################################################################################
"""
SynapticFilter: This class is the base class for all SynapticFilter subclasses. A synaptic filter is used by neural synapses to filter
the spiking frequency of the synapsing neurode. This base class can be used for synapses that have no filter, since it returns 1 as its
output.
"""
class SynapticFilter:
    DEF_PREFERENCE = 0.5 # the default filter preference value
    DEF_SENSITIVITY = 1.0 # the default filter sensitivity value

    """
    Initializes the synaptic filter based on the specified preference and sensitivity values.
    self: object reference
    preference: the preference value (the input value that causes the maximal output)
    sensitivity: the sensitivity value (the wideness of the input response of the synaptic filter function)
    """
    def __init__(self, preference=None, sensitivity=None):
        self.preference = SynapticFilter.DEF_PREFERENCE if preference is None else preference
        self.sensitivity = SynapticFilter.DEF_SENSITIVITY if sensitivity is None else sensitivity

    """
    Sets the preference and sensitivity values to the specified values.
    self: object reference
    preference: the preference value (the input value that causes the maximal output)
    sensitivity: the sensitivity value (the wideness of the input response of the synaptic filter function)
    """
    def config(self, preference=None, sensitivity=None):
        if preference is not None: self.preference = preference
        if sensitivity is not None: self.sensitivity = sensitivity

    """
    Filters the specified spike frequency value. This function must be implemented by all subclasses.
    self: object reference
    frequency: the spike frequency input value
    returns: 1 (can be used for synapses without filters)
    """
    def filter(self, frequency):
        return 1.0

"""
BandpassFilter: This class represents a bandpass synaptic filter, which is a filter than responds maximally to spike frequencies close to
the preference value.
"""
class BandpassFilter(SynapticFilter):
    """
    Initializes the synaptic filter based on the specified preference and sensitivity values.
    self: object reference
    preference: the preference value (the input value that causes the maximal output)
    sensitivity: the sensitivity value (the wideness of the input response of the synaptic filter function)
    """
    def __init__(self, preference=None, sensitivity=None):
        super().__init__(preference, sensitivity)

    """
    Filters the specified spike frequency value using the gaussian function.
    self: object reference
    frequency: the spike frequency input value
    returns: 1 (can be used for synapses without filters)
    """
    def filter(self, frequency):
        return gaussian(x=frequency, center=self.preference, width=self.sensitivity)

"""
HighpassFilter: This class represents a highpass synaptic filter, which is a filter than responds maximally to spike frequencies above
the preference value.
"""
class HighpassFilter(SynapticFilter):
    """
    Initializes the synaptic filter based on the specified preference and sensitivity values.
    self: object reference
    preference: the preference value (the input value that causes the maximal output)
    sensitivity: the sensitivity value (the wideness of the input response of the synaptic filter function)
    """
    def __init__(self, preference=None, sensitivity=None):
        super().__init__(preference, sensitivity)

    """
    Filters the specified spike frequency value using the sigmoid function.
    self: object reference
    frequency: the spike frequency input value
    returns: 1 (can be used for synapses without filters)
    """
    def filter(self, frequency):
        return sigmoid(x=frequency, center=self.preference, sharpness=self.sensitivity)

"""
LowpassFilter: This class represents a lowpass synaptic filter, which is a filter than responds maximally to spike frequencies under
the preference value.
"""
class LowpassFilter(SynapticFilter):
    """
    Initializes the synaptic filter based on the specified preference and sensitivity values.
    self: object reference
    preference: the preference value (the input value that causes the maximal output)
    sensitivity: the sensitivity value (the wideness of the input response of the synaptic filter function)
    """
    def __init__(self, preference=None, sensitivity=None):
        super().__init__(preference, sensitivity)
        
    """
    Filters the specified spike frequency value using the sigmoid function of the negative spike frequency input.
    self: object reference
    frequency: the spike frequency input value
    returns: 1 (can be used for synapses without filters)
    """
    def filter(self, frequency):
        return sigmoid(x=-frequency, center=self.preference, sharpness=self.sensitivity)

########################################################################################
# LearningFunction class
########################################################################################
"""
LearningFunction: This class represents the learning function of a synapse or neurode. It uses the spike-timing-dependent-plasticity learning
function, which is based directly off the STDP rules of real neurons.
"""
class LearningFunction:
    """
    Initializes the learning function based on pre-peak, pre-max, pre-window decay rate, post-peak, post-max and post-window decay rate values.
    self: object reference
    prePeak: the x value that causes the maximal +/- y value when x is negative
    preMax: the maximal +/- y value when x is negative
    preWndDecay: the decay rate of the y value when x is negative
    postPeak: the x value that causes the maximal +/- y value when x is positive
    postMax: the maximal +/- y value when x is positive
    postWndDecay: the decay rate of the y value when x is positive
    """
    def __init__(self, prePeak=-1, preMax=1, preWndDecay=1, postPeak=1, postMax=-1, postWndDecay=1):
        self.prePeak = prePeak
        self.preMax = preMax
        self.preWndDecay = preWndDecay
        self.postPeak = postPeak
        self.postMax = postMax
        self.postWndDecay = postWndDecay
        self.midPoints = []

    """
    Adds a mid-point to the list of mid-points used in the processing of the learning function. The mid-points are the points used to connect
    the preMax value to the postMax value.
    self: object reference
    x: the x coordinate
    y: the y coordinate
    returns: true if the midpoint was added successfully, otherwise false
    """
    def addMidPoint(self, x, y):
        if x is not None and y is not None:
            self.midPoints.append((x, y))
            return True

        return False

    """
    Sets the pre-peak, pre-max, pre-window decay rate, post-peak, post-max and post-window decay rate values to the specified values.
    self: object reference
    prePeak: the x value that causes the maximal +/- y value when x is negative
    preMax: the maximal +/- y value when x is negative
    preWndDecay: the decay rate of the y value when x is negative
    postPeak: the x value that causes the maximal +/- y value when x is positive
    postMax: the maximal +/- y value when x is positive
    postWndDecay: the decay rate of the y value when x is positive
    """
    def config(self, prePeak=None, preMax=None, preWndDecay=None, postPeak=None, postMax=None, postWndDecay=None):
        if prePeak is not None: self.prePeak = prePeak
        if preMax is not None: self.preMax = preMax
        if preWndDecay is not None: self.preWndDecay = preWndDecay
        if postPeak is not None: self.postPeak = postPeak
        if postMax is not None: self.postMax = postMax
        if postWndDecay is not None: self.postWndDecay = postWndDecay

    """
    Processes the learning function based on the specified input value.
    self: object reference
    x: the input value
    returns: the STDP function of x
    """
    def learn(self, x):
        return stdp(x, self.prePeak, self.preMax, self.preWndDecay, self.postPeak, self.postMax, self.postWndDecay, self.midPoints)

    """
    Removes the mid-point at the specified index.
    self: object reference
    index: the index of the mid-point to remove
    returns: true if the mid-point at index was successfully removed, otherwise false
    """
    def removeMidPoint(self, index):
        if index is not None and index >= 0 and index < len(self.midPoints):
            self.midPoints.pop(index)
            return True

        return False