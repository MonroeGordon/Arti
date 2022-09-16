from numpy import maximum
from numpy.lib.twodim_base import vander
from neuralmath import *
from threading import *
import time
import warnings

warnings.filterwarnings('error')

MIN_VALUE = 0.001

class Arti_1_0(Thread):
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
        self.eNeuron = []
        self.mNeuron = []
        self.tNeuron = SensoryNeuron(soma=SomaticNeurode(activation=SigmoidActivation(1, 10)))
        self.neuron = Neuron(soma=SomaticNeurode(activation=HardSigmoidActivation(100.5, 2), oscillator=ReverseSawtoothOscillator(0.8, 0.5, -0.05), threshold=1 / 9))
        self.output = 0

        self.tNeuron.soma.synInput[0].config(sign=-1)
        self.tNeuron.soma.addSynapticInput(SensorySynapse(sign=1))

        for i in range(9):
            self.xNeuron.append(SensoryNeuron(soma=SomaticNeurode(activation=SigmoidActivation(2.3, 10), oscillator=SawtoothOscillator(1, 0.5, i / 4.5))))
            self.oNeuron.append(SensoryNeuron(soma=SomaticNeurode(activation=SigmoidActivation(2.3, 10), oscillator=SawtoothOscillator(1, 0.5, i / 4.5))))
            self.mNeuron.append(SensoryNeuron(soma=SomaticNeurode(activation=SigmoidActivation(2.63, 10), oscillator=CosineOscillator(1, 0.9, 0))))

        self.neuron.addDendrite(DendriticNeurode(activation=GaussianActivation(0.22, 0.05), oscillator=CosineOscillator(0.1, 0.9, 0)))
        self.neuron.connectNeurodes(self.neuron.dendrite[0], self.neuron.soma, weight=100)
        self.neuron.createSynapse(self.tNeuron.soma, self.neuron.dendrite[0], ExcitatorySynapse(decay=0.1, filter=HighpassFilter()))

        for i in range(9):
            self.neuron.addDendrite(DendriticNeurode(activation=GaussianActivation(0.014, 0.01)))
            self.neuron.connectNeurodes(self.neuron.dendrite[i + 1], self.neuron.dendrite[0], weight=100)
            self.neuron.createSynapse(self.mNeuron[i].soma, self.neuron.dendrite[i + 1], ExcitatorySynapse(filter=BandpassFilter(1, 10), inputDecay=0.01))

            for j in range(9):
                if j != i:
                    self.neuron.createSynapse(self.mNeuron[j].soma, self.neuron.dendrite[i + 1], InhibitorySynapse())

            self.neuron.addAxon(AxonalNeurode(activation=HardSigmoidActivation(4.5, 2 / 9), threshold=1 / 9))
            self.neuron.connectNeurodes(self.neuron.soma, self.neuron.axon[i])
            self.neuron.axon[i].addSynapticOutput(ExcitatorySynapse(filter=BandpassFilter(i + 1, 1)))

        for i in range(9):
            for j in range(9):
                self.neuron.addDendrite(DendriticNeurode(activation=GaussianActivation(1.15, 1 / 9), decay=0.1, oscillator=SawtoothOscillator(1, 0.5, j / 4.5)))
                self.neuron.connectNeurodes(self.neuron.dendrite[1 + j + ((i + 1) * 9)], self.neuron.dendrite[i + 1], weight=10)
                self.neuron.createSynapse(self.xNeuron[j].soma, self.neuron.dendrite[1 + j + ((i + 1) * 9)], InhibitorySynapse(weight=100))
                self.neuron.createSynapse(self.oNeuron[j].soma, self.neuron.dendrite[1 + j + ((i + 1) * 9)], InhibitorySynapse(weight=100))

        self.lock = Lock()
        self.stopped = True

        self.listeners = []

    def addListener(self, listener):
        if listener is not None:
            self.listeners.append(listener)
            return True

        return False

    def removeListener(self, listener):
        if self.listeners.__contains__(listener):
            self.listeners.remove(listener)
            return True

        return False

    def run(self):
        self.update()

    def setInput(self, turn, moveNum, board):
        with self.lock:
            self.turn = turn
            self.moveNum = moveNum
            self.board = board

    def setPlayer(self, p):
        with self.lock:
            self.player = 'X' if p == 'X' else 'O'

    def stop(self):
        with self.lock:
            self.stopped = True

    def update(self):
        with self.lock:
            self.stopped = False

        while self.stopped == False:
            if self.turn == self.player:
                self.tNeuron.soma.synInput[0].currentInput = 1
            else:
                self.tNeuron.soma.synInput[1].currentInput = 1

            self.tNeuron.update(self.time)

            for i in range(9):
                if i == self.moveNum - 1:
                    self.mNeuron[i].soma.synInput[0].currentInput = 1

                if self.board[i] == 'X':
                    self.xNeuron[i].soma.synInput[0].currentInput = 1
                elif self.board[i] == 'O':
                    self.oNeuron[i].soma.synInput[0].currentInput = 1

                self.mNeuron[i].update(self.time)
                self.xNeuron[i].update(self.time)
                self.oNeuron[i].update(self.time)

            self.neuron.update(self.time)

            for i in range(9):
                self.neuron.axon[i].synOutput[0].update(self.time, 0)

            self.output = 0
            greatest = 0.0

            for i in range(9):
                if self.neuron.axon[i].synOutput[0].sum > greatest:
                    greatest = self.neuron.axon[i].synOutput[0].sum
                    if greatest > 0.5:
                        self.output = i + 1

            self.time += 0.01

            if self.turn == self.player and self.output != 0:
                print("Arti picked square ", self.output)

                for i in range(len(self.listeners)):
                    self.listeners[i].messageFromArti(ArtiMessage(self.output))
                    

            else:
                np.set_printoptions(formatter={'float_kind': '{0:.5f}'.format})
                print(self.neuron.soma.spike)

