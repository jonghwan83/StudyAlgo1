'''
LCS(Longest Common Subsequence, 최장 공통 부분 수열)문제는 두 수열이 주어졌을 때, 
모두의 부분 수열이 되는 수열 중 가장 긴 것을 찾는 문제이다.

예를 들어, ACAYKP와 CAPCAK의 LCS는 ACAK가 된다.
'''

import sys

a = sys.stdin.readline().strip()
b = sys.stdin.readline().strip()


dp = [[0 for _ in range(len(a)+1)] for _ in range(len(b)+1)]
for i in range(1, len(b)+1):
    for j in range(1, len(a)+1):
        dp[i][j] = max(dp[i-1][j-1] + (b[i-1] == a[j-1]), dp[i][j-1], dp[i-1][j])

print(dp[len(b)][len(a)])