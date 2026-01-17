#include <iostream>
#include <stack>
#include <deque>
#include <vector>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    
    // freopen("sample_intput.txt", "r", stdin);

    std::deque<int> deq;
    std::stack<int> s;

    int N;
    std::cin >> N;

    std::vector<int> A(N);

    for (int i = 0; i < N; i++)
    {
        std::cin >> A[i];
    }

    for(int i = 0; i < N; i++)
    {
        int b; 
        std::cin >> b;
        
        if(A[i] == 0)
        {
            deq.push_back(b); 
        }
        else
        {
            s.push(b);
        }
    }

    int M;
    std::cin >> M;

    for(int i = 0; i < M; i++)
    {
        int c;
        std::cin >> c;

        deq.push_front(c);
        c = deq.back();
        deq.pop_back();

        s.push(c);
        c = s.top();
        s.pop();

        std::cout << c << " ";
    }

}