class ArtiMessage:
    def __init__(self, output):
        self.output = output

class ArtiListener:
    def __init__(self):
        pass

    def messageFromArti(self, message):
        pass

########################################################################################
# Neuron classes
class Neuron:
    def __init__(self, soma=None):
        self.dendrite = []
        self.soma = SomaticNeurode() if soma is None else soma
        self.axon = []

    def addAxon(self, a):
        if a is not None: self.axon.append(a)

    def addDendrite(self, d):
        if d is not None: self.dendrite.append(d)

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

    def createSynapse(self, preNeurode, postNeurode, synapse=None):
        s = ExcitatorySynapse() if synapse is None else synapse
        preNeurode.addSynapticOutput(s)
        postNeurode.addSynapticInput(s)

    def update(self, time):
        mostAccountability = 0.0
        occurrenceSum = 0.0

        for i in range(len(self.dendrite)):
            self.dendrite[i].update(time)
            if self.dendrite[i].accountability > mostAccountability: mostAccountability = self.dendrite[i].accountability
            occurrenceSum += self.dendrite[i].occurrence

        self.soma.update(time)

        for i in range(len(self.axon)):
            self.axon[i].update(time)

        for i in range(len(self.dendrite)):
            self.dendrite[i].scale(mostAccountability, occurrenceSum)

class SensoryNeuron(Neuron):
    def __init__(self, soma=None, nInputs=1):
        super().__init__(soma)

        for i in range(nInputs):
            self.soma.addSynapticInput(SensorySynapse())

