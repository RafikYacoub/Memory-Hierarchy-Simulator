using namespace std;
#ifndef _GLIBCXX_NO_ASSERT
#include <cassert>
#endif
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#if __cplusplus >= 201103L
#include <ccomplex>
#include <cfenv>
#include <cinttypes>
#include <cstdalign>
#include <cstdbool>
#include <cstdint>
#include <ctgmath>
#include <cwchar>
#include <cwctype>
#endif

// C++
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>

#if __cplusplus >= 201103L
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <forward_list>
#include <future>
#include <initializer_list>
#include <mutex>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <system_error>
#include <thread>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#endif


int hitC = 0,missC=0;

//struct that represents the cache line
struct cacheLine {
    bool valid=0;
    int tag;
};

//search function which is used to access the specified address in the cashe and displays if it's a hit or a miss then update the cache line accordingly.
void search(int add,int L,int C,vector<cacheLine>& cache) {
    int tag, index, temp, flag, oldvalid, oldtag;

    temp = add / L;   //temp = address / line size = (index and tag) 
    index =temp% C;   //Index = temp (index and tag) % number of lines (C)
    tag = temp / C;   //Index = temp (index and tag) / number of lines (C)

    oldvalid = cache.at(index).valid;   //to store the old value of the valid bit to display it if it's updated after the access.
    oldtag = cache.at(index).tag;       //to store the old value of the tag to display it if it's updated after the access.


    //for an access to be hit -> valid bit =1 and tag of the address = the tag stored in the index 

    if (cache.at(index).valid && tag == cache.at(index).tag) {
        flag = 1;
        hitC++;
    }
    else {
        missC++;
        flag = 0;
        cache.at(index).valid = 1;
        cache.at(index).tag = tag;
    }

    //displaying the outcomes of the access 
    cout << endl;
    cout << "-------------------------------------------------------------------------------" << endl;
    cout << "At the index: " << index <<endl;
    cout << "before accessing address: " << add << " valid bit was: " << oldvalid << " and tag was: " << oldtag << endl;
    cout << "After accessing address: " << add << " valid bit became: " << 1 << " and tag became: " << tag << endl;
    cout << "Hit/Miss: " << (flag ? "Hit" : "Miss") << endl;
    cout << "-------------------------------------------------------------------------------" << endl;
}

int main()
{
    ifstream in("input.txt"); //specify the input test case
    string line;
    string word, word1;
    getline(in, line);
   
    stringstream s(line);           //reading from the text file and parsing the cache size and line size then the number of clock to access cache
    getline(s, line, ',');
    int S, L, clk;
    S = stoi(line);
    getline(s, word1, ' ');
    getline(s, word1, ',');
    L = stoi(word1);
    getline(s, word1, ',');
    clk = stoi(word1.substr(1, word1.size() - 1));

    cout << "Cache size (bytes) = "<< S <<  " ,line size (bytes) = " << L <<  " ,clock cycles to access the cache : " << clk << endl;
    
    int access;
    vector<int>adds;
    while (in >> access)
        adds.push_back(access);

    access = adds.size(); //total number of accesses.

    int C = S / L;        //number of lines in the cache (C) = cache size (S) / line size (L)
    cout << "Number of lines/block in the cache = " << C <<  endl;

    vector<cacheLine> cache(C);

    for (auto i : adds) //iterating over each address in the sequence
    {
        search(i, L, C, cache);
    }

    cout << endl;
    cout << "Total number of Accesses: " << access << endl; //display the total number of accesses
    cout << "The hit rate is: "<<float(hitC)/ access * 100 << "%" << endl;  //display the hit ratio as %
    cout << "The miss rate is: " << float(missC) / access * 100 << "%" << endl; //display the miss ratio as %
    cout << "AMAT (Average Memory Access Time)= " << clk  + (float(missC) / access * 100 ) <<" Cycles"<<endl; //display the AMAT = hit time + (miss ratio *miss penalty)
    return 0;

}
