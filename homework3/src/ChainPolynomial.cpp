#include <iostream>
#include <cmath>

// Term structure
struct Term {
    float coef;
    int   exp;
};

template <class T> class Chain;
template <class T> class ChainIterator;
template <class T> class AvailableList;

// ChainNode
template <class T>
class ChainNode {
    // Correct friend declaration without <T>
    friend class Chain<T>;
    friend class ChainIterator<T>;
    friend class AvailableList<T>;

private:
    T element;
    ChainNode<T>* next = nullptr;

public:
    ChainNode() = default;
    ChainNode(const T& elem) : element(elem) {}
    ChainNode(const T& elem, ChainNode<T>* nextNode) : element(elem), next(nextNode) {}

    void setElement(const T& elem) { element = elem; }
    void setNext(ChainNode<T>* nextNode) { next = nextNode; }
};

// Chain
template <class T>
class Chain {
    friend class ChainIterator<T>;
    friend class AvailableList<T>;

private:
    ChainNode<T>* head = nullptr;

public:
    Chain() = default;

    ~Chain() {
        ChainNode<T>* cur = head;
        while (cur) {
            ChainNode<T>* temp = cur;
            cur = cur->next;
            delete temp;
        }
    }

    ChainNode<T>* release() {
        ChainNode<T>* old = head;
        head = nullptr;
        return old;
    }

    ChainIterator<T> begin() const { return ChainIterator<T>(head); }
    ChainIterator<T> end()   const { return ChainIterator<T>(nullptr); }

    void insert(int idx, const T& element) {
        ChainNode<T>* newNode = AvailableList<T>::getNode();
        if (!newNode) {
            newNode = new ChainNode<T>(element);
        }
        else {
            newNode->element = element;
            newNode->next = nullptr;
        }

        if (idx == 0 || head == nullptr) {
            newNode->next = head;
            head = newNode;
        }
        else {
            ChainNode<T>* prev = head;
            for (int i = 0; i < idx - 1 && prev->next; ++i) {
                prev = prev->next;
            }
            newNode->next = prev->next;
            prev->next = newNode;
        }
    }
};

// ChainIterator
template <class T>
class ChainIterator {
private:
    ChainNode<T>* current;

public:
    ChainIterator(ChainNode<T>* node = nullptr) : current(node) {}

    T& operator*() const { return current->element; }
    T* operator->() const { return &(current->element); }

    ChainIterator& operator++() { current = current->next; return *this; }
    ChainIterator operator++(int) { ChainIterator tmp = *this; ++(*this); return tmp; }

    bool operator==(const ChainIterator& other) const { return current == other.current; }
    bool operator!=(const ChainIterator& other) const { return !(*this == other); }
};

// AvailableList
template <class T>
class AvailableList {
private:
    static Chain<T> availableItems;

public:
    static ChainNode<T>* getNode() {
        if (!availableItems.head) return nullptr;
        ChainNode<T>* node = availableItems.head;
        availableItems.head = availableItems.head->next;
        node->next = nullptr;
        return node;
    }

    static void getBack(ChainNode<T>* node) {
        if (!node) return;
        ChainNode<T>* last = node;
        while (last->next) last = last->next;
        last->next = availableItems.head;
        availableItems.head = node;
    }
};

// Static member definition
template <class T>
Chain<T> AvailableList<T>::availableItems;

// Polynomial
class Polynomial {
    friend std::istream& operator>>(std::istream& is, Polynomial& p);
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& p);

private:
    Chain<Term> terms;

public:
    Polynomial() = default;

    Polynomial(const Polynomial& other) {
        for (ChainIterator<Term> it = other.terms.begin(); it != other.terms.end(); ++it) {
            terms.insert(0, *it);  // 簡單插入頭部，後續 newTerm 會排序
        }
    }

    Polynomial& operator=(const Polynomial& other) {
        if (this != &other) {
            AvailableList<Term>::getBack(terms.release());
            for (ChainIterator<Term> it = other.terms.begin(); it != other.terms.end(); ++it) {
                newTerm(it->coef, it->exp);
            }
        }
        return *this;
    }

    ~Polynomial() {
        AvailableList<Term>::getBack(terms.release());
    }

    void newTerm(double c, int e) {
        float coef = static_cast<float>(c);
        if (std::abs(coef) < 1e-6f) return;  // 忽略接近 0 的係數

        int index = 0;
        for (ChainIterator<Term> it = terms.begin(); it != terms.end(); ++it, ++index) {
            if (it->exp < e) break;
            if (it->exp == e) {
                it->coef += coef;
                if (std::abs(it->coef) < 1e-6f) {
                    // 這裡簡化不實作刪除（需要 erase 函式），實際可忽略小係數
                }
                return;
            }
        }
        terms.insert(index, Term{ coef, e });
    }

    Polynomial operator+(const Polynomial& other) const {
        Polynomial res;
        ChainIterator<Term> it1 = terms.begin();
        ChainIterator<Term> it2 = other.terms.begin();

        while (it1 != terms.end() && it2 != other.terms.end()) {
            if (it1->exp > it2->exp) {
                res.newTerm(it1->coef, it1->exp);
                ++it1;
            }
            else if (it1->exp < it2->exp) {
                res.newTerm(it2->coef, it2->exp);
                ++it2;
            }
            else {
                res.newTerm(it1->coef + it2->coef, it1->exp);
                ++it1; ++it2;
            }
        }
        while (it1 != terms.end()) { res.newTerm(it1->coef, it1->exp); ++it1; }
        while (it2 != other.terms.end()) { res.newTerm(it2->coef, it2->exp); ++it2; }
        return res;
    }

    Polynomial operator-(const Polynomial& other) const {
        Polynomial neg;
        for (ChainIterator<Term> it = other.terms.begin(); it != other.terms.end(); ++it) {
            neg.newTerm(-it->coef, it->exp);
        }
        return *this + neg;
    }

    Polynomial operator*(const Polynomial& other) const {
        Polynomial res;
        for (ChainIterator<Term> it1 = terms.begin(); it1 != terms.end(); ++it1) {
            for (ChainIterator<Term> it2 = other.terms.begin(); it2 != other.terms.end(); ++it2) {
                res.newTerm(it1->coef * it2->coef, it1->exp + it2->exp);
            }
        }
        return res;
    }
};

// Input / Output
std::istream& operator>>(std::istream& is, Polynomial& p) {
    int n;
    double c; int e;
    is >> n;
    for (int i = 0; i < n; ++i) {
        is >> c >> e;
        p.newTerm(c, e);
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const Polynomial& p) {
    bool first = true;
    for (ChainIterator<Term> it = p.terms.begin(); it != p.terms.end(); ++it) {
        if (!first) os << " + ";
        first = false;
        os << it->coef << "x^" << it->exp;
    }
    if (first) os << "0";
    return os;
}

int main() {
    Polynomial a, b;
    std::cout << "輸入第一個多項式 (項數 係數1 指數1 ...): ";
    std::cin >> a;
    std::cout << "輸入第二個多項式 (項數 係數1 指數1 ...): ";
    std::cin >> b;

    std::cout << "\na = " << a << "\n";
    std::cout << "b = " << b << "\n";
    std::cout << "a + b = " << (a + b) << "\n";
    std::cout << "a - b = " << (a - b) << "\n";
    std::cout << "a * b = " << (a * b) << "\n";

    return 0;
}
