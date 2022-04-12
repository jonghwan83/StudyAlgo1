import sys

class HeapQ():
    def __init__(self):
        self.length = 0
        self.H = []

    def getParent(self, idx):
        return (idx-1) // 2

    def getleftChild(self, idx):
        return int(2 * idx + 1)

    def getrightChild(self, idx):
        return int(2 * idx + 2)

    def push(self, a):
        self.H.append(a)
        self.length += 1

        idx = self.length - 1
        
        while (idx > 0):
            parent = self.getParent(idx)
            if self.H[idx] > self.H[parent]:
                self.H[idx], self.H[parent] = self.H[parent], self.H[idx]
                idx = parent
            else:
                break;

    def pop(self):
        if self.length < 1:
            return 0
        
        answer = self.H[0]
        self.H[0] = self.H[self.length-1]
        if (self.length > 1):
            self.H.pop()
        self.length -= 1

        idx = 0
        while True:
            leftChild = self.getleftChild(idx)
            rightChild = self.getrightChild(idx)
            
            if (leftChild >= self.length and rightChild >= self.length):
                break
            elif (leftChild >= self.length and rightChild < self.length):
                if (self.H[idx] < self.H[rightChild]):
                    self.H[idx], self.H[rightChild] = self.H[rightChild], self.H[idx]
                    idx = rightChild
                else:
                    break
            elif (leftChild < self.length and rightChild >= self.length):
                if (self.H[idx] < self.H[leftChild]):
                    self.H[idx], self.H[leftChild] = self.H[leftChild], self.H[idx]
                    idx = leftChild
                else:
                    break
            else:
                if (self.H[leftChild] > self.H[rightChild]):
                    if (self.H[idx] < self.H[leftChild]):
                        self.H[idx], self.H[leftChild] = self.H[leftChild], self.H[idx]
                        idx = leftChild
                    else:
                        break
                else:
                    if (self.H[idx] < self.H[rightChild]):
                        self.H[idx], self.H[rightChild] = self.H[rightChild], self.H[idx]
                        idx = rightChild
                    else:
                        break
        
        return answer

hq = HeapQ()

n = int(sys.stdin.readline())
for _ in range(n):
    k = int(sys.stdin.readline())
    if k == 0:
        print(hq.pop())
    else:
        hq.push(k)