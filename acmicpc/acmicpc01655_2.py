import sys

def get_parent(idx):
    return (idx - 1) // 2

def get_left(idx):
    return 2 * idx + 1

def get_right(idx):
    return 2 * idx + 2


class PriorityQ:
    def __init__(self):
        self.arr = []

    def push(self, m):
        self.arr.append(m)
        
        idx = len(self.arr) - 1
        while idx > 0:
            parent = get_parent(idx)
            if self.arr[parent][0] > self.arr[idx][0]:
                self.arr[parent], self.arr[idx] = self.arr[idx], self.arr[parent]
                idx = parent
            else:
                break
    
    def pop(self):
        idx = len(self.arr) - 1
        
        self.arr[0], self.arr[idx] = self.arr[idx], self.arr[0]
        result = self.arr.pop()
        idx = 0

        while idx < len(self.arr):
            left = get_left(idx)
            right = get_right(idx)
            if left < len(self.arr) and right < len(self.arr): 
                if self.arr[left][0] < self.arr[idx][0]:
                    self.arr[left], self.arr[idx] = self.arr[idx], self.arr[left]
                    idx = left
                elif self.arr[right][0] < self.arr[idx][0]:
                    self.arr[right], self.arr[idx] = self.arr[idx], self.arr[right]
                    idx = right

            elif left < len(self.arr) and right >= len(self.arr):
                if self.arr[left][0] < self.arr[idx][0]:
                    self.arr[left], self.arr[idx] = self.arr[idx], self.arr[left]
                    idx = left

            elif left >= len(self.arr) and right < len(self.arr):
                if self.arr[right][0] < self.arr[idx][0]:
                    self.arr[right], self.arr[idx] = self.arr[idx], self.arr[right]
                    idx = right

            else:
                break
        return result


# sys.stdin = open('acmicpc/sample.txt', 'r')
k = int(sys.stdin.readline())

leftq = PriorityQ()
rightq = PriorityQ()

for i in range(k):
    m = int(sys.stdin.readline())
    if len(leftq.arr) == len(rightq.arr):
        leftq.push([-m, m])
    else:
        rightq.push([m, m])

    if leftq.arr and rightq.arr:
        if leftq.arr[0][1] > rightq.arr[0][1]:
            a = leftq.pop()[1]
            b = rightq.pop()[1]
            leftq.push([-b, b])
            rightq.push([a, a])
    
    print(leftq.arr[0][1])
