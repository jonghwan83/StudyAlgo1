import sys

sys.stdin = open('acmicpc/sample.txt', 'r')

v, e = map(int, sys.stdin.readline().split())

weights = []
for i in range(e):
    node, nxt, w = map(int, sys.stdin.readline().split())
    weights.append([w, node, nxt])

weights.sort()
parent = [i for i in range(v+1)]

def find(x):
    if parent[x] == x:
        return x
    else:
        return find(parent[x])

answer = 0
for w, node, nxt in weights:
    x = find(node)
    y = find(nxt)
    if x != y:
        parent[y] = x
        answer += w

print(answer)