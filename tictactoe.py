import threading
from arti import *
from queue import Queue
import sys
from tkinter import *
from tkinter import messagebox
from tkinter import simpledialog
from tkinter import ttk

"""
ProgressDialog: This class is used to create a progressbar dialog box that closes only after the progressbar completes.
"""
class ProgressDialog(simpledialog.SimpleDialog):
    """
    Initializes a simple dialog that can have a title and contain text along with the progressbar that is positioned at the bottom of the dialog.
    self: object reference
    master: parent of this dialog
    test: optional message to display in the dialog
    title: optional title of this dialog
    """
    def __init__(self, master, text='', title=None):
        super().__init__(master=master, text=text, title=title)

        self.default = None
        self.cancel = None

        self.pbar = ttk.Progressbar(self.root, orient="horizontal", length=200, mode="determinate")
        self.pbar.pack(expand=True, fill=X, side=BOTTOM)

        self.root.attributes("-topmost", True) # Keep the dialog on top

        self.queue = Queue() # Create a queue to store the next progressbar values

        self.root.after(200, self.update) # Update the progressbar every 200 milliseconds

    """
    This adds the next value of the progressbar to the queue.
    self: object reference
    value: the next progressbar value
    """
    def setProgress(self, value):
        self.queue.put(value)

    """
    This updates the progressbar by pulling the next value from the queue.
    self: object reference
    """
    def update(self):
        while self.queue.qsize():
            try:
                self.pbar['value'] = self.queue.get(0)
            except Queue.Empty:
                pass

        self.root.after(200, self.update) # Update the progressbar every 200 milliseconds

