#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;


int count_char_using_find_first_of(string s, char delim)
{
    int count = 0;
    // note: string::size_type pos = s.find_first_of(delim);
    auto pos = s.find_first_of(delim);
    while ((pos = s.find_first_of(delim, pos)) != string::npos)
    {
        count++;
        pos++;
    }
    return count;
}

// - count char using fast std::count
int count_char_using_count(string s, char delim)
{
    return count(s.begin(), s.end(), delim);
}

string str_generator(int size) {
    string result = "";
    int min = 65; 
    int max = 122;
    for (int i = 0; i < size; i++) {
        int randChar = min + (rand() % (max - min));
        result += (char)randChar;
    }
 
    return result;
}


void single_test()
{
    cout << " << Single Test >> " << endl;

    auto start = steady_clock::now();

    pow(2, 2);

    auto end = steady_clock::now();
    duration<double> diff = end - start;

    cout << fixed << "time for pow(2,2): " << diff.count() << "s" << endl;
    auto _dur = duration_cast<nanoseconds>(end - start).count();
    cout << "time in nano seconds: " << _dur << endl;
}

void single_test_multiple()
{
    cout << " << Single Test with Multiple Functions>> " << endl;

    auto start = steady_clock::now();

    pow(100, 2);

    auto middle = steady_clock::now();

    sqrt(100);

    auto end = steady_clock::now();
    duration<double> diff = end - start;
    duration<double> fun1 = middle - start;
    duration<double> fun2 = end - middle;

    cout << fixed << "100 to the power of 2: " << fun1.count() << "s" << endl;
    cout << fixed << "square root of 100: " << fun2.count() << "s" << endl;
    cout << fixed << "total time: " << diff.count() << " s" << endl;
}

void linear_rampup_test()
{
    cout << " << Linear Ramp-up Test >> " << endl;

    for (auto size = 1; size <= 5; size += 1)
    {
        vector<int> v;


        for (auto i = 1; i <= size * 1000; i += 1)
        {
            v.push_back(i);
        }

        auto start = steady_clock::now();
        sort(v.begin(), v.end(), greater<int>());
        auto end = steady_clock::now();
        duration<double> diff = end - start;

        cout << " - size: " << size * 1000 << ", time: " << diff.count() << "s" << endl;
    }

}

void exponential_rampup_test()
{
    cout << " << Exponential Ramp-up Test >> " << endl;

    for (auto size = 1; size <= pow(2,4); size *= 2)
    {
        vector<int> v;


        for (auto i = 1; i <= size * 1000; i += 1)
        {
            v.push_back(i);
        }

        auto start = steady_clock::now();
        sort(v.begin(), v.end(), greater<int>());
        auto end = steady_clock::now();
        duration<double> diff = end - start;

        cout << " - size: " << size * 1000 << ", time: " << diff.count() << "s" << endl;
    }

}


void repeated_single_test()
{
    cout << " << Repeated Single Test >> " << endl;
    for (auto size = 1; size <= 5; size += 1)
    {
        single_test();
    }
}


void function_comparison()
{
    cout << " << Function Comparison >> " << endl;

    
    for (auto size = 1; size <= 5; size += 1)
    {
        string s = str_generator(size*10000);
        cout << "string: " << size << endl;

        auto start1 = steady_clock::now();
        count_char_using_find_first_of(s, 's');
        auto end1 = steady_clock::now();

        duration<double> diff1 = end1 - start1;

        auto start2 = steady_clock::now();
        count_char_using_count(s, 's');
        auto end2 = steady_clock::now();

        duration<double> diff2 = end2 - start2;


        cout << "using first time: " << diff1.count() << "s" << endl;
        cout << "using count time: " << diff2.count() << "s" << endl;
        cout << "difference: " << (diff2 - diff1).count() << "s" << endl;
    }

   
}





int main()
{
    single_test();

    cout << endl;

    single_test_multiple();

    cout << endl;

    linear_rampup_test();

    cout << endl;

    exponential_rampup_test();

    cout << endl;

    repeated_single_test();

    cout << endl;

    function_comparison();
}