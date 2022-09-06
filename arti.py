from neuralmath import *

class Arti:
    def __init__(self):
        self.xNeuron = []
        self.oNeuron = []
        self.eNeuron = []
        self.mNeuron = []
        self.tNeuron = SensoryNeuron()
        self.neuron = Neuron()

        for i in range(9):
            self.xNeuron.append(SensoryNeuron())
            self.oNeuron.append(SensoryNeuron())
            self.eNeuron.append(SensoryNeuron())
            self.mNeuron.append(SensoryNeuron())

        self.tNeuron.soma.config(oscillator=SawtoothOscillator(0.5, 1, 0))

        self.neuron.addDendrite(DendriticNeurode(activation=GaussianActivation(1.5, 0.5), decay=0.1))
        self.neuron.connectNeurodes(self.neuron.dendrite[0], self.neuron.soma, Neurode.MULTIPLY_FUNCTION)
        self.neuron.createSynapse(self.tNeuron.axon[0], self.neuron.dendrite[0])

########################################################################################
# Neuron classes
class Neuron:
    def __init__(self, soma=None):
        self.dendrite = []
        self.soma = SomaticNeurode() if soma is None else soma
        self.axon = []

    def addAxon(self, axon):
        if axon is not None: self.axon.append(axon)

    def addDendrite(self, dendrite):
        if dendrite is not None: self.dendrite.append(dendrite)

    def connectNeurodes(self, prevNeurode, nextNeurode, func=None):
        if prevNeurode is not None and nextNeurode is not None:
            success = prevNeurode.addNext(nextNeurode)
            if not success: return False

            success = nextNeurode.addPrevious(prevNeurode, func)
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
            occurrenceSum += self.occurrence

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
        self.axon.append(AxonalNeurode())
        self.soma.addNext(self.axon[0])
        self.axon[0].addPrevious(self.soma)

########################################################################################
# Neurode classes
class Neurode:
    DEF_DECAY = 0.5
    ADD_FUNCTION = 'add'
    MULTIPLY_FUNCTION = 'multiply'
    SPIKE_THRESHOLD = 0.5

    def __init__(self, activation=None, decay=DEF_DECAY, learnFunc=None, oscillator=None):
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
        self.previous = []
        self.prevFunc = []
        self.prevBpSpikeTime = 0.0
        self.prevSpikeTime = 0.0
        self.spike = 0.0
        self.sum = 0.0
        self.synInput = []
        self.synOutput = []

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

    def addPrevious(self, neurode, func=None):
        if neurode is not None:
            self.previous.append(neurode)
            self.prevFunc.append(func if func is not None else Neurode.ADD_FUNCTION)
            return True

        return False

    def backpropagate(self, spike):
        bp = spike * self.accountability * self.occurrence
        if bp > self.backpropagated: self.backpropagated = bp

    def config(self, activation=None, decay=None, learnFunc=None, oscillator=None):
        if activation is not None: self.activation = activation
        if decay is not None: self.decay = decay
        if learnFunc is not None: self.learnFunc = learnFunc
        if oscillator is not None: self.oscillator = oscillator

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
            return True

        return False

    def scale(self, mostAccountability, occurrenceSum):
        self.accountability /= mostAccountability if mostAccountability > 0.0 else 1.0
        self.occurrence /= occurrenceSum if occurrenceSum > 0.0 else 1.0

    def setPrevious(self, index, neurode=None, func=None):
        if index >= 0 and index < len(self.previous):
            if neurode is not None:
                self.previous[index] = neurode

            if func is not None:
                self.prevFunc[index] = func

    def update(self, time):
        return 0.0

