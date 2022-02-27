import sys

left = []
right = []

txt = sys.stdin.readline().strip()
for s in txt:
    left.append(s)

n = int(sys.stdin.readline())
for _ in range(n):
    cmd = sys.stdin.readline()
    if cmd[0] == 'L':
        try:
            temp = left.pop()
            right.append(temp)
        except:
            pass
    elif cmd[0] == 'D':
        try:
            temp = right.pop()
            left.append(temp)
        except:
            pass
    elif cmd[0] == 'P':
        left.append(cmd[2])
    elif cmd[0] == 'B':
        try:
            left.pop()
        except:
            pass


for s in left:
    print(s, end='')
for i in range(len(right) - 1, -1, -1):
    print(right[i], end='')
print()