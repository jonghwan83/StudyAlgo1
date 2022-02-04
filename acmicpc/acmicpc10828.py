import sys

class Stack:
    def __init__(self):
        self.arr = []
        self.int_pushed = 0

    def getInput(self, command):
        if command == 'push':
            self.arr.append(self.int_pushed)
        elif command == 'pop':
            if len(self.arr) == 0:
                print(-1)
            else:
                print(self.arr[-1])
                self.arr = self.arr[:-1]
        elif command == 'size':
            print(len(self.arr))
        elif command == 'empty':
            if len(self.arr) == 0:
                print(1)
            else:
                print(0)
        elif command == 'top':
            if len(self.arr) == 0:
                print(-1)
            else:
                print(self.arr[-1])

stack = Stack()

T = int(sys.stdin.readline())
for _ in range(T):
    C = sys.stdin.readline().split()
    if len(C) > 1:
        stack.int_pushed = C[1]
    stack.getInput(C[0])
