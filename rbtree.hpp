#include <algorithm>
#include <set>
#include <utility>
#include <vector>

template <typename _T> class RbTree {
    using vectorType = std::vector<_T>;
    using setType = std::set<_T>;

    vectorType data;
    setType tree;
    
  public:
    RbTree(const vectorType &data) {
        this->data = data;
        tree.insert(this->data.begin(), this->data.end());
    }
    unsigned int depth(_T elem){
        auto iter = this->tree.find(elem);
        return std::distance(this->tree.begin(), iter);
    }
    _T search(_T elem){
        return *this->tree.find(elem);
    }

    
};
