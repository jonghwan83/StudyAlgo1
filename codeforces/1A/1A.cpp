#include <iostream>
#include <cmath>

int main()
{
    // freopen("sample_input.txt", "r", stdin);

    long long n, m, a;
    std::cin >> n >> m >> a;

    long long na, ma;

    na = n / a;
    ma = m / a;

    if(n % a != 0) na++;
    if(m % a != 0) ma++;

    std::cout << na * ma << "\n";
}