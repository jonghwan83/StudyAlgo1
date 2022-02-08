import sys

def solution(w):
    if w <= 2:
        return 'NO'
    for i in range(w // 2):
        if (w - (2*i)) % 2 == 0:
            return 'YES'
    return 'NO'

w = int(sys.stdin.readline())
print(solution(w))