########################################################################################
# Neurode classes
class Neurode:
    DEF_DECAY = 0.1
    DEF_WEIGHT = 1.0
    ADD_FUNCTION = 'add'
    MULTIPLY_FUNCTION = 'multiply'
    DEF_THRESHOLD = 0.9

    def __init__(self, activation=None, decay=DEF_DECAY, learnFunc=None, oscillator=None, threshold=DEF_THRESHOLD):
        self.accountability = 1.0
        self.activation = SigmoidActivation() if activation is None else activation
        self.backpropagated = 0.0
        self.bpFrequency = 0.0
        self.decay = decay
        self.frequency = 0.0
        self.learnFunc = LearningFunction() if learnFunc is None else learnFunc
        self.next = []
        self.occurrence = 1.0
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

    def addNext(self, neurode):
        if neurode is not None: 
            self.next.append(neurode)
            return True

        return False

    def addSynapticInput(self, synapse):
        if synapse is not None: 
            self.synInput.append(synapse)
            return True

        return False

    def addSynapticOutput(self, synapse):
        if synapse is not None: 
            self.synOutput.append(synapse)
            return True

        return False

    def addPrevious(self, neurode, func=None, weight=None):
        if neurode is not None:
            self.previous.append(neurode)
            self.prevFunc.append(func if func is not None else Neurode.ADD_FUNCTION)
            self.prevWeight.append(weight if weight is not None else Neurode.DEF_WEIGHT)
            return True

        return False

    def backpropagate(self, spike):
        bp = spike * self.accountability * self.occurrence
        if bp > self.backpropagated: self.backpropagated = bp

    def config(self, activation=None, decay=None, learnFunc=None, oscillator=None, threshold=None):
        if activation is not None: self.activation = activation
        if decay is not None: self.decay = decay
        if learnFunc is not None: self.learnFunc = learnFunc
        if oscillator is not None: self.oscillator = oscillator
        if threshold is not None: self.threshold = threshold

    def nextSize(self):
        return len(self.next)

    def previousSize(self):
        return len(self.previous)

    def removeNext(self, neurode):
        if self.next.__contains__(neurode): 
            self.next.remove(neurode)
            return True

        return False

    def removePrevious(self, neurode):
        if self.previous.__contains__(neurode):
            index = self.previous.index(neurode)
            self.previous.remove(neurode)
            self.prevFunc.pop(index)
            self.prevWeight.pop(index)
            return True

        return False

    def scale(self, mostAccountability, occurrenceSum):
        if self.accountability >= MIN_VALUE:
            self.accountability /= mostAccountability if mostAccountability >= 1.0 else 1.0

        if self.occurrence >= MIN_VALUE:
            self.occurrence /= occurrenceSum if occurrenceSum >= 1.0 else 1.0

    def setPrevious(self, index, neurode=None, func=None, weight=None):
        if index >= 0 and index < len(self.previous):
            if neurode is not None:
                self.previous[index] = neurode

            if func is not None:
                self.prevFunc[index] = func

            if weight is not None:
                self.prevWeight[index] = weight

    def update(self, time):
        return 0.0

