import tkinter as tk
import torch
from src.Utils.helper import get_all_direct, calculate, in_chessboard
from src.model.DQN_v2 import DQN
from tkinter import messagebox

# ====================================================== #
MODEL_PATH = '../trained_model/best_model.pth'
FIRST_PLAYER = 1  # 1为人类， 0为AI
# ====================================================== #


def trans(board, now):
    tmp = torch.zeros((15, 15), dtype=torch.float, device='cuda')

    for i in range(15):
        for j in range(15):
            if board[i][j] == now:
                tmp[i][j] = 1.
            elif board[i][j] is not None:
                tmp[i][j] = -1.

    return tmp


dx = [1, 1, 1, 0]
dy = [0, 1, -1, 1]


def eq(a, b):
    return abs(a - b) < 0.1


flag = 1


class Gomoku:
    def __init__(self, master):
        self.master = master
        self.master.title("五子棋")
        self.board_size = 15
        self.cell_size = 40
        self.board = [[None for _ in range(self.board_size)] for _ in range(self.board_size)]
        self.current_player = "黑"
        self.canvas = tk.Canvas(master, width=self.board_size * self.cell_size, height=self.board_size * self.cell_size)
        self.canvas.pack()
        self.draw_board()
        self.canvas.bind("<Button-1>", self.on_click)
        self.model = DQN(8)
        self.model.load_state_dict(torch.load(MODEL_PATH).state_dict())
        self.model = self.model.cuda().eval()

    def draw_board(self):
        for i in range(self.board_size):
            self.canvas.create_line(i * self.cell_size, 0, i * self.cell_size, self.board_size * self.cell_size)
            self.canvas.create_line(0, i * self.cell_size, self.board_size * self.cell_size, i * self.cell_size)

    def on_click(self, event):
        col = event.x // self.cell_size
        row = event.y // self.cell_size
        if 0 <= row < self.board_size and 0 <= col < self.board_size and self.board[row][col] is None:
            self.make_move(row, col)

    def ai_player(self, color):
        state = trans(self.board, color)
        predict = self.model(state.unsqueeze(0).unsqueeze(0)).squeeze().view(15, 15)

        flags = [0, 0, 0, 0]
        pos = [(0, 0), (0, 0), (0, 0), (0, 0)]

        for i in range(15):
            for j in range(15):
                if not eq(state[i][j], 0):
                    continue
                state[i][j] = 1
                tmp, _ = get_all_direct(state, i, j, 1)
                for s in tmp:
                    if 'xxxxx' in s:
                        return i, j
                    if not flags[2] and '_xxxx_' in s:
                        flags[2] = 1
                        pos[2] = (i, j)
                state[i][j] = -1
                tmp, _ = get_all_direct(state, i, j, -1)
                for s in tmp:
                    if not flags[1] and 'xxxxx' in s:
                        flags[1] = 1
                        pos[1] = (i, j)
                    if not flags[3] and '_xxxx_' in s:
                        flags[3] = 1
                        pos[3] = (i, j)
                state[i][j] = 0

        for i in range(1, 4):
            if flags[i]:
                return pos[i]

        choose_list = []
        for i in range(15):
            for j in range(15):
                if not eq(state[i][j], 0):
                    continue
                for h in range(4):
                    if in_chessboard(i + dx[h], j + dy[h]) and not eq(state[i + dx[h]][j + dy[h]], 0):
                        choose_list.append([i, j])
                    elif in_chessboard(i - dx[h], j - dy[h]) and not eq(state[i - dx[h]][j - dy[h]], 0):
                        choose_list.append([i, j])
        if not len(choose_list):
            return 9, 9
        idx = [0, 0]
        m = -1000
        for x, y in choose_list:
            if predict[x][y] > m:
                m = predict[x][y]
                idx = [x, y]
        return idx[0], idx[1]

    def human_player(self, row, col):
        self.board[row][col] = self.current_player
        x = col * self.cell_size + self.cell_size // 2
        y = row * self.cell_size + self.cell_size // 2
        color = "black" if self.current_player == "黑" else "white"
        self.canvas.create_oval(x - 15, y - 15, x + 15, y + 15, fill=color)
        self.canvas.update()

        if self.check_win(row, col):
            messagebox.showinfo("游戏结束", f"{self.current_player} 胜利！")
            self.reset_board()
        else:
            self.current_player = "白" if self.current_player == "黑" else "黑"

    def ai(self):
        row, col = self.ai_player(self.current_player)
        self.board[row][col] = self.current_player
        x = col * self.cell_size + self.cell_size // 2
        y = row * self.cell_size + self.cell_size // 2
        color = "black" if self.current_player == "黑" else "white"
        self.canvas.create_oval(x - 15, y - 15, x + 15, y + 15, fill=color)
        self.canvas.update()

        if self.check_win(row, col):
            messagebox.showinfo("游戏结束", f"{self.current_player} 胜利！")
            self.reset_board()
        else:
            self.current_player = "白" if self.current_player == "黑" else "黑"

    def make_move(self, row, col):

        global flag
        if flag == 1:
            self.human_player(row, col)
            self.ai()

        else:
            self.ai()
            self.human_player(row, col)

    def check_win(self, row, col):
        tmp = trans(self.board, self.current_player)
        save, _ = get_all_direct(tmp, row, col, 1)
        _, done = calculate(save, 0)
        return done

    def reset_board(self):
        self.board = [[None for _ in range(self.board_size)] for _ in range(self.board_size)]
        self.canvas.delete("all")
        self.draw_board()
        self.current_player = "黑"


if __name__ == "__main__":
    flag = FIRST_PLAYER
    root = tk.Tk()
    gomoku_game = Gomoku(root)
    root.mainloop()
