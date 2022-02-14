import sys

N, K = map(int, sys.stdin.readline().split())

visited = [0] * 100001
arr = [N]

while arr:
    temp = arr.pop(0)
    if temp == K:
        print(visited[temp])
        break

    for val in (temp+1, temp-1, temp*2):
        if (0 <= val <= 100000) and not visited[val]:
            visited[val] = visited[temp] + 1
            arr.append(val)