#ifndef _STU_CLASS_
#define _STU_CLASS_
#include<string>
using namespace std;

class Student{
    private:
        string name;
        mutable int age;
        char gender;
    public:
        Student();//空参构造
        Student(string name,int age,char gender);
        ~Student();
        void setAge(int age);
        int getAge()const;
        void setName(string name);
        string getName();
        void showAge() const;
};

#endif