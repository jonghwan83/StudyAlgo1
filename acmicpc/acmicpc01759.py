import sys

l, c = map(int, sys.stdin.readline().split())
chars = list(sys.stdin.readline().split())
chars.sort()

check = [0] * c
vowels = ['a', 'e', 'i', 'o', 'u']

def dfs(k):
    if sum(check) == l:
        ans = []
        for i in range(c):
            if check[i] == 1:
                ans.append(chars[i])
        
        n_vowels = 0
        n_consonant = 0
        for s in ans:
            if s in vowels:
                n_vowels += 1
            else:
                n_consonant += 1
        if n_vowels > 0 and n_consonant > 1:
            print(*ans, sep='')
        return

    for i in range(k, c):
        check[i] = 1
        dfs(i+1)
        check[i] = 0

dfs(0)
