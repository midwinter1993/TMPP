#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <functional>

// -------------------------------------

// template<bool Cond>
// struct IfCond;

// template<>
// struct IfCond<true> {
    // template<typename T, typename F>
    // struct ThenSelect {
        // typedef T value;
    // };
// };

// template<>
// struct IfCond<false> {
    // template<typename T, typename F>
    // struct ThenSelect {
        // typedef F value;
    // };
// };

// #define IF_COND(Cond, T, F) typename IfCond<Cond>::template ThenSelect<T, F>::value

template<bool Cond, typename T, typename F>
struct IfCond;

template<typename T, typename F>
struct IfCond<true, T, F> {
    typedef T value;
};

template<typename T, typename F>
struct IfCond<false, T, F> {
    typedef F value;
};

// -------------------------------------

struct Nil;

template<int x, typename xs>
struct Cons;

#define LIST_1(n1) Cons<n1, Nil>
#define LIST_2(n1, n2) Cons<n1, LIST_1(n2)>
#define LIST_3(n1, n2, n3) Cons<n1, LIST_2(n2, n3)>
#define LIST_4(n1, n2, n3, n4) Cons<n1, LIST_3(n2, n3, n4)>

// -------------------------------------

template<int... Args>
struct List;

template<>
struct List<> {
    typedef Nil value;
};

template<int x, int... Args>
struct List<x, Args...> {
    typedef Cons<x, typename List<Args...>::value> value;
};

// -------------------------------------

template<typename Lst>
struct Head;

template<int x, typename xs>
struct Head<Cons<x, xs>> {
    enum { value = x };
};

#define HEAD(Lst) Head<Lst>::value

// -------------------------------------

template<typename Lst>
struct Tail;

template<int x, typename xs>
struct Tail<Cons<x, xs>> {
    typedef xs value;
};

#define TAIL(Lst) Tail<Lst>::value

// -------------------------------------

template<typename Lst_1, typename Lst_2>
struct Append;

template<typename Lst_2>
struct Append<Nil, Lst_2> {
    typedef Lst_2 value;
};

template<int x, typename xs, typename Lst_2>
struct Append<Cons<x, xs>, Lst_2> {
    typedef typename Append<xs, Lst_2>::value new_lst;
    typedef Cons<x, new_lst> value;
};

// -------------------------------------

template<int n, typename Lst>
struct Take;

template<int n>
struct Take<n, Nil> {
    enum { value = -1 };
};

template<int n, int x, typename xs>
struct Take<n, Cons<x, xs>> {
    enum { value = n == 0 ? x : Take<n-1, xs>::value };
};

// #define TAKE(n, Lst) Take<n, Lst>::value

// -------------------------------------

template<typename Lst>
struct Len;

template<>
struct Len<Nil> {
    enum { value = 0 };
};

template<int x, typename xs>
struct Len<Cons<x, xs>> {
    enum { value = 1 + Len<xs>::value };
};

// #define LEN(Lst) Len<Lst>::value

// -------------------------------------

template<typename Lst>
struct Min;

template<int x>
struct Min<Cons<x, Nil>> {
    enum { value = x };
};

template<int x, typename xs>
struct Min<Cons<x, xs>> {
    enum { least = Min<xs>::value };
    enum { value = x < least ? x : least };
};

// #define MIN(Lst) Min<Lst>::value

// -------------------------------------

template<int v, typename Lst>
struct LessEq;

template<int v>
struct LessEq<v, Nil> {
    typedef Nil value;
};

template<int v, int x, typename xs>
struct LessEq<v, Cons<x, xs>> {
    typedef typename IfCond<x <= v,
                            Cons<x, typename LessEq<v, xs>::value>,
                            typename LessEq<v, xs>::value
                     >::value value;
};

// -------------------------------------

template<int v, typename Lst>
struct Greater;

template<int v>
struct Greater<v, Nil> {
    typedef Nil value;
};

template<int v, int x, typename xs>
struct Greater<v, Cons<x, xs>> {
    typedef typename IfCond<x <= v,
                            typename Greater<v, xs>::value,
                            Cons<x, typename Greater<v, xs>::value>
                     >::value value;
};

// -------------------------------------

template<int v, typename Lst>
struct Remove;

template<int v>
struct Remove<v, Nil> {
    typedef Nil value;
};

template<int v, int x, typename xs>
struct Remove<v, Cons<x, xs>> {
    typedef typename IfCond<v == x,
                            xs,
                            Cons<x,typename Remove<v, xs>::value>
                     >::value value;
};

// -------------------------------------

template<typename Lst>
struct PrintList;

template<>
struct PrintList<Nil> {
    void operator()() {
        std::cout << "Nil" << std::endl;
    }
};

template<int x, typename xs>
struct PrintList<Cons<x, xs>> {
    void operator()() {
        std::cout << x << ' ';
        // PrintList<xs>(): instantiate
        // PrintList<xs>()(): object member function call
        PrintList<xs>()();
    }
};

// #define PRINT_LIST(Lst) PrintList<Lst>()()

// -------------------------------------

template<typename Lst>
struct SelectSort;

template<>
struct SelectSort<Nil> {
    typedef Nil value;
};

template<int x>
struct SelectSort<Cons<x, Nil>> {
    typedef Cons<x, Nil> value;
};

template<int x, typename xs>
struct SelectSort<Cons<x, xs>> {
    typedef Cons<x, xs> lst;

