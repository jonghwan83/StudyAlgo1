import time
import sys
from solution import init, addBar, removeBar



def run():
    query_num = int(sys.stdin.readline())
    ok = False

    for q in range(query_num):
        input_list = list(map(int, sys.stdin.readline().split()))
        query = input_list[0]

        if query == 100:
            N = input_list[1]
            init(N)
            ok = True

        elif query == 200:
            mID, mLength, mRow, mCol, mDir, ans = input_list[1:]
            ret = addBar(mID, mLength, mRow, mCol, mDir)
            if ans != ret:
                ok = False

        elif query == 300:
            mID, ans = input_list[1:]
            ret = removeBar(mID)
            if ans != ret:
                ok = False

        if q == 2:
            break

    return ok

if __name__ == "__main__":
    start = time.time()

    sys.stdin = open('sample_input.txt', 'r')
    T, MARK = map(int, sys.stdin.readline().split())
    
    for tc in range(1, T+1):
        score = run()
        print("#{} {}".format(tc, score*100))
        if tc == 1:
            break

    end = time.time()
    print('elapsed:', end - start)
