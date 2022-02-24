import sys

class Heap:
    def __init__(self):
        self.arr = []

    def root_node(self):
        return self.arr[0]
    
    def last_node(self):
        return self.arr[-1]

    def left_child_index(self, index):
        return index * 2 + 1

    def right_child_index(self, index):
        return index * 2 + 2

    def parent_index(self, index):
        return (index - 1) // 2

    def has_fewer_child(self, index):
        if (len(self.arr) > self.left_child_index(index)) and (self.arr[index] > self.arr[self.left_child_index(index)]):
            return True
        elif (len(self.arr) > self.right_child_index(index)) and (self.arr[index] > self.arr[self.right_child_index(index)]):
            return True
        else:
            return False
    
    def calculate_fewer_child_index(self, index):
        if len(self.arr) - 1 < self.right_child_index(index):
            return self.left_child_index(index)
        
        if self.arr[self.right_child_index(index)] < self.arr[self.left_child_index(index)]:
            return self.right_child_index(index)
        else:
            return self.left_child_index(index)

    def insert(self, value):
        self.arr.append(value)
        trickle_node = len(self.arr) - 1

        while True:
            if trickle_node == 0 or self.arr[self.parent_index(trickle_node)] < self.arr[trickle_node]:
                break
            self.arr[self.parent_index(trickle_node)], self.arr[trickle_node] = self.arr[trickle_node], self.arr[self.parent_index(trickle_node)]
            trickle_node = self.parent_index(trickle_node)

    def delete(self):
        if len(self.arr) < 1:
            return 0
        if len(self.arr) == 1:
            return self.arr.pop()
        temp = self.arr[0]
        self.arr[0] = self.arr.pop()

        trickle_node_index = 0
        while self.has_fewer_child(trickle_node_index):
            fewer_child_index = self.calculate_fewer_child_index(trickle_node_index)

            self.arr[trickle_node_index], self.arr[fewer_child_index] = self.arr[fewer_child_index], self.arr[trickle_node_index]

            trickle_node_index = fewer_child_index

        return temp

n = int(sys.stdin.readline())
heap = Heap()
for _ in range(n):
    k = int(sys.stdin.readline())
    if k == 0:
        print(heap.delete())
    else:
        heap.insert(k)
