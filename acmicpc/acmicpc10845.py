import sys

class Node:
    def __init__(self, value):
        self.data = value
        self.next = None

class Queue:
    def __init__(self):
        self.head = None
        self.tail = None
        self.length = 0

    def size(self):
        return self.length
    
    def empty(self):
        if self.length == 0:
            return 1
        else:
            return 0

    def push(self, a):
        new_node = Node(a)
        if self.head:
            self.tail.next = new_node
            self.tail = new_node
        else:
            self.head = new_node
            self.tail = self.head
        self.length += 1
    
    def pop(self):
        if self.length < 1:
            return -1
        temp = self.head.data
        self.length -= 1
        nxt = self.head.next
        self.head = nxt
        
        if self.length < 1:
            self.tail = None
        elif self.length == 1:
            self.tail = self.head
        return temp

    def front(self):
        if self.head:
            return self.head.data
        else:
            return -1
        
    def back(self):
        if self.tail:
            return self.tail.data
        else:
            return -1

queue = Queue()

N = int(sys.stdin.readline())
for _ in range(N):
    cmd = list(sys.stdin.readline().split())
    if cmd[0] == 'push':
        queue.push(int(cmd[1]))
    elif cmd[0] == 'pop':
        print(queue.pop())
    elif cmd[0] == 'size':
        print(queue.size())
    elif cmd[0] == 'empty':
        print(queue.empty())
    elif cmd[0] == 'front':
        print(queue.front())
    elif cmd[0] == 'back':
        print(queue.back())
