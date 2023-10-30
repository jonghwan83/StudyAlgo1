import os

import sys

import time

from solution import init, add, remove, count



CMD_INIT = 1

CMD_ADD = 2

CMD_REMOVE = 3

CMD_COUNT = 4



def run():

    q = int(input())
    
    okay = False
    
    for i in range(q):
    
        inputarray = input().split()
        
        cmd = int(inputarray[0])
    
    
    
        if cmd == CMD_INIT:
        
            n = int(inputarray[1])
        
            init(n)
            
            okay = True
        
        elif cmd == CMD_ADD:
        
            mid = int(inputarray[1])
            
            msize = int(inputarray[2])
            
            ans = int(inputarray[3])
            
            ret = add(mid, msize)
        
            if ans != ret:
            
                okay = False
            
        elif cmd == CMD_REMOVE:
            
            mid = int(inputarray[1])
            
            ans = int(inputarray[2])
            
            ret = remove(mid)
        
            if ans != ret:
            
                okay = False
        
        elif cmd == CMD_COUNT:
        
            mstart = int(inputarray[1])
            
            mend = int(inputarray[2])
            
            ans = int(inputarray[3])
            
            ret = count(mstart, mend)
        
            if ans != ret:
            
                okay = False
        
        else:
        
            okay = False

    return okay



if __name__ == '__main__':

    start = time.time()    

    sys.stdin = open('Algorithm/FileRepo/sample_input.txt', 'r')
    
    inputarray = input().split()
    
    TC = int(inputarray[0])
    
    MARK = int(inputarray[1])
    
    
    for testcase in range(1, TC + 1):
    
        score = MARK if run() else 0
    
        print("#%d %d" % (testcase, score), flush = True)

    print('elapsed: ', time.time() - start)
