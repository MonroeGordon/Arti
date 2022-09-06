import matplotlib.pyplot as plt
import neuralmath as nmath
import numpy as np
import time

class GraphParam:
    def __init__(self, width=12.8, height=2.4, overlay=True, grid=True, xmin=0, xmax=10, precision=100, timescale='real', cycles=1000):
        self.width = width
        self.height = height
        self.overlay = overlay
        self.grid = grid
        self.xmin = xmin
        self.xmax = xmax
        self.precision = precision
        self.timescale = timescale
        self.cycles = cycles

def graph(gparam, func, *argv):
    et = time.time() if gparam.timescale == 'real' else 0

    x = np.linspace(gparam.xmin, gparam.xmax, gparam.precision * abs(gparam.xmax - gparam.xmin))
    y = []

    if gparam.cycles > 1: plt.ion()

    if gparam.overlay: 
        figure, ax = plt.subplots(figsize=(gparam.width, gparam.height), layout='tight')
        ax.set_xlabel("Time (s)")
        ax.set_ylabel("Output")

        for i in range(len(func)):
            y.append(func[i](x, *argv[i]))
            ax.plot(x, y[i])

        if gparam.grid: ax.grid()

        et = time.time() - et if gparam.timescale == 'real' else gparam.timescale

        for t in range(gparam.cycles):
            for i in range(len(func)):
                y[i] = func[i](x, *argv[i])
    
                ax.set_xlim(gparam.xmin + (t * et), gparam.xmax + (t * et))
                ax.lines[i].set_xdata(x)
                ax.lines[i].set_ydata(y[i])
    
            figure.canvas.draw()
            figure.canvas.flush_events()

            x = x + et
    else: 
        figure, ax = plt.subplots(len(func), sharex=True, sharey=True, figsize=(gparam.width, gparam.height * len(func)), layout='tight')

        for i in range(len(func)):
            y.append(func[i](x, *argv[i]))
            ax[i].plot(x, y[i])
            ax[i].set_xlabel("Time (s)")
            ax[i].set_ylabel("Output")

            if gparam.grid: ax[i].grid()

        et = time.time() - et if gparam.timescale == 'real' else gparam.timescale

        for t in range(gparam.cycles):
            for i in range(len(func)):
                y[i] = func[i](x, *argv[i])

                ax[i].set_xlim(gparam.xmin + (t * et), gparam.xmax + (t * et))
                ax[i].lines[0].set_xdata(x)
                ax[i].lines[0].set_ydata(y[i])
    
            figure.canvas.draw()
            figure.canvas.flush_events()

            x = x + et