# import time
# import random

# start = time.time()

# import sys

# def get_parent(idx):
#     return (idx - 1) // 2

# def get_left(idx):
#     return 2 * idx + 1

# def get_right(idx):
#     return 2 * idx + 2

# class Maxheap:
#     def __init__(self):
#         self.arr = []

#     def push(self, m):
#         self.arr.append(m)
        
#         idx = len(self.arr) - 1
#         while idx > 0:
#             parent = get_parent(idx)
#             if self.arr[parent] < self.arr[idx]:
#                 self.arr[parent], self.arr[idx] = self.arr[idx], self.arr[parent]
#                 idx = parent
#             else:
#                 break
    
#     def pop(self):
#         idx = len(self.arr) - 1
#         if idx < 0:
#             return -1000000
        
#         self.arr[0], self.arr[idx] = self.arr[idx], self.arr[0]
#         result = self.arr.pop()
#         idx = 0

#         while idx < len(self.arr):
#             left = get_left(idx)
#             right = get_right(idx)
#             if left < len(self.arr) and right < len(self.arr): 
#                 if self.arr[left] > self.arr[idx]:
#                     self.arr[left], self.arr[idx] = self.arr[idx], self.arr[left]
#                     idx = left
#                 elif self.arr[right] > self.arr[idx]:
#                     self.arr[right], self.arr[idx] = self.arr[idx], self.arr[right]
#                     idx = right
#                 else:
#                     return result
#             elif left < len(self.arr) and right >= len(self.arr):
#                 if self.arr[left] > self.arr[idx]:
#                     self.arr[left], self.arr[idx] = self.arr[idx], self.arr[left]
#                     idx = left
#                 else:
#                     return result
#             else:
#                 return result
        
#         return result
    

# class MinHeap:
#     def __init__(self):
#         self.arr = []

#     def push(self, m):
#         self.arr.append(m)
        
#         idx = len(self.arr) - 1
#         while idx > 0:
#             parent = get_parent(idx)
#             if self.arr[parent] > self.arr[idx]:
#                 self.arr[parent], self.arr[idx] = self.arr[idx], self.arr[parent]
#                 idx = parent
#             else:
#                 break
    
#     def pop(self):
#         idx = len(self.arr) - 1
#         if idx < 0:
#             return -1000000
        
#         self.arr[0], self.arr[idx] = self.arr[idx], self.arr[0]
#         result = self.arr.pop()
#         idx = 0

#         while idx < len(self.arr):
#             left = get_left(idx)
#             right = get_right(idx)
#             if left < len(self.arr) and right < len(self.arr): 
#                 if self.arr[left] < self.arr[idx]:
#                     self.arr[left], self.arr[idx] = self.arr[idx], self.arr[left]
#                     idx = left
#                 elif self.arr[right] < self.arr[idx]:
#                     self.arr[right], self.arr[idx] = self.arr[idx], self.arr[right]
#                     idx = right
#                 else:
#                     return result
#             elif left < len(self.arr) and right >= len(self.arr):
#                 if self.arr[left] < self.arr[idx]:
#                     self.arr[left], self.arr[idx] = self.arr[idx], self.arr[left]
#                     idx = left
#                 else:
#                     return result
#             else:
#                 return result
        
#         return result


# # sys.stdin = open('acmicpc/sample.txt', 'r')
# k = int(sys.stdin.readline())

# maxheap = Maxheap()
# minheap = MinHeap()


# for _ in range(k):
#     m = int(sys.stdin.readline())
#     if (len(maxheap.arr) == len(minheap.arr)):
#         maxheap.push(m)
#     else:
#         minheap.push(m)

#     if len(minheap.arr) > 0:
#         if maxheap.arr[0] > minheap.arr[0]:
#             a = minheap.pop()
#             b = maxheap.pop()
#             maxheap.push(a)
#             minheap.push(b)

#     print(maxheap.arr[0])


# end = time.time()
# print('elapsed: ', end - start)

import time

start = time.time()

import sys
import heapq

# sys.stdin = open('acmicpc/sample.txt', 'r')
k = int(sys.stdin.readline())

maxheap = []
minheap = []

for _ in range(k):
    m = int(sys.stdin.readline())
    if len(maxheap) == len(minheap):
        heapq.heappush(maxheap, (-m, m))
    else:
        heapq.heappush(minheap, (m, m))

    if minheap and maxheap[0][1] > minheap[0][1]:
        a = heapq.heappop(minheap)[0]
        b = heapq.heappop(maxheap)[1]
        heapq.heappush(maxheap, (-a, a))
        heapq.heappush(minheap, (b, b))

    print(maxheap[0][1])

end = time.time()
print('elapsed: ', end - start)