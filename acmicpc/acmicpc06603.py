import sys

while True:
    lotto = list(map(int, sys.stdin.readline().split()))
    if lotto == [0]:
        break
    
    S = lotto[1:]
    check = [0] * len(S)

    def dfs(c):
        if sum(check) == 6:
            ans = []
            for k in range(len(S)):
                if check[k] == 1:
                    ans.append(S[k])
            print(*ans, sep = ' ')
            return

        for i in range(c, len(check)):
            check[i] = 1
            dfs(i+1)
            check[i] = 0
        return

    dfs(0)
    print()