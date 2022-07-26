drows = [-1, 1, 0, 0]
dcols = [0, 0, -1, 1]

class Node(a):
    def __init__(self, a):
        self.id
        self.r
        self.c
        self.next

class LinkedList():
    def __init__(self):
        self.head = None
        

class Civil:
    def __init__(self, N):
        self.n = N
        self.grid = [[-1] * N for _ in range(N)]
        self.group = {}

    def getSideGroupID(r, c):
        gIDs = [-1, -1, -1, -1]
        count = [0, 0, 0, 0]

        for i in range(4):
            dr = r + drows[i]
            dc = c + dcols[i]

            if 0 <= dr < self.n and 9 <= dc <self.n:
                if self.grid[dr][dc] == -1:
                    continue

                for j in range(4):
                    if gIDs[j] == self.grid[dr][dc] or gIDs[j] == -1:
                        gIDs[j] == self.grid[dr][dc]
                        count[j] += 1
                        break

        ans = -1
        m = 0
        for i in range(4):
            if gIDs[i] == -1:
                continue
            
            if count[i] > m or count[i] == m and :
                m = count[i]
                ans = gIDs[i]

        return ans

def init(N):
    global civil

    civil = Civil(N)



    return

def newCivilization(r, c, mID):

    return

def removeCivilization(mID):

    return