    enum { min_value = Min<lst>::value };

    typedef typename Remove<min_value, lst>::value remain_lst;

    typedef Cons<min_value, typename SelectSort<remain_lst>::value> value;
};

// -------------------------------------

template<int v, typename Lst>
struct Insert;

template<int v>
struct Insert<v, Nil> {
    typedef Cons<v, Nil> value;
};

template<int v, int x, typename xs>
struct Insert<v, Cons<x, xs>> {
    typedef Cons<v, Cons<x, xs>> value_1;

    typedef Cons<x, typename Insert<v, xs>::value > value_2;

    typedef typename IfCond<v <= x, value_1, value_2>::value value;
};

template<typename Lst>
struct InsertSort;

template<>
struct InsertSort<Nil> {
    typedef Nil value;
};

template<int x>
struct InsertSort<Cons<x, Nil>> {
    typedef Cons<x, Nil> value;
};

template<int x, typename xs>
struct InsertSort<Cons<x, xs>> {
    typedef typename InsertSort<xs>::value sorted;

    typedef typename Insert<x, sorted>::value value;
};

// -------------------------------------

template<typename Lst>
struct QuickSort;

template<>
struct QuickSort<Nil> {
    typedef Nil value;
};

template<int x>
struct QuickSort<Cons<x, Nil>> {
    typedef Cons<x, Nil> value;
};

template<int x, typename xs>
struct QuickSort<Cons<x, xs>> {
    typedef typename LessEq<x, xs>::value less_eq;
    typedef typename Greater<x, xs>::value greater;

    typedef typename Append<typename QuickSort<less_eq>::value,
                            Cons<x, typename QuickSort<greater>::value>
                     >::value value;
};

// -------------------------------------

int main() {
    std::cout << "--- TEST: Cons, Head, Tail ---" << std::endl;
    std::cout << Head<Cons<8,Cons<7,Nil>>>::value << std::endl;
    std::cout << Head<Tail<Cons<8,Cons<7,Nil>>>::value>::value << std::endl;
    std::cout << HEAD(LIST_1(8)) << std::endl;
    std::cout << HEAD(LIST_2(8, 7)) << std::endl;
    std::cout << HEAD(LIST_3(8, 7, 6)) << std::endl;
    std::cout << HEAD(TAIL(LIST_3(8, 7, 6))) << std::endl;
    std::cout << HEAD(TAIL(TAIL(LIST_3(8, 7, 6)))) << std::endl;

    std::cout << "--- TEST: List ---" << std::endl;
    PrintList< List<>::value >()();
    PrintList< List<1>::value >()();
    PrintList< List<1, 2>::value >()();
    PrintList< List<1, 2, 3>::value >()();
    PrintList< List<1, 2, 3, 4>::value >()();
    PrintList< List<1, 2, 3, 4, 5>::value >()();

    std::cout << "--- TEST: PrintList ---" << std::endl;
    PrintList<Nil>()();
    PrintList<LIST_2(2, 3)>()();

    std::cout << "--- TEST: IfCond ---" << std::endl;
    PrintList< IfCond<2 == 2, LIST_2(200, 300), LIST_2(400, 500)>::value  >()();

    std::cout << "--- TEST: Take ---" << std::endl;
    std::cout << Take< 2, LIST_2(2, 3) >::value << std::endl;

    std::cout << "--- TEST: Len ---" << std::endl;
    std::cout << Len< LIST_2(7, 8) >::value << std::endl;

    std::cout << "--- TEST: Min ---" << std::endl;
    std::cout << Min< LIST_3(33, 44, 55) >::value << std::endl;
    std::cout << Min< LIST_3(66, 44, 55) >::value << std::endl;

    std::cout << "--- TEST: Remove ---" << std::endl;
    PrintList< Remove<4, LIST_4(4, 5, 6, 7)>::value >()();
    PrintList< Remove<9, LIST_4(4, 5, 6, 7)>::value >()();

    std::cout << "--- TEST: SelectSort ---" << std::endl;
    PrintList< SelectSort<List<4, 2, 6, 4, 1, 0>::value>::value >()();
    PrintList< SelectSort<List<4>::value>::value >()();

    std::cout << "--- TEST: InsertSort ---" << std::endl;
    PrintList< InsertSort<List<4, 2, 6, 4, 1, 0>::value>::value >()();
    PrintList< InsertSort<List<4>::value>::value >()();

    std::cout << "--- TEST: LessEq ---" << std::endl;
    PrintList< LessEq<4, List<4, 2, 6, 4, 1, 0>::value>::value >()();
    PrintList< LessEq<9, List<4>::value>::value >()();

    std::cout << "--- TEST: Greater ---" << std::endl;
    PrintList< Greater<4, List<4, 2, 6, 4, 1, 0>::value>::value >()();
    PrintList< Greater<9, List<4>::value>::value >()();

    std::cout << "--- TEST: Append ---" << std::endl;
    PrintList< Append<List<4>::value, List<5, 6, 1, 0>::value>::value >()();
    PrintList< Append<List<1, 2, 3>::value, List<4>::value>::value >()();

    std::cout << "--- TEST: QuickSort ---" << std::endl;
    PrintList< QuickSort<List<4, 2, 6, 4, 1, 0>::value>::value >()();
    PrintList< QuickSort<List<4>::value>::value >()();

    return 0;
}
