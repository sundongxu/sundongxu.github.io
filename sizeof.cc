#include <iostream>
using namespace std;

class A // 空类大小为1
{
};

class B // 只有一个虚函数的类
{
    virtual void f() {} // 非纯虚函数则需要定义函数体，纯虚函数(=0)则为抽象类不能实例化
};

class C : public A, public B
{
};

class D // 只有成员函数的类
{
    D() {}
    ~D() {}
    void f() {}
};

class E // 有成员函数及一个成员变量的类
{
    E() {}
    ~E() {}
    void f() {}
    int i;
};

class F // 有成员函数、一个成员变量和一个类成员变量(静态)的类
{
    F() {}
    ~F() {}
    void f() {}
    int i;
    static int si;
};

class G
{
    int i;
    char c;
    char cc;
    int ii;
};

int main()
{
    cout << "空类大小：" << sizeof(A) << endl;
    cout << "只有一个虚函数的类大小：" << sizeof(B) << endl;
    cout << "继承自一个空类和一个只有虚函数的类大小：" << sizeof(C) << endl;
    cout << "含有构造、析构以及一个成员函数的类大小：" << sizeof(D) << endl;
    cout << "含有构造、析构以及一个成员函数和一个int成员变量的类大小：" << sizeof(E) << endl;
    cout << "含有构造、析构以及一个成员函数和一个int成员变量与一个静态int成员变量的类大小：" << sizeof(F) << endl;
    cout << sizeof(G) << endl;
    return 0;
}