class DendriticNeurode(Neurode):
    def __init__(self, activation=None, decay=Neurode.DEF_DECAY, learnFunc=None, oscillator=None, threshold=Neurode.DEF_THRESHOLD):
        super().__init__(GaussianActivation() if activation is None else activation, decay, learnFunc, oscillator, threshold)

    def addNext(self, neurode):
        if neurode is not None and len(self.next) < 1:
            self.next.append(neurode)
            return True

        return False

    def update(self, time):
        mostAccountability = 0.0
        occurrenceSum = 0.0
        add = 0.0
        adders = 0
        mul = 0.0
        multipliers = 0

        self.sum *= self.decay
        self.sum += self.oscillator.update(time) + self.backpropagated

        self.refractory = max(0, self.refractory - 0.1)

        for i in range(len(self.previous)):
            self.previous[i].backpropagate(self.spike)

            if self.prevFunc[i] == Neurode.ADD_FUNCTION:
                add += self.previous[i].spike * self.prevWeight[i] * self.previous[i].accountability * self.previous[i].occurrence
                adders += 1
            else:
                mul += self.previous[i].spike * self.prevWeight[i] * self.previous[i].accountability * self.previous[i].occurrence
                multipliers += 1

        if adders > 0:
            self.sum += add

        if multipliers > 0:
            self.sum *= mul / multipliers

        for i in range(len(self.synInput)):
            self.sum += self.synInput[i].update(time, self.spike + self.backpropagated)
            if self.synInput[i].accountability > mostAccountability: mostAccountability = self.synInput[i].accountability
            occurrenceSum += self.synInput[i].occurrence
            
        for i in range(len(self.synInput)):
            self.synInput[i].scale(mostAccountability, occurrenceSum)
            
        self.output = self.activation.update(self.sum)
        self.spike = self.output // (self.threshold + self.refractory)
        self.refractory += self.spike * 0.1

        if self.spike > 0.0:
            self.spikeCount += self.spike
        else:
            self.spikeCount = 0

        for i in range(len(self.synOutput)):
            self.synOutput[i].setCurrentInput(self.spike + self.backpropagated)
            
        if self.backpropagated > 0.0: 
            self.bpFrequency = time - self.prevBpSpikeTime
            self.prevBpSpikeTime = time

        if self.spike > 0.0:
            self.frequency = time - self.prevSpikeTime
            self.prevSpikeTime = time

        self.accountability = max(MIN_VALUE, self.accountability + 
                                  self.learnFunc.learn(max(-1, min(1, self.prevBpSpikeTime - self.prevSpikeTime))) * self.prevSum * self.backpropagated)
        self.occurrence = max(MIN_VALUE, self.occurrence + self.learnFunc.learn(max(-1, min(1, self.prevBpSpikeTime - self.prevSpikeTime))) * self.backpropagated)

        self.prevSum = self.sum
        self.backpropagated = 0.0

        return self.spike

class SomaticNeurode(Neurode):
    def __init__(self, activation=None, decay=Neurode.DEF_DECAY, oscillator=None, threshold=Neurode.DEF_THRESHOLD):
        super().__init__(activation=SigmoidActivation(sensitivity=np.exp(np.pi)) if activation is None else activation, decay=decay, oscillator=oscillator, threshold=threshold)

    def update(self, time):
        mostAccountability = 0.0
        occurrenceSum = 0.0
        add = 0.0
        adders = 0
        mul = 0.0
        multipliers = 0

        self.sum *= self.decay
        self.sum += self.oscillator.update(time) + self.backpropagated

        self.refractory = max(0, self.refractory - 0.1)

        for i in range(len(self.previous)):
            self.previous[i].backpropagate(self.spike)

            if self.prevFunc[i] == Neurode.ADD_FUNCTION:
                add += self.previous[i].spike * self.prevWeight[i]
                adders += 1
            else:
                mul += self.previous[i].spike * self.prevWeight[i]
                multipliers += 1

        if adders > 0:
            self.sum += add

        if multipliers > 0:
            self.sum *= mul / multipliers

        for i in range(len(self.synInput)):
            self.sum += self.synInput[i].update(time, self.spike + self.backpropagated)
            if self.synInput[i].accountability > mostAccountability: mostAccountability = self.synInput[i].accountability
            occurrenceSum += self.synInput[i].occurrence

        for i in range(len(self.synInput)):
            self.synInput[i].scale(mostAccountability, occurrenceSum)

        self.output = self.activation.update(self.sum)
        self.spike = self.output // (self.threshold + self.refractory)
        self.refractory += self.spike * 0.1

        if self.spike > 0.0:
            self.spikeCount += self.spike
        else:
            self.spikeCount = 0

        if self.spike > 0.0:
            self.frequency = time - self.prevSpikeTime
            self.prevSpikeTime = time

        for i in range(len(self.synOutput)):
            self.synOutput[i].setCurrentInput(self.spike + self.backpropagated)

        self.prevSum = self.sum
        self.backpropagated = 0.0

        return self.spike

