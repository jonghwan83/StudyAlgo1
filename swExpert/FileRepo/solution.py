from heapq import heappush, heappop

MAXFILE = 12001

SUBN = 5001

class Range:
    def __init__(self, left, right):
        self.left = left
        self.right = right


class Partition:
    def __init__(self):
        self.id = set()


class File:
    def __init__(self):
        self.id = 0
        self.size = 0
        self.piece = []

    def push(self, rng : Range):
        if len(self.piece) > 0:
            if self.piece[-1].right == rng.left:
                self.piece[-1].right = rng.right
                return
        
        self.piece.append(rng)
            

def overlap(a : Range, b : Range):
    ans = Range(0, 0)
    ans.left = max(a.left, b.left)
    ans.right = min(a.right, b.right)

    return ans


files = [File() for _ in range(MAXFILE)]

empties = []

hashID = {}

ptts = [Partition() for _ in range(SUBN + 1)]


def push_ptr(rng: Range, mId):
    st = rng.left / SUBN
    ed = rng.right / SUBN



def init(N):
    global space, fIdx

    fIdx = 1

    space = N

    empties.clear()

    heappush(empties, [1, N + 1])

    hashID.clear()

    for i in range(SUBN + 1):
        ptts[i] = Partition()

    return


def add(mId, mSize):
    global space, fIdx

    if (space < mSize):
        return -1

    if hashID.get(mId):
        idx = hashID[mId]
    else:
        idx = fIdx
        fIdx += 1
        hashID[mId] = idx
        

    files[idx] = File()
    files[idx].id = mId
    files[idx].size = mSize

    space -= mSize
    
    while (mSize > 0 and len(empties) > 0):
        empty = heappop(empties)
        empty = Range(empty[0], empty[1])

        rng = Range(empty.left, empty.left + mSize)

        ovl = overlap(empty, rng)

        mSize -= ovl.right - ovl.left

        files[idx].push(ovl)

        if (ovl.right < empty.right and mSize == 0):
            heappush(empties, [ovl.right, empty.right])

        st = int(ovl.left / SUBN)
        ed = int(ovl.right / SUBN)

        for i in range(st, ed + 1):
            ptts[i].id.add(idx)

    return files[idx].piece[0].left


def remove(mId):
    global space

    idx = hashID.get(mId)

    ans = len(files[idx].piece)

    space += files[idx].size

    for rng in files[idx].piece:
        heappush(empties, [rng.left, rng.right])
    
    files[idx] = File()

    return ans


def count(mStart, mEnd):
    global fIdx

    ans = 0

    blk = Range(mStart, mEnd + 1)

    visited = {}

    st = int(mStart / SUBN)
    ed = int(mEnd / SUBN)

    for i in range(st, ed + 1, 1):
        
        for idx in ptts[i].id:

            if visited.get(idx):
                continue

            if files[idx].size == 0:
                continue
            
            for rng in files[idx].piece:

                ovl = overlap(rng, blk)

                if ovl.right > ovl.left:
                    ans += 1
                    visited[idx] = True
                    break
            
    return ans
    
