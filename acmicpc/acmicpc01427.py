import sys

n = int(sys.stdin.readline())
count = [0 for _ in range(10)]
for s in str(n):
    count[int(s)] += 1
output = ''

for i in range(len(count)-1, -1, -1):
    if count[i] != 0:
        output += str(i) * count[i]

print(output)