class DendriticNeurode(Neurode):
    def __init__(self, activation=None, decay=Neurode.DEF_DECAY, oscillator=None):
        super().__init__(GaussianActivation() if activation is None else activation, decay, oscillator)

    def addNext(self, neurode):
        if neurode is not None and len(self.next) < 1:
            self.next.append(neurode)
            return True

        return False

    def update(self, time):
        add = 0.0
        mul = 0.0
        mostAccountability = 0.0
        occurrenceSum = 0.0

        self.sum *= self.decay
        self.sum += self.oscillator.update(time) + self.backpropagated

        for i in range(len(self.previous)):
            self.previous[i].backpropagate(self.spike)

            if self.prevFunc[i] == Neurode.ADD_FUNCTION:
                add += self.previous[i].spike
            else:
                mul += self.previous[i].spike

        if len(self.previous) > 0:
            self.sum += add / len(self.previous)
            self.sum *= mul / len(self.previous)

        for i in range(len(self.synInput)):
            self.sum += self.synInput[i].update(time, self.spike + self.backpropagated)
            if self.synInput[i].accountability > mostAccountability: mostAccountability = self.synInput[i].accountability
            occurrenceSum += self.occurrence

        for i in range(len(self.synInput)):
            self.synInput[i].scale(mostAccountability, occurrenceSum)

        self.spike = self.activation.update(self.sum, self.frequency) * self.accountability * self.occurrence

        for i in range(len(self.synOutput)):
            self.synOutput[i].setCurrentInput(self.spike + self.backpropagated)

        if self.backpropagated >= Neurode.SPIKE_THRESHOLD:
            self.bpFrequency = 1 / (time - self.prevBpSpikeTime)
            self.prevBpSpikeTime = time
        
        if self.spike >= Neurode.SPIKE_THRESHOLD:
            self.frequency = 1 / (time - self.prevSpikeTime)
            self.prevSpikeTime = time

        self.accountability += self.learnFunc.learn(self.bpFrequency - self.frequency) * self.prevInput * self.backpropagated
        self.occurrence += self.learnFunc.learn(self.bpFrequency - self.frequency) * self.backpropagated

        return self.spike

class SomaticNeurode(Neurode):
    def __init__(self, activation=None, decay=Neurode.DEF_DECAY, oscillator=None):
        super().__init__(SigmoidActivation(sensitivity=np.exp(np.pi)) if activation is None else activation, decay, oscillator)

    def update(self, time):
        mostAccountability = 0.0
        occurrenceSum = 0.0
        add = 0.0
        mul = 0.0

        self.sum *= self.decay
        self.sum += self.oscillator.update(time) + self.backpropagated

        for i in range(len(self.previous)):
            self.previous[i].backpropagate(self.spike)

            if self.prevFunc[i] == Neurode.ADD_FUNCTION:
                add += self.previous[i].spike
            else:
                mul += self.previous[i].spike

        if len(self.previous) > 0:
            self.sum += add / len(self.previous)
            self.sum *= mul / len(self.previous)

        for i in range(len(self.synInput)):
            self.sum += self.synInput[i].update(time, self.spike + self.backpropagated)
            if self.synInput[i].accountability > mostAccountability: mostAccountability = self.synInput[i].accountability
            occurrenceSum += self.occurrence

        for i in range(len(self.synInput)):
            self.synInput[i].scale(mostAccountability, occurrenceSum)

        self.spike = self.activation.update(self.sum, self.frequency)

        for i in range(len(self.synOutput)):
            self.synOutput[i].setCurrentInput(self.spike + self.backpropagated)

        return self.spike

class AxonalNeurode(Neurode):
    def __init__(self, activation=None, decay=Neurode.DEF_DECAY, oscillator=None):
        super().__init__(SigmoidActivation(preference=0.9, sensitivity=np.exp(np.pi)) if activation is None else activation, decay, oscillator)

    def addPrevious(self, neurode, func=None):
        if neurode is not None and len(self.previous) < 1:
            self.next.append(neurode)
            self.next.append(func if func is not None else Neurode.ADD_FUNCTION)
            return True

        return False

    def update(self, time):
        mostAccountability = 0.0
        occurrenceSum = 0.0
        add = 0.0
        mul = 0.0

        self.sum *= self.decay
        self.sum += self.oscillator.update(time) + self.backpropagated

        for i in range(len(self.previous)):
            self.previous[i].backpropagate(self.spike)

            if self.prevFunc[i] == Neurode.ADD_FUNCTION:
                add += self.previous[i].spike
            else:
                mul += self.previous[i].spike

        if len(self.previous) > 0:
            self.sum += add / len(self.previous)
            self.sum *= mul / len(self.previous)

        for i in range(len(self.synInput)):
            self.sum += self.synInput[i].update(time, self.spike + self.backpropagated)
            if self.synInput[i].accountability > mostAccountability: mostAccountability = self.synInput[i].accountability
            occurrenceSum += self.occurrence

        for i in range(len(self.synInput)):
            self.synInput[i].scale(mostAccountability, occurrenceSum)

        self.spike = self.activation.update(self.sum, self.frequency)

        for i in range(len(self.synOutput)):
            self.synOutput[i].setCurrentInput(self.spike + self.backpropagated)

        return self.spike

