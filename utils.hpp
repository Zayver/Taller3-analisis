#include <algorithm>
#include <fstream>
#include <iterator>
#include <limits>
#include <utility>
#include <vector>
#include <numeric>

namespace utils {

template <typename _T> struct Frequency {
    _T minElem;
    _T maxElem;
    double minFrequency;
    double maxFrequency;
};

template <typename _T> std::vector<_T> readFile(const std::string &fname) {
    std::ifstream fstr(fname, std::ios::binary | std::ios::ate);
    if (!fstr.good())
        throw std::runtime_error("Error opening file: '" + fname + "'");
    unsigned long long size = fstr.tellg();
    fstr.seekg(0, std::ios::beg);
    std::vector<_T> data(size / sizeof(_T));

    char *buffer = reinterpret_cast<char *>(data.data());
    fstr.read(buffer, data.size() * sizeof(_T));
    return data;
}
template <typename _T>
std::vector<double> buildHistogram(const std::vector<_T> &data) {
    std::vector<double> P(1 << (sizeof(_T) << 3), 0);
    for (unsigned long long i = 0; i < data.size(); ++i) {
        P[data[i]] += double(1) / double(data.size());
    }
    return P;
}
template <typename _T>
Frequency<_T> findFrequencies(const std::vector<_T> &data,
                              const std::vector<double> &P) {

    //NECESARIO IGNORAR 0s
    auto min = std::numeric_limits<double>::max();
    for (int i = 0; i<P.size(); i++){
        if(P[i] == 0)
            continue;
        if(P[i] < min){
            min = P[i];
        }
    }
    auto max = std::max_element(P.begin(), P.end());
    _T minElem, maxElem;
    for(int i = 0; i< data.size(); i++){
        if(P[data[i]] == min){
            minElem = (_T)data[i];
            break;
        }
    }
    for(int i = 0; i< data.size(); i++){
        if(P[data[i]] == *max){
            maxElem = (_T)data[i];
            break;
        }
    }

    return {
        minElem,
        maxElem,
        min,
        *max,
    };
}

template <typename _T>
std::vector<_T> generateTypes(){
    std::vector<_T> vec(1 << (sizeof(_T) << 3));
    for(int i = 0; i < vec.size(); i++ ){
        vec[i] = static_cast<_T>(i);
    }
    return vec;
}
} // namespace utils