#include <iostream>
#include <string>
namespace overloadMid {
    class Person{
        public:
            Person()=default;
            Person(std::string name,int age):name(name),age(age){};
            auto operator[](const std::string& name);
        private:
            std::string name;
            int age;
    };
    auto Person::operator[](const std::string &name){
        return ;
    }
}