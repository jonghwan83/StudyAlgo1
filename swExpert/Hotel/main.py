import time
import sys
from solution import init, addRoom, findRoom, riseCosts

ADDROOM = 100
FINDROOM = 200
RISECOSTS = 300
END = 400

hotelID = 0
roomID = 0
roomInfo = [ 0, 0, 0, 0, 0]
filterInfo = [ 0, 0, 0, 0, 0, 0]
roomCnt = []

def run():
    inputs = iter(sys.stdin.readline().split())
    n = int(next(inputs))

    roomCnt.clear();
    for _ in range(n):
        roomCnt.append(int(next(inputs)))

    init(n, roomCnt)

    isCorrect = True
    
    q = 0
    while(1):
        q += 1
        inputs = iter(sys.stdin.readline().split())
        cmd = int(next(inputs))

        if cmd == ADDROOM:
            hotelID = int(next(inputs))
            roomID = int(next(inputs))
            for i in range(5):
                roomInfo[i] = int(next(inputs))

            addRoom(hotelID, roomID, roomInfo)

        elif cmd == FINDROOM:
            for i in range(6):
                filterInfo[i] = int(next(inputs))

            userAns = findRoom(filterInfo)

            correctAns = int(next(inputs))
            if userAns != correctAns:
                isCorrect = False
            # print(q, userAns, correctAns, isCorrect)

        elif cmd == RISECOSTS:
            hotelID = int(next(inputs))
            userAns = riseCosts(hotelID)
            correctAns = int(next(inputs))

            if userAns != correctAns:
                isCorrect = False
            # print(q, hotelID, userAns, correctAns, isCorrect)
        elif cmd == END:
            break

    return isCorrect

if __name__ == '__main__':
    start = time.time()
    sys.stdin = open('sample_input.txt', 'r')
    inputarray = input().split()
    TC = int(inputarray[0])
    MARK = int(inputarray[1])

    for testcase in range(1, TC + 1):
        score = MARK if run() else 0
        print("#%d %d" % (testcase, score), flush = True)
        # if testcase == 2:
        #     break

    end = time.time()
    print('elpased:', end - start)
