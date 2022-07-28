import time
import sys
from solution import init, loginID, closeIDs, connectCnt, waitOrder

def run():
    q = int(sys.stdin.readline())
    okay = False

    for i in range(q):
        cmd = list(sys.stdin.readline().split())

        if int(cmd[0]) == 0:
            init()
            okay = True
            
        elif int(cmd[0]) == 1:
            loginID(cmd[1])
        
        elif int(cmd[0]) == 2:
            ret = closeIDs(cmd[2])
            ans = int(cmd[1])
            if ans != ret:
                okay = False
            # print(cmd[0], ans, ret, okay)

        elif int(cmd[0]) == 3:
            connectCnt(int(cmd[1]))

        elif int(cmd[0]) == 4:
            ans = int(cmd[1])
            ret = waitOrder(cmd[2])
            if ans != ret:
                okay = False
            # print(cmd[0], ans, ret, okay)
        
    return okay


if __name__ == '__main__':
    start = time.time()
    sys.stdin = open('sample_input.txt', 'r')

    T, MARK = map(int, sys.stdin.readline().split())

    for tc in range(1, T+1):
        score = run()
        print('#{} {}'.format(tc, score * 100))
        # if tc == 1:
        #     break

    end = time.time()
    print('elapsed:', end - start)
