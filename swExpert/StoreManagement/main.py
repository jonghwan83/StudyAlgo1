import sys
from solution import init, buy, sell, cancel, refund
import time

start = time.time()
    
def run():
    q = int(sys.stdin.readline())
    okay = False
    
    for _ in range(q):
        input_list = list(map(int, sys.stdin.readline().split()))
        cmd = input_list[0]
        
        if cmd == 1:
            init()
            okay = True

        elif cmd == 2:
            bid, product, price, quantity, ans = input_list[1:]
            ret = buy(bid, product, price, quantity)
            if (ans != ret):
                okay = False

        elif cmd == 3:
            bid, ans = input_list[1:]
            ret = cancel(bid)
            if (ans != ans):
                okay = False

        elif cmd == 4:
            sid, product, price, quantity, ans = input_list[1:]
            ret = sell(sid, product, price, quantity)
            if (ans != ret):
                okay = False

        elif cmd == 5:
            sid, ans = input_list[1:]
            ret = refund(sid)
            if ans != ret:
                okay = False

    return okay
        

sys.stdin = open('swExpert/StoreManagement/sample_input.txt', 'r')
T, mark = map(int, sys.stdin.readline().split())

score = run()
print('#{} {}'.format(1, score*100))

end = time.time()
print('elapsed: ', end - start)
