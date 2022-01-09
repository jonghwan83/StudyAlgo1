'''
For a positive integer N, the digit-sum of N is defined as the sum of N itself and its digits. 
When M is the digitsum of N, we call N a generator of M.
For example, the digit-sum of 245 is 256 (= 245 + 2 + 4 + 5). Therefore, 245 is a generator of 256.

Not surprisingly, some numbers do not have any generators and some numbers have more than one generator. 
For example, the generators of 216 are 198 and 207.

You are to write a program to find the smallest generator of the given integer.

입력

출력
the smallest generator
no generator -> 0
'''
import sys

def get_smallest(n):
    for i in range(n):
        temp = []
        for s in str(i):
            temp.append(int(s))
        temp_k = sum(temp) + i
        if temp_k == n:
            return i
    else:
        return 0

n = int(sys.stdin.readline())
print(get_smallest(n))
