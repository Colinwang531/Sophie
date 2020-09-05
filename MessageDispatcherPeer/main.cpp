#include <iostream>
#include <vector>

int main()
{
    std::vector<int> x;
    x.push_back(1);
    x.push_back(2);
    x.erase(x.end() - 1);
    return 0;
}