import sys
import time
from solution import init, newCivilization, getCivilization, mergeCivilization, removeCivilization, getCivilizationArea

def run():
    okay = False

    Q = int(sys.stdin.readline())

    for _ in range(Q+1):
        cmd = list(map(int, sys.stdin.readline().split()))
        if cmd[0] == 100:
            init(cmd[1])
            okay = True
        
        elif cmd[0] == 200:
            if okay:
                ret = newCivilization(cmd[1], cmd[2], cmd[3])
            ans = cmd[4]
            if ans != ret:
                okay= False
        
        elif cmd[0] == 300:
            if okay:
                ret = removeCivilization(cmd[1])
            ans = cmd[2]
            if ans != ret:
                okay = False
        
        elif cmd[0] == 400:
            if okay:
                ret = getCivilization(cmd[1], cmd[2])
            ans = cmd[3]
            if ans != ret:
                okay = False
        
        elif cmd[0] == 500:
            if okay:
                ret = getCivilizationArea(cmd[1])
            ans = cmd[2]
            if ans != ret:
                okay = False

        elif cmd[0] == 600:
            if okay:
                ret = mergeCivilization(cmd[1], cmd[2])
            ans = cmd[3]
            if ans != ret:
                okay = False

    return okay

start = time.time()

sys.stdin = open('sample_input.txt', 'r')

T, MARK = map(int, sys.stdin.readline().split())

for tc in range(1, T+1):
    score = run()
    print('#{} {}'.format(tc, score * 100))
    # if tc == 19:
    #     break

end = time.time()
print('elapsed: ', end - start)