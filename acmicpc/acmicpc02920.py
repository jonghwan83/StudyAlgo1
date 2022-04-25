import sys

scale = list(map(int, sys.stdin.readline().split()))

diff = 0
for i in range(len(scale)-1):
    if scale[i+1] > scale[i]:
        diff += 1
    else:
        diff -= 1

if diff == 7:
    print('ascending')
elif diff == -7:
    print('descending')
else:
    print('mixed')