class AxonalNeurode(Neurode):
    def __init__(self, activation=None, decay=Neurode.DEF_DECAY, oscillator=None, threshold=Neurode.DEF_THRESHOLD):
        super().__init__(activation=SigmoidActivation(preference=0.9, sensitivity=np.exp(np.pi)) if activation is None else activation, decay=decay, oscillator=oscillator, 
                         threshold=threshold)

    def addPrevious(self, neurode, func=None, weight=None):
        if neurode is not None and len(self.previous) < 1:
            self.previous.append(neurode)
            self.prevFunc.append(func if func is not None else Neurode.ADD_FUNCTION)
            self.prevWeight.append(weight if weight is not None else Neurode.DEF_WEIGHT)
            return True

        return False

    def update(self, time):
        mostAccountability = 0.0
        occurrenceSum = 0.0
        add = 0.0
        adders = 0
        mul = 0.0
        multipliers = 0

        self.sum *= self.decay
        self.sum += self.oscillator.update(time) + self.backpropagated

        self.refractory = max(0, self.refractory - 0.1)

        for i in range(len(self.previous)):
            self.previous[i].backpropagate(self.spike)

            if self.prevFunc[i] == Neurode.ADD_FUNCTION:
                add += self.previous[i].spike * self.prevWeight[i] * self.previous[i].accountability * self.previous[i].occurrence
                adders += 1
            else:
                mul += self.previous[i].spike * self.prevWeight[i] * self.previous[i].accountability * self.previous[i].occurrence
                multipliers += 1

        if adders > 0:
            self.sum += add

        if multipliers > 0:
            self.sum *= mul / multipliers

        for i in range(len(self.synInput)):
            self.sum += self.synInput[i].update(time, self.spike + self.backpropagated)
            if self.synInput[i].accountability > mostAccountability: mostAccountability = self.synInput[i].accountability
            occurrenceSum += self.synInput[i].occurrence

        for i in range(len(self.synInput)):
            self.synInput[i].scale(mostAccountability, occurrenceSum)

        self.output = self.activation.update(self.sum)
        self.spike = self.output // (self.threshold + self.refractory)
        self.refractory += self.spike * 0.1

        if self.spike > 0.0:
            self.spikeCount += self.spike
        else:
            self.spikeCount = 0

        if self.spike > 0.0:
            self.frequency = time - self.prevSpikeTime
            self.prevSpikeTime = time

        for i in range(len(self.synOutput)):
            self.synOutput[i].setCurrentInput(self.spike + self.backpropagated)

        self.prevSum = self.sum
        self.backpropagated = 0.0

        return self.spike

########################################################################################
# Activation classes
class Activation:
    DEF_PREFERENCE = 0.5
    DEF_SENSITIVITY = 1.0

    def __init__(self, preference=DEF_PREFERENCE, sensitivity=DEF_SENSITIVITY):
        self.preference = preference
        self.sensitivity = sensitivity

    def config(self, preference=None, sensitivity=None):
        if preference is not None: self.preference = preference
        if sensitivity is not None: self.sensitivity = sensitivity

    def update(self, x):
        return 0.0

class GaussianActivation(Activation):
    def __init__(self, preference=Activation.DEF_PREFERENCE, sensitivity=Activation.DEF_SENSITIVITY):
        super().__init__(preference, sensitivity)

    def update(self, x):
        return gaussian(x, 1, self.preference, self.sensitivity)

class HardSigmoidActivation(Activation):
    def __init__(self, preference=Activation.DEF_PREFERENCE, sensitivity=Activation.DEF_SENSITIVITY):
        super().__init__(preference, sensitivity)

    def update(self, x):
        return hardsigmoid(x, 1, self.preference, self.sensitivity)

class SigmoidActivation(Activation):
    def __init__(self, preference=Activation.DEF_PREFERENCE, sensitivity=Activation.DEF_SENSITIVITY):
        super().__init__(preference, sensitivity)

    def update(self, x):
        return sigmoid(x, 1, self.preference, self.sensitivity)

########################################################################################
# Oscillator classes
class Oscillator:
    def __init__(self, amplitude=0.0, frequency=0.0, phase=0.0):
        self.amplitude = amplitude
        self.frequency = frequency
        self.phase = phase

    def config(self, amplitude=None, frequency=None, phase=None):
        if amplitude is not None: self.amplitude = amplitude
        if frequency is not None: self.frequency = frequency
        if phase is not None: self.phase = phase

    def print(self):
        print('Oscillator:')
        print('Amplitude = ', self.amplitude)
        print('Frequency = ', self.frequency)
        print('Phase = ', self.phase)

    def update(self, time):
        return 0.0

