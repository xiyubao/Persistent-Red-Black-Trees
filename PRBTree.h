#pragma once
#pragma once
//#include"List.h"
#include <cassert>
#include <memory>

template<class T>
class RBTree
{
    enum Color { W, R, B, BB };

    struct Node
    {
        Node(Color c,
            std::shared_ptr<const Node> const& lft,
            T val,
            std::shared_ptr<const Node> const& rgt)
            : _c(c), _lft(lft), _val(val), _rgt(rgt)
        {}
        Color _c;
        std::shared_ptr<const Node> _lft;
        T _val;
        std::shared_ptr<const Node> _rgt;
    };
    explicit RBTree(std::shared_ptr<const Node> const& node) : _root(node) {}
    Color rootColor() const
    {
        assert(!isEmpty());
        return _root->_c;
    }
public:
    RBTree() {}
    RBTree(Color c, RBTree const& lft, T val, RBTree const& rgt)
        : _root(std::make_shared<const Node>(c, lft._root, val, rgt._root))
    {
        assert(lft.isEmpty() || lft.root() < val);
        assert(rgt.isEmpty() || val < rgt.root());
    }
    RBTree(std::initializer_list<T> init)
    {
        RBTree t;
        for (T v : init)
        {
            t = t.inserted(v);
        }
        _root = t._root;
    }
    template<class I>
    RBTree(I b, I e)
    {
        RBTree t;
        for_each(b, e, [&t](T const& v) {
            t = t.inserted(v);
            });
        _root = t._root;
    }
    bool isEmpty() const { return !_root; }
    Color color() const
    {
        assert(!isEmpty());
        return _root->_c;
    }
    int color_() const {
        assert(!isEmpty());
        return (int)_root->_c-1;
    }
    T root() const
    {
        assert(!isEmpty());
        return _root->_val;
    }
    RBTree left() const
    {
        assert(!isEmpty());
        return RBTree(_root->_lft);
    }
    RBTree right() const
    {
        assert(!isEmpty());
        return RBTree(_root->_rgt);
    }
    bool member(T x) const
    {
        if (isEmpty())
            return false;
        T y = root();
        if (x < y)
            return left().member(x);
        else if (y < x)
            return right().member(x);
        else
            return true;
    }
    RBTree find(T x) const
    {
        //if (isEmpty())
        assert(!isEmpty());
        T y = root();
        if (x < y)
            return left().find(x);
        else if (y < x)
            return right().find(x);
        else
            return *this;
    }
    //RBTree set(T x) {
    //  // _root->_val = x;
    //   
    //}
    RBTree inserted(T x) const
    {
        RBTree t = ins(x);
        return RBTree(B, t.left(), t.root(), t.right());
    }

    RBTree deleted(T x) const
    {
        if (!member(x))
            return *this;
        RBTree t = del(x);
        return RBTree(B, t.left(), t.root(), t.right());
    }

    // 1. No red node has a red child.
    void assert1() const
    {
        if (!isEmpty())
        {
            auto lft = left();
            auto rgt = right();
            if (rootColor() == R)
            {
                assert(lft.isEmpty() || lft.rootColor() == B);
                assert(rgt.isEmpty() || rgt.rootColor() == B);
            }
            lft.assert1();
            rgt.assert1();
        }
    }
    // 2. Every path from root to empty node contains the same
    // number of black nodes.
    int countB() const
    {
        if (isEmpty())
            return 0;
        int lft = left().countB();
        int rgt = right().countB();
        assert(lft == rgt);
        return (rootColor() == B) ? 1 + lft : lft;
    }
private:
    RBTree ins(T x) const
    {
        assert1();
        if (isEmpty())
            return RBTree(R, RBTree(), x, RBTree());
        T y = root();
        //爷值
        Color c = rootColor();
        //爷颜色
        if (c == B) //爷为黑
        {
            if (x < y) //子为左
                return balance(left().ins(x), y, right());
            else if (y < x) //子为右
                return balance(left(), y, right().ins(x));
            else
                return *this; // no duplicates
        }
        else //爷为红
        {
            if (x < y) //子为左
                return RBTree(c, left().ins(x), y, right());
            else if (y < x) //子为右
                return RBTree(c, left(), y, right().ins(x));
            else
                return *this; // no duplicates
        }
    }
    RBTree del(T k) const
    {
        assert1();
        assert(!isEmpty());

        T y = root();
        Color c = rootColor();

        if (y > k) {
            return bubble(c, left().del(k), y, right());
        }
        else if (y == k) {
            return remove();
        }
        else {
            return bubble(c, left(), y, right().del(k));
        }

        return RBTree();
    }

