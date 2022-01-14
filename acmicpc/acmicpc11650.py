'''
2차원 평면 위의 점 N개가 주어진다. 
좌표를 x좌표가 증가하는 순으로, 
x좌표가 같으면 y좌표가 증가하는 순서로 정렬한 다음 출력하는 프로그램을 작성하시오.
'''
import sys

N = int(sys.stdin.readline())
lst = [[] for _ in range(N)]
for i in range(N):
    lst[i] = list(map(int, sys.stdin.readline().split()))

lst.sort()

for val in lst:
    print(val[0], val[1])