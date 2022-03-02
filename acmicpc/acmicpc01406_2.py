'''
출력 초과 발생
'''

import sys

class Node:
    def __init__(self, value):
        self.data = value
        self.next = None
        self.prev = None

class Dlinked:
    def __init__(self):
        self.head = None
        self.current = None
        self.length = 0
        self.cursor = -1   

    def read(self):
        return self.current.data

    def moveleft(self):
        if self.cursor > -1:
            self.cursor -= 1
            if self.current.prev:
                self.current = self.current.prev

    def moveright(self):
        if self.cursor < self.length - 1:
            self.cursor += 1
            self.current = self.current.next
        
    def insert(self, value):
        new_node = Node(value)
        if self.head:
            if self.cursor == -1:
                self.current.prev = new_node
                new_node.next = self.current
                self.head = new_node
                self.current = self.head
            elif self.cursor == self.length - 1:
                new_node.prev = self.current
                self.current.next = new_node
                self.current = new_node
            else:
                current_next = self.current.next
                new_node.prev = self.current
                self.current.next = new_node

                current_next.prev = new_node
                new_node.next = current_next

                self.current = new_node
        else:
            self.head = new_node
            self.current = self.head
        self.length += 1
        self.cursor += 1

    def delete(self):
        if self.cursor < 0:
            return
        if self.cursor == 0:
            self.head = self.head.next
            self.current = self.head
        elif self.cursor == self.length - 1:
            self.current = self.current.prev
            self.current.next = None
        else:
            current_prev = self.current.prev
            current_next = self.current.next
            current_prev.next = current_next
            current_next.prev = current_prev
        self.cursor -= 1
        self.length -= 1
        return
        

linked =Dlinked()

txt = sys.stdin.readline().strip()
for s in txt:
    linked.insert(s)

n = int(sys.stdin.readline())
for _ in range(n):
    cmd = sys.stdin.readline()
    if cmd[0] == 'L':
        linked.moveleft()
    elif cmd[0] == 'D':
        linked.moveright()
    elif cmd[0] == 'B':
        linked.delete()
    elif cmd[0] == 'P':
        linked.insert(cmd[2])
        
linked.cursor = -1
linked.current = linked.head
while True:
    try:
        print(linked.read(), end='')
        linked.moveright()
    except:
        break
print()