class CosineOscillator(Oscillator):
    def __init__(self, amplitude=0.0, frequency=0.0, phase=0.0):
        super().__init__(amplitude, frequency, phase)

    def print(self):
        print('CosineOscillator:')
        print('Amplitude = ', self.amplitude)
        print('Frequency = ', self.frequency)
        print('Phase = ', self.phase)

    def update(self, time):
        return cosine(time, self.amplitude, self.frequency, self.phase)

class ReverseSawtoothOscillator(Oscillator):
    def __init__(self, amplitude=0.0, frequency=0.0, phase=0.0):
        super().__init__(amplitude, frequency, phase)

    def print(self):
        print('ReverseSawtoothOscillator:')
        print('Amplitude = ', self.amplitude)
        print('Frequency = ', self.frequency)
        print('Phase = ', self.phase)

    def update(self, time):
        return sawtoothreverse(time, self.amplitude, self.frequency, self.phase)

class SawtoothOscillator(Oscillator):
    def __init__(self, amplitude=0.0, frequency=0.0, phase=0.0):
        super().__init__(amplitude, frequency, phase)

    def print(self):
        print('SawtoothOscillator:')
        print('Amplitude = ', self.amplitude)
        print('Frequency = ', self.frequency)
        print('Phase = ', self.phase)

    def update(self, time):
        return sawtooth(time, self.amplitude, self.frequency, self.phase)

class SineOscillator(Oscillator):
    def __init__(self, amplitude=0.0, frequency=0.0, phase=0.0):
        super().__init__(amplitude, frequency, phase)

    def print(self):
        print('SineOscillator:')
        print('Amplitude = ', self.amplitude)
        print('Frequency = ', self.frequency)
        print('Phase = ', self.phase)

    def update(self, time):
        return sine(time, self.amplitude, self.frequency, self.phase)

########################################################################################
# Synapse classes
class NeuralInput:
    DEF_BASE = 1.0
    DEF_DECAY = 0.1

    def __init__(self, base=DEF_BASE, decay=DEF_DECAY, start=0.0, sign=1):
        self.base = base
        self.decay = decay
        self.input = 0.0
        self.start = start
        self.sign = 1 if sign >= 0.0 else -1

    def config(self, base=None, decay=None, start=None, sign=None):
        if base is not None: self.base = base
        if decay is not None: self.decay = decay
        if start is not None: self.start = start
        if sign is not None: self.sign = 1 if sign >= 0.0 else -1

    def getInput(self):
        return self.input

    def update(self, time):
        self.input = potential(time, self.start, self.sign, self.decay, precision=1) * self.base
        return self.input

class Synapse:
    DEF_DECAY = NeuralInput.DEF_DECAY
    DEF_WEIGHT = 1.0

    def __init__(self, decay=DEF_DECAY, weight=DEF_WEIGHT, learnFunc=None):
        self.accountability = 1.0
        self.currentInput = 0.0
        self.decay = decay
        self.frequency = 0.0
        self.input = []
        self.learnFunc = LearningFunction() if learnFunc is None else learnFunc
        self.occurrence = 1.0
        self.prevInput = 0.0
        self.prevInputTime = 0.0
        self.prevSpikeTime = 0.0
        self.sum = 0.0
        self.weight = weight

    def config(self, decay=None, weight=None, learnFunc=None, sign=None):
        if decay is not None: 
            self.decay = decay

            for i in range(len(self.input)):
                self.input[i].config(decay=decay)

        if weight is not None: self.weight = weight

        if learnFunc is not None: self.learnFunc = learnFunc

    def getInput(self, index):
        if index >= 0 and index < len(self.input): return self.input[index]

    def getInputs(self):
        return self.input

    def inputSize(self):
        return len(self.input)

    def scale(self, mostAccountability, occurrenceSum):
        if self.accountability >= MIN_VALUE:
            self.accountability /= mostAccountability if mostAccountability >= 1.0 else 1.0

        if self.occurrence >= MIN_VALUE:
            self.occurrence /= occurrenceSum if occurrenceSum >= 1.0 else 1.0

    def setCurrentInput(self, x):
        self.currentInput = x

    def sum(self):
        return self.sum

    def update(self, time, spike):
        return 0.0

