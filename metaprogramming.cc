/*

目标：
使用模板元编程，实现编译期识别是否某个类含有某个public成员（普通/静态成员方法，普通/静态成员变量，内嵌type）


example:

1. 判断类中是否有名为foo1的普通成员方法
2. 判断类中是否有名为foo2的静态成员方法
3. 判断类中是否有名为bar1的普通成员变量
4. 判断类中是否有名为bar2的静态成员变量
5. 判断类中是否有名为tt的内嵌type

*/
#include <iostream>

class A {
public:
    void        foo1(){};
    static void foo2(){};

    int              bar1;
    static const int bar2 = 0;

    using tt = int;  // same as  typedef int tt;
};

class B {
public:
    typedef void tt;
};

class C {
};

//////////////////////////////////
// 判断类中是否有名为foo1的普通成员方法
template <typename T>
std::false_type has_foo1(...);

template <typename T>
std::true_type has_foo1(decltype(std::declval<T>().foo1())*);
// std::true_type has_foo1(decltype(&T::foo1)*); // 也可以使用成员方法指针的写法

template <typename T>
inline constexpr bool has_foo1_v = decltype(has_foo1<T>(nullptr))::value;

//////////////////////////////////
// 判断类中是否有名为foo2的静态成员方法, 此处用另一种写法
template <typename T>
constexpr bool has_foo2(...)
{
    return false;
};

template <typename T>
constexpr bool has_foo2(decltype(std::declval<T>().foo1())*)
// constexpr bool has_foo2(decltype(&T::foo1)*) // 也可以使用成员方法指针的写法
{
    return true;
};

template <typename T>
inline constexpr bool has_foo2_v =
    std::bool_constant<has_foo2<T>(nullptr)>::value;
// 除了基本的函数式SFINAE写法，可以用std::void_t来实现

template <typename T, typename = void>
struct has_foo1_1 : std::false_type {
};

template <typename T>
struct has_foo1_1<T, std::void_t<decltype(std::declval<T>().foo1())>>
    : std::true_type {
};

template <typename T>
inline constexpr bool has_foo1_v_1 = has_foo1_1<T>::value;

//////////////////////////////////
// 判断类中是否有名为bar1的普通成员变量，
//
//////////////////////////////////

// std::false_type has_bar1(...);

// template <typename T>
// std::true_type has_bar1(decltype(std::declval<T>().bar1)*);
// // std::true_type has_bar1(decltype(T::bar1)*);
// // 也可以使用成员变量的写法，或者用其指针形式&T::bar2

// template <typename T>
// inline constexpr bool has_bar1_v = decltype(has_bar1<T>(nullptr))::value;
template <typename T, typename = void>
struct has_bar1 : std::false_type {
};

template <typename T>
struct has_bar1<T, std::void_t<decltype(std::declval<T>().bar2)>>
    : std::true_type {
};

template <typename T>
inline constexpr bool has_bar1_v = has_bar1<T>::value;

//////////////////////////////////
// 判断类中是否有名为bar2的静态成员变量，
//////////////////////////////////

// std::false_type has_bar2(...);

// template <typename T>
// std::true_type has_bar2(decltype(std::declval<T>().bar2)*);
// // std::true_type has_bar2(decltype(T::bar2)*);  //
// // 也可以使用成员变量的写法，或者用其指针形式&T::bar2

// template <typename T>
// inline constexpr bool has_bar2_v = decltype(has_bar2<T>(nullptr))::value;

template <typename T, typename = void>
struct has_bar2 : std::false_type {
};

template <typename T>
struct has_bar2<T, std::void_t<decltype(T::bar2)>>
    : std::true_type {  // 对于成员变量，也可用 T::bar
};

template <typename T>
inline constexpr bool has_bar2_v = has_bar2<T>::value;

//////////////////////////////////
// 判断是否有内嵌类型tt
// 除了上述的std::void_t写法，也可以将这种逻辑抽象出来，
//////////////////////////////////

template <typename T, typename = void>
struct has_tt : std::false_type {
};

template <typename T>
struct has_tt<T, std::void_t<typename T::tt>> : std::true_type {
};

template <typename T>
inline constexpr bool has_tt_v = has_tt<T>::value;

int main()
{
    std::cout << std::boolalpha;
    std::cout << "has_foo1_v<A> = " << has_foo1_v<A> << std::endl;
    std::cout << "has_foo1_v<B> = " << has_foo1_v<B> << std::endl;
    std::cout << "has_foo2_v<A> = " << has_foo2_v<A> << std::endl;
    std::cout << "has_foo2_v<B> = " << has_foo2_v<B> << std::endl;
    std::cout << "has_foo1_v_1<A> = " << has_foo1_v_1<A> << std::endl;
    std::cout << "has_foo1_v_1<B> = " << has_foo1_v_1<B> << std::endl;
    std::cout << "has_bar1_v<A> = " << has_bar1_v<A> << std::endl;
    std::cout << "has_bar1_v<B> = " << has_bar1_v<B> << std::endl;
    std::cout << "has_bar2_v<A> = " << has_bar2_v<A> << std::endl;
    std::cout << "has_bar2_v<B> = " << has_bar2_v<B> << std::endl;
    std::cout << "has_tt_v<A> = " << has_tt_v<A> << std::endl;
    std::cout << "has_tt_v<B> = " << has_tt_v<B> << std::endl;
    std::cout << "has_tt_v<C> = " << has_tt_v<C> << std::endl;
    return 0;
};
