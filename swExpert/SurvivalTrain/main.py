import sys
from solution import init, destroy, update, updateByJob, move
import time

CMD_INIT = 100
CMD_DESTROY = 200
CMD_UPDATE = 300
CMD_UPDATE_JOB = 400
CMD_MOVE = 500

def run():
    N = int(sys.stdin.readline())
    isOK = 0
    for c in range(N):
        cmd = list(map(int, sys.stdin.readline().split()))

        if cmd[0] == CMD_INIT:
            mN, mM, mJ = cmd[1:4]
            Point = cmd[4:4+mN]
            JobID = cmd[4+mN:]
            init(mN, mM, mJ, Point, JobID)
            isOK = 1

        if cmd[0] == CMD_UPDATE:
            mID, mPoint, check = cmd[1:]
            result = update(mID, mPoint)
            if result != check:
                isOK = 0
            # print(c, result, check, isOK)

        if cmd[0] == CMD_UPDATE_JOB:
            mJobID, mPoint, check = cmd[1:]
            result = updateByJob(mJobID, mPoint)
            if result != check:
                isOK = 0
            # print(c, result, check, isOK)

        if cmd[0] == CMD_MOVE:
            mNum, check = cmd[1:]
            result = move(mNum)
            if result != check:
                isOK = 0
            # print(c, result, check, isOK)

    return isOK


if __name__=="__main__":
    start = time.time()

    sys.stdin = open('sample_input.txt', 'r')

    T, MARK = map(int, sys.stdin.readline().split())
    for tc in range(1, T+1):
        score = run()
        print('#{} {}'.format(tc, score*100))
        # if tc == 2:
        #     break

    end = time.time()
    print('elapsed:', end - start)
