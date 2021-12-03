#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <limits.h>
using namespace std;

struct point {
    int x, y;
    point(int x, int y) : x(x), y(y) {};
    void print() {cout << x << " " << y << endl;}
};

struct node {
    shared_ptr<point> cur; // current point
    shared_ptr<node> left; // left node
    shared_ptr<node> right; // right node
    node(shared_ptr<point> cur) : cur(cur), left(nullptr), right(nullptr) {};
};

int partition(vector<shared_ptr<point> > &A, int p, int level, int start, int end) {
    std::swap(A[end-1], A[p]);
    int i = start-1, j = end-1, v, t;
    if (level % 2) {v = A[end-1]->y;} 
    else {v = A[end-1]->x;}
    while (true) {
        do {i++; if (level % 2) {t=A[i]->y;} else {t=A[i]->x;}} while (t < v);
        do {j--; if (level % 2) {t=A[j]->y;} else {t=A[j]->x;}} while (j >= i && t > v);
        if (i >= j) break; 
        else {std::swap(A[i], A[j]);}
    }
    std::swap(A[end-1], A[i]);
    return i;
}

// quick select that has the running time of O(n)
shared_ptr<point> quick_select(vector<shared_ptr<point> >&A, int k, int level, int start, int end) {
    random_device device;
    default_random_engine engine(device());
    uniform_int_distribution<int> distribution(start, end-1);
    int p = distribution(engine);
    int i = partition(A, p, level, start, end);
    if (i == k) {
        return A[i];
    } else if (i > k) {
        return quick_select(A, k, level, start, i);
    } else {
        return quick_select(A, k, level, i+1, end);
    }
}

// floor operation
int m_floor(int a, int b) {
    return a / b;
}

// ceiling operation
int m_ceil(int a, int b) {
    return (a + b - 1) / b;
}

struct region {
    int xmax, xmin, ymax, ymin; // four cornor of the region
    // constructor
    region(int xmax, int xmin, int ymax, int ymin) : xmax(xmax), xmin(xmin), ymax(ymax), ymin(ymin) {}
    // copy constructor
    region(const region &r) {xmax = r.xmax; xmin = r.xmin; ymax = r.ymax; ymin = r.ymin;}
};

// detect if a region belongs to another region
bool belong(region r1, region r2) {
    return (r1.xmax < r2.xmax && r1.xmin >= r2.xmin && r1.ymax < r2.ymax && r1.ymin >= r2.ymin);
}

// detect if a region intersects with another region
bool non_intersection(region r1, region r2) {
    return r1.xmin >= r2.xmax || r2.xmin > r2.xmax || r1.ymin >= r2.ymax || r2.ymin > r1.ymax;
}

// deletect if a point inside a region
bool inside(int x, int y, region r2) {
    return (x < r2.xmax && x >= r2.xmin && y < r2.ymax && y >= r2.ymin);
}

struct kdtree {
    shared_ptr<node> root = nullptr;
    vector<shared_ptr<point> > A;
    void build(shared_ptr<node> &cur, int level, int start, int end); // build kd tree
    void rangeSearch(shared_ptr<node> r, region B, region &T, int level); // perform range search
    void print(shared_ptr<node> cur); // print kd tree including nodes
    void print_no_inner_node(shared_ptr<node> cur); // print kd tree only leaves
    void clear(); // delete
};

void kdtree::build(shared_ptr<node> &cur, int level, int start, int end) {
    if (end - start <= 1) {
        if (end - start == 0) return;
        cur = make_shared<node>(A[start]);
    } else {
        cur = make_shared<node>(quick_select(A, start+m_floor((end-start), 2), level, start, end));
        build(cur->left, level+1, start, m_floor((end-start), 2)+start);
        build(cur->right, level+1, end-m_ceil((end-start), 2), end);
    }
}

void kdtree::rangeSearch(shared_ptr<node> r, region B, region &T, int level) {
    if (belong(T, B)) {print_no_inner_node(r); return;} 
    if (non_intersection(T, B)) {return;}
    if (r->left == nullptr && r->right == nullptr) {
        if (inside(r->cur->x, r->cur->y, B)) print(r); 
        return;
    }
    region C = T;
    if (r->left) { 
        if (level%2) {T.ymax = r->cur->y;} else {T.xmax = r->cur->x;}
        rangeSearch(r->left, B, T, level+1); 
    }
    if (r->right) {
        if (level%2) {C.ymin = r->cur->y;} else {C.xmin = r->cur->x;}
         rangeSearch(r->right, B, C, level+1); 
    }
}

void kdtree::print(shared_ptr<node> cur) {
    cur->cur->print();
    if (cur->left) print(cur->left);
    if (cur->right) print(cur->right);
}

void kdtree::print_no_inner_node(shared_ptr<node> cur) {
    if (cur->right == nullptr && cur->left == nullptr) {
        cur->cur->print();
    }
    if (cur->left) print_no_inner_node(cur->left);
    if (cur->right) print_no_inner_node(cur->right);
}

void kdtree::clear() {
    root = nullptr;
    A.clear();
}

int main() {
    kdtree tree;
    string cmd;
    while ( cin >> cmd ) {
        try { 
            if ( cmd == "r" ) {
                tree.clear();
            } else if ( cmd == "ikd" ) {
                tree.clear();
                int n, p1, p2;
                vector<shared_ptr<point> > A;
                cin >> n;
                while (n) {
                    cin >> p1 >> p2;
                    A.push_back(make_shared<point>(p1, p2));
                    n--;
                }
                tree.A = A;
                tree.build(tree.root, 0, 0, A.size());
            } else if ( cmd == "pkd" ) {
                tree.print(tree.root);
            } else if ( cmd == "skd" ) {
                int xmin, xmax, ymin, ymax;
                cin >> xmin >> xmax >> ymin >> ymax;
                region B(xmax, xmin, ymax, ymin);
                region T(INT_MAX, 0, INT_MAX, 0);
                tree.rangeSearch(tree.root, B, T, 0);
            } else if ( cmd == "x" ) {
                tree.clear();
                exit(0);
            } else {
                throw std::invalid_argument(cmd + " is Invalid syntasx");
            }
        } catch (std::invalid_argument & e) {
            std::cerr << e.what() << std::endl;
            exit(1);
        }  // catch
    }
}