class SensorySynapse(Synapse):
    DEF_SENSORY_DECAY = 0.01

    def __init__(self, decay=DEF_SENSORY_DECAY, weight=Synapse.DEF_WEIGHT, sign=1):
        super().__init__(decay, weight)
        self.sign = 1 if sign >= 0 else -1

    def config(self, decay=None, weight=None, learnFunc=None, sign=None):
        if decay is not None: 
            self.decay = decay

            for i in range(len(self.input)):
                self.input[i].config(decay=decay)

        if weight is not None: self.weight = weight

        if learnFunc is not None: self.learnFunc = learnFunc

        if sign is not None: self.sign = 1 if sign >= 0 else -1

    def update(self, time, spike):
        self.input.append(NeuralInput(self.currentInput, self.decay, time, self.sign))
        self.currentInput = 0.0

        self.sum = 0.0

        for i in reversed(range(len(self.input))):
            self.sum += self.input[i].update(time)
            if self.input[i].getInput() < MIN_VALUE: 
                self.input.pop(i)

        return self.sum * self.weight

class NeuralSynapse(Synapse):
    def __init__(self, decay=Synapse.DEF_DECAY, weight=Synapse.DEF_WEIGHT, learnFunc=None, filter=None, inputDecay=Synapse.DEF_DECAY):
        super().__init__(decay, weight, learnFunc)
        self.filter = SynapticFilter() if filter is None else filter
        self.inputDecay = inputDecay

    def config(self, decay=None, weight=None, filter=None, inputDecay=None):
        if decay is not None:
            self.decay = decay

            for i in range(len(self.input)):
                self.input[i].config(decay=decay)

        if weight is not None: self.weight = weight
        if filter is not None: self.filter = filter
        if inputDecay is not None: self.inputDecay = inputDecay

class ExcitatorySynapse(NeuralSynapse):
    def __init__(self, decay=Synapse.DEF_DECAY, weight=Synapse.DEF_WEIGHT, learnFunc=None, filter=None, inputDecay=Synapse.DEF_DECAY):
        super().__init__(decay, weight, learnFunc, filter, inputDecay)

    def update(self, time, spike):
        filtered = 0

        self.frequency *= self.inputDecay

        if self.currentInput > 0.0:
            self.frequency += self.currentInput
            self.prevInputTime = time
            filtered = self.currentInput * self.filter.filter(self.frequency)
            self.input.append(NeuralInput(filtered, self.decay, time))
            
        self.currentInput = 0.0

        if spike > 0.0:
            self.prevSpikeTime = time

        self.sum = 0.0

        for i in reversed(range(len(self.input))):
            self.sum += self.input[i].update(time)
            if self.input[i].getInput() < MIN_VALUE: 
                self.input.pop(i)

        self.sum *= self.accountability * self.occurrence

        self.accountability = max(MIN_VALUE, self.accountability + 
                                  self.learnFunc.learn(max(-1, min(1, self.prevSpikeTime - self.prevInputTime))) * self.prevInput * spike)
        self.occurrence = max(MIN_VALUE, self.occurrence + self.learnFunc.learn(max(-1, min(1, self.prevSpikeTime - self.prevInputTime))) * spike)

        self.prevInput = filtered

        return self.sum * self.weight

