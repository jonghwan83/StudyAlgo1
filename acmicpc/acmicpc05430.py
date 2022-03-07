import time

t0 = time.time()

import sys

class Node:
    def __init__(self, value):
        self.data = value
        self.prev = None
        self.next = None

class Queue:
    def __init__(self):
        self.head = None
        self.tail = None
        self.length = 0
        self.revflag = 0

    def push(self, a):
        new_node = Node(a)
        if self.head:
            if self.length > 1:
                last = self.tail
                last.next = new_node
                new_node.prev = last
                self.tail = new_node
            elif self.length == 1:
                self.head.next = new_node
                new_node.prev= self.head
                self.tail = new_node

        else:
            self.head = new_node
            self.tail = self.head
        self.length += 1

    def pop(self):
        if self.revflag == 0:
            if self.head:
                if self.length > 1:
                    self.head = self.head.next
                    self.head.prev = None
                elif self.length == 1:
                    self.head = None
                    self.tail = None
                self.length -= 1
                if self.length == 1:
                    self.tail = self.head
                return True
            else:
                return False
        else:
            if self.tail:
                if self.length > 1:
                    self.tail = self.tail.prev
                    self.tail.next = None
                elif self.length == 1:
                    self.head = None
                    self.tail = None
                self.length -= 1
                if self.length == 1:
                    self.head = self.tail
                return True
            else:
                return False

    def traverse(self):
        if self.length == 0:
            return '[]'

        arr = ''
        if self.revflag == 0:
            cnode = self.head
            arr += (str(cnode.data) + ',')
            while cnode.next:
                cnode = cnode.next
                arr += (str(cnode.data) + ',')

        elif self.revflag == 1:
            cnode = self.tail
            arr += (str(cnode.data) + ',')
            while cnode.prev:
                cnode = cnode.prev
                arr += (str(cnode.data) + ',')

        return '[' + arr[:len(arr)-1] + ']'

    def changeflag(self):
        if self.revflag == 0:
            self.revflag = 1
        else:
            self.revflag = 0

t = int(sys.stdin.readline())
for _ in range(t):
    cmd = sys.stdin.readline().strip()
    n = int(sys.stdin.readline())
    numbers = sys.stdin.readline().strip()

    queue = Queue()
    i = 0
    temp = ''
    while True:
        if numbers[i] == ']':
            try:
                queue.push(int(temp))
            except:
                pass
            temp = ''
            break
        
        if numbers[i] != '[' and numbers[i] != ',':
            temp += numbers[i]
        
        if numbers[i] == ',':
            try:
                queue.push(int(temp))
            except:
                pass
            temp = ''
        i += 1

    ans = True
    for s in cmd:
        if s == 'R':
            queue.changeflag()
        elif s == 'D':
            ans = queue.pop()
    if ans:
        print(queue.traverse())
    else:
        print('error')

t1 = time.time()
print('elapsed: {}'.format(t1- t0))