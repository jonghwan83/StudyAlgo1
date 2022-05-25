from ast import operator
import string
import sys

sys.stdin = open('acmicpc/sample.txt', 'r')

strings = sys.stdin.readline()

operands = []
operators = []
temp = ''
for s in strings:
    if (s == '+') or (s == '-'):
        operators.append(s)
        operands.append(int(temp))
        temp = ''
    else:
        temp += s
operands.append(int(temp))

visited = [0] * len(operator)
