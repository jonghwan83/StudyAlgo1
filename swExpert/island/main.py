import sys
import time
from solution import init, numberOfCandidate, maxArea


def run():
    numQuery = int(sys.stdin.readline())
    isCorrect = False

    for i in range(numQuery):
        sample_data = list(map(int, sys.stdin.readline().split()))
        
        cmd = sample_data[0]
        
        if cmd == 1:
            N = sample_data[1]
            mMap = [[0] * N for _ in range(N)]
            for i, val in enumerate(sample_data[2:]):
                mMap[i//N][i%N] = val
            
            init(N, mMap)
            isCorrect = True

        elif cmd == 2:
            M = sample_data[1]
            mStructure = sample_data[2:-1]
            ans = sample_data[-1]
            userAns = numberOfCandidate(M, mStructure)
            if userAns != ans:
                isCorrect = False

        elif cmd == 3:
            M = sample_data[1]
            mSturcture = sample_data[2:-2]
            mSeaLevel = sample_data[-2]
            ans = sample_data[-1]
            userAns = maxArea(M, mStructure, mSeaLevel)
            if userAns != ans:
                isCorrect = False    

    return isCorrect


if __name__ == "__main__":
    start = time.time()

    sys.stdin = open('sample_input.txt', 'r')
    T, MARK = map(int, sys.stdin.readline().split())

    for tc in range(1, T+1):
        score = run()
        print("#{} {}".format(tc, score * 100))

    end = time.time()
    print('elapsed:', end - start)
