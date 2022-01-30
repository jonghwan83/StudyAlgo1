import sys

n = int(sys.stdin.readline())
for _ in range(n):
    chk = 0
    parenthesis = sys.stdin.readline().strip()
    for s in parenthesis:
        if chk < 0:
            break
        if s == '(':
            chk += 1
        else:
            chk -= 1
    if chk == 0:
        print('YES')
    else:
        print('NO')
        