########################################################################################
# Activation classes
class Activation:
    DEF_PREFERENCE = 0.5
    DEF_SENSITIVITY = 1.0

    def __init__(self, preference=DEF_PREFERENCE, sensitivity=DEF_SENSITIVITY):
        self.preference = preference
        self.sensitivity = sensitivity
        self.threshold = preference

    def config(self, preference=None, sensitivity=None):
        if preference is not None: self.preference = preference
        if sensitivity is not None: self.sensitivity = sensitivity

    def update(self, x, spike):
        return 0.0

class GaussianActivation(Activation):
    def __init__(self, preference=Activation.DEF_PREFERENCE, sensitivity=Activation.DEF_SENSITIVITY):
        super().__init__(preference, sensitivity)

    def update(self, x, spikeFrequency):
        self.threshold = self.sensitivity * (2 - (spikeFrequency * 1.999))
        return gaussian(x, 1, self.preference, self.threshold)

class SigmoidActivation(Activation):
    def __init__(self, preference=Activation.DEF_PREFERENCE, sensitivity=Activation.DEF_SENSITIVITY):
        super().__init__(preference, sensitivity)

    def update(self, x, spikeFrequency):
        self.threshold = self.preference + spikeFrequency - 0.5
        return sigmoid(x, 1, self.threshold, self.sensitivity)

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
        return sawtooth(time, self.amplitude, self.frequency, self.phase)

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
    DEF_DECAY = 1.0 / 7.0

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

    def input(self):
        return self.input

    def update(self, time):
        self.input = potential(time, self.start, self.sign, self.decay, precision=1) * self.base
        return self.input

class Synapse:
    DEF_DECAY = NeuralInput.DEF_DECAY
    SPIKE_THRESHOLD = 0.5
    MIN_INPUT = 0.001

    def __init__(self, decay=DEF_DECAY, learnFunc=None):
        self.accountability = 1.0
        self.bpFrequency = 0.0
        self.currentInput = 0.0
        self.decay = decay
        self.frequency = 0.0
        self.input = []
        self.learnFunc = LearningFunction() if learnFunc is None else learnFunc
        self.occurrence = 0.0
        self.prevInput = 0.0
        self.prevInputTime = 0.0
        self.prevSpikeTime = 0.0
        self.sum = 0.0

    def config(self, decay=None, learnFunc=None):
        if decay is not None: 
            self.decay = decay

            for i in range(len(self.input)):
                self.input[i].config(decay=decay)

        if learnFunc is not None: self.learnFunc = learnFunc

    def getInput(self, index):
        if index >= 0 and index < len(self.input): return self.input[index]

    def getInputs(self):
        return self.input

    def inputSize(self):
        return len(self.input)

    def scale(self, mostAccountability, occurrenceSum):
        self.accountability /= mostAccountability if mostAccountability > 0.0 else 1.0
        self.occurrence /= occurrenceSum if occurrenceSum > 0.0 else 1.0

    def setCurrentInput(self, x):
        self.currentInput = x

    def sum(self):
        return self.sum

    def update(self, time, spike):
        return 0.0

