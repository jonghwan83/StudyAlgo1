import sys

# sys.stdin = open('acmicpc/sample.txt', 'r')

square = []
P = []
for i in range(9):
    temp = list(map(int, sys.stdin.readline().split()))
    for j in range(9):
        if temp[j] == 0:
            P.append([i, j])
    square.append(temp)

def verify(k, idx):
    H = True
    for i in range(9):
        if square[P[k][0]][i] == idx:
            H = False
            return H
    
    V = True
    for i in range(9):
        if square[i][P[k][1]] == idx:
            V = False
            return V

    S = True
    r = 3 * (P[k][0] // 3)
    c = 3 * (P[k][1] // 3)

    for i in range(3):
        for j in range(3):
            if square[r+i][c+j] == idx:
                S = False
                return S
    
    result = H & V & S
    
    return result

def solve(k):
    # base case
    if k >= len(P):
        for i in range(9):
            print(*square[i], sep=' ')
        exit(0)

    for i in range(1, 10):
        if verify(k, i): # 어떤 조건을 만족할 때, 
            square[P[k][0]][P[k][1]] = i
            solve(k+1)
            square[P[k][0]][P[k][1]] = 0 # back tracking을 위해서 원래 값을 되돌려 놓음 


solve(0)
