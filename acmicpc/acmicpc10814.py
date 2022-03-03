import sys

members = [[] for _ in range(201)]

n = int(sys.stdin.readline())
for _ in range(n):
    age, name = list(sys.stdin.readline().split())
    members[int(age)].append(name)

for age in range(len(members)):
    for name in members[age]:
        print(age, name)
