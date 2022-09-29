import sys
import time
from solution import init, selectSeat

CMD_INIT = 100
CMD_SELECT_SEAT = 200

def run():
    okay = False
    queryCnt = int(sys.stdin.readline())
    for i in range(queryCnt):
        data = list(map(int, sys.stdin.readline().split()))
        cmd = data[0]

        if cmd == CMD_INIT:
            W, H = data[1:]
            init(W, H)
            okay = True
        
        if cmd == CMD_SELECT_SEAT:
            mSeatNum, ans = data[1:]
            res = selectSeat(mSeatNum)
            if ans != res:
                okay = False

    return okay


if __name__ == "__main__":
    start = time.time()
    sys.stdin = open('sample_input.txt', 'r')

    T, MARK = map(int, sys.stdin.readline().split())
    for tc in range(1, T + 1):
        score = run()
        print('#{} {}'.format(tc, score*100))

    end = time.time()
    print('elapsed:', end - start)