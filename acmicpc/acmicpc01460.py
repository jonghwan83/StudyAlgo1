'''
time complexity of insert -> O(N)
'''

import time

t0 = time.time()

import sys

class Node:
    def __init__(self, value):
        self.data = value
        self.next = None

class DLinked:
    def __init__(self):
        self.head = None
        self.length = 0

    def read(self, index):
        current_node = self.head
        current_index = 0

        while current_index < index:
            current_node = current_node.next
            current_index += 1

        try:
            return current_node.data
        except:
            return None

    def insert(self, index, value):
        new_node = Node(value)
        self.length += 1

        if self.head:
            if index == 0:
                new_node.next = self.head
                self.head = new_node
            else:
                current_node = self.head
                current_index = 0

                while current_index < index - 1:
                    current_node = current_node.next
                    current_index += 1
                new_node.next = current_node.next
                current_node.next = new_node
        else:
            self.head = new_node

    def delete(self, index):
        self.length -= 1
        if index == 0:
            self.head = self.head.next
            return
        
        current_node = self.head
        current_index = 0
        while current_index < index - 1:
            current_node = current_node.next
            current_index += 1
        node_after_deleted_node = current_node.next.next
        current_node.next = node_after_deleted_node


txt = sys.stdin.readline().strip()

linked = DLinked()
for i, s in enumerate(txt):
    linked.insert(i, s)
cursor = i

n = int(sys.stdin.readline())
for _ in range(n):
    cmd = sys.stdin.readline().strip()
    if cmd[0] == 'L' and cursor > -1:
        cursor -= 1
    elif cmd[0] == 'D' and cursor < linked.length - 1:
        cursor += 1
    elif cmd[0] == 'P':
        cursor += 1
        linked.insert(cursor, cmd[2])
    elif cmd[0] == 'B' and cursor > -1:
        linked.delete(cursor)
        cursor -= 1

index = 0
while linked.read(index):
    print(linked.read(index), end='')
    index += 1
print()

t1 = time.time()
print('elapsed: {}'.format(t1- t0))