class InhibitorySynapse(NeuralSynapse):
    def __init__(self, decay=Synapse.DEF_DECAY, weight=Synapse.DEF_WEIGHT, learnFunc=None, filter=None, inputDecay=Synapse.DEF_DECAY):
        super().__init__(decay, weight, learnFunc, filter, inputDecay)

    def update(self, time, spike):
        filtered = 0

        self.frequency *= self.inputDecay

        if self.currentInput > 0.0:
            self.frequency += spike
            self.prevInputTime = time
            filtered = self.currentInput * self.filter.filter(self.frequency)
            self.input.append(NeuralInput(filtered, self.decay, time, -1))
            
        self.currentInput = 0.0

        if spike > 0.0:
            self.prevSpikeTime = time

        self.sum = 0.0

        for i in reversed(range(len(self.input))):
            self.sum += self.input[i].update(time)
            if self.input[i].getInput() < MIN_VALUE: 
                self.input.pop(i)

        self.sum *= self.accountability * self.occurrence

        self.accountability = max(MIN_VALUE, self.accountability + 
                                  self.learnFunc.learn(max(-1, min(1, self.prevSpikeTime - self.prevInputTime))) * self.prevInput * spike)
        self.occurrence = max(MIN_VALUE, self.occurrence + self.learnFunc.learn(max(-1, min(1, self.prevSpikeTime - self.prevInputTime))) * spike)

        self.prevInput = filtered

        return self.sum * self.weight

########################################################################################
# SynapticFilter classes
class SynapticFilter:
    DEF_PREFERENCE = 0.5
    DEF_SENSITIVITY = 1.0

    def __init__(self, preference=None, sensitivity=None):
        self.preference = SynapticFilter.DEF_PREFERENCE if preference is None else preference
        self.sensitivity = SynapticFilter.DEF_SENSITIVITY if sensitivity is None else sensitivity

    def config(self, preference=None, sensitivity=None):
        if preference is not None: self.preference = preference
        if sensitivity is not None: self.sensitivity = sensitivity

    def filter(self, frequency):
        return 1.0

class BandpassFilter(SynapticFilter):
    def __init__(self, preference=None, sensitivity=None):
        super().__init__(preference, sensitivity)

    def filter(self, frequency):
        return gaussian(x=frequency, center=self.preference, width=self.sensitivity)

class HighpassFilter(SynapticFilter):
    def __init__(self, preference=None, sensitivity=None):
        super().__init__(preference, sensitivity)

    def filter(self, frequency):
        return sigmoid(x=frequency, center=self.preference, sharpness=self.sensitivity)

class LowpassFilter(SynapticFilter):
    def __init__(self, preference=None, sensitivity=None):
        super().__init__(preference, sensitivity)
        
    def filter(self, frequency):
        return sigmoid(x=-frequency, center=self.preference, sharpness=self.sensitivity)

########################################################################################
# LearningFunction class
class LearningFunction:
    def __init__(self, prePeak=-1, preMax=1, preWndDecay=1, postPeak=1, postMax=-1, postWndDecay=1):
        self.prePeak = prePeak
        self.preMax = preMax
        self.preWndDecay = preWndDecay
        self.postPeak = postPeak
        self.postMax = postMax
        self.postWndDecay = postWndDecay
        self.midPoints = []

    def addMidPoint(self, x, y):
        if x is not None and y is not None:
            self.midPoints.append((x, y))
            return True

        return False

    def config(self, prePeak=None, preMax=None, preWndDecay=None, postPeak=None, postMax=None, postWndDecay=None):
        if prePeak is not None: self.prePeak = prePeak
        if preMax is not None: self.preMax = preMax
        if preWndDecay is not None: self.preWndDecay = preWndDecay
        if postPeak is not None: self.postPeak = postPeak
        if postMax is not None: self.postMax = postMax
        if postWndDecay is not None: self.postWndDecay = postWndDecay

    def learn(self, x):
        return stdp(x, self.prePeak, self.preMax, self.preWndDecay, self.postPeak, self.postMax, self.postWndDecay, self.midPoints)

    def removeMidPoint(self, index):
        if index is not None and index >= 0 and index < len(self.midPoints):
            self.midPoints.pop(index)
            return True

        return False