class SensorySynapse(Synapse):
    DEF_SENSORY_DECAY = 10

    def __init__(self, decay=DEF_SENSORY_DECAY, sign=1):
        super().__init__(decay)
        self.sign = 1 if sign >= 0 else -1

    def update(self, time, spike):
        self.input.append(NeuralInput(self.currentInput, self.decay, time, self.sign))
        self.currentInput = 0.0

        self.sum = 0.0

        for i in range(len(self.input)):
            self.sum += self.input[i].update(time)
            if self.input[i].input() < Synapse.MIN_INPUT: self.input.remove(i)

        return self.sum

class NeuralSynapse(Synapse):
    def __init__(self, decay=Synapse.DEF_DECAY, learnFunc=None, filter=None):
        super().__init__(decay, learnFunc)
        self.filter = SynapticFilter() if filter is None else filter

    def config(self, decay=None, filter=None):
        if decay is not None:
            self.decay = decay

            for i in range(len(self.input)):
                self.input[i].config(decay=decay)

        if filter is not None: self.filter = filter

class ExcitatorySynapse(NeuralSynapse):
    def __init__(self, decay=Synapse.DEF_DECAY, learnFunc=None, filter=None):
        super().__init__(decay, learnFunc, filter)

    def update(self, time, spike):
        filtered = 0

        if self.currentInput >= Synapse.SPIKE_THRESHOLD:
            self.frequency = 1 / (time - self.prevInputTime)
            self.prevInputTime = time
            filtered = self.currentInput * self.filter.filter(self.frequency)
            self.input.append(NeuralInput(filtered, self.decay, time))
            self.currentInput = 0.0

        if spike >= Synapse.SPIKE_THRESHOLD:
            self.bpFrequency = 1 / (time - self.prevSpikeTime)
            self.prevSpikeTime = time

        self.sum = 0.0

        for i in range(len(self.input)):
            self.sum += self.input[i].update(time)
            if self.input[i].input() < Synapse.MIN_INPUT: self.input.pop(i)

        self.sum *= self.accountability * self.occurrence

        self.accountability += self.learnFunc.learn(self.bpFrequency - self.frequency) * self.prevInput * spike
        self.occurrence += self.learnFunc.learn(self.bpFrequency - self.frequency) * spike

        self.prevInput = filtered

        return self.sum

class InhibitorySynapse(NeuralSynapse):
    def __init__(self, decay=Synapse.DEF_DECAY, learnFunc=None, filter=None):
        super().__init__(decay, learnFunc, filter)

    def update(self, time, spike):
        filtered = 0

        if self.currentInput >= Synapse.SPIKE_THRESHOLD:
            self.frequency = 1 / (time - self.prevInputTime)
            self.prevInputTime = time
            filtered = self.currentInput * self.filter.filter(self.frequency)
            self.input.append(NeuralInput(filtered, self.decay, time, -1))
            self.currentInput = 0.0

        if spike >= Synapse.SPIKE_THRESHOLD:
            self.bpFrequency = 1 / (time - self.prevSpikeTime)
            self.prevSpikeTime = time

        self.sum = 0.0

        for i in range(len(self.input)):
            self.sum += self.input[i].update(time)
            if self.input[i].input() < Synapse.MIN_INPUT: self.input.pop(i)

        self.sum *= self.accountability * self.occurrence

        self.accountability += self.learnFunc.learn(self.bpFrequency - self.frequency) * self.prevInput * spike
        self.occurrence += self.learnFunc.learn(self.bpFrequency - self.frequency) * spike

        self.prevInput = filtered

        return self.sum

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
        return 0.0

class BandpassFilter(SynapticFilter):
    def __init__(self, preference=None, sensitivity=None):
        super().__init__(preference, sensitivity)

    def filter(self, frequency):
        return gaussian(x=frequency, center=self.preference, width=self.sensitivity)

class HighpassFilter(SynapticFilter):
    def __init__(self, preference=None, sensitivity=None):
        super().__init__(preference, sensitivity)

    def filter(self, frequency):
        return sigmoid(x=frequency, center=self.preference, width=self.sensitivity)

class LowpassFilter(SynapticFilter):
    def __init__(self, preference=None, sensitivity=None):
        super().__init__(preference, sensitivity)

    def filter(self, frequency):
        return sigmoid(x=-frequency, center=self.preference, width=self.sensitivity)

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