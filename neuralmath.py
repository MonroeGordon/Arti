import numpy as np
from math import *

"""
This function processes the cosine of x with the specified amplitude, frequency and phase.
x: the input value
amplitude: the amplitude of the function
frequency: the frequency of the function
phase: the phase of the function
returns: the output of amplitude * cosine(2 * pi * frequency * (x - phase))
"""
def cosine(x, amplitude=1, frequency=1, phase=0):
    return amplitude * np.cos(2 * np.pi * frequency * (x - phase))

"""
This function processes the gaussian distribution of x with the specified amplitude, center and width.
x: the input value
amplitude: the amplitude of the gaussian distribution curve
center: the x value with the maximal y value
width: the width of the gaussian distribution at half of its maximal output value
returns: the output of amplitude * e^(-4 * ln(2) * ((x - center)^2 / (width^2)))
"""
def gaussian(x, amplitude=1, center=0, width=1):
    return amplitude * np.exp(-4 * log(2) * ((x - center)**2) / (width**2))

"""
This function processes the hard sigmoid of x with the specified amplitude, center and sharpness.
x: the input value
amplitude: the amplitude of the function
center: the x value with the y value of half the amplitude
sharpness: the steepness of the change from a y value of 0 to a y value of amplitude
returns: the output of max(0, min(amplitude, amplitude * (((x - center) * sharpness) + 1) * 0.5))
"""
def hardsigmoid(x, amplitude=1, center=0, sharpness=1):
    return np.maximum(np.zeros(np.shape(x)), np.minimum(np.ones(np.shape(x)) * amplitude, amplitude * (((x - center) * sharpness) + 1) * 0.5))

"""
This function processes the hard hyperbolic tangent of x with the specified amplitude, center and sharpness.
x: the input value
amplitude: the amplitude of the function
center: the x value with the y value of 0
sharpness: the steepness of the change from a y value of -amplitude to a y value of amplitude
returns: the output of max(-amplitude, min(amplitude, amplitude * (x - center) * sharpness))
"""
def hardtanh(x, amplitude=1, center=0, sharpness=1):
    return np.maximum(np.ones(np.shape(x)) * -amplitude, np.minimum(np.ones(np.shape(x)) * amplitude, amplitude * (x - center) * sharpness))

"""
This function processes the synaptic input potential of x (where x is time) with the specified start, sign, decay and precision.
x: the input value (time)
start: the x value that the function starts at
sign: the sign (+/-) of the y value of the function
decay: the rate at which the y value decreases as x increases
precision: the precision of the x values
"""
def potential(x, start=0, sign=1, decay=1, precision=1000):
    return sign * np.exp(np.fmax(np.zeros(precision), x - start) * -(1 / decay))

"""
This function processes the sawtooth wave of x with the specified amplitude, frequency and phase.
x: the input value
amplitude: the amplitude of the function
frequency: the frequency of the function
phase: the phase of the function
returns: the output of the sawtooth function of x (a composite of sine functions)
"""
def sawtooth(x, amplitude=1, frequency=1, phase=0):
    _x = (x - phase) * 2 * np.pi * frequency
    s = 0.0
    c = -1.0

    for i in range(1, 21):
        c *= -1
        s += c * np.sin(_x * i) / i

    return amplitude * (0.5 - (1 / np.pi) * s)

"""
This function processes the reverse sawtooth wave of x with the specified amplitude, frequency and phase.
x: the input value
amplitude: the amplitude of the function
frequency: the frequency of the function
phase: the phase of the function
returns: the output of the reverse sawtooth function of x (a composite of sine functions)
"""
def sawtoothreverse(x, amplitude=1, frequency=1, phase=0):
    _x = (x - phase) * 2 * np.pi * frequency
    s = 0.0
    c = -1.0

    for i in range(1, 21):
        c *= -1
        s += c * np.sin(_x * i) / i

    return (amplitude / np.pi * s) + 0.5

"""
This function processes the sigmoid of x with the specified amplitude, center and sharpness.
x: the input value
amplitude: the amplitude of the function
center: the x value with the y value of half the amplitude
sharpness: the steepness of the change from a y value of 0 to a y value of amplitude
returns: the output of amplitude / (1 + e^(-(x - center) * sharpness))
"""
def sigmoid(x, amplitude=1, center=0, sharpness=1):
    return amplitude / (1 + np.exp(-(x - center) * sharpness))

"""
This function processes the sine of x with the specified amplitude, frequency and phase.
x: the input value
amplitude: the amplitude of the function
frequency: the frequency of the function
phase: the phase of the function
returns: the output of amplitude * sine(2 * pi * frequency * (x - phase))
"""
def sine(x, amplitude=1, frequency=1, phase=0):
    return amplitude * np.sin(2 * np.pi * frequency * (x - phase))

"""
This function processes the spike-timing-dependent-plasticity of x with the specified prePeak, preMax, preWndDecay, postPeak, postMax,
postWndDecay and midpoints.
x: the input value (post-synaptic neuron spike - pre-synaptic input time)
prePeak: the x value at which the y value peaks for pre before post
preMax: the peak y value for pre before post
preWndDecay: the rate at which the y value decays for pre before post
postPeak: the x value at which the y value peaks for post before pre
postMax: the peak y value for post before pre
postWndDecay: the rate at which the y value decays for post before pre
midpoints: x, y points between the pre and post peaks that complete the function (no midpoints means a line from preMax to postMax)
"""
def stdp(x, prePeak=-1, preMax=1, preWndDecay=1, postPeak=1, postMax=-1, postWndDecay=1, midPoints = None):
    y = 0.0

    if x <= prePeak:
        y = preMax * np.exp(np.sign(preMax) * (prePeak - x) * preWndDecay)
    elif x > prePeak and x < postPeak:
        m = ((postMax - preMax) / (postPeak - prePeak))
        b = (postMax / m) - postPeak
        y = m * x + b

        for i in range(len(midPoints)):
            if i == 0 and x > prePeak and x < midPoints[i][0]:
                m = ((midPoints[i][1] - preMax) / (midPoints[i][0] - prePeak))
                b = (midPoints[i][1] / m) - midPoints[i][0]
                y = m * x + b
            elif i == len(midPoints) - 1 and x >= midPoints[i][0] and x < postPeak:
                m = ((postMax - midPoints[i][1]) / (postPeak - midPoints[i][0]))
                b = (postMax / m) - postPeak
                y = m * x + b
            elif x >= midPoints[i - 1][0] and x < midPoints[i][0]:
                m = ((midPoints[i][1] - midPoints[i - 1][1]) / (midPoints[i][0] - midPoints[i - 1][0]))
                b = (midPoints[i][1] / m) - midPoints[i][0]
                y = m * x + b
    else:
        y = postMax * np.exp(np.sign(-postMax) * (x - postPeak) * postWndDecay)

    return y

"""
This function processes the hyperbolic tangent of x with the specified amplitude, center and sharpness.
x: the input value
amplitude: the amplitude of the function
center: the x value with the y value of 0
sharpness: the steepness of the change from a y value of -amplitude to a y value of amplitude
returns: the output of amplitude * tanh((x - center) * sharpness)
"""
def tanh(x, amplitude=1, center=0, sharpness=1):
    return amplitude * np.tanh((x - center) * sharpness)
