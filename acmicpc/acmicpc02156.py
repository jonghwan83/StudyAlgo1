'''
포도주 잔을 선택하면 그 잔에 들어있는 포도주는 모두 마셔야 하고, 
마신 후에는 원래 위치에 다시 놓아야 한다.
연속으로 놓여 있는 3잔을 모두 마실 수는 없다.
input:  6, 10, 13, 9, 8, 1
output: 33 (0, 1, 3, 4)

'''

import sys

n = int(sys.stdin.readline())
wines = []
for _ in range(n):
    wines.append(int(sys.stdin.readline()))

lst = [0] * n
if n >= 1:
    lst[0] = wines[0]
if n >= 2:
    lst[1] = wines[1] + wines[0]
if n >= 3:
    lst[2] = max(wines[0] + wines[1], wines[1] + wines[2],
    wines[0] + wines[2])
if n > 3:
    for i in range(3, len(wines)):
        lst[i] = max(lst[i-3] + wines[i-1] + wines[i], lst[i-2] + wines[i])
        lst[i] = max(lst[i-1], lst[i])

print(max(lst))