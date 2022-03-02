import sys
from collections import deque

n = int(sys.stdin.readline())
queue = deque()
for _ in range(n):
    queue.append(int(sys.stdin.readline()))

stack = [0]
ans = []
i = 1
while queue:
    k = queue.popleft()
    while k >= i:
        stack.append(i)
        ans.append('+')
        i += 1

    temp = 0
    try:
        while temp != k:
            temp = stack.pop()
            ans.append('-')
            if not stack:
                break
    except:
        break

if not stack:
    print('NO')
else:
    for s in ans:
        print(s)
        