    RBTree remove() const
    {
        Color c = rootColor();
        if (left().isEmpty() && right().isEmpty()) {
            if (c == B)
                return RBTree<T>(BB, left(), -1, right());
            else
                return RBTree<T>(B, left(), -1, right());
        }
        else if (left().isEmpty() && !right().isEmpty()) {
            return RBTree<T>(B, right().left(), right().root(), right().right());
        }
        else if (!left().isEmpty() && right().isEmpty()) {
            return balance2(B, left().left(), left().root(), left().right());
        }
        else {
            T l = less_max(left());
            return bubble(c, left().del(l), l, right());
        }
    }

    T less_max(RBTree const& lft) const {
        RBTree temp = lft;
        while (!temp.right().isEmpty())
            temp = temp.right();
        return temp.root();
    }

    static RBTree bubble(Color c, RBTree const& lft, T x, RBTree const& rgt)
    {
        if (lft.IsColor(BB) || rgt.IsColor(BB))
        {
            RBTree lt = !lft.isEmpty() && lft.root() == -1 ? RBTree() : lft.paint(Minus(lft.color()));
            RBTree rt = !rgt.isEmpty() && rgt.root() == -1 ? RBTree() : rgt.paint(Minus(rgt.color()));
            return balance2(Add(c), lt, x, rt);
        }
        else {
            RBTree lt = !lft.isEmpty() && lft.root() == -1? RBTree(): RBTree(lft);
            RBTree rt = !rgt.isEmpty() && rgt.root() == -1 ? RBTree() : RBTree(rgt);
            return RBTree<T>(c, lt, x, rt);
        }

        /*if (!lft.isEmpty() && lft.root() == -1) {
            if (rgt.isEmpty())
                return RBTree<T>(Add(c), RBTree(), x, RBTree());
            else if(rgt.IsColor(BB))
                return RBTree<T>(Add(c), RBTree(), x, rgt.paint(Minus(rgt.color())));
            else
                return RBTree<T>(c, RBTree(), x, rgt);
        }
        else if (!rgt.isEmpty() && rgt.root() == -1) {
            if (lft.isEmpty())
                return RBTree<T>(Add(c), RBTree(), x, RBTree());
            else if(lft.IsColor(BB))
                return RBTree<T>(Add(c), lft.paint(Minus(lft.color())), x, RBTree());
            else
                return RBTree<T>(c, lft, x, RBTree());
        }
        else if (lft.IsColor(BB) || rgt.IsColor(BB)) {
            return balance2(Add(c), lft.paint(Minus(lft.color())), x, rgt.paint(Minus(rgt.color())));
        }
        else {
            return RBTree<T>(c, lft, x, rgt);
        }*/
    }

