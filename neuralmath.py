import numpy as np
import math

def cosine(x, amplitude=1, frequency=1, phase=0):
    return amplitude * np.cos(2 * np.pi * frequency * (x - phase))

def gaussian(x, amplitude=1, center=0, width=1):
    return amplitude * np.exp(-((x - center)**2) / (2 * (width / 4.29193)**2))

def potential(x, start=0, sign=1, decay=1, precision=1000):
    return sign * np.exp(np.fmax(np.zeros(precision), x - start) * -decay)

def sawtooth(x, amplitude=1, frequency=1, phase=0):
    _x = (x - phase) * 2 * np.pi * frequency
    s = 0.0
    c = -1.0

    for i in range(1, 11):
        c *= -1
        s += c * np.sin(_x * i) / i

    return 0.375 * amplitude * (-(1 / np.pi) * s)

def sawtoothreverse(x, amplitude=1, frequency=1, phase=0):
    _x = (x - phase) * 2 * np.pi * frequency
    s = 0.0
    c = -1.0

    for i in range(1, 11):
        c *= -1
        s += c * np.sin(_x * i) / i

    return 0.375 * amplitude / np.pi * s

def sigmoid(x, amplitude=1, center=0, sharpness=1):
    return amplitude / (1 + np.exp(-(x - center) * sharpness))

def sine(x, amplitude=1, frequency=1, phase=0):
    return amplitude * np.sin(2 * np.pi * frequency * (x - phase))

def stdp(x, prePeak=-1, preMax=1, preWndDecay=1, postPeak=1, postMax=-1, postWndDecay=1, midPoints = None):
    y = 0.0

    if x <= prePeak:
        y = preMax * np.exp(np.sign(preMax) * (x - prePeak) * preWndDecay)
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

def tanh(x, amplitude=1, center=0, sharpness=1):
    return amplitude * np.tanh((x - center) * sharpness)
