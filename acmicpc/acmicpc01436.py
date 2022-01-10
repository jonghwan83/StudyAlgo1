import sys

n = int(sys.stdin.readline())

i = 665
count = 0
while (count < n):
    if '666' in str(i):
        count += 1
    i += 1
print(i-1)