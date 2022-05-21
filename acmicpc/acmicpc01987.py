import sys

# sys.stdin = open('acmicpc\sample.txt', 'r')

row, col = map(int, sys.stdin.readline().split())
board = [[0 for _ in range(col)] for _ in range(row)]
for i in range(row):
    temp = sys.stdin.readline().strip()
    temp2 = []
    for idx, val in enumerate(temp):
        board[i][idx] = ord(val) - 65

visited = [[0 for _ in range(col)] for _ in range(row)]
visited_letter = [0] * 26

drows = [-1, 1, 0, 0]
dcols = [0, 0, -1, 1]

n_letters = 0

def dfs(r, c, count):
    global n_letters

    n_letters = max(n_letters, count)

    for i in range(4):
        dr = r + drows[i]
        dc = c + dcols[i]

        if (0 <= dr < row) and (0 <= dc < col) and not visited[dr][dc] and not visited_letter[board[dr][dc]]:
            visited[dr][dc] = 1
            visited_letter[board[dr][dc]] = 1
            dfs(dr, dc, count + 1)
            visited[dr][dc] = 0
            visited_letter[board[dr][dc]] = 0

visited[0][0] = 0
visited_letter[board[0][0]] = 1
dfs(0, 0, 1)

print(n_letters)
