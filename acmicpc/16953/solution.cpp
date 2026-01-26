#include <iostream>

int main()
{
    // freopen("sample_input.txt", "r", stdin);

    int A, B;

    std::cin >> A >> B;

    int cnt = 1;

    if(A == B)
    {
        std::cout << 0 << "\n";
        return 0;
    }

    while(true)
    {
        if(B % 10 == 1)
        {
            B /= 10;
        }
        else if(B % 2 == 1)
        {
            break;
        }
        else
        {
            B /= 2;
        }

        cnt++;

        if(B <= A)
        {
            break;
        }
    }

    if(B == A)
    {
        std::cout << cnt << "\n";
    }
    else
    {
        std::cout << -1 << "\n";
    }
}