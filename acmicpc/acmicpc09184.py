'''
if a <= 0 or b <= 0 or c <= 0, then w(a, b, c) returns:
    1

if a > 20 or b > 20 or c > 20, then w(a, b, c) returns:
    w(20, 20, 20)

if a < b and b < c, then w(a, b, c) returns:
    w(a, b, c-1) + w(a, b-1, c-1) - w(a, b-1, c)

otherwise it returns:
    w(a-1, b, c) + w(a-1, b-1, c) + w(a-1, b, c-1) - w(a-1, b-1, c-1)

The input for your program will be a series of integer triples, one per line, 
until the end-of-file flag of -1 -1 -1. Using the above technique, 
you are to calculate w(a, b, c) efficiently and print the result.
'''

import sys

def w(a, b, c, mz):
    ind = str(a) + '_' + str(b) + '_' + str(c)
    if ind in mz:
        return mz[ind]
    if a <= 0 or b <= 0 or c <= 0:
        mz[ind] = 1
    elif a > 20 or b > 20 or c > 20:
        return w(20, 20, 20, mz)
    elif a < b and b < c:
        mz[ind] = w(a, b, c-1, mz) + w(a, b-1, c-1, mz) - w(a, b-1, c, mz)
    else:
        mz[ind] = w(a-1, b, c, mz) + w(a-1, b-1, c, mz) + w(a-1, b, c-1, mz) - w(a-1, b-1, c-1, mz)
    return mz[ind]
mz = {}
while True:
    a, b, c = map(int, sys.stdin.readline().split())
    if a == -1 and b == -1 and c == -1:
        break
    
    print('w({}, {}, {}) = {}'.format(a, b, c, w(a, b, c, mz)))