import sys

# sys.stdin = open('sample.txt', 'r')
n = int(sys.stdin.readline())
A = list(map(int, sys.stdin.readline().split()))
operators = list(map(int, sys.stdin.readline().split()))

max_val = -sys.maxsize
min_val = sys.maxsize

def calculate(ans, idx):
    global max_val, min_val

    if idx == n:
        if ans > max_val:
            max_val = ans
        if ans < min_val:
            min_val = ans
        return

    for i in range(len(operators)):
        
        if operators[i] > 0:
            operators[i] -= 1
            if i == 0:
                calculate(ans + A[idx], idx + 1)   
            elif i == 1:
                calculate(ans - A[idx], idx + 1)
            elif i == 2:
                calculate(ans * A[idx], idx + 1)
            else:
                if ans // A[idx] < 0:
                    calculate(-(-ans // A[idx]), idx + 1)
                else:
                    calculate(ans // A[idx], idx + 1)
            operators[i] += 1


ans = A[0]
idx = 1
calculate(ans, idx)
print(max_val)
print(min_val)
