
//part A.1
#include <vector>

template<typename Iterator, typename Predicate>
int countPairs(Iterator begin, Iterator end, Predicate pred) {
    int counter = 0;

    Iterator i = begin;

    for (i = begin; i != end; ++i)
    {
        Iterator j = i;
        for (++j; j < end; ++j)
        {
            if (i == j) continue;
            if (pred(*i, *j))
                counter++;
        }
    }
    return counter;
}

//Part A.2
class isFirstBigger
{
public:
    bool operator()(int a, int b)
    {
        return a > b;
    }
};

bool isSorted(std::vector<int> v)
{
    isFirstBigger pred;
    return countPairs(v.begin(), v.end(), pred) == 0;
}

