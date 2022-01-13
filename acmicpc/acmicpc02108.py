import sys

T = int(sys.stdin.readline())
lst = []
for _ in range(T):
    lst.append(int(sys.stdin.readline()))
lst.sort()

def average(lst):
    return round(sum(lst) / len(lst))


def median(lst):
    return lst[len(lst) // 2]


def freq(lst):
    from collections import Counter
    cnt = Counter(lst).most_common(2)

    if len(lst) > 1:
        if cnt[0][1] == cnt[1][1]:
            return cnt[1][0]
        else:
            return cnt[0][0]
    else:
        return cnt[0][0]

def rng(lst):
    return max(lst) - min(lst)


print(average(lst))
print(median(lst))
print(freq(lst))
print(rng(lst))
