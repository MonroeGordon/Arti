from arti import *
from tkinter import *
import random as rnd

class TicTacToe(Tk, ArtiListener):
    def __init__(self, arti):
        super().__init__()

        self.arti = arti
        
        self.wm_title('Tic-Tac-Toe')
        self.protocol("WM_DELETE_WINDOW", self.exit)

        self.started = False
        self.turn = 'X'
        self.player = 'X'
        self.moveNum = 1
        self.board = [' ', ' ', ' ',
                      ' ', ' ', ' ',
                      ' ', ' ', ' ']
        self.square = [
            Label(text='', bg='blue', fg='white', font=("Arial", 48)),
            Label(text='', bg='blue', fg='white', font=("Arial", 48)),
            Label(text='', bg='blue', fg='white', font=("Arial", 48)),
            Label(text='', bg='blue', fg='white', font=("Arial", 48)),
            Label(text='', bg='blue', fg='white', font=("Arial", 48)),
            Label(text='', bg='blue', fg='white', font=("Arial", 48)),
            Label(text='', bg='blue', fg='white', font=("Arial", 48)),
            Label(text='', bg='blue', fg='white', font=("Arial", 48)),
            Label(text='', bg='blue', fg='white', font=("Arial", 48))]

        self.window_height = 300
        self.window_width = 300

        self.screen_width = self.winfo_screenwidth()
        self.screen_height = self.winfo_screenheight()

        self.x_cordinate = int((self.screen_width / 2) - (self.window_width / 2))
        self.y_cordinate = int((self.screen_height / 2) - (self.window_height / 2))

        self.geometry("{}x{}+{}+{}".format(self.window_width, self.window_height, self.x_cordinate, self.y_cordinate))
        self.rowconfigure([0, 1, 2], minsize=100)
        self.columnconfigure([0, 1, 2], minsize=100)

        self.menubar = Menu(self)
        self.config(menu=self.menubar)

        self.fileMenu = Menu(self.menubar, tearoff=False)
        self.newGameMenu = Menu(self.fileMenu, tearoff=False)
        self.newGameMenu.add_command(label='Play X', command=self.playX)
        self.newGameMenu.add_command(label='Play O', command=self.playO)
        self.fileMenu.add_cascade(label='New Game', menu=self.newGameMenu, underline=0)
        self.fileMenu.add_command(label='Exit', command=self.exit, underline=1)

        self.menubar.add_cascade(label='File', menu=self.fileMenu, underline=0)

        for i in range(len(self.square)):
            self.square[i].grid(row=i // 3, column=i % 3, sticky='nsew', padx=1, pady=1)

            def select(event, square=self.square[i]):
                self.setSquare(square)

            self.square[i].bind('<Button-1>', select)

        arti.start()

    def exit(self):
        self.arti.stop()
        self.destroy()
        sys.exit()

    def messageFromArti(self, message):
        self.setArtiSquare(message.output)

    def playO(self):
        self.started = True
        self.player = 'O'
        self.arti.setPlayer('X')
        self.turn = 'X'
        self.moveNum = 1

        for i in range(len(self.square)):
            self.square[i].config(text='')
            self.board[i] = ' '

        self.arti.setInput(self.turn, self.moveNum, self.board)

    def playX(self):
        self.started = True
        self.player = 'X'
        self.arti.setPlayer('O')
        self.turn = 'X'
        self.moveNum = 1
        for i in range(len(self.square)):
            self.square[i].config(text='')

        self.arti.setInput(self.turn, self.moveNum, self.board)

    def setArtiSquare(self, square):
        self.square[square].config(text=self.arti.player)
        self.board[square] = self.arti.player
        self.turn = self.player
        self.moveNum += 1

        self.arti.setInput(self.turn, self.moveNum, self.board)

    def setSquare(self, square):
        if self.started and self.turn == self.player:
            square.config(text=self.player)

            for i in range(len(self.square)):
                self.board[i] = self.square[i].cget("text")

            self.turn = self.arti.player
            self.moveNum += 1

            self.arti.setInput(self.turn, self.moveNum, self.board)

if __name__ == "__main__":
    arti = Arti_1_0()
    ttt = TicTacToe(arti)
    arti.addListener(ttt)

    ttt.mainloop()