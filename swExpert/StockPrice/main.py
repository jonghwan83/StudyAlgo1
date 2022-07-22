import sys
import time
from solution import init, changePrices, getPrice, getMostIncreasedStock

sys.stdin = open('sample_input.txt', 'r')
T, MARK = map(int, sys.stdin.readline().split())

def run():
    ok = False

    N = int(sys.stdin.readline())
    mStocks = []
    mStocks = list(sys.stdin.readline().split())
    mPrices = list(sys.stdin.readline().split())

    M = int(sys.stdin.readline())

    for i in range(M):
        if i == 0:
            init(N, mStocks, mPrices)
            ok = True
        
        command = list(sys.stdin.readline().split())

        if int(command[0]) == 100:
            
            changePrices(command[1], command[2], command[3])
        
        elif int(command[0]) == 200:
            ret = getPrice(command[1])
            ans = int(command[2])
            if ans != ret:
                ok = False

        elif int(command[0]) == 300:
            
            ret = getMostIncreasedStock(command[1], command[2])
            ans = int(command[3])
            if ans != ret:
                ok = False
    
    return ok

if __name__ == "__main__":
    start = time.time()

    for tc in range(1, T+1):
        score = run()
        print('#{} {}'.format(tc, score * 100))
        # if tc == 4:
        #     break
    
    end = time.time()
    print('elapsed: ', end - start)