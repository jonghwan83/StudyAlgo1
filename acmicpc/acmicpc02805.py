import sys

def cut(output):
    target = 0
    for tree in trees:
        if (tree - output) > 0:
            target += (tree - output)
    return target

n, m = map(int, sys.stdin.readline().split())
trees = list(map(int, sys.stdin.readline().split()))

left = 0
right = max(trees)
output = (left + right) // 2
while left <= right:
    target = cut(output)

    if target >= m:
        left = output + 1
    elif target < m:
        right = output - 1
    output = (left + right) // 2

print(output)