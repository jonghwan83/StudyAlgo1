# need to imporve time complexity

import sys

row, col = map(int, sys.stdin.readline().split())
board = []
for _ in range(row):
    board.append(sys.stdin.readline().strip())

visited = [[0 for _ in range(col)] for _ in range(row)]
visited_letter = [[[] for _ in range(col)] for _ in range(row)]

drows = [-1, 1, 0, 0]
dcols = [0, 0, -1, 1]

trip = []
def dfs(board):
    stack = []
    stack.append([0, 0])
    visited[0][0] = 1
    visited_letter[0][0].append(board[0][0])

    drows = [-1, 1, 0, 0]
    dcols = [0, 0, -1, 1]
    while stack:
        crow, ccol = stack.pop()
        for d in range(4):
            drow = crow + drows[d]
            dcol = ccol + dcols[d]
            if 0 <= drow < row and 0 <= dcol < col:
                if board[drow][dcol] not in visited_letter[crow][ccol]:
                    visited_letter[drow][dcol] = visited_letter[crow][ccol] + [board[drow][dcol]]
                    visited[drow][dcol] = visited[crow][ccol] + 1
                    stack.append([drow, dcol])
    
    return visited


print(dfs(board))
