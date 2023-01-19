class Node:
    def __init__(self):
        self.row = 0
        self.col = 0
        self.next = None


class LinkedList:
    def __init__(self):
        self.head = None
        self.tail = None
        self.length = 0
        self.id = 0
        self.isRemoved = False

    def append(self, r, c):
        current_node = Node()
        current_node.row = r
        current_node.col = c
        current_node.next = self.head

        if (not self.tail):
            self.tail = current_node

        self.head = current_node
        self.length += 1

MAXCIVLIZATION = 60001
drows = [-1, 1, 0, 0]
dcols = [0, 0, -1, 1]

def assembleLink(idx1, idx2):
    global civilization

    civilization[idx1].tail.next = civilization[idx2].head

    node = civilization[idx2].head
    while node:
        grid[node.row][node.col] = idx1
        node = node.next

    civilization[idx1].tail = civilization[idx2].tail
    civilization[idx1].length += civilization[idx2].length

    civilization[idx2].isRemoved = True
    civilization[idx2].length = 0


def checkNeighbor(r, c):
    global grid, civilization

    ans = -1

    neighbors = {}
    for i in range(4):
        dr = r + drows[i]
        dc = c + dcols[i]

        if 0 < dr < n_grid and 0 < dc < n_grid:
            if grid[dr][dc] > -1 and not civilization[grid[dr][dc]].isRemoved:
                if neighbors.get(grid[dr][dc]):
                    neighbors[grid[dr][dc]] += 1
                else:
                    neighbors[grid[dr][dc]] = 1

    cnt = 0
    for key, val in neighbors.items():
        if val > cnt:
            ans = key
            cnt = val
        elif val == cnt:
            if civilization[ans].id > civilization[key].id:
                ans = key

    return ans


def init(N):
    global civilization, hashCivil, grid, n_grid, cIdx

    if (N * N + 1 <= MAXCIVLIZATION):
        civilization = [LinkedList() for _ in range(N * N + 1)]
    else:
        civilization = [LinkedList() for _ in range(MAXCIVLIZATION)]

    cIdx = 1
    hashCivil = {}

    grid = [[-1 for _ in range(N + 1)] for _ in range(N + 1)]
    n_grid = N + 1

    return


def newCivilization(r, c, mID):
    global civilization, hashCivil, grid, cIdx

    neighbor = checkNeighbor(r, c)
    if (neighbor > 0):
        civilization[neighbor].append(r, c)

        grid[r][c] = neighbor
    else:
        civilization[cIdx].id = mID
        civilization[cIdx].append(r, c)

        grid[r][c] = cIdx
        hashCivil[mID] = cIdx

        cIdx += 1

    return civilization[grid[r][c]].id


def removeCivilization(mID):
    global civilization, hashCivil

    if hashCivil.get(mID):
        idx = hashCivil[mID]
    else:
        return 0

    if civilization[idx].isRemoved:
        return 0

    ans = civilization[idx].length
    civilization[idx].isRemoved = True

    return ans


def getCivilization(r, c):
    global civilization, grid

    idx = grid[r][c]

    if idx == -1:
        return 0

    if civilization[idx].isRemoved:
        return 0

    return civilization[idx].id


def getCivilizationArea(mID):
    global civilization, hashCivil

    if hashCivil.get(mID):
        idx = hashCivil[mID]
    else:
        return 0

    if civilization[idx].isRemoved:
        return 0

    return civilization[idx].length


def mergeCivilization(mID1, mID2):
    global civilization, hashCivil

    idx1 = hashCivil[mID1]
    idx2 = hashCivil[mID2]

    assembleLink(idx1, idx2)

    return civilization[idx1].length
