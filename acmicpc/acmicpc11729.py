'''
하노이의 탑
세 개의 장대가 있고 첫 번째 장대에는 반경이 서로 다른 n개의 원판이 쌓여 있다. 
각 원판은 반경이 큰 순서대로 쌓여있다. 
이제 수도승들이 다음 규칙에 따라 첫 번째 장대에서 세 번째 장대로 옮기려 한다.

1. 한 번에 한 개의 원판만을 다른 탑으로 옮길 수 있다.
2. 쌓아놓은 원판은 항상 위의 것이 아래의 것보다 작아야 한다.

최소의 이동 순서를 출력

츨력: 이동 횟수 K
수행과정 A B -> A: A번째 탑 에서 B번째 탑으로

장대는 3개

base problem n = 1
'''

import sys

steps = []
def solve_hanoi(n, source, target, aux):
	if (n == 1):
		steps.append([source, target])
		return
	solve_hanoi(n-1, source, aux, target)
	steps.append([source, target])
	solve_hanoi(n-1, aux, target, source)

n = int(sys.stdin.readline())

solve_hanoi(n, 1, 3, 2)
print(len(steps))
for s in steps:
	print(s[0], s[1])