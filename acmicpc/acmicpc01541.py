import sys

# sys.stdin = open('acmicpc\sample.txt', 'r')

k = sys.stdin.readline()
n = k.split('-')

answer = 0
x = sum(map(int, n[0].split('+')))
if n[0] == '-':
    answer -= x
else:
    answer += x

for y in n[1:]:
    y = sum(map(int, y.split('+')))
    answer -= y

print(answer)