"""
TicTacToe: This class creates and controls the GUI window for Tic-Tac-Toe as well as controls and handles the game loop and functions. It also
contains Arti, the artificial intelligence player.
"""
class TicTacToe(Tk, ArtiListener):
    """
    Initializes the Tic-Tac-Toe GUI window and starts Arti on a separate thread.
    self: object reference
    arti: Arti, the AI
    """
    def __init__(self, arti):
        super().__init__()

        self.arti = arti
        
        self.wm_title('Tic-Tac-Toe')
        self.protocol("WM_DELETE_WINDOW", self.exit) # Call the exit function when clicking the red X button

        # Initialize game state variables
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
        self.newGameMenu.add_command(label='Play X', command=self.playX) # Play as X menu option
        self.newGameMenu.add_command(label='Play O', command=self.playO) # Play as O menu option
        self.fileMenu.add_cascade(label='New Game', menu=self.newGameMenu, underline=0)
        self.fileMenu.add_command(label='Exit', command=self.exit, underline=1)

        self.menubar.add_cascade(label='File', menu=self.fileMenu, underline=0)

        # Create the Tic-Tac-Toe gameboard
        for i in range(len(self.square)):
            self.square[i].grid(row=i // 3, column=i % 3, sticky='nsew', padx=1, pady=1)

            def select(event, square=self.square[i]):
                self.setSquare(square)

            self.square[i].bind('<Button-1>', select)

        arti.start() # Start Arti's separate thread

    """
    This handles exiting the program. It stops Arti's thread, writes Arti's datalog and closes the GUI window before exiting completely.
    self: object reference
    """
    def exit(self):
        self.arti.stop()
        self.arti.join() # Wait for Arti to finish

        # Create a progress dialog showing the completion of compiling Arti's datalog to an Excel file
        if self.arti.dataLog.verifyLog():
            pbar = ProgressDialog(self, title="Compiling Data Log File", text="Compiling data log file.\nPlease wait...")
            t = threading.Thread(target=self.arti.dataLog.compileLog, args=(pbar,))
            t.start()
            pbar.go()
            
        self.destroy()
        sys.exit(0)

    """
    This handles messages sent from Arti. In this case, it uses Arti's output to select the next square Arti will occupy on the gameboard.
    self: object reference
    message: the message from Arti
    """
    def messageFromArti(self, message):
        self.setArtiSquare(message.output)

    """
    This checks if player O has won the game. It changes the color of the O's in the winning line to red. It returns true if O has won,
    otherwise false.
    self: object reference
    """
    def oWins(self):
        if self.board[0] == 'O' and self.board[1] == 'O' and self.board[2] == 'O':
            self.square[0].config(fg='red')
            self.square[1].config(fg='red')
            self.square[2].config(fg='red')
            return True

        if self.board[0] == 'O' and self.board[4] == 'O' and self.board[8] == 'O':
            self.square[0].config(fg='red')
            self.square[4].config(fg='red')
            self.square[8].config(fg='red')
            return True

        if self.board[0] == 'O' and self.board[3] == 'O' and self.board[6] == 'O':
            self.square[0].config(fg='red')
            self.square[3].config(fg='red')
            self.square[6].config(fg='red')
            return True

        if self.board[1] == 'O' and self.board[4] == 'O' and self.board[7] == 'O':
            self.square[1].config(fg='red')
            self.square[4].config(fg='red')
            self.square[7].config(fg='red')
            return True

        if self.board[2] == 'O' and self.board[5] == 'O' and self.board[8] == 'O':
            self.square[2].config(fg='red')
            self.square[5].config(fg='red')
            self.square[8].config(fg='red')
            return True

        if self.board[2] == 'O' and self.board[4] == 'O' and self.board[6] == 'O':
            self.square[2].config(fg='red')
            self.square[4].config(fg='red')
            self.square[6].config(fg='red')
            return True

        if self.board[3] == 'O' and self.board[4] == 'O' and self.board[5] == 'O':
            self.square[3].config(fg='red')
            self.square[4].config(fg='red')
            self.square[5].config(fg='red')
            return True

        if self.board[6] == 'O' and self.board[7] == 'O' and self.board[8] == 'O':
            self.square[6].config(fg='red')
            self.square[7].config(fg='red')
            self.square[8].config(fg='red')
            return True

        return False

    """
    This sets the human player to player O and starts a new game by clearing the board and letting Arti go first, since he is set to
    player X.
    self: object reference
    """
    def playO(self):
        self.started = True
        self.player = 'O'
        self.arti.setPlayer('X')
        self.turn = 'X'
        self.moveNum = 1

        for i in range(len(self.square)):
            self.square[i].config(text='', fg='white')
            self.board[i] = ' '

        self.arti.setInput(self.turn, self.moveNum, self.board) # Send initial game state info to Arti

    """
    This sets the human player to player X and starts a new game by clearing the board and letting the human go first.
    self: object reference
    """
    def playX(self):
        self.started = True
        self.player = 'X'
        self.arti.setPlayer('O')
        self.turn = 'X'
        self.moveNum = 1
        for i in range(len(self.square)):
            self.square[i].config(text='', fg='white')

        self.arti.setInput(self.turn, self.moveNum, self.board) # Send initial game state info to Arti

    """
    This sets the square that Arti has selected as his next move based on his latest output. It then updates game information and
    switches to the human's turn, unless Arti has won the game or the game is a draw.
    self: object reference
    square: the square Arti has selected to occupy
    """
    def setArtiSquare(self, square):
        if self.board[square - 1] != 'X' and self.board[square - 1] != 'O':
            self.square[square - 1].config(text=self.arti.player)
            self.board[square - 1] = self.arti.player
            self.turn = self.player
            self.moveNum += 1

            if self.arti.player == 'O':
                if self.oWins():
                    self.moveNum = 9
                    self.started = False
            else:
                if self.xWins():
                    self.moveNum = 9
                    self.started = False

            self.arti.setInput(self.turn, self.moveNum, self.board) # Send updated game state info to Arti
        else:
            self.turn = ''
            self.arti.setInput(self.turn, self.moveNum, self.board) # Send updated game state info to Arti

            # Display error message from Arti
            messagebox.showerror("From Arti", "I apologize. I made an erroneous move. Let me try again.")

            self.turn = self.arti.player
            self.arti.setInput(self.turn, self.moveNum, self.board) # Send updated game state info to Arti

    """
    This sets the square that the human player has selected. It then updates game information and switches to Arti's turn, unless the 
    human player has won the game or the game is a draw.
    self: object reference
    square: the square the human player has selected to occupy
    """
    def setSquare(self, square):
        if self.started and self.turn == self.player:
            square.config(text=self.player)

            for i in range(len(self.square)):
                self.board[i] = self.square[i].cget("text")

            if self.player == 'O':
                if self.oWins():
                    self.moveNum = 9
                    self.started = False # End the game is O wins
                else:
                    self.turn = self.arti.player
                    self.moveNum += 1
                    self.arti.setInput(self.turn, self.moveNum, self.board) # Send updated game state info to Arti
            else:
                if self.xWins():
                    self.moveNum = 9
                    self.started = False # End the game if X wins
                else:
                    self.turn = self.arti.player
                    self.moveNum += 1
                    self.arti.setInput(self.turn, self.moveNum, self.board) # Send updated game state info to Arti

    """
    This checks if player X has won the game. It changes the color of the X's in the winning line to red. It returns true if X has won,
    otherwise false.
    self: object reference
    """
    def xWins(self):
        if self.board[0] == 'X' and self.board[1] == 'X' and self.board[2] == 'X':
            self.square[0].config(fg='red')
            self.square[1].config(fg='red')
            self.square[2].config(fg='red')
            return True

        if self.board[0] == 'X' and self.board[4] == 'X' and self.board[8] == 'X':
            self.square[0].config(fg='red')
            self.square[4].config(fg='red')
            self.square[8].config(fg='red')
            return True

        if self.board[0] == 'X' and self.board[3] == 'X' and self.board[6] == 'X':
            self.square[0].config(fg='red')
            self.square[3].config(fg='red')
            self.square[6].config(fg='red')
            return True

        if self.board[1] == 'X' and self.board[4] == 'X' and self.board[7] == 'X':
            self.square[1].config(fg='red')
            self.square[4].config(fg='red')
            self.square[7].config(fg='red')
            return True

        if self.board[2] == 'X' and self.board[5] == 'X' and self.board[8] == 'X':
            self.square[2].config(fg='red')
            self.square[5].config(fg='red')
            self.square[8].config(fg='red')
            return True

        if self.board[2] == 'X' and self.board[4] == 'X' and self.board[6] == 'X':
            self.square[2].config(fg='red')
            self.square[4].config(fg='red')
            self.square[6].config(fg='red')
            return True

        if self.board[3] == 'X' and self.board[4] == 'X' and self.board[5] == 'X':
            self.square[3].config(fg='red')
            self.square[4].config(fg='red')
            self.square[5].config(fg='red')
            return True

        if self.board[6] == 'X' and self.board[7] == 'X' and self.board[8] == 'X':
            self.square[6].config(fg='red')
            self.square[7].config(fg='red')
            self.square[8].config(fg='red')
            return True

        return False

"""
This starts the program. It initializes Arti and the initializes the Tic-Tac-Toe GUI window.
"""
if __name__ == "__main__":
    arti = Arti_1_0()
    ttt = TicTacToe(arti)
    arti.addListener(ttt) # Have the Tic-Tac-Toe game and GUI receive messages from Arti

    ttt.mainloop()