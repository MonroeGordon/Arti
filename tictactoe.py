from arti import *
from tkinter import *

class TicTacToe:
    def __init__(self):
        self.window = Tk()
        self.window.wm_title('Tic-Tac-Toe')

        self.player = 'X'
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

        self.screen_width = self.window.winfo_screenwidth()
        self.screen_height = self.window.winfo_screenheight()

        self.x_cordinate = int((self.screen_width / 2) - (self.window_width / 2))
        self.y_cordinate = int((self.screen_height / 2) - (self.window_height / 2))

        self.window.geometry("{}x{}+{}+{}".format(self.window_width, self.window_height, self.x_cordinate, self.y_cordinate))
        self.window.rowconfigure([0, 1, 2], minsize=100)
        self.window.columnconfigure([0, 1, 2], minsize=100)

        self.menubar = Menu(self.window)
        self.window.config(menu=self.menubar)

        self.fileMenu = Menu(self.menubar, tearoff=False)
        self.newGameMenu = Menu(self.fileMenu, tearoff=False)
        self.newGameMenu.add_command(label='Play X', command=self.playX)
        self.newGameMenu.add_command(label='Play O', command=self.playO)
        self.fileMenu.add_cascade(label='New Game', menu=self.newGameMenu, underline=0)
        self.fileMenu.add_command(label='Exit', command=self.window.quit, underline=1)

        self.menubar.add_cascade(label='File', menu=self.fileMenu, underline=0)

        for i in range(len(self.square)):
            self.square[i].grid(row=i // 3, column=i % 3, sticky='nsew', padx=1, pady=1)

            def select(event, square=self.square[i]):
                self.setSquare(square)

            self.square[i].bind('<Button-1>', select)

        self.window.mainloop()

    def playO(self):
        self.player = 'O'
        for i in range(len(self.square)):
            self.square[i].config(text='')

    def playX(self):
        self.player = 'X'
        for i in range(len(self.square)):
            self.square[i].config(text='')

    def setSquare(self, square):
        square.config(text=self.player)

def main():
    TicTacToe()

if __name__ == "__main__":
    main()
