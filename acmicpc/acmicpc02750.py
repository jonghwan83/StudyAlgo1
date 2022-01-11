'''
N개의 수가 주어졌을 때, 이를 오름차순으로 정렬하는 프로그램을 작성하시오.
[3, 2, 1]
'''

import sys

T = int(sys.stdin.readline())
lst = []
for _ in range(T):
    lst.append(int(sys.stdin.readline()))

for i in range(1, len(lst)):
    temp = lst[i]
    position = i-1
    while (position >= 0):
        if (lst[position] > temp):
            lst[position + 1] = lst[position]
            position -= 1

        else:
            break
    lst[position+1] = temp

for i in lst:
    print(i)