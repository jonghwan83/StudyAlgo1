import sys

n = int(sys.stdin.readline())
words = [[] for _ in range(51)]
for _ in range(n):
    word = sys.stdin.readline().strip()
    if word not in words[len(word)]:
        words[len(word)].append(word)

for lst in words:
    lst.sort()
    for word in lst:
        print(word)
        