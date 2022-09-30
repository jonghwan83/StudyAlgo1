# evaluate worst case
# worst case: R - 700, C - 10000, M: 0 ~ 30000
# grid size = 700 * 10000
# max query 31 in 1 tc
# worst total loop in 1 tc: 700 * 10000 * 31 = 217,000,000 (about 2 seconds)

from collections import deque

def binarySearch(element, arr):     # find right-side index
    left = 0
    right = len(arr) - 1

    while left < right:
        mid = (left + right) // 2
        if arr[mid] < element:
            left = mid + 1
        else:
            right = mid
        
    return right

def bfs(sPoint, ePoint):
    global structureR, structureC, nRow, nCol, visited

    visited = [[0] * nCol for _ in range(nRow)]

    queue = deque()
    queue.append(sPoint)
    visited[sPoint[0]][sPoint[1]] = 1

    while queue:
        r, c = queue.popleft()

        # binary search by direction
        # row direction
        if structureR[r]:
            col_loc = binarySearch(c, structureR[r])    # target: column
            col_idx = structureR[r][col_loc]

            # col_idx on left
            if col_idx < c:
                if ((r == ePoint[0]) and (col_idx <= ePoint[1] <= c)) or ((r == ePoint[0]) and (c <= ePoint[1])):
                    visited[ePoint[0]][ePoint[1]] = visited[r][c]
                    return

                if not visited[r][col_idx + 1]:
                    visited[r][col_idx + 1] = visited[r][c] + 1
                    queue.append([r, col_idx + 1])
            else:   # col_idx on right
                if (r == ePoint[0]) and (c <= ePoint[1] <= col_idx):
                    visited[ePoint[0]][ePoint[1]] = visited[r][c]
                    return

                if not visited[r][col_idx - 1]:
                    visited[r][col_idx - 1] = visited[r][c] + 1
                    queue.append([r, col_idx - 1])

                if len(structureR[r]) == 1 or col_loc == 0:
                    if (r == ePoint[0]) and (0 <= ePoint[1] < c):
                        visited[ePoint[0]][ePoint[1]] = visited[r][c]
                        return

                # left direction
                if col_loc > 0:
                    col_idx2 = structureR[r][col_loc - 1]
                    if (r == ePoint[0]) and (col_idx2 <= ePoint[1] <= c):
                        visited[ePoint[0]][ePoint[1]] = visited[r][c]
                        return

                    if not visited[r][col_idx2 + 1]:
                        visited[r][col_idx2 + 1] = visited[r][c] + 1
                        queue.append([r, col_idx2 + 1])

        else: # no structure in col
            if ePoint[0] == r:
                visited[ePoint[0]][ePoint[1]] = visited[r][c]
                return

        # col direction
        if structureC[c]:
            row_loc = binarySearch(r, structureC[c])
            row_idx = structureC[c][row_loc]

            # row_idx on upper
            if row_idx < r:
                if ((row_idx <= ePoint[0] <= r) and (c == ePoint[1])) or ((r <= ePoint[0]) and (c == ePoint[1])):
                    visited[ePoint[0]][ePoint[1]] = visited[r][c]
                    return

                if not visited[row_idx + 1][c]:
                    visited[row_idx + 1][c] = visited[r][c] + 1
                    queue.append([row_idx + 1, c])
            else:   # row_idx on down
                if (r <= ePoint[0] <= row_idx) and (c == ePoint[1]):
                    visited[ePoint[0]][ePoint[1]] = visited[r][c]
                    return
                
                if len(structureC[c]) == 1 or row_loc == 0:
                    if (0 <= ePoint[0] <= r) and (c == ePoint[1]):
                        visited[ePoint[0]][ePoint[1]] = visited[r][c]
                        return

                if not visited[row_idx - 1][c]:
                    visited[row_idx - 1][c] = visited[r][c] + 1
                    queue.append([row_idx - 1, c])

                # upper direction
                if row_loc > 0:
                    row_idx2 = structureC[c][row_loc - 1]
                    if (row_idx2 <= ePoint[0] <= r) and (c == ePoint[1]):
                        visited[ePoint[0]][ePoint[1]] = visited[r][c]
                        return

                    if not visited[row_idx2 + 1][c]:
                        visited[row_idx2 + 1][c] = visited[r][c] + 1
                        queue.append([row_idx2 + 1, c])
        
        else: # no structure in row
            if ePoint[1] == c:
                visited[ePoint[0]][ePoint[1]] = visited[r][c]
                return

    return


def init(R, C, M, mStructureR, mStructureC):
    global nRow, nCol, structureR, structureC

    nRow = R
    nCol = C
 
    structureR = [[] for _ in range(R)]
    structureC = [[] for _ in range(C)]

    for r, c in zip(mStructureR, mStructureC):
        structureR[r].append(c)
        structureC[c].append(r)

        structureR[r].sort()
        structureC[c].sort()

    return

def minDamage(mStartR, mStratC, mEndR, mEndC):
    global visited

    if [mStartR, mStratC] == [mEndR, mEndC]:
        return 0
    bfs([mStartR, mStratC], [mEndR, mEndC])

    return visited[mEndR][mEndC] - 1
