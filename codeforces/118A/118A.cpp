#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int main()
{
    // freopen("sample_input.txt", "r", stdin);

    std::string a;
    std::cin >> a;

    int offset = 32;

    std::vector<char> vowels = { 'A', 'E', 'I', 'O', 'U', 'Y'
                                , 'a', 'e', 'i', 'o', 'u', 'y' };

    std::string output;

    for(int i = 0; i < a.size(); i++)
    {
        if(vowels.end() == std::find(vowels.begin(), vowels.end(), a[i]))
        {
            output += '.';
            if(a[i] < 'a')
                output += a[i] + offset;
            else
                output += a[i];
        }
    }

    std::cout << output;
}