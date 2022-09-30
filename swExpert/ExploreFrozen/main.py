import sys
import time
from solution import init, minDamage

def run():
    CMD_INIT = 1
    CMD_MIN_DAMAGE = 2

    isCorrect = False

    numQuery = int(sys.stdin.readline())

    for q in range(numQuery):
        input_data = list(map(int, sys.stdin.readline().split()))
        cmd = input_data[0]
        
        if cmd == CMD_INIT:
            R, C, M = input_data[1:4]
            mStructureR = input_data[4:4+M]
            mStructureC = input_data[4+M:4+M+M]
            init(R, C, M, mStructureR, mStructureC)
            isCorrect = True

        elif cmd == CMD_MIN_DAMAGE:
            mStartR, mStartC, mEndR, mEndC, ans = input_data[1:]
            userAns = minDamage(mStartR, mStartC, mEndR, mEndC)
            if userAns != ans:
                isCorrect = False

    return isCorrect

if __name__ == "__main__":
    start = time.time()

    sys.stdin = open('sample_input.txt', 'r')

    T, MARK = map(int, sys.stdin.readline().split())
    for tc in range(1, T+1):
        score = run()   
        print("#{} {}".format(tc, MARK * score))

    end = time.time()
    print('elapsed:', end - start)