    static RBTree balance2(Color c, RBTree const& lft, T x, RBTree const& rgt) {

        // case of double RR

        if (lft.IsColor(R) && lft.right().IsColor(R)) {
            return RBTree<T>(Minus(c),
                RBTree<T>(B, lft.left(), lft.root(), lft.right().left()),
                lft.right().root(),
                RBTree<T>(B, lft.right().right(), x, rgt));
        }
        else if (lft.doubledLeft())
        {
            return RBTree<T>(Minus(c),
                lft.left().paint(B),
                lft.left().root(),
                RBTree<T>(B, lft.left().right(), x, rgt));
        }
        else if (rgt.IsColor(R) && rgt.left().IsColor(R))
        {
            return RBTree<T>(Minus(c),
                RBTree<T>(B, lft, x, rgt.left().left()),
                rgt.left().root(),
                RBTree<T>(B, rgt.left().right(), rgt.root(), rgt.right()));
        }
        else if (rgt.doubledRight())
        {
            return RBTree<T>(Minus(c),
                RBTree<T>(B, lft, x, rgt.left()),
                rgt.root(),
                rgt.right().paint(B));
        }

        //case of  W
        if (c == BB) {

            if (lft.IsColor(W) && lft.left().IsColor(B) && lft.right().IsColor(B))
            {
                return RBTree(B,
                    balance2(B, lft.left().paint(R), lft.root(), lft.right().left()),
                    lft.right().root(),
                    RBTree<T>(B, lft.right().right(), x, rgt));
            }
            else if (rgt.IsColor(W) && rgt.left().IsColor(B) && rgt.right().IsColor(B))
            {
                return RBTree<T>(B,
                    RBTree<T>(B, lft, x, rgt.left().left()),
                    rgt.left().root(),
                    balance2(B, rgt.left().right(), rgt.root(), rgt.right().paint(R)));
            }
        }

        return RBTree(c, lft, x, rgt);
    }

    // Called only when parent is black
    static RBTree balance(RBTree const& lft, T x, RBTree const& rgt)
    {
        if (lft.doubledLeft())
            return RBTree(R
                , lft.left().paint(B)
                , lft.root()
                , RBTree(B, lft.right(), x, rgt));
        else if (lft.doubledRight())
            return RBTree(R
                , RBTree(B, lft.left(), lft.root(), lft.right().left())
                , lft.right().root()
                , RBTree(B, lft.right().right(), x, rgt));
        else if (rgt.doubledLeft())
            return RBTree(R
                , RBTree(B, lft, x, rgt.left().left())
                , rgt.left().root()
                , RBTree(B, rgt.left().right(), rgt.root(), rgt.right()));
        else if (rgt.doubledRight())
            return RBTree(R
                , RBTree(B, lft, x, rgt.left())
                , rgt.root()
                , rgt.right().paint(B));
        else
            return RBTree(B, lft, x, rgt);
    }

    bool doubledLeft() const
    {
        return !isEmpty()
            && rootColor() == R
            && !left().isEmpty()
            && left().rootColor() == R;
    }
    bool doubledRight() const
    {
        return !isEmpty()
            && rootColor() == R
            && !right().isEmpty()
            && right().rootColor() == R;
    }
    bool IsColor(Color c)const {
        return !isEmpty() && rootColor() == c;
    }
    RBTree paint(Color c) const
    {
        assert(!isEmpty());
        return RBTree(c, left(), root(), right());
    }
    static Color Add(Color c)  {
        return (Color)((int)c + 1);
    }
    static Color Minus(Color c)  {
        return (Color)((int)c - 1);
    }
private:
    std::shared_ptr<const Node> _root;
};

template<class T, class F>
void forEach(RBTree<T> const& t, F f) {
    if (!t.isEmpty()) {
        forEach(t.left(), f);
        f(t.root());
        forEach(t.right(), f);
    }
}

template<class T, class Beg, class End>
RBTree<T> inserted(RBTree<T> t, Beg it, End end)
{
    if (it == end)
        return t;
    T item = *it;
    auto t1 = inserted(t, ++it, end);
    return t1.inserted(item);
}

template<class T>
RBTree<T> treeUnion(RBTree<T> const& a, RBTree<T> const& b)
{
    // a u b = a + (b \ a)
    RBTree<T> res = a;
    forEach(b, [&res, &a](T const& v) {
        if (!a.member(v))
            res.inserted(v);
        });
    return res;
}

//// Remove elements in set from a list
//template<class T>
//List<T> rem_from_list(List<T> const& lst, RBTree<T> const& set)
//{
//    List<T> res;
//    lst.forEach([&res, &set](T const& v) {
//        if (!set.member(v))
//            res = res.pushed_front(v);
//        });
//    return res;
//}