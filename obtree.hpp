#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <vector>

template <typename _T> class ObTree {
    using vectorType = std::vector<double>;
    using matrixType = std::vector<std::vector<double>>;

    struct Node {
        _T data;
        std::shared_ptr<Node> left, right;
        Node(_T data) { this->data = data; }
    };

    struct Dynamic{
        double cost;
        std::vector<int> backtrack;
    };
    using nodeType = std::shared_ptr<Node>;

    nodeType root;
    std::size_t size;

    vectorType P;
    vectorType Q;
    std::vector<_T> data;

    // UNUSED
    double buildTree_Inocent(unsigned long long i, unsigned long long j) {
        if (j == i - 1) {
            return Q[i - 1];
        } else {
            auto q = std::numeric_limits<double>::max();

            for (auto r = i; r <= j; r += 1) {
                auto el = buildTree_Inocent(i, r - 1);
                auto er = buildTree_Inocent(r + 1, j);
                auto w = Q[i - 1];
                for (auto l = i; l <= j; l += 1) {
                    w += P[l - 1] + Q[l];
                }
                auto e = el + er + w;

                if (e < q) {
                    q = e;
                }
            }
            return q;
        }
    }

    // UNUSED
    double buildTree_Memoized(unsigned long long i, unsigned long long j,
                              matrixType M) {
        if (M[i][j] == std::numeric_limits<double>::max()) {
            if (j == i - 1) {
                M[i][j] = Q[i - 1];
            } else {
                auto q = std::numeric_limits<double>::max();

                for (auto r = i; r <= j; r += 1) {
                    auto el = buildTree_Memoized(i, r - 1, M);
                    auto er = buildTree_Memoized(r + 1, j, M);
                    auto w = Q[i - 1];
                    for (auto l = i; l <= j; l += 1) {
                        w += P[l - 1] + Q[l];
                    }
                    auto e = el + er + w;

                    if (e < q) {
                        q = e;
                    }
                }
                M[i][j] = q;
            }
        }
        return M[i][j];
    }

    // unused
    double buildTree_BottomUp() {
        matrixType M{Q.size(), vectorType(Q.size(), 0.0)};
        for (int i = 0; i < Q.size(); i += 1) {
            M[i][i] = Q[i];
        }

        for (int i = P.size(); i > 0; i -= 1) {
            for (int j = i; j < Q.size(); j += 1) {
                auto q = std::numeric_limits<double>::max();

                for (auto r = i; r <= j; r += 1) {
                    auto el = M[i - 1][r - 1];
                    auto er = M[r][j];
                    auto w = Q[i - 1];
                    for (auto l = i; l <= j; l += 1) {
                        w += P[l - 1] + Q[l];
                    }
                    auto e = el + er + w;

                    if (e < q) {
                        q = e;
                    }
                }
                M[i - 1][j] = q;
            }
        }
        return M[0][P.size()];
    }

    Dynamic buildTree_BottomUp_Backtracking() {
        matrixType M{Q.size(), vectorType(Q.size(), 0.0)};
        matrixType B{Q.size(), vectorType(Q.size(), 0.0)};
        for (int i = 0; i < Q.size(); i += 1) {
            M[i][i] = Q[i];
        }

        for (int i = P.size(); i > 0; i -= 1) {
            for (int j = i; j < Q.size(); j += 1) {
                auto q = std::numeric_limits<double>::max();

                for (auto r = i; r <= j; r += 1) {
                    auto el = M[i - 1][r - 1];
                    auto er = M[r][j];
                    auto w = Q[i - 1];
                    for (auto l = i; l <= j; l += 1) {
                        w += P[l - 1] + Q[l];
                    }
                    auto e = el + er + w;

                    if (e < q) {
                        q = e;
                        B[i - 1][j] = r;
                    }
                }
                M[i - 1][j] = q;
            }
        }
        auto d = backtracking(B);
        return {M[0][P.size()], d};
    }

    void insert(_T elem) {
        if (size == 0) {
            this->root = std::make_shared<Node>(elem);
        } else {
            nodeType aux = this->root;
            nodeType node;

            while (aux != nullptr) {
                if (aux->data < elem) {
                    if (aux->left == nullptr) {
                        aux->left = std::make_shared<Node>(elem);
                        break;
                    }
                    aux = aux->left;
                } else {
                    if (aux->right == nullptr) {
                        aux->right = std::make_shared<Node>(elem);
                        break;
                    }
                    aux = aux->right;
                }
            }
        }
        size += 1;
    }

    void backtracking_aux(int i, int j, const matrixType &B,
                          std::vector<int> &T) {
        if (B[i][j] == 0) {
            return;
        }
        int k = B[i][j];
        T.push_back(B[i][j]);
        backtracking_aux(i, k - 1, B, T);
        backtracking_aux(k, j, B, T);
    }

    std::vector<int> backtracking(const matrixType &B) {
        std::vector<int> T;
        T.reserve(1 << (sizeof(_T) << 3));
        backtracking_aux(0, P.size(), B, T);
        return T;
    }

  public:
    ObTree(const vectorType &P, const vectorType &Q,
           const std::vector<_T> &data) {
        this->size = 0;
        this->root = nullptr;
        this->P = P;
        this->Q = Q;
        this->data = data;
    }

    double build() { 
        Dynamic d = buildTree_BottomUp_Backtracking(); 
        for(auto & temp: d.backtrack){
            insert(temp);
        }
        return d.cost;
    }

    _T search(_T elem) {
        nodeType aux = this->root;
        while (aux != nullptr) {
            if (aux->data == elem) {
                return aux->data;
            }
            if (aux->data < elem) {
                aux = aux->left;
            } else {
                aux = aux->right;
            }
        }
        // Por la naturaleza del ejercicio no debería llegar acá...
        throw std::out_of_range("No key present in OBTREE");
    }

    int depth(_T elem) {
        int counter = 0;
        nodeType aux = this->root;
        assert(aux != nullptr);
        while (aux != nullptr) {
            if (aux->data == elem) {
                return counter;
            }
            if (aux->data < elem) {
                aux = aux->left;
            } else {
                aux = aux->right;
            }
            counter += 1;
        }
        throw std::out_of_range("No key present in OBTREE");
    }
};