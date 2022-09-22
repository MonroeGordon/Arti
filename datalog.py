from arti import *
import pandas as pd

"""
Arti_1_0_DataLog: This class contains Arti's datalog data for the runtime of the Tic-Tac-Toe program. It also handles writing the data to a file
at the end of the program. The data the is recorded during the runtime can be specified as well, anywhere from no data to all data.
"""
class Arti_1_0_DataLog:
    """
    Initializes the data arrays that record Arti's data during runtime.
    self: object reference
    """
    def __init__(self):
        self.nolog = True
        self.size = 0
        self.inputs = {
            'Time': [],
            'Turn': [],
            'Arti': [],
            'Human': [],
            'Move #': [],
            'Square 1': [],
            'Square 2': [],
            'Square 3': [],
            'Square 4': [],
            'Square 5': [],
            'Square 6': [],
            'Square 7': [],
            'Square 8': [],
            'Square 9': []}
        self.xNeuron = []
        self.oNeuron = []
        self.mNeuron = []
        self.tNeuron = {
            'Turn SynInput 0': [],
            'Turn SynInput 1': [],
            'Turn Oscillator': [],
            'Turn Backprop': [],
            'Turn Sum': [],
            'Turn Output': [],
            'Turn Spike': []}
        self.xoDend = [[], [], [], [], [], [], [], [], []]
        self.mDend = []
        self.tDend = {
            'Turn Dend SynInput 0': [],
            'Turn Dend Oscillator': [],
            'Turn Dend Backprop': [],
            'Turn Dend Sum': [],
            'Turn Dend Output': [],
            'Turn Dend Spike': []}
        self.soma = {
            'Soma Oscillator': [],
            'Soma Backprop': [],
            'Soma Sum': [],
            'Soma Output': [],
            'Soma Spike': []}
        self.axon = []

        for i in range(9):
            self.xNeuron.append({
                'X' + str(i) + ' SynInput 0': [],
                'X' + str(i) + ' Oscillator': [],
                'X' + str(i) + ' Backprop': [],
                'X' + str(i) + ' Sum': [],
                'X' + str(i) + ' Output': [],
                'X' + str(i) + ' Spike': []})
            self.oNeuron.append({
                'O' + str(i) + ' SynInput 0': [],
                'O' + str(i) + ' Oscillator': [],
                'O' + str(i) + ' Backprop': [],
                'O' + str(i) + ' Sum': [],
                'O' + str(i) + ' Output': [],
                'O' + str(i) + ' Spike': []})
            self.mNeuron.append({
                'M' + str(i) + ' SynInput 0': [],
                'M' + str(i) + ' Oscillator': [],
                'M' + str(i) + ' Backprop': [],
                'M' + str(i) + ' Sum': [],
                'M' + str(i) + ' Output': [],
                'M' + str(i) + ' Spike': []})

            for j in range(9):
                self.xoDend[i].append({
                    'XO(' + str(i) + ',' + str(j) + ') Dend SynInput 0': [],
                    'XO(' + str(i) + ',' + str(j) + ') Dend SynInput 1': [],
                    'XO(' + str(i) + ',' + str(j) + ') Dend Oscillator': [],
                    'XO(' + str(i) + ',' + str(j) + ') Dend Backprop': [],
                    'XO(' + str(i) + ',' + str(j) + ') Dend Sum': [],
                    'XO(' + str(i) + ',' + str(j) + ') Dend Output': [],
                    'XO(' + str(i) + ',' + str(j) + ') Dend Spike': []})

            self.mDend.append({
                'M' + str(i) + ' Dend SynInput 0': [],
                'M' + str(i) + ' Dend SynInput 1': [],
                'M' + str(i) + ' Dend SynInput 2': [],
                'M' + str(i) + ' Dend SynInput 3': [],
                'M' + str(i) + ' Dend SynInput 4': [],
                'M' + str(i) + ' Dend SynInput 5': [],
                'M' + str(i) + ' Dend SynInput 6': [],
                'M' + str(i) + ' Dend SynInput 7': [],
                'M' + str(i) + ' Dend SynInput 8': [],
                'M' + str(i) + ' Dend Oscillator': [],
                'M' + str(i) + ' Dend Backprop': [],
                'M' + str(i) + ' Dend Sum': [],
                'M' + str(i) + ' Dend Output': [],
                'M' + str(i) + ' Dend Spike': []})

            self.axon.append({
                'Axon' + str(i) + ' Oscillator': [],
                'Axon' + str(i) + ' Backprop': [],
                'Axon' + str(i) + ' Sum': [],
                'Axon' + str(i) + ' Output': [],
                'Axon' + str(i) + ' Spike': [],
                'Axon' + str(i) + ' SynOutput 0': []})
        
        self.recordInputs = {
            'Time': False,
            'Turn': False,
            'Arti': False,
            'Human': False,
            'Move #': False,
            'Square 1': False,
            'Square 2': False,
            'Square 3': False,
            'Square 4': False,
            'Square 5': False,
            'Square 6': False,
            'Square 7': False,
            'Square 8': False,
            'Square 9': False}
        self.recordXNeurons = []
        self.recordONeurons = []
        self.recordMNeurons = []
        self.recordTNeuron = {
            'Turn SynInput 0': False,
            'Turn SynInput 1': False,
            'Turn Oscillator': False,
            'Turn Backprop': False,
            'Turn Sum': False,
            'Turn Output': False,
            'Turn Spike': False}
        self.recordXODends = [[], [], [], [], [], [], [], [], []]
        self.recordMDends = []
        self.recordTDend = {
            'Turn Dend SynInput 0': False,
            'Turn Dend Oscillator': False,
            'Turn Dend Backprop': False,
            'Turn Dend Sum': False,
            'Turn Dend Output': False,
            'Turn Dend Spike': False}
        self.recordSoma = {
            'Soma Oscillator': False,
            'Soma Backprop': False,
            'Soma Sum': False,
            'Soma Output': False,
            'Soma Spike': False}
        self.recordAxons = []

        for i in range(9):
            self.recordXNeurons.append({
                'X' + str(i) + ' SynInput 0': False,
                'X' + str(i) + ' Oscillator': False,
                'X' + str(i) + ' Backprop': False,
                'X' + str(i) + ' Sum': False,
                'X' + str(i) + ' Output': False,
                'X' + str(i) + ' Spike': False})
            self.recordONeurons.append({
                'O' + str(i) + ' SynInput 0': False,
                'O' + str(i) + ' Oscillator': False,
                'O' + str(i) + ' Backprop': False,
                'O' + str(i) + ' Sum': False,
                'O' + str(i) + ' Output': False,
                'O' + str(i) + ' Spike': False})
            self.recordMNeurons.append({
                'M' + str(i) + ' SynInput 0': False,
                'M' + str(i) + ' Oscillator': False,
                'M' + str(i) + ' Backprop': False,
                'M' + str(i) + ' Sum': False,
                'M' + str(i) + ' Output': False,
                'M' + str(i) + ' Spike': False})

            for j in range(9):
                self.recordXODends[i].append({
                    'XO(' + str(i) + ',' + str(j) + ') Dend SynInput 0': False,
                    'XO(' + str(i) + ',' + str(j) + ') Dend SynInput 1': False,
                    'XO(' + str(i) + ',' + str(j) + ') Dend Oscillator': False,
                    'XO(' + str(i) + ',' + str(j) + ') Dend Backprop': False,
                    'XO(' + str(i) + ',' + str(j) + ') Dend Sum': False,
                    'XO(' + str(i) + ',' + str(j) + ') Dend Output': False,
                    'XO(' + str(i) + ',' + str(j) + ') Dend Spike': False})

            self.recordMDends.append({
                'M' + str(i) + ' Dend SynInput 0': False,
                'M' + str(i) + ' Dend SynInput 1': False,
                'M' + str(i) + ' Dend SynInput 2': False,
                'M' + str(i) + ' Dend SynInput 3': False,
                'M' + str(i) + ' Dend SynInput 4': False,
                'M' + str(i) + ' Dend SynInput 5': False,
                'M' + str(i) + ' Dend SynInput 6': False,
                'M' + str(i) + ' Dend SynInput 7': False,
                'M' + str(i) + ' Dend SynInput 8': False,
                'M' + str(i) + ' Dend Oscillator': False,
                'M' + str(i) + ' Dend Backprop': False,
                'M' + str(i) + ' Dend Sum': False,
                'M' + str(i) + ' Dend Output': False,
                'M' + str(i) + ' Dend Spike': False})

            self.recordAxons.append({
                'Axon' + str(i) + ' Oscillator': False,
                'Axon' + str(i) + ' Backprop': False,
                'Axon' + str(i) + ' Sum': False,
                'Axon' + str(i) + ' Output': False,
                'Axon' + str(i) + ' Spike': False,
                'Axon' + str(i) + ' SynOutput 0': False})

    """
    This function compiles the recorded data into a dataframe that is written to an Excel file, if any logging is being performed.
    self: object reference
    pbar: the progressbar tracking the completion of the compilation of the datalog
    filename: the name of the Excel file the log is saved to
    """
    def compileLog(self, pbar, filename='arti_1_0.xlsx'):
        if not self.nolog:
            cl = []
            dfl = []
            df = pd.DataFrame()
            progress = 0.0

            if self.recordInputs['Time'] is True:
                cl.append('Time')
                dfl.append(pd.DataFrame(self.inputs['Time']))
            if self.recordInputs['Turn'] is True:
                cl.append('Turn')
                dfl.append(pd.DataFrame(self.inputs['Turn']))
            if self.recordInputs['Arti'] is True:
                cl.append('Arti')
                dfl.append(pd.DataFrame(self.inputs['Arti']))
            if self.recordInputs['Human'] is True:
                cl.append('Human')
                dfl.append(pd.DataFrame(self.inputs['Human']))
            if self.recordInputs['Move #'] is True:
                cl.append('Move #')
                dfl.append(pd.DataFrame(self.inputs['Move #']))
            for i in range(9):
                if self.recordInputs['Square ' + str(i + 1)] is True:
                    cl.append('Square' + str(i + 1))
                    dfl.append(pd.DataFrame(self.inputs['Square ' + str(i + 1)]))

            progress += 1.0 / 200.0
            pbar.setProgress(progress * 100)

            if self.recordTNeuron['Turn SynInput 0'] is True:
                cl.append('Turn SynInput 0')
                dfl.append(pd.DataFrame(self.tNeuron['Turn SynInput 0']))
            if self.recordTNeuron['Turn SynInput 1'] is True:
                cl.append('Turn SynInput 1')
                dfl.append(pd.DataFrame(self.tNeuron['Turn SynInput 1']))
            if self.recordTNeuron['Turn Oscillator'] is True:
                cl.append('Turn Oscillator')
                dfl.append(pd.DataFrame(self.tNeuron['Turn Oscillator']))
            if self.recordTNeuron['Turn Backprop'] is True:
                cl.append('Turn Backprop')
                dfl.append(pd.DataFrame(self.tNeuron['Turn Backprop']))
            if self.recordTNeuron['Turn Sum'] is True:
                cl.append('Turn Sum')
                dfl.append(pd.DataFrame(self.tNeuron['Turn Sum']))
            if self.recordTNeuron['Turn Output'] is True:
                cl.append('Turn Output')
                dfl.append(pd.DataFrame(self.tNeuron['Turn Output']))
            if self.recordTNeuron['Turn Spike'] is True:
                cl.append('Turn Spike')
                dfl.append(pd.DataFrame(self.tNeuron['Turn Spike']))

            progress += 1.0 / 200.0
            pbar.setProgress(progress * 100)

            for i in range(9):
                if self.recordXNeurons[i]['X' + str(i) + ' SynInput 0'] is True:
                    cl.append('X' + str(i) + ' SynInput 0')
                    dfl.append(pd.DataFrame(self.xNeuron[i]['X' + str(i) + ' SynInput 0']))
                if self.recordXNeurons[i]['X' + str(i) + ' Oscillator'] is True:
                    cl.append('X' + str(i) + ' Oscillator')
                    dfl.append(pd.DataFrame(self.xNeuron[i]['X' + str(i) + ' Oscillator']))
                if self.recordXNeurons[i]['X' + str(i) + ' Backprop'] is True:
                    cl.append('X' + str(i) + ' Backprop')
                    dfl.append(pd.DataFrame(self.xNeuron[i]['X' + str(i) + ' Backprop']))
                if self.recordXNeurons[i]['X' + str(i) + ' Sum'] is True:
                    cl.append('X' + str(i) + ' Sum')
                    dfl.append(pd.DataFrame(self.xNeuron[i]['X' + str(i) + ' Sum']))
                if self.recordXNeurons[i]['X' + str(i) + ' Output'] is True:
                    cl.append('X' + str(i) + ' Output')
                    dfl.append(pd.DataFrame(self.xNeuron[i]['X' + str(i) + ' Output']))
                if self.recordXNeurons[i]['X' + str(i) + ' Spike'] is True:
                    cl.append('X' + str(i) + ' Spike')
                    dfl.append(pd.DataFrame(self.xNeuron[i]['X' + str(i) + ' Spike']))

                progress += 1.0 / 200.0
                pbar.setProgress(progress * 100)

                if self.recordONeurons[i]['O' + str(i) + ' SynInput 0'] is True:
                    cl.append('O' + str(i) + ' SynInput 0')
                    dfl.append(pd.DataFrame(self.oNeuron[i]['O' + str(i) + ' SynInput 0']))
                if self.recordONeurons[i]['O' + str(i) + ' Oscillator'] is True:
                    cl.append('O' + str(i) + ' Oscillator')
                    dfl.append(pd.DataFrame(self.oNeuron[i]['O' + str(i) + ' Oscillator']))
                if self.recordONeurons[i]['O' + str(i) + ' Backprop'] is True:
                    cl.append('O' + str(i) + ' Backprop')
                    dfl.append(pd.DataFrame(self.oNeuron[i]['O' + str(i) + ' Backprop']))
                if self.recordONeurons[i]['O' + str(i) + ' Sum'] is True:
                    cl.append('O' + str(i) + ' Sum')
                    dfl.append(pd.DataFrame(self.oNeuron[i]['O' + str(i) + ' Sum']))
                if self.recordONeurons[i]['O' + str(i) + ' Output'] is True:
                    cl.append('O' + str(i) + ' Output')
                    dfl.append(pd.DataFrame(self.oNeuron[i]['O' + str(i) + ' Output']))
                if self.recordONeurons[i]['O' + str(i) + ' Spike'] is True:
                    cl.append('O' + str(i) + ' Spike')
                    dfl.append(pd.DataFrame(self.oNeuron[i]['O' + str(i) + ' Spike']))

                progress += 1.0 / 200.0
                pbar.setProgress(progress * 100)

                if self.recordMNeurons[i]['M' + str(i) + ' SynInput 0'] is True:
                    cl.append('M' + str(i) + ' SynInput 0')
                    dfl.append(pd.DataFrame(self.mNeuron[i]['M' + str(i) + ' SynInput 0']))
                if self.recordMNeurons[i]['M' + str(i) + ' Oscillator'] is True:
                    cl.append('M' + str(i) + ' Oscillator')
                    dfl.append(pd.DataFrame(self.mNeuron[i]['M' + str(i) + ' Oscillator']))
                if self.recordMNeurons[i]['M' + str(i) + ' Backprop'] is True:
                    cl.append('M' + str(i) + ' Backprop')
                    dfl.append(pd.DataFrame(self.mNeuron[i]['M' + str(i) + ' Backprop']))
                if self.recordMNeurons[i]['M' + str(i) + ' Sum'] is True:
                    cl.append('M' + str(i) + ' Sum')
                    dfl.append(pd.DataFrame(self.mNeuron[i]['M' + str(i) + ' Sum']))
                if self.recordMNeurons[i]['M' + str(i) + ' Output'] is True:
                    cl.append('M' + str(i) + ' Output')
                    dfl.append(pd.DataFrame(self.mNeuron[i]['M' + str(i) + ' Output']))
                if self.recordMNeurons[i]['M' + str(i) + ' Spike'] is True:
                    cl.append('M' + str(i) + ' Spike')
                    dfl.append(pd.DataFrame(self.mNeuron[i]['M' + str(i) + ' Spike']))

                progress += 1.0 / 200.0
                pbar.setProgress(progress * 100)

                for j in range(9):
                    if self.recordXODends[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend SynInput 0'] is True:
                        cl.append('XO(' + str(i) + ',' + str(j) + ') Dend SynInput 0')
                        dfl.append(pd.DataFrame(self.xoDend[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend SynInput 0']))
                    if self.recordXODends[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend SynInput 1'] is True:
                        cl.append('XO(' + str(i) + ',' + str(j) + ') Dend SynInput 1')
                        dfl.append(pd.DataFrame(self.xoDend[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend SynInput 1']))
                    if self.recordXODends[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend Oscillator'] is True:
                        cl.append('XO(' + str(i) + ',' + str(j) + ') Dend Oscillator')
                        dfl.append(pd.DataFrame(self.xoDend[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend Oscillator']))
                    if self.recordXODends[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend Backprop'] is True:
                        cl.append('XO(' + str(i) + ',' + str(j) + ') Dend Backprop')
                        dfl.append(pd.DataFrame(self.xoDend[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend Backprop']))
                    if self.recordXODends[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend Sum'] is True:
                        cl.append('XO(' + str(i) + ',' + str(j) + ') Dend Sum')
                        dfl.append(pd.DataFrame(self.xoDend[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend Sum']))
                    if self.recordXODends[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend Output'] is True:
                        cl.append('XO(' + str(i) + ',' + str(j) + ') Dend Output')
                        dfl.append(pd.DataFrame(self.xoDend[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend Output']))
                    if self.recordXODends[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend Spike'] is True:
                        cl.append('XO(' + str(i) + ',' + str(j) + ') Dend Spike')
                        dfl.append(pd.DataFrame(self.xoDend[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend Spike']))

                    progress += 1.0 / 200.0
                    pbar.setProgress(progress * 100)

                if self.recordMDends[i]['M' + str(i) + ' Dend SynInput 0'] is True:
                    cl.append('M' + str(i) + ' Dend SynInput 0')
                    dfl.append(pd.DataFrame(self.mDend[i]['M' + str(i) + ' Dend SynInput 0']))
                if self.recordMDends[i]['M' + str(i) + ' Dend SynInput 1'] is True:
                    cl.append('M' + str(i) + ' Dend SynInput 1')
                    dfl.append(pd.DataFrame(self.mDend[i]['M' + str(i) + ' Dend SynInput 1']))
                if self.recordMDends[i]['M' + str(i) + ' Dend SynInput 2'] is True:
                    cl.append('M' + str(i) + ' Dend SynInput 2')
                    dfl.append(pd.DataFrame(self.mDend[i]['M' + str(i) + ' Dend SynInput 2']))
                if self.recordMDends[i]['M' + str(i) + ' Dend SynInput 3'] is True:
                    cl.append('M' + str(i) + ' Dend SynInput 3')
                    dfl.append(pd.DataFrame(self.mDend[i]['M' + str(i) + ' Dend SynInput 3']))
                if self.recordMDends[i]['M' + str(i) + ' Dend SynInput 4'] is True:
                    cl.append('M' + str(i) + ' Dend SynInput 4')
                    dfl.append(pd.DataFrame(self.mDend[i]['M' + str(i) + ' Dend SynInput 4']))
                if self.recordMDends[i]['M' + str(i) + ' Dend SynInput 5'] is True:
                    cl.append('M' + str(i) + ' Dend SynInput 5')
                    dfl.append(pd.DataFrame(self.mDend[i]['M' + str(i) + ' Dend SynInput 5']))
                if self.recordMDends[i]['M' + str(i) + ' Dend SynInput 6'] is True:
                    cl.append('M' + str(i) + ' Dend SynInput 6')
                    dfl.append(pd.DataFrame(self.mDend[i]['M' + str(i) + ' Dend SynInput 6']))
                if self.recordMDends[i]['M' + str(i) + ' Dend SynInput 7'] is True:
                    cl.append('M' + str(i) + ' Dend SynInput 7')
                    dfl.append(pd.DataFrame(self.mDend[i]['M' + str(i) + ' Dend SynInput 7']))
                if self.recordMDends[i]['M' + str(i) + ' Dend SynInput 8'] is True:
                    cl.append('M' + str(i) + ' Dend SynInput 8')
                    dfl.append(pd.DataFrame(self.mDend[i]['M' + str(i) + ' Dend SynInput 8']))
                if self.recordMDends[i]['M' + str(i) + ' Dend Oscillator'] is True:
                    cl.append('M' + str(i) + ' Dend Oscillator')
                    dfl.append(pd.DataFrame(self.mDend[i]['M' + str(i) + ' Dend Oscillator']))
                if self.recordMDends[i]['M' + str(i) + ' Dend Backprop'] is True:
                    cl.append('M' + str(i) + ' Dend Backprop')
                    dfl.append(pd.DataFrame(self.mDend[i]['M' + str(i) + ' Dend Backprop']))
                if self.recordMDends[i]['M' + str(i) + ' Dend Sum'] is True:
                    cl.append('M' + str(i) + ' Dend Sum')
                    dfl.append(pd.DataFrame(self.mDend[i]['M' + str(i) + ' Dend Sum']))
                if self.recordMDends[i]['M' + str(i) + ' Dend Output'] is True:
                    cl.append('M' + str(i) + ' Dend Output')
                    dfl.append(pd.DataFrame(self.mDend[i]['M' + str(i) + ' Dend Output']))
                if self.recordMDends[i]['M' + str(i) + ' Dend Spike'] is True:
                    cl.append('M' + str(i) + ' Dend Spike')
                    dfl.append(pd.DataFrame(self.mDend[i]['M' + str(i) + ' Dend Spike']))

                progress += 1.0 / 200.0
                pbar.setProgress(progress * 100)
                    
                if self.recordAxons[i]['Axon' + str(i) + ' Oscillator'] is True:
                    cl.append('Axon' + str(i) + ' Oscillator')
                    dfl.append(pd.DataFrame(self.axon[i]['Axon' + str(i) + ' Oscillator']))
                if self.recordAxons[i]['Axon' + str(i) + ' Backprop'] is True:
                    cl.append('Axon' + str(i) + ' Backprop')
                    dfl.append(pd.DataFrame(self.axon[i]['Axon' + str(i) + ' Backprop']))
                if self.recordAxons[i]['Axon' + str(i) + ' Sum'] is True:
                    cl.append('Axon' + str(i) + ' Sum')
                    dfl.append(pd.DataFrame(self.axon[i]['Axon' + str(i) + ' Sum']))
                if self.recordAxons[i]['Axon' + str(i) + ' Output'] is True:
                    cl.append('Axon' + str(i) + ' Output')
                    dfl.append(pd.DataFrame(self.axon[i]['Axon' + str(i) + ' Output']))
                if self.recordAxons[i]['Axon' + str(i) + ' Spike'] is True:
                    cl.append('Axon' + str(i) + ' Spike')
                    dfl.append(pd.DataFrame(self.axon[i]['Axon' + str(i) + ' Spike']))
                if self.recordAxons[i]['Axon' + str(i) + ' SynOutput 0'] is True:
                    cl.append('Axon' + str(i) + ' SynOutput 0')
                    dfl.append(pd.DataFrame(self.axon[i]['Axon' + str(i) + ' SynOutput 0']))

                progress += 1.0 / 200.0
                pbar.setProgress(progress * 100)

            if self.recordTDend['Turn Dend SynInput 0'] is True:
                cl.append('Turn Dend SynInput 0')
                dfl.append(pd.DataFrame(self.tDend['Turn Dend SynInput 0']))
            if self.recordTDend['Turn Dend Oscillator'] is True:
                cl.append('Turn Dend Oscillator')
                dfl.append(pd.DataFrame(self.tDend['Turn Dend Oscillator']))
            if self.recordTDend['Turn Dend Backprop'] is True:
                cl.append('Turn Dend Backprop')
                dfl.append(pd.DataFrame(self.tDend['Turn Dend Backprop']))
            if self.recordTDend['Turn Dend Sum'] is True:
                cl.append('Turn Dend Sum')
                dfl.append(pd.DataFrame(self.tDend['Turn Dend Sum']))
            if self.recordTDend['Turn Dend Output'] is True:
                cl.append('Turn Dend Output')
                dfl.append(pd.DataFrame(self.tDend['Turn Dend Output']))
            if self.recordTDend['Turn Dend Spike'] is True:
                cl.append('Turn Dend Spike')
                dfl.append(pd.DataFrame(self.tDend['Turn Dend Spike']))

            progress += 1.0 / 200.0
            pbar.setProgress(progress * 100)

            if self.recordSoma['Soma Oscillator'] is True:
                cl.append('Soma Oscillator')
                dfl.append(pd.DataFrame(self.soma['Soma Oscillator']))
            if self.recordSoma['Soma Backprop'] is True:
                cl.append('Soma Backprop')
                dfl.append(pd.DataFrame(self.soma['Soma Backprop']))
            if self.recordSoma['Soma Sum'] is True:
                cl.append('Soma Sum')
                dfl.append(pd.DataFrame(self.soma['Soma Sum']))
            if self.recordSoma['Soma Output'] is True:
                cl.append('Soma Output')
                dfl.append(pd.DataFrame(self.soma['Soma Output']))
            if self.recordSoma['Soma Spike'] is True:
                cl.append('Soma Spike')
                dfl.append(pd.DataFrame(self.soma['Soma Spike']))

            progress += 1.0 / 200.0
            pbar.setProgress(progress * 100)

            df = pd.concat(dfl, axis=1)
            df.columns = cl
            writer = pd.ExcelWriter(filename, engine='xlsxwriter')
            df.to_excel(writer, sheet_name='Sheet 1')
            writer.close()

            progress += 70.0 / 200.0
            pbar.setProgress(progress * 100)
            pbar.done(0)

    """
    This function marks the specified data item to be logged or not logged.
    self: object reference
    name: the name of the data item
    value: true if logging the data item, otherwise false
    """
    def log(self, name, value=True):
        if value == True:
            self.nolog = False

        for key in self.recordInputs:
            if key == name:
                self.recordInputs.update(name, value)
                return True

        for key in self.recordTNeuron:
            if key == name:
                self.recordTNeuron.update(name, value)
                return True

        for key in self.recordTDend:
            if key == name:
                self.recordTDend.update(name, value)
                return True

        for key in self.recordSoma:
            if key == name:
                self.recordSoma.update(name, value)
                return True

        for i in range(9):
            for key in self.recordXNeurons[i]:
                if key == name:
                    self.recordXNeurons[i].update(name, value)
                    return True

            for key in self.recordONeurons[i]:
                if key == name:
                    self.recordONeurons[i].update(name, value)
                    return True

            for key in self.recordMNeurons[i]:
                if key == name:
                    self.recordMNeurons[i].update(name, value)
                    return True

            for j in range(9):
                for key in self.recordXODends[i][j]:
                    if key == name:
                        self.recordXODends[i][j].update(name, value)
                        return True

            for key in self.recordMDends[i]:
                if key == name:
                    self.recordMDends[i].update(name, value)
                    return True

            for key in self.recordAxons[i]:
                if key == name:
                    self.recordAxons[i].update(name, value)
                    return True

        return False

    """
    This function marks all data items to be logged.
    self: object reference
    """
    def logAll(self):
        self.nolog = False
        for key in self.recordInputs:
            self.recordInputs.update({key: True})
        for key in self.recordSoma:
            self.recordSoma.update({key: True})
        for key in self.recordTDend:
            self.recordTDend.update({key: True})
        for key in self.recordTNeuron:
            self.recordTNeuron.update({key: True})
        for i in range(9):
            for key in self.recordAxons[i]:
                self.recordAxons[i].update({key: True})
            for key in self.recordMDends[i]:
                self.recordMDends[i].update({key: True})
            for key in self.recordMNeurons[i]:
                self.recordMNeurons[i].update({key: True})
            for key in self.recordONeurons[i]:
                self.recordONeurons[i].update({key: True})
            for key in self.recordXNeurons[i]:
                self.recordXNeurons[i].update({key: True})
            for j in range(9):
                for key in self.recordXODends[i][j]:
                    self.recordXODends[i][j].update({key: True})

    """
    This functions marks the specified axon data to be logged or not logged.
    self: object reference
    index: index of the axon
    value: true if the axon is to be logged, otherwise false
    """
    def logAxon(self, index, value=True):
        if value == True:
            self.nolog = False

        for key in self.recordAxons[index]:
            self.recordAxons[index].update({key: value})

    """
    This functions marks all axon backpropagation data to be logged or not logged.
    self: object reference
    value: true if all axon backpropagation data is to be logged, otherwise false
    """
    def logAxonBackpropagations(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordAxons[i].update({'Axon' + str(i) + ' Backprop': value})

    """
    This functions marks all axon oscillator data to be logged or not logged.
    self: object reference
    value: true if all axon oscillator data is to be logged, otherwise false
    """
    def logAxonOscillators(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordAxons[i].update({'Axon' + str(i) + ' Oscillator': value})

    """
    This functions marks all axon output data to be logged or not logged.
    self: object reference
    value: true if all axon output data is to be logged, otherwise false
    """
    def logAxonOutputs(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordAxons[i].update({'Axon' + str(i) + ' Output': value})


    """
    This functions marks all axon data to be logged or not logged.
    self: object reference
    value: true if all axon data is to be logged, otherwise false
    """
    def logAxons(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            for key in self.recordAxons[i]:
                self.recordAxons[i].update({key: value})

    """
    This functions marks all axon spike data to be logged or not logged.
    self: object reference
    value: true if all axon spike data is to be logged, otherwise false
    """
    def logAxonSpikes(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordAxons[i].update({'Axon' + str(i) + ' Spike': value})

    """
    This functions marks all axon sum data to be logged or not logged.
    self: object reference
    value: true if all axon sum data is to be logged, otherwise false
    """
    def logAxonSums(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordAxons[i].update({'Axon' + str(i) + ' Sum': value})

    """
    This functions marks all axon synaptic output data to be logged or not logged.
    self: object reference
    value: true if all axon synaptic output data is to be logged, otherwise false
    """
    def logAxonSynapticOutputs(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordAxons[i].update({'Axon' + str(i) + ' SynOutput 0': value})
            
    """
    This functions marks all input data to be logged or not logged.
    self: object reference
    value: true if all input data is to be logged, otherwise false
    """
    def logInputs(self, value=True):
        if value == True:
            self.nolog = False

        for key in self.recordInputs:
            self.recordInputs.update({key: value})

    """
    This functions marks the specified move dendrite data to be logged or not logged.
    self: object reference
    index: index of the move dendrite
    value: true if the move dendrite data is to be logged, otherwise false
    """
    def logMDend(self, index, value=True):
        if value == True:
            self.nolog = False

        for key in self.recordMDends[index]:
            self.recordMDends[index].update({key: value})

    """
    This functions marks all move dendrite backpropagation data to be logged or not logged.
    self: object reference
    value: true if all move dendrite backpropagation data is to be logged, otherwise false
    """
    def logMDendBackpropagations(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordMDends[i].update({'M' + str(i) + ' Dend Backprop': value})

    """
    This functions marks all move dendrite oscillator data to be logged or not logged.
    self: object reference
    value: true if all move dendrite oscillator data is to be logged, otherwise false
    """
    def logMDendOscillators(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordMDends[i].update({'M' + str(i) + ' Dend Oscillator': value})

    """
    This functions marks all move dendrite output data to be logged or not logged.
    self: object reference
    value: true if all move dendrite output data is to be logged, otherwise false
    """
    def logMDendOutputs(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordMDends[i].update({'M' + str(i) + ' Dend Output': value})

    """
    This functions marks all move dendrite data to be logged or not logged.
    self: object reference
    value: true if all move dendrite data is to be logged, otherwise false
    """
    def logMDends(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            for key in self.recordMDends[i]:
                self.recordMDends[i].update({key: value})

    """
    This functions marks all move dendrite spike data to be logged or not logged.
    self: object reference
    value: true if all move dendrite spike data is to be logged, otherwise false
    """
    def logMDendSpikes(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordMDends[i].update({'M' + str(i) + ' Dend Spike': value})

    """
    This functions marks all move dendrite sum data to be logged or not logged.
    self: object reference
    value: true if all move dendrite sum data is to be logged, otherwise false
    """
    def logMDendSums(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordMDends[i].update({'M' + str(i) + ' Dend Sum': value})

    """
    This functions marks the specified move dendrite synaptic input data to be logged or not logged.
    self: object reference
    index: index of the move dendrite synaptic input
    value: true if the move dendrite synaptic input data is to be logged, otherwise false
    """
    def logMDendSynapticInputs(self, index, value=True):
        if value == True:
            self.nolog = False
            
        self.recordMDends[index].update({'M' + str(index) + ' Dend SynInput 0': value})
        self.recordMDends[index].update({'M' + str(index) + ' Dend SynInput 1': value})
        self.recordMDends[index].update({'M' + str(index) + ' Dend SynInput 2': value})
        self.recordMDends[index].update({'M' + str(index) + ' Dend SynInput 3': value})
        self.recordMDends[index].update({'M' + str(index) + ' Dend SynInput 4': value})
        self.recordMDends[index].update({'M' + str(index) + ' Dend SynInput 5': value})
        self.recordMDends[index].update({'M' + str(index) + ' Dend SynInput 6': value})
        self.recordMDends[index].update({'M' + str(index) + ' Dend SynInput 7': value})
        self.recordMDends[index].update({'M' + str(index) + ' Dend SynInput 8': value})
        
    """
    This functions marks all move dendrite synaptic input data to be logged or not logged.
    self: object reference
    value: true if all move dendrite synaptic input data is to be logged, otherwise false
    """
    def logMDendSynapticInputs(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordMDends[i].update({'M' + str(i) + ' Dend SynInput 0': value})
            self.recordMDends[i].update({'M' + str(i) + ' Dend SynInput 1': value})
            self.recordMDends[i].update({'M' + str(i) + ' Dend SynInput 2': value})
            self.recordMDends[i].update({'M' + str(i) + ' Dend SynInput 3': value})
            self.recordMDends[i].update({'M' + str(i) + ' Dend SynInput 4': value})
            self.recordMDends[i].update({'M' + str(i) + ' Dend SynInput 5': value})
            self.recordMDends[i].update({'M' + str(i) + ' Dend SynInput 6': value})
            self.recordMDends[i].update({'M' + str(i) + ' Dend SynInput 7': value})
            self.recordMDends[i].update({'M' + str(i) + ' Dend SynInput 8': value})

    """
    This functions marks the specified move neuron data to be logged or not logged.
    self: object reference
    index: index of the move neuron
    value: true if the move neuron data is to be logged, otherwise false
    """
    def logMNeuron(self, index, value=True):
        if value == True:
            self.nolog = False

        for key in self.recordMNeurons[index]:
            self.recordMNeurons[index].update({key: value})

    """
    This functions marks all move neuron backpropagation data to be logged or not logged.
    self: object reference
    value: true if all move neuron backpropagation data is to be logged, otherwise false
    """
    def logMNeuronBackpropagations(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordMDends[i].update({'M' + str(i) + ' Backprop': value})

    """
    This functions marks all move neuron oscillator data to be logged or not logged.
    self: object reference
    value: true if all move neuron oscillator data is to be logged, otherwise false
    """
    def logMNeuronOscillators(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordMDends[i].update({'M' + str(i) + ' Oscillator': value})

    """
    This functions marks all move neuron output data to be logged or not logged.
    self: object reference
    value: true if all move neuron output data is to be logged, otherwise false
    """
    def logMNeuronOutputs(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordMDends[i].update({'M' + str(i) + ' Output': value})

    """
    This functions marks all move neuron data to be logged or not logged.
    self: object reference
    value: true if all move neuron data is to be logged, otherwise false
    """
    def logMNeurons(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            for key in self.recordMNeurons[i]:
                self.recordMNeurons[i].update({key: value})

    """
    This functions marks all move neuron spike data to be logged or not logged.
    self: object reference
    value: true if all move neuron spike data is to be logged, otherwise false
    """
    def logMNeuronSpikes(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordMDends[i].update({'M' + str(i) + ' Spike': value})

    """
    This functions marks all move neuron sum data to be logged or not logged.
    self: object reference
    value: true if all move neuron sum data is to be logged, otherwise false
    """
    def logMNeuronSums(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordMDends[i].update({'M' + str(i) + ' Sum': value})

    """
    This functions marks all move neuron synaptic input data to be logged or not logged.
    self: object reference
    value: true if all move neuron synaptic input data is to be logged, otherwise false
    """
    def logMNeuronSynapticInputs(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordMDends[i].update({'M' + str(i) + ' SynInput 0': value})

    """
    This functions marks the specified O neuron data to be logged or not logged.
    self: object reference
    index: index of the O neuron
    value: true if the O neuron data is to be logged, otherwise false
    """
    def logONeuron(self, index, value=True):
        if value == True:
            self.nolog = False

        for key in self.recordONeurons[index]:
            self.recordONeurons[index].update({key: value})

    """
    This functions marks all O neuron backpropagation data to be logged or not logged.
    self: object reference
    value: true if all O neuron backpropagation data is to be logged, otherwise false
    """
    def logONeuronBackpropagations(self, value=True):
        if value == True:
            self.nolog = False
            
        for i in range(9):
            self.recordONeurons[i].update({'O' + str(i) + ' Backprop': value})

    """
    This functions marks all O neuron oscillator data to be logged or not logged.
    self: object reference
    value: true if all O neuron oscillator data is to be logged, otherwise false
    """
    def logONeuronOscillators(self, value=True):
        if value == True:
            self.nolog = False
           
        for i in range(9):
            self.recordONeurons[i].update({'O' + str(i) + ' Oscillator': value})

    """
    This functions marks all O neuron output data to be logged or not logged.
    self: object reference
    value: true if all O neuron output data is to be logged, otherwise false
    """
    def logONeuronOutputs(self, value=True):
        if value == True:
            self.nolog = False
            
        for i in range(9):
            self.recordONeurons[i].update({'O' + str(i) + ' Output': value})

    """
    This functions marks all O neuron data to be logged or not logged.
    self: object reference
    value: true if all O neuron data is to be logged, otherwise false
    """
    def logONeurons(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            for key in self.recordONeurons[i]:
                self.recordONeurons[i].update({key: value})

    """
    This functions marks all O neuron spike data to be logged or not logged.
    self: object reference
    value: true if all O neuron spike data is to be logged, otherwise false
    """
    def logONeuronSpikes(self, value=True):
        if value == True:
            self.nolog = False
            
        for i in range(9):
            self.recordONeurons[i].update({'O' + str(i) + ' Spike': value})

    """
    This functions marks all O neuron sum data to be logged or not logged.
    self: object reference
    value: true if all O neuron sum data is to be logged, otherwise false
    """
    def logONeuronSums(self, value=True):
        if value == True:
            self.nolog = False
            
        for i in range(9):
            self.recordONeurons[i].update({'O' + str(i) + ' Sum': value})

    """
    This functions marks all O neuron synaptic input data to be logged or not logged.
    self: object reference
    value: true if all O neuron synaptic input data is to be logged, otherwise false
    """
    def logONeuronSynapticInputs(self, value=True):
        if value == True:
            self.nolog = False
            
        for i in range(9):
            self.recordONeurons[i].update({'O' + str(i) + ' SynInput 0': value})

    """
    This functions marks all soma data to be logged or not logged.
    self: object reference
    value: true if all soma data is to be logged, otherwise false
    """
    def logSoma(self, value=True):
        if value == True:
            self.nolog = False

        for key in self.recordSoma:
            self.recordSoma.update({key: value})

    """
    This functions marks the soma backpropagation data to be logged or not logged.
    self: object reference
    value: true if the soma backpropagation data is to be logged, otherwise false
    """
    def logSomaBackpropagation(self, value=True):
        if value == True:
            self.nolog = False
            
        self.recordSoma.update({'Soma Backprop': value})

    """
    This functions marks the soma oscillator data to be logged or not logged.
    self: object reference
    value: true if the soma oscillator data is to be logged, otherwise false
    """
    def logSomaOscillator(self, value=True):
        if value == True:
            self.nolog = False
            
        self.recordSoma.update({'Soma Oscillator': value})

    """
    This functions marks the soma output data to be logged or not logged.
    self: object reference
    value: true if the soma output data is to be logged, otherwise false
    """
    def logSomaOutput(self, value=True):
        if value == True:
            self.nolog = False
            
        self.recordSoma.update({'Soma Output': value})

    """
    This functions marks the soma spike data to be logged or not logged.
    self: object reference
    value: true if the soma spike data is to be logged, otherwise false
    """
    def logSomaSpike(self, value=True):
        if value == True:
            self.nolog = False
            
        self.recordSoma.update({'Soma Spike': value})

    """
    This functions marks the soma sum data to be logged or not logged.
    self: object reference
    value: true if the soma sum data is to be logged, otherwise false
    """
    def logSomaSum(self, value=True):
        if value == True:
            self.nolog = False
            
        self.recordSoma.update({'Soma Sum': value})

    """
    This functions marks all turn dendrite data to be logged or not logged.
    self: object reference
    value: true if all turn dendrite data is to be logged, otherwise false
    """
    def logTDend(self, value=True):
        if value == True:
            self.nolog = False

        for key in self.recordTDend:
            self.recordTDend.update({key: value})

    """
    This functions marks the turn dendrite backpropagation data to be logged or not logged.
    self: object reference
    value: true if the turn dendrite backpropagation data is to be logged, otherwise false
    """
    def logTDendBackpropagation(self, value=True):
        if value == True:
            self.nolog = False
            
        self.recordTDend.update({'Turn Dend Backprop': value})

    """
    This functions marks the turn dendrite oscillator data to be logged or not logged.
    self: object reference
    value: true if the turn dendrite oscillator data is to be logged, otherwise false
    """
    def logTDendOscillator(self, value=True):
        if value == True:
            self.nolog = False
            
        self.recordTDend.update({'Turn Dend Oscillator': value})

    """
    This functions marks the turn dendrite output data to be logged or not logged.
    self: object reference
    value: true if the turn dendrite output data is to be logged, otherwise false
    """
    def logTDendOutput(self, value=True):
        if value == True:
            self.nolog = False
            
        self.recordTDend.update({'Turn Dend Output': value})

    """
    This functions marks the turn dendrite spike data to be logged or not logged.
    self: object reference
    value: true if the turn dendrite spike data is to be logged, otherwise false
    """
    def logTDendSpike(self, value=True):
        if value == True:
            self.nolog = False
            
        self.recordTDend.update({'Turn Dend Spike': value})

    """
    This functions marks the turn dendrite sum data to be logged or not logged.
    self: object reference
    value: true if the turn dendrite sum data is to be logged, otherwise false
    """
    def logTDendSum(self, value=True):
        if value == True:
            self.nolog = False
            
        self.recordTDend.update({'Turn Dend Sum': value})

    """
    This functions marks the turn dendrite synaptic input data to be logged or not logged.
    self: object reference
    value: true if the turn dendrite synaptic input data is to be logged, otherwise false
    """
    def logTDendSynapticInput(self, value=True):
        if value == True:
            self.nolog = False
            
        self.recordTDend.update({'Turn Dend SynInput 0': value})

    """
    This functions marks the turn neuron data to be logged or not logged.
    self: object reference
    value: true if the turn neuron data is to be logged, otherwise false
    """
    def logTNeuron(self, value=True):
        if value == True:
            self.nolog = False

        for key in self.recordTNeuron:
            self.recordTNeuron.update({key: value})

    """
    This functions marks the turn neuron backpropagation data to be logged or not logged.
    self: object reference
    value: true if the turn neuron backpropagation data is to be logged, otherwise false
    """
    def logTNeuronBackpropagation(self, value=True):
        if value == True:
            self.nolog = False
            
        self.recordTNeuron.update({'Turn Backprop': value})

    """
    This functions marks the turn neuron oscillator data to be logged or not logged.
    self: object reference
    value: true if the turn neuron oscillator data is to be logged, otherwise false
    """
    def logTNeuronOscillator(self, value=True):
        if value == True:
            self.nolog = False
            
        self.recordTNeuron.update({'Turn Oscillator': value})

    """
    This functions marks the turn neuron output data to be logged or not logged.
    self: object reference
    value: true if the turn neuron output data is to be logged, otherwise false
    """
    def logTNeuronOutput(self, value=True):
        if value == True:
            self.nolog = False
            
        self.recordTNeuron.update({'Turn Output': value})

    """
    This functions marks the turn neuron spike data to be logged or not logged.
    self: object reference
    value: true if the turn neuron spike data is to be logged, otherwise false
    """
    def logTNeuronSpike(self, value=True):
        if value == True:
            self.nolog = False
            
        self.recordTNeuron.update({'Turn Spike': value})

    """
    This functions marks the turn neuron sum data to be logged or not logged.
    self: object reference
    value: true if the turn neuron sum data is to be logged, otherwise false
    """
    def logTNeuronSum(self, value=True):
        if value == True:
            self.nolog = False
            
        self.recordTNeuron.update({'Turn Sum': value})

    """
    This functions marks all turn neuron synaptic input data to be logged or not logged.
    self: object reference
    value: true if all turn neuron synaptic input data is to be logged, otherwise false
    """
    def logTNeuronSynapticInputs(self, value=True):
        if value == True:
            self.nolog = False
            
        self.recordTNeuron.update({'Turn SynInput 0': value})
        self.recordTNeuron.update({'Turn SynInput 1': value})

    """
    This functions marks the specified X neuron data to be logged or not logged.
    self: object reference
    index: the index of the X neuron
    value: true if the X neuron data is to be logged, otherwise false
    """
    def logXNeuron(self, index, value=True):
        if value == True:
            self.nolog = False

        for key in self.recordXNeurons[index]:
            self.recordXNeurons[index].update({key: value})

    """
    This functions marks all X neuron backpropagation data to be logged or not logged.
    self: object reference
    value: true if all X neuron backpropagation data is to be logged, otherwise false
    """
    def logXNeuronBackpropagations(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordXNeurons[i].update({'X' + str(i) + ' Backprop': value})

    """
    This functions marks all X neuron oscillator data to be logged or not logged.
    self: object reference
    value: true if all X neuron oscillator data is to be logged, otherwise false
    """
    def logXNeuronOscillators(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordXNeurons[i].update({'X' + str(i) + ' Oscillator': value})

    """
    This functions marks all X neuron output data to be logged or not logged.
    self: object reference
    value: true if all X neuron output data is to be logged, otherwise false
    """
    def logXNeuronOutputs(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordXNeurons[i].update({'X' + str(i) + ' Output': value})

    """
    This functions marks all X neuron data to be logged or not logged.
    self: object reference
    value: true if all X neuron data is to be logged, otherwise false
    """
    def logXNeurons(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            for key in self.recordXNeurons[i]:
                self.recordXNeurons[i].update({key: value})

    """
    This functions marks all X neuron spike data to be logged or not logged.
    self: object reference
    value: true if all X neuron spike data is to be logged, otherwise false
    """
    def logXNeuronSpikes(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordXNeurons[i].update({'X' + str(i) + ' Spike': value})

    """
    This functions marks all X neuron sum data to be logged or not logged.
    self: object reference
    value: true if all X neuron sum data is to be logged, otherwise false
    """
    def logXNeuronSums(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordXNeurons[i].update({'X' + str(i) + ' Sum': value})

    """
    This functions marks all X neuron synaptic input data to be logged or not logged.
    self: object reference
    value: true if all X neuron synaptic input data is to be logged, otherwise false
    """
    def logXNeuronSynapticInputs(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            self.recordXNeurons[i].update({'X' + str(i) + ' SynInput 0': value})

    """
    This functions marks the specified XO dendrite data to be logged or not logged.
    self: object reference
    moveIndex: the move number index of the XO dendrite
    index: the index of the XO dendrite
    value: true if the XO dendrite data is to be logged, otherwise false
    """
    def logXODend(self, moveIndex, index, value=True):
        if value == True:
            self.nolog = False

        for key in self.recordXODends[moveIndex][index]:
            self.recordXODends[moveIndex][index].update({key: value})

    """
    This functions marks the specified XO dendrite backpropagation data to be logged or not logged.
    self: object reference
    moveIndex: the move number index of the XO dendrite
    index: the index of the XO dendrite
    value: true if the XO dendrite backpropagation data is to be logged, otherwise false
    """
    def logXODendBackpropagations(self, moveIndex=None, index=None, value=True):
        if value == True:
            self.nolog = False

        if moveIndex is None:
            if index is None:
                for i in range(9):
                    for j in range(9):
                        self.recordXODends[i][j].update({'XO(' + str(i) + ',' + str(j) + ') Dend Backprop': value})
            else:
                for i in range(9):
                    self.recordXODends[i][index].update({'XO(' + str(i) + ',' + str(index) + ') Dend Backprop': value})
        else:
            if index is None:
                for i in range(9):
                    self.recordXODends[moveIndex][i].update({'XO(' + str(moveIndex) + ',' + str(i) + ') Dend Backprop': value})
            else:
                self.recordXODends[moveIndex][index].update({'XO(' + str(moveIndex) + ',' + str(index) + ') Dend Backprop': value})

    """
    This functions marks the specified XO dendrite oscillator data to be logged or not logged.
    self: object reference
    moveIndex: the move number index of the XO dendrite
    index: the index of the XO dendrite
    value: true if the XO dendrite oscillator data is to be logged, otherwise false
    """
    def logXODendOscillators(self, moveIndex=None, index=None, value=True):
        if value == True:
            self.nolog = False

        if moveIndex is None:
            if index is None:
                for i in range(9):
                    for j in range(9):
                        self.recordXODends[i][j].update({'XO(' + str(i) + ',' + str(j) + ') Dend Oscillator': value})
            else:
                for i in range(9):
                    self.recordXODends[i][index].update({'XO(' + str(i) + ',' + str(index) + ') Dend Oscillator': value})
        else:
            if index is None:
                for i in range(9):
                    self.recordXODends[moveIndex][i].update({'XO(' + str(moveIndex) + ',' + str(i) + ') Dend Oscillator': value})
            else:
                self.recordXODends[moveIndex][index].update({'XO(' + str(moveIndex) + ',' + str(index) + ') Dend Oscillator': value})

    """
    This functions marks the specified XO dendrite output data to be logged or not logged.
    self: object reference
    moveIndex: the move number index of the XO dendrite
    index: the index of the XO dendrite
    value: true if the XO dendrite output data is to be logged, otherwise false
    """
    def logXODendOutputs(self, moveIndex=None, index=None, value=True):
        if value == True:
            self.nolog = False

        if moveIndex is None:
            if index is None:
                for i in range(9):
                    for j in range(9):
                        self.recordXODends[i][j].update({'XO(' + str(i) + ',' + str(j) + ') Dend Output': value})
            else:
                for i in range(9):
                    self.recordXODends[i][index].update({'XO(' + str(i) + ',' + str(index) + ') Dend Output': value})
        else:
            if index is None:
                for i in range(9):
                    self.recordXODends[moveIndex][i].update({'XO(' + str(moveIndex) + ',' + str(i) + ') Dend Output': value})
            else:
                self.recordXODends[moveIndex][index].update({'XO(' + str(moveIndex) + ',' + str(index) + ') Dend Output': value})

    """
    This functions marks all XO dendrite data to be logged or not logged.
    self: object reference
    value: true if all XO dendrite data is to be logged, otherwise false
    """
    def logXODends(self, value=True):
        if value == True:
            self.nolog = False

        for i in range(9):
            for j in range(9):
                for key in self.recordXODends[i][j]:
                    self.recordXODends[i][j].update({key: value})

    """
    This functions marks the specified XO dendrite spike data to be logged or not logged.
    self: object reference
    moveIndex: the move number index of the XO dendrite
    index: the index of the XO dendrite
    value: true if the XO dendrite spike data is to be logged, otherwise false
    """
    def logXODendSpikes(self, moveIndex=None, index=None, value=True):
        if value == True:
            self.nolog = False

        if moveIndex is None:
            if index is None:
                for i in range(9):
                    for j in range(9):
                        self.recordXODends[i][j].update({'XO(' + str(i) + ',' + str(j) + ') Dend Spike': value})
            else:
                for i in range(9):
                    self.recordXODends[i][index].update({'XO(' + str(i) + ',' + str(index) + ') Dend Spike': value})
        else:
            if index is None:
                for i in range(9):
                    self.recordXODends[moveIndex][i].update({'XO(' + str(moveIndex) + ',' + str(i) + ') Dend Spike': value})
            else:
                self.recordXODends[moveIndex][index].update({'XO(' + str(moveIndex) + ',' + str(index) + ') Dend Spike': value})

    """
    This functions marks the specified XO dendrite sum data to be logged or not logged.
    self: object reference
    moveIndex: the move number index of the XO dendrite
    index: the index of the XO dendrite
    value: true if the XO dendrite sum data is to be logged, otherwise false
    """
    def logXODendSums(self, moveIndex=None, index=None, value=True):
        if value == True:
            self.nolog = False

        if moveIndex is None:
            if index is None:
                for i in range(9):
                    for j in range(9):
                        self.recordXODends[i][j].update({'XO(' + str(i) + ',' + str(j) + ') Dend Sum': value})
            else:
                for i in range(9):
                    self.recordXODends[i][index].update({'XO(' + str(i) + ',' + str(index) + ') Dend Sum': value})
        else:
            if index is None:
                for i in range(9):
                    self.recordXODends[moveIndex][i].update({'XO(' + str(moveIndex) + ',' + str(i) + ') Dend Sum': value})
            else:
                self.recordXODends[moveIndex][index].update({'XO(' + str(moveIndex) + ',' + str(index) + ') Dend Sum': value})

    """
    This functions marks the specified XO dendrite synaptic input data to be logged or not logged.
    self: object reference
    moveIndex: the move number index of the XO dendrite
    index: the index of the XO dendrite
    value: true if the XO dendrite synaptic input data is to be logged, otherwise false
    """
    def logXODendSynapticInputs(self, moveIndex=None, index=None, value=True):
        if value == True:
            self.nolog = False

        if moveIndex is None:
            if index is None:
                for i in range(9):
                    for j in range(9):
                        self.recordXODends[i][j].update({'XO(' + str(i) + ',' + str(j) + ') Dend SynInput 0': value})
                        self.recordXODends[i][j].update({'XO(' + str(i) + ',' + str(j) + ') Dend SynInput 1': value})
            else:
                for i in range(9):
                    self.recordXODends[i][index].update({'XO(' + str(i) + ',' + str(index) + ') Dend SynInput 0': value})
                    self.recordXODends[i][index].update({'XO(' + str(i) + ',' + str(index) + ') Dend SynInput 1': value})
        else:
            if index is None:
                for i in range(9):
                    self.recordXODends[moveIndex][i].update({'XO(' + str(moveIndex) + ',' + str(i) + ') Dend SynInput 0': value})
                    self.recordXODends[moveIndex][i].update({'XO(' + str(moveIndex) + ',' + str(i) + ') Dend SynInput 1': value})
            else:
                self.recordXODends[moveIndex][index].update({'XO(' + str(moveIndex) + ',' + str(index) + ') Dend SynInput 0': value})
                self.recordXODends[moveIndex][index].update({'XO(' + str(moveIndex) + ',' + str(index) + ') Dend SynInput 1': value})

    """
    This function marks all data items as to not be logged.
    self: object reference
    """
    def noLogging(self):
        self.nolog = True
        for key in self.recordInputs:
            self.recordInputs.update({key: False})
        for key in self.recordSoma:
            self.recordSoma.update({key: False})
        for key in self.recordTDend:
            self.recordTDend.update({key: False})
        for key in self.recordTNeuron:
            self.recordTNeuron.update({key: False})
        for i in range(9):
            for key in self.recordAxons[i]:
                self.recordAxons[i].update({key: False})
            for key in self.recordMDends[i]:
                self.recordMDends[i].update({key: False})
            for key in self.recordMNeurons[i]:
                self.recordMNeurons[i].update({key: False})
            for key in self.recordONeurons[i]:
                self.recordONeurons[i].update({key: False})
            for key in self.recordXNeurons[i]:
                self.recordXNeurons[i].update({key: False})
            for j in range(9):
                for key in self.recordXODends[i][j]:
                    self.recordXODends[i][j].update({key: False})

    """
    This function updates all data items marked for logging with the new data from Arti. This function is called every cycle Arti runs.
    self: object reference
    arti: Arti, the AI
    """
    def updateLog(self, arti):
        if not self.nolog:
            self.size += 1

            if self.recordInputs['Time'] is True:
                self.inputs['Time'].append(arti.time)
            if self.recordInputs['Turn'] is True:
                self.inputs['Turn'].append(arti.turn)
            if self.recordInputs['Arti'] is True:
                self.inputs['Arti'].append(arti.player)
            if self.recordInputs['Human'] is True:
                self.inputs['Human'].append('O' if arti.player == 'X' else 'X')
            if self.recordInputs['Move #'] is True:
                self.inputs['Move #'].append(arti.moveNum)
            for i in range(9):
                if self.recordInputs['Square ' + str(i + 1)] is True:
                    self.inputs['Square ' + str(i + 1)].append(arti.board[i])

            if self.recordTNeuron['Turn SynInput 0'] is True:
                self.tNeuron['Turn SynInput 0'].append(arti.tNeuron.soma.synInput[0].sum)
            if self.recordTNeuron['Turn SynInput 1'] is True:
                self.tNeuron['Turn SynInput 1'].append(arti.tNeuron.soma.synInput[1].sum)
            if self.recordTNeuron['Turn Oscillator'] is True:
                self.tNeuron['Turn Oscillator'].append(arti.tNeuron.soma.oscillator.update(arti.time))
            if self.recordTNeuron['Turn Backprop'] is True:
                self.tNeuron['Turn Backprop'].append(arti.tNeuron.soma.backpropagated)
            if self.recordTNeuron['Turn Sum'] is True:
                self.tNeuron['Turn Sum'].append(arti.tNeuron.soma.sum)
            if self.recordTNeuron['Turn Output'] is True:
                self.tNeuron['Turn Output'].append(arti.tNeuron.soma.output)
            if self.recordTNeuron['Turn Spike'] is True:
                self.tNeuron['Turn Spike'].append(arti.tNeuron.soma.spike)

            for i in range(9):
                if self.recordXNeurons[i]['X' + str(i) + ' SynInput 0'] is True:
                    self.xNeuron[i]['X' + str(i) + ' SynInput 0'].append(arti.xNeuron[i].soma.synInput[0].sum)
                if self.recordXNeurons[i]['X' + str(i) + ' Oscillator'] is True:
                    self.xNeuron[i]['X' + str(i) + ' Oscillator'].append(arti.xNeuron[i].soma.oscillator.update(arti.time))
                if self.recordXNeurons[i]['X' + str(i) + ' Backprop'] is True:
                    self.xNeuron[i]['X' + str(i) + ' Backprop'].append(arti.xNeuron[i].soma.backpropagated)
                if self.recordXNeurons[i]['X' + str(i) + ' Sum'] is True:
                    self.xNeuron[i]['X' + str(i) + ' Sum'].append(arti.xNeuron[i].soma.sum)
                if self.recordXNeurons[i]['X' + str(i) + ' Output'] is True:
                    self.xNeuron[i]['X' + str(i) + ' Output'].append(arti.xNeuron[i].soma.output)
                if self.recordXNeurons[i]['X' + str(i) + ' Spike'] is True:
                    self.xNeuron[i]['X' + str(i) + ' Spike'].append(arti.xNeuron[i].soma.spike)

                if self.recordONeurons[i]['O' + str(i) + ' SynInput 0'] is True:
                    self.oNeuron[i]['O' + str(i) + ' SynInput 0'].append(arti.oNeuron[i].soma.synInput[0].sum)
                if self.recordONeurons[i]['O' + str(i) + ' Oscillator'] is True:
                    self.oNeuron[i]['O' + str(i) + ' Oscillator'].append(arti.oNeuron[i].soma.oscillator.update(arti.time))
                if self.recordONeurons[i]['O' + str(i) + ' Backprop'] is True:
                    self.oNeuron[i]['O' + str(i) + ' Backprop'].append(arti.oNeuron[i].soma.backpropagated)
                if self.recordONeurons[i]['O' + str(i) + ' Sum'] is True:
                    self.oNeuron[i]['O' + str(i) + ' Sum'].append(arti.oNeuron[i].soma.sum)
                if self.recordONeurons[i]['O' + str(i) + ' Output'] is True:
                    self.oNeuron[i]['O' + str(i) + ' Output'].append(arti.oNeuron[i].soma.output)
                if self.recordONeurons[i]['O' + str(i) + ' Spike'] is True:
                    self.oNeuron[i]['O' + str(i) + ' Spike'].append(arti.oNeuron[i].soma.spike)

                if self.recordMNeurons[i]['M' + str(i) + ' SynInput 0'] is True:
                    self.mNeuron[i]['M' + str(i) + ' SynInput 0'].append(arti.mNeuron[i].soma.synInput[0].sum)
                if self.recordMNeurons[i]['M' + str(i) + ' Oscillator'] is True:
                    self.mNeuron[i]['M' + str(i) + ' Oscillator'].append(arti.mNeuron[i].soma.oscillator.update(arti.time))
                if self.recordMNeurons[i]['M' + str(i) + ' Backprop'] is True:
                    self.mNeuron[i]['M' + str(i) + ' Backprop'].append(arti.mNeuron[i].soma.backpropagated)
                if self.recordMNeurons[i]['M' + str(i) + ' Sum'] is True:
                    self.mNeuron[i]['M' + str(i) + ' Sum'].append(arti.mNeuron[i].soma.sum)
                if self.recordMNeurons[i]['M' + str(i) + ' Output'] is True:
                    self.mNeuron[i]['M' + str(i) + ' Output'].append(arti.mNeuron[i].soma.output)
                if self.recordMNeurons[i]['M' + str(i) + ' Spike'] is True:
                    self.mNeuron[i]['M' + str(i) + ' Spike'].append(arti.mNeuron[i].soma.spike)

                for j in range(9):
                    if self.recordXODends[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend SynInput 0'] is True:
                        self.xoDend[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend SynInput 0'].append(arti.neuron.dendrite[10 + (j + (i * 9))].synInput[0].sum)
                    if self.recordXODends[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend SynInput 1'] is True:
                        self.xoDend[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend SynInput 1'].append(arti.neuron.dendrite[10 + (j + (i * 9))].synInput[1].sum)
                    if self.recordXODends[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend Oscillator'] is True:
                        self.xoDend[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend Oscillator'].append(arti.neuron.dendrite[10 + (j + (i * 9))].oscillator.update(arti.time))
                    if self.recordXODends[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend Backprop'] is True:
                        self.xoDend[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend Backprop'].append(arti.neuron.dendrite[10 + (j + (i * 9))].backpropagated)
                    if self.recordXODends[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend Sum'] is True:
                        self.xoDend[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend Sum'].append(arti.neuron.dendrite[10 + (j + (i * 9))].sum)
                    if self.recordXODends[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend Output'] is True:
                        self.xoDend[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend Output'].append(arti.neuron.dendrite[10 + (j + (i * 9))].output)
                    if self.recordXODends[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend Spike'] is True:
                        self.xoDend[i][j]['XO(' + str(i) + ',' + str(j) + ') Dend Spike'].append(arti.neuron.dendrite[10 + (j + (i * 9))].spike)

                if self.recordMDends[i]['M' + str(i) + ' Dend SynInput 0'] is True:
                    self.mDend[i]['M' + str(i) + ' Dend SynInput 0'].append(arti.neuron.dendrite[i + 1].synInput[0].sum)
                if self.recordMDends[i]['M' + str(i) + ' Dend SynInput 1'] is True:
                    self.mDend[i]['M' + str(i) + ' Dend SynInput 1'].append(arti.neuron.dendrite[i + 1].synInput[1].sum)
                if self.recordMDends[i]['M' + str(i) + ' Dend SynInput 2'] is True:
                    self.mDend[i]['M' + str(i) + ' Dend SynInput 2'].append(arti.neuron.dendrite[i + 1].synInput[2].sum)
                if self.recordMDends[i]['M' + str(i) + ' Dend SynInput 3'] is True:
                    self.mDend[i]['M' + str(i) + ' Dend SynInput 3'].append(arti.neuron.dendrite[i + 1].synInput[3].sum)
                if self.recordMDends[i]['M' + str(i) + ' Dend SynInput 4'] is True:
                    self.mDend[i]['M' + str(i) + ' Dend SynInput 4'].append(arti.neuron.dendrite[i + 1].synInput[4].sum)
                if self.recordMDends[i]['M' + str(i) + ' Dend SynInput 5'] is True:
                    self.mDend[i]['M' + str(i) + ' Dend SynInput 5'].append(arti.neuron.dendrite[i + 1].synInput[5].sum)
                if self.recordMDends[i]['M' + str(i) + ' Dend SynInput 6'] is True:
                    self.mDend[i]['M' + str(i) + ' Dend SynInput 6'].append(arti.neuron.dendrite[i + 1].synInput[6].sum)
                if self.recordMDends[i]['M' + str(i) + ' Dend SynInput 7'] is True:
                    self.mDend[i]['M' + str(i) + ' Dend SynInput 7'].append(arti.neuron.dendrite[i + 1].synInput[7].sum)
                if self.recordMDends[i]['M' + str(i) + ' Dend SynInput 8'] is True:
                    self.mDend[i]['M' + str(i) + ' Dend SynInput 8'].append(arti.neuron.dendrite[i + 1].synInput[8].sum)
                if self.recordMDends[i]['M' + str(i) + ' Dend Oscillator'] is True:
                    self.mDend[i]['M' + str(i) + ' Dend Oscillator'].append(arti.neuron.dendrite[i + 1].oscillator.update(arti.time))
                if self.recordMDends[i]['M' + str(i) + ' Dend Backprop'] is True:
                    self.mDend[i]['M' + str(i) + ' Dend Backprop'].append(arti.neuron.dendrite[i + 1].backpropagated)
                if self.recordMDends[i]['M' + str(i) + ' Dend Sum'] is True:
                    self.mDend[i]['M' + str(i) + ' Dend Sum'].append(arti.neuron.dendrite[i + 1].sum)
                if self.recordMDends[i]['M' + str(i) + ' Dend Output'] is True:
                    self.mDend[i]['M' + str(i) + ' Dend Output'].append(arti.neuron.dendrite[i + 1].output)
                if self.recordMDends[i]['M' + str(i) + ' Dend Spike'] is True:
                    self.mDend[i]['M' + str(i) + ' Dend Spike'].append(arti.neuron.dendrite[i + 1].spike)
                    
                if self.recordAxons[i]['Axon' + str(i) + ' Oscillator'] is True:
                    self.axon[i]['Axon' + str(i) + ' Oscillator'].append(arti.neuron.axon[i].oscillator.update(arti.time))
                if self.recordAxons[i]['Axon' + str(i) + ' Backprop'] is True:
                    self.axon[i]['Axon' + str(i) + ' Backprop'].append(arti.neuron.axon[i].backpropagated)
                if self.recordAxons[i]['Axon' + str(i) + ' Sum'] is True:
                    self.axon[i]['Axon' + str(i) + ' Sum'].append(arti.neuron.axon[i].sum)
                if self.recordAxons[i]['Axon' + str(i) + ' Output'] is True:
                    self.axon[i]['Axon' + str(i) + ' Output'].append(arti.neuron.axon[i].output)
                if self.recordAxons[i]['Axon' + str(i) + ' Spike'] is True:
                    self.axon[i]['Axon' + str(i) + ' Spike'].append(arti.neuron.axon[i].spike)
                if self.recordAxons[i]['Axon' + str(i) + ' SynOutput 0'] is True:
                    self.axon[i]['Axon' + str(i) + ' SynOutput 0'].append(arti.neuron.axon[i].synOutput[0].sum)

            if self.recordTDend['Turn Dend SynInput 0'] is True:
                self.tDend['Turn Dend SynInput 0'].append(arti.neuron.dendrite[0].synInput[0].sum)
            if self.recordTDend['Turn Dend Oscillator'] is True:
                self.tDend['Turn Dend Oscillator'].append(arti.neuron.dendrite[0].oscillator.update(arti.time))
            if self.recordTDend['Turn Dend Backprop'] is True:
                self.tDend['Turn Dend Backprop'].append(arti.neuron.dendrite[0].backpropagated)
            if self.recordTDend['Turn Dend Sum'] is True:
                self.tDend['Turn Dend Sum'].append(arti.neuron.dendrite[0].sum)
            if self.recordTDend['Turn Dend Output'] is True:
                self.tDend['Turn Dend Output'].append(arti.neuron.dendrite[0].output)
            if self.recordTDend['Turn Dend Spike'] is True:
                self.tDend['Turn Dend Spike'].append(arti.neuron.dendrite[0].spike)

            if self.recordSoma['Soma Oscillator'] is True:
                self.soma['Soma Oscillator'].append(arti.neuron.soma.oscillator.update(arti.time))
            if self.recordSoma['Soma Backprop'] is True:
                self.soma['Soma Backprop'].append(arti.neuron.soma.backpropagated)
            if self.recordSoma['Soma Sum'] is True:
                self.soma['Soma Sum'].append(arti.neuron.soma.sum)
            if self.recordSoma['Soma Output'] is True:
                self.soma['Soma Output'].append(arti.neuron.soma.output)
            if self.recordSoma['Soma Spike'] is True:
                self.soma['Soma Spike'].append(arti.neuron.soma.spike)

    """
    This function verifies that all data arrays are the same size before the datalog is compiled to a file.
    self: object reference
    returns: true if all logged data arrays are the same size, otherwise false
    """
    def verifyLog(self):
        size = self.size

        if self.recordInputs['Time'] is True and len(self.inputs['Turn']) != size:
            print("Inputs[Turn] size mismatch: Expected " + str(size) + ", Found ", str(len(self.inputs['Turn'])))
            return False
        if self.recordInputs['Turn'] is True and len(self.inputs['Turn']) != size:
            print("Inputs[Turn] size mismatch: Expected " + str(size) + ", Found ", str(len(self.inputs['Turn'])))
            return False
        if self.recordInputs['Arti'] is True and len(self.inputs['Arti']) != size:
            print("Inputs[Arti] size mismatch: Expected " + str(size) + ", Found ", str(len(self.inputs['Arti'])))
            return False
        if self.recordInputs['Human'] is True and len(self.inputs['Human']) != size:
            print("Inputs[Human] size mismatch: Expected " + str(size) + ", Found ", str(len(self.inputs['Human'])))
            return False
        if self.recordInputs['Move #'] is True and len(self.inputs['Move #']) != size:
            print("Inputs[Move #] size mismatch: Expected " + str(size) + ", Found ", str(len(self.inputs['Move #'])))
            return False

        if self.recordTNeuron['Turn SynInput 0'] is True and len(self.tNeuron['Turn SynInput 0']) != size:
            print("TNeuron[Turn SynInput 0] size mismatch: Expected " + str(size) + ", Found ", str(len(self.tNeuron['Turn SynInput 0'])))
            return False
        if self.recordTNeuron['Turn Oscillator'] is True and len(self.tNeuron['Turn Oscillator']) != size:
            print("TNeuron[Turn Oscillator] size mismatch: Expected " + str(size) + ", Found ", str(len(self.tNeuron['Turn Oscillator'])))
            return False
        if self.recordTNeuron['Turn Backprop'] is True and len(self.tNeuron['Turn Backprop']) != size:
            print("TNeuron[Turn Backprop] size mismatch: Expected " + str(size) + ", Found ", str(len(self.tNeuron['Turn Backprop'])))
            return False
        if self.recordTNeuron['Turn Sum'] is True and len(self.tNeuron['Turn Sum']) != size:
            print("TNeuron[Turn Sum] size mismatch: Expected " + str(size) + ", Found ", str(len(self.tNeuron['Turn Sum'])))
            return False
        if self.recordTNeuron['Turn Output'] is True and len(self.tNeuron['Turn Output']) != size:
            print("TNeuron[Turn Output] size mismatch: Expected " + str(size) + ", Found ", str(len(self.tNeuron['Turn Output'])))
            return False
        if self.recordTNeuron['Turn Spike'] is True and len(self.tNeuron['Turn Spike']) != size:
            print("TNeuron[Turn Spike] size mismatch: Expected " + str(size) + ", Found ", str(len(self.tNeuron['Turn Spike'])))
            return False

        if self.recordTDend['Turn Dend SynInput 0'] is True and len(self.tDend['Turn Dend SynInput 0']) != size:
            print("TDend[Turn Dend SynInput 0] size mismatch: Expected " + str(size) + ", Found ", str(len(self.tDend['Turn Dend SynInput 0'])))
            return False
        if self.recordTDend['Turn Dend Oscillator'] is True and len(self.tDend['Turn Dend Oscillator']) != size:
            print("TDend[Turn Dend Oscillator] size mismatch: Expected " + str(size) + ", Found ", str(len(self.tDend['Turn Dend Oscillator'])))
            return False
        if self.recordTDend['Turn Dend Backprop'] is True and len(self.tDend['Turn Dend Backprop']) != size:
            print("TDend[Turn Dend OsciBackpropllator] size mismatch: Expected " + str(size) + ", Found ", str(len(self.tDend['Turn Dend Backprop'])))
            return False
        if self.recordTDend['Turn Dend Sum'] is True and len(self.tDend['Turn Dend Sum']) != size:
            print("TDend[Turn Dend Sum] size mismatch: Expected " + str(size) + ", Found ", str(len(self.tDend['Turn Dend Sum'])))
            return False
        if self.recordTDend['Turn Dend Output'] is True and len(self.tDend['Turn Dend Output']) != size:
            print("TDend[Turn Dend Output] size mismatch: Expected " + str(size) + ", Found ", str(len(self.tDend['Turn Dend Output'])))
            return False
        if self.recordTDend['Turn Dend Spike'] is True and len(self.tDend['Turn Dend Spike']) != size:
            print("TDend[Turn Dend Spike] size mismatch: Expected " + str(size) + ", Found ", str(len(self.tDend['Turn Dend Spike'])))
            return False
        
        if self.recordSoma['Soma Oscillator'] is True and len(self.soma['Soma Oscillator']) != size:
            print("Soma[Soma Oscillator] size mismatch: Expected " + str(size) + ", Found ", str(len(self.soma['Soma Oscillator'])))
            return False
        if self.recordSoma['Soma Backprop'] is True and len(self.soma['Soma Backprop']) != size:
            print("Soma[Soma Backprop] size mismatch: Expected " + str(size) + ", Found ", str(len(self.soma['Soma Backprop'])))
            return False
        if self.recordSoma['Soma Sum'] is True and len(self.soma['Soma Sum']) != size:
            print("Soma[Soma Sum] size mismatch: Expected " + str(size) + ", Found ", str(len(self.soma['Soma Sum'])))
            return False
        if self.recordSoma['Soma Output'] is True and len(self.soma['Soma Output']) != size:
            print("Soma[Soma Output] size mismatch: Expected " + str(size) + ", Found ", str(len(self.soma['Soma Output'])))
            return False
        if self.recordSoma['Soma Spike'] is True and len(self.soma['Soma Spike']) != size:
            print("Soma[Soma Spike] size mismatch: Expected " + str(size) + ", Found ", str(len(self.soma['Soma Spike'])))
            return False

        for i in range(9):
            if self.recordInputs['Square ' + str(i + 1)] is True and len(self.inputs['Square ' + str(i + 1)]) != size:
                print("Inputs[Square " + str(i + 1) + "] size mismatch: Expected " + str(size) + ", Found ", str(len(self.inputs['Square ' + str(i + 1)])))
                return False

            if self.recordXNeurons[i]["X" + str(i) + " SynInput 0"] is True and len(self.xNeuron[i]["X" + str(i) + " SynInput 0"]) != size:
                print("XNeuron[" + str(i) + "][X" + str(i) + " SynInput 0] size mismatch: Expected " + str(size) + ", Found ", str(len(self.xNeuron["X" + str(i) + " SynInput 0"])))
                return False
            if self.recordXNeurons[i]["X" + str(i) + " Oscillator"] is True and len(self.xNeuron[i]["X" + str(i) + " Oscillator"]) != size:
                print("XNeuron[" + str(i) + "][X" + str(i) + " Oscillator] size mismatch: Expected " + str(size) + ", Found ", str(len(self.xNeuron["X" + str(i) + " Oscillator"])))
                return False
            if self.recordXNeurons[i]["X" + str(i) + " Backprop"] is True and len(self.xNeuron[i]["X" + str(i) + " Backprop"]) != size:
                print("XNeuron[" + str(i) + "][X" + str(i) + " Backprop] size mismatch: Expected " + str(size) + ", Found ", str(len(self.xNeuron["X" + str(i) + " Backprop"])))
                return False
            if self.recordXNeurons[i]["X" + str(i) + " Sum"] is True and len(self.xNeuron[i]["X" + str(i) + " Sum"]) != size:
                print("XNeuron[" + str(i) + "][X" + str(i) + " Sum] size mismatch: Expected " + str(size) + ", Found ", str(len(self.xNeuron["X" + str(i) + " Sum"])))
                return False
            if self.recordXNeurons[i]["X" + str(i) + " Output"] is True and len(self.xNeuron[i]["X" + str(i) + " Output"]) != size:
                print("XNeuron[" + str(i) + "][X" + str(i) + " Output] size mismatch: Expected " + str(size) + ", Found ", str(len(self.xNeuron["X" + str(i) + " Output"])))
                return False
            if self.recordXNeurons[i]["X" + str(i) + " Spike"] is True and len(self.xNeuron[i]["X" + str(i) + " Spike"]) != size:
                print("XNeuron[" + str(i) + "][X" + str(i) + " Spike] size mismatch: Expected " + str(size) + ", Found ", str(len(self.xNeuron["X" + str(i) + " Spike"])))
                return False

            if self.recordONeurons[i]["O" + str(i) + " SynInput 0"] is True and len(self.oNeuron[i]["O" + str(i) + " SynInput 0"]) != size:
                print("ONeuron[" + str(i) + "][O" + str(i) + " SynInput 0] size mismatch: Expected " + str(size) + ", Found ", str(len(self.oNeuron["O" + str(i) + " SynInput 0"])))
                return False
            if self.recordONeurons[i]["O" + str(i) + " Oscillator"] is True and len(self.oNeuron[i]["O" + str(i) + " Oscillator"]) != size:
                print("ONeuron[" + str(i) + "][O" + str(i) + " Oscillator] size mismatch: Expected " + str(size) + ", Found ", str(len(self.oNeuron["O" + str(i) + " Oscillator"])))
                return False
            if self.recordONeurons[i]["O" + str(i) + " Backprop"] is True and len(self.oNeuron[i]["O" + str(i) + " Backprop"]) != size:
                print("ONeuron[" + str(i) + "][O" + str(i) + " Backprop] size mismatch: Expected " + str(size) + ", Found ", str(len(self.oNeuron["O" + str(i) + " Backprop"])))
                return False
            if self.recordONeurons[i]["O" + str(i) + " Sum"] is True and len(self.oNeuron[i]["O" + str(i) + " Sum"]) != size:
                print("ONeuron[" + str(i) + "][O" + str(i) + " Sum] size mismatch: Expected " + str(size) + ", Found ", str(len(self.oNeuron["O" + str(i) + " Sum"])))
                return False
            if self.recordONeurons[i]["O" + str(i) + " Output"] is True and len(self.oNeuron[i]["O" + str(i) + " Output"]) != size:
                print("ONeuron[" + str(i) + "][O" + str(i) + " Output] size mismatch: Expected " + str(size) + ", Found ", str(len(self.oNeuron["O" + str(i) + " Output"])))
                return False
            if self.recordONeurons[i]["O" + str(i) + " Spike"] is True and len(self.oNeuron[i]["O" + str(i) + " Spike"]) != size:
                print("ONeuron[" + str(i) + "][O" + str(i) + " Spike] size mismatch: Expected " + str(size) + ", Found ", str(len(self.oNeuron["O" + str(i) + " Spike"])))
                return False

            if self.recordMNeurons[i]["M" + str(i) + " SynInput 0"] is True and len(self.mNeuron[i]["M" + str(i) + " SynInput 0"]) != size:
                print("MNeuron[" + str(i) + "][M" + str(i) + " SynInput 0] size mismatch: Expected " + str(size) + ", Found ", str(len(self.mNeuron["M" + str(i) + " SynInput 0"])))
                return False
            if self.recordMNeurons[i]["M" + str(i) + " Oscillator"] is True and len(self.mNeuron[i]["M" + str(i) + " Oscillator"]) != size:
                print("MNeuron[" + str(i) + "][M" + str(i) + " Oscillator] size mismatch: Expected " + str(size) + ", Found ", str(len(self.mNeuron["M" + str(i) + " Oscillator"])))
                return False
            if self.recordMNeurons[i]["M" + str(i) + " Backprop"] is True and len(self.mNeuron[i]["M" + str(i) + " Backprop"]) != size:
                print("MNeuron[" + str(i) + "][M" + str(i) + " Backprop] size mismatch: Expected " + str(size) + ", Found ", str(len(self.mNeuron["M" + str(i) + " Backprop"])))
                return False
            if self.recordMNeurons[i]["M" + str(i) + " Sum"] is True and len(self.mNeuron[i]["M" + str(i) + " Sum"]) != size:
                print("MNeuron[" + str(i) + "][M" + str(i) + " Sum] size mismatch: Expected " + str(size) + ", Found ", str(len(self.mNeuron["M" + str(i) + " Sum"])))
                return False
            if self.recordMNeurons[i]["M" + str(i) + " Output"] is True and len(self.mNeuron[i]["M" + str(i) + " Output"]) != size:
                print("MNeuron[" + str(i) + "][M" + str(i) + " Output] size mismatch: Expected " + str(size) + ", Found ", str(len(self.mNeuron["M" + str(i) + " Output"])))
                return False
            if self.recordMNeurons[i]["M" + str(i) + " Spike"] is True and len(self.mNeuron[i]["M" + str(i) + " Spike"]) != size:
                print("MNeuron[" + str(i) + "][M" + str(i) + " Spike] size mismatch: Expected " + str(size) + ", Found ", str(len(self.mNeuron["M" + str(i) + " Spike"])))
                return False

            if self.recordMDends[i]["M" + str(i) + " Dend SynInput 0"] is True and len(self.mDend[i]["M" + str(i) + " Dend SynInput 0"]) != size:
                print("MDend[" + str(i) + "][M" + str(i) + " Dend SynInput 0] size mismatch: Expected " + str(size) + ", Found ", 
                      str(len(self.mDend["M" + str(i) + " Dend SynInput 0"])))
                return False
            if self.recordMDends[i]["M" + str(i) + " Dend SynInput 1"] is True and len(self.mDend[i]["M" + str(i) + " Dend SynInput 1"]) != size:
                print("MDend[" + str(i) + "][M" + str(i) + " Dend SynInput 1] size mismatch: Expected " + str(size) + ", Found ", 
                      str(len(self.mDend["M" + str(i) + " Dend SynInput 1"])))
                return False
            if self.recordMDends[i]["M" + str(i) + " Dend SynInput 2"] is True and len(self.mDend[i]["M" + str(i) + " Dend SynInput 2"]) != size:
                print("MDend[" + str(i) + "][M" + str(i) + " Dend SynInput 2] size mismatch: Expected " + str(size) + ", Found ", 
                      str(len(self.mDend["M" + str(i) + " Dend SynInput 2"])))
                return False
            if self.recordMDends[i]["M" + str(i) + " Dend SynInput 3"] is True and len(self.mDend[i]["M" + str(i) + " Dend SynInput 3"]) != size:
                print("MDend[" + str(i) + "][M" + str(i) + " Dend SynInput 3] size mismatch: Expected " + str(size) + ", Found ", 
                      str(len(self.mDend["M" + str(i) + " Dend SynInput 3"])))
                return False
            if self.recordMDends[i]["M" + str(i) + " Dend SynInput 4"] is True and len(self.mDend[i]["M" + str(i) + " Dend SynInput 4"]) != size:
                print("MDend[" + str(i) + "][M" + str(i) + " Dend SynInput 4] size mismatch: Expected " + str(size) + ", Found ", 
                      str(len(self.mDend["M" + str(i) + " Dend SynInput 4"])))
                return False
            if self.recordMDends[i]["M" + str(i) + " Dend SynInput 5"] is True and len(self.mDend[i]["M" + str(i) + " Dend SynInput 5"]) != size:
                print("MDend[" + str(i) + "][M" + str(i) + " Dend SynInput 5] size mismatch: Expected " + str(size) + ", Found ", 
                      str(len(self.mDend["M" + str(i) + " Dend SynInput 5"])))
                return False
            if self.recordMDends[i]["M" + str(i) + " Dend SynInput 6"] is True and len(self.mDend[i]["M" + str(i) + " Dend SynInput 6"]) != size:
                print("MDend[" + str(i) + "][M" + str(i) + " Dend SynInput 6] size mismatch: Expected " + str(size) + ", Found ", 
                      str(len(self.mDend["M" + str(i) + " Dend SynInput 6"])))
                return False
            if self.recordMDends[i]["M" + str(i) + " Dend SynInput 7"] is True and len(self.mDend[i]["M" + str(i) + " Dend SynInput 7"]) != size:
                print("MDend[" + str(i) + "][M" + str(i) + " Dend SynInput 7] size mismatch: Expected " + str(size) + ", Found ", 
                      str(len(self.mDend["M" + str(i) + " Dend SynInput 7"])))
                return False
            if self.recordMDends[i]["M" + str(i) + " Dend SynInput 8"] is True and len(self.mDend[i]["M" + str(i) + " Dend SynInput 8"]) != size:
                print("MDend[" + str(i) + "][M" + str(i) + " Dend SynInput 8] size mismatch: Expected " + str(size) + ", Found ", 
                      str(len(self.mDend["M" + str(i) + " Dend SynInput 8"])))
                return False
            if self.recordMDends[i]["M" + str(i) + " Dend Oscillator"] is True and len(self.mDend[i]["M" + str(i) + " Dend Oscillator"]) != size:
                print("MDend[" + str(i) + "][M" + str(i) + " Dend Oscillator] size mismatch: Expected " + str(size) + ", Found ", 
                      str(len(self.mDend["M" + str(i) + " Dend Oscillator"])))
                return False
            if self.recordMDends[i]["M" + str(i) + " Dend Backprop"] is True and len(self.mDend[i]["M" + str(i) + " Dend Backprop"]) != size:
                print("MDend[" + str(i) + "][M" + str(i) + " Dend Backprop] size mismatch: Expected " + str(size) + ", Found ", 
                      str(len(self.mDend["M" + str(i) + " Dend Backprop"])))
                return False
            if self.recordMDends[i]["M" + str(i) + " Dend Sum"] is True and len(self.mDend[i]["M" + str(i) + " Dend Sum"]) != size:
                print("MDend[" + str(i) + "][M" + str(i) + " Dend Sum] size mismatch: Expected " + str(size) + ", Found ", str(len(self.mDend["M" + str(i) + " Dend Sum"])))
                return False
            if self.recordMDends[i]["M" + str(i) + " Dend Output"] is True and len(self.mDend[i]["M" + str(i) + " Dend Output"]) != size:
                print("MDend[" + str(i) + "][M" + str(i) + " Dend Output] size mismatch: Expected " + str(size) + ", Found ", str(len(self.mDend["M" + str(i) + " Dend Output"])))
                return False
            if self.recordMDends[i]["M" + str(i) + " Dend Spike"] is True and len(self.mDend[i]["M" + str(i) + " Dend Spike"]) != size:
                print("MDend[" + str(i) + "][M" + str(i) + " Dend Spike] size mismatch: Expected " + str(size) + ", Found ", str(len(self.mDend["M" + str(i) + " Dend Spike"])))
                return False

            if self.recordAxons[i]["Axon" + str(i) + " Oscillator"] is True and len(self.axon[i]["Axon" + str(i) + " Oscillator"]) != size:
                print("Axon[" + str(i) + "][Axon" + str(i) + " Oscillator] size mismatch: Expected " + str(size) + ", Found ", 
                      str(len(self.inputs["Axon" + str(i) + " Oscillator"])))
                return False
            if self.recordAxons[i]["Axon" + str(i) + " Backprop"] is True and len(self.axon[i]["Axon" + str(i) + " Backprop"]) != size:
                print("Axon[" + str(i) + "][Axon" + str(i) + " Backprop] size mismatch: Expected " + str(size) + ", Found ", str(len(self.axon["Axon" + str(i) + " Backprop"])))
                return False
            if self.recordAxons[i]["Axon" + str(i) + " Sum"] is True and len(self.axon[i]["Axon" + str(i) + " Sum"]) != size:
                print("Axon[" + str(i) + "][Axon" + str(i) + " Sum] size mismatch: Expected " + str(size) + ", Found ", str(len(self.axon["Axon" + str(i) + " Sum"])))
                return False
            if self.recordAxons[i]["Axon" + str(i) + " Output"] is True and len(self.axon[i]["Axon" + str(i) + " Output"]) != size:
                print("Axon[" + str(i) + "][Axon" + str(i) + " Output] size mismatch: Expected " + str(size) + ", Found ", str(len(self.axon["Axon" + str(i) + " Output"])))
                return False
            if self.recordAxons[i]["Axon" + str(i) + " Spike"] is True and len(self.axon[i]["Axon" + str(i) + " Spike"]) != size:
                print("Axon[" + str(i) + "][Axon" + str(i) + " Spike] size mismatch: Expected " + str(size) + ", Found ", str(len(self.axon["Axon" + str(i) + " Spike"])))
                return False
            if self.recordAxons[i]["Axon" + str(i) + " SynOutput 0"] is True and len(self.axon[i]["Axon" + str(i) + " SynOutput 0"]) != size:
                print("Axon[" + str(i) + "][Axon" + str(i) + " SynOutput 0] size mismatch: Expected " + str(size) + ", Found ", 
                      str(len(self.axon["Axon" + str(i) + " SynOutput 0"])))
                return False

            for j in range(9):
                if self.recordXODends[i][j]["XO(" + str(i) + "," + str(j) + ") Dend SynInput 0"] is True: 
                    if len(self.xoDend[i][j]["XO(" + str(i) + "," + str(j) + ") Dend SynInput 0"]) != size:
                        print("XODend[" + str(i) + "][" + str(j) + "][X " + str(i) + " Dend SynInput 0] size mismatch: Expected " + str(size) + ", Found ", 
                              str(len(self.xoDend["XO(" + str(i) + "," + str(j) + ") Dend SynInput 0"])))
                        return False
                if self.recordXODends[i][j]["XO(" + str(i) + "," + str(j) + ") Dend SynInput 1"] is True: 
                    if len(self.xoDend[i][j]["XO(" + str(i) + "," + str(j) + ") Dend SynInput 1"]) != size:
                        print("XODend[" + str(i) + "][" + str(j) + "][X " + str(i) + " Dend SynInput 1] size mismatch: Expected " + str(size) + ", Found ", 
                              str(len(self.xoDend["XO(" + str(i) + "," + str(j) + ") Dend SynInput 1"])))
                        return False
                if self.recordXODends[i][j]["XO(" + str(i) + "," + str(j) + ") Dend Oscillator"] is True: 
                    if len(self.xoDend[i][j]["XO(" + str(i) + "," + str(j) + ") Dend Oscillator"]) != size:
                        print("XODend[" + str(i) + "][" + str(j) + "][X " + str(i) + " Dend Oscillator] size mismatch: Expected " + str(size) + ", Found ", 
                              str(len(self.xoDend["XO(" + str(i) + "," + str(j) + ") Dend Oscillator"])))
                        return False
                if self.recordXODends[i][j]["XO(" + str(i) + "," + str(j) + ") Dend Sum"] is True: 
                    if len(self.xoDend[i][j]["XO(" + str(i) + "," + str(j) + ") Dend Sum"]) != size:
                        print("XODend[" + str(i) + "][" + str(j) + "][X " + str(i) + " Dend Sum] size mismatch: Expected " + str(size) + ", Found ", 
                              str(len(self.xoDend["XO(" + str(i) + "," + str(j) + ") Dend Sum"])))
                        return False
                if self.recordXODends[i][j]["XO(" + str(i) + "," + str(j) + ") Dend Output"] is True: 
                    if len(self.xoDend[i][j]["XO(" + str(i) + "," + str(j) + ") Dend Output"]) != size:
                        print("XODend[" + str(i) + "][" + str(j) + "][X " + str(i) + " Dend Output] size mismatch: Expected " + str(size) + ", Found ", 
                              str(len(self.xoDend["XO(" + str(i) + "," + str(j) + ") Dend Output"])))
                        return False
                if self.recordXODends[i][j]["XO(" + str(i) + "," + str(j) + ") Dend Spike"] is True: 
                    if len(self.xoDend[i][j]["XO(" + str(i) + "," + str(j) + ") Dend Spike"]) != size:
                        print("XODend[" + str(i) + "][" + str(j) + "][X " + str(i) + " Dend Spike] size mismatch: Expected " + str(size) + ", Found ", 
                              str(len(self.xoDend["XO(" + str(i) + "," + str(j) + ") Dend Spike"])))
                        return False

        return True