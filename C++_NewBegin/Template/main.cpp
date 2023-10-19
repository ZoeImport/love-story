#include <iostream>
#include <string>
using namespace std;
//func Template
namespace voidTemplate {
    template <class T> T max(T a, T b) { return a > b ? a : b; }
}

//class Template
namespace classTemplate {
    template <class nameType,class ageType=int>
    class Person{
        public:
            nameType name;
            ageType age;
            Person()=default;
            Person(nameType name,ageType age):name(name),age(age){};
            void showInfo(){
                std::cout<<this->name<<","<<this->age<<std::endl;
            }
    };
}

int main(){
// template
// 函数模板和普通函数调用顺序：
// 如果普通函数和函数模板都可以实现，调用普通函数
// 可以使用空模板参数强制调用函数模板
// 函数模板可以发生重载
// 如果模板可以实现更好的匹配就调用函数模板
    cout << voidTemplate::max(1, 3) << endl;

//类模板可以自动进行类型推导
    classTemplate::Person("Lucy",18).showInfo();
    classTemplate::Person(18,18).showInfo();
//类模板可以在类型中有默认参数
    classTemplate::Person<std::string>("Tom",18).showInfo();

//类模板中的成员函数只有在被调用时才被创建
//普通类中的成员函数在一开始就创建

}