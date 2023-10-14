#include <stdlib.h>
#include"PRBTree.h"
#include <iostream>
#include <string>
#include <algorithm>
#include<vector>

template<class T>
void print(RBTree<T> const& t)
{
    forEach(t, [](T v)
        {
            std::cout << v << " ";
        });
    std::cout << std::endl;
}

template<class T>
void preorder(RBTree<T> const& t)
{
    if (t.isEmpty()) {
        return;
    }

    int c = t.color_();
    if (c== 0)
        std::cout << "Red " << t.root() << std::endl;
    else
        std::cout << "Black " << t.root() << std::endl;
    preorder(t.left());
    preorder(t.right());


}

void testInit()
{
    RBTree<int> t{ 50, 40, 30, 10, 20, 30, 100, 0, 45, 55, 25, 15 };
    print(t);
}

void testInit2() {
    RBTree<int> t0{1};
    preorder(t0);
    std::cout << "------------------" << std::endl;

    std::vector<int> lis{  5, 7, 8, 9, 10, 11};

    // RBTree<int> t0;

    for (int i = 0; i < 6; ++i) {
        std::vector<int> addi0;
        addi0.push_back(lis[i]);
        t0 = inserted(t0, addi0.begin(), addi0.end());
        preorder(t0);
        std::cout << "------------------" << std::endl;

    }


    RBTree<int> t1{1, 5, 7, 8, 9, 10, 11};
    print(t1);
    preorder(t1);

    RBTree<int> t_ = t1.find(1);

    std::vector<int> addi = { 3 };
    //  t1 = inserted(t1, addi.begin(), addi.end());
     // print(t1);
    t1.assert1();

    RBTree<int> t2(t1);
    // t2.inserted(3);
    t2 = inserted(t1, addi.begin(), addi.end());
    print(t2);
    preorder(t2);
    t2.assert1();

    std::vector<int> addi2 = { 6 };
    RBTree<int> t3(t2);
    t3 = inserted(t2, addi2.begin(), addi2.end());
    print(t3);
    preorder(t3);

}

void testInit3() {
    RBTree<int> t1{1, 5, 7, 8, 9, 10, 11};
    print(t1);
    preorder(t1);
    RBTree<int> t2 = t1.deleted(5);
    print(t2);
    preorder(t2);
    t1 = RBTree<int>(t2);

    for (int i = 12; i < 40; ++i) {
        t2 = t1.inserted(i);
        t1 = RBTree<int>(t2);
    }

    for (int i =7; i < 40; ++i) {

        t2 = t1.deleted(i);
        print(t2);
        preorder(t2);
        t2.assert1();
        t2.countB();
        t1 = RBTree<int>(t2);
    }
   

}

int main() {
    testInit3();
    return 0;
}
