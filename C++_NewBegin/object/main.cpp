#include<iostream>
#include<string>
#include"Student.h"
#include"Teacher.h"
using namespace std;
//面向对象篇
//对象需要使用类进行定义,对于其的属性进行权限控制
//三个权限关键词：private protected public
//private:  类内可以直接使用，类外不可直接使用
//protected:类内可以直接使用，类外不可直接使用，子类可以使用
//public:   类内可以直接使用，类外可以直接使用


//this指针：成员函数和成员变量是分开存储的，对于每一个成员函数在C++中只会创建一份实例，为了区分同一类型的不同对象的调用就需要this指针
//即this指针指向被调用成员函数所属的对象
//this指针用途：
//1.在形参与成员变量同名时，就可以用this指针进行区分
//2.在类的非静态成员函数中返回对象本身return *this


//构造函数，    析构函数，构造拷贝函数
//构造函数：    创建对象（默认空定义）
//析构函数：    释放对象（默认空定义）
//构造拷贝函数： 对象进行拷贝时调用的函数，默认为值传递
//如果重写了构造函数和析构函数，构造拷贝函数可以不写（保持值传递），如果重写了构造拷贝函数就需要重写构造函数和析构函数

//深拷贝与浅拷贝
//浅拷贝：将存储的东西进行复制
//深拷贝：在堆内存中重新开辟一块新的空间再进行复制


int main(){
    //创建Student对象
    //调用构造方法的三种方式:
    //括号法：
    //Ps：在使用括号法调用时，调用无参构造不需要使用空的小括号即不该写为Student stu();因为编译器会认为这句话是声明一个函数，应该写为Student stu;
    Student stu1;//无参构造
    Student stu2("Tom",18,'M');

    //显示法：
    Student stu3=Student();
    Student stu4=Student("Lucy",20,'W');
    Student stu5=Student(stu2);//此处调用了构造拷贝函数（值传递）

    //创建匿名对象，匿名对象会在创建后被删除
    Student();//上方的显示法创建就是创建一个匿名对象再将对象传给stu3 stu4 stu5
    
    //隐式转换法：
    Student stu6={"Jack",22,'M'};
    
    // cout<<"name:"<<stu6.name<<" age:"<<stu6.age<<" gender:"<<stu6.gender<<endl;
    cout<<"Jack's age is "<<stu6.getAge()<<endl;
    
    /*
    程序输出：
         was killed
        Jack's age is 22
        Jack was killed
        Tom was killed
        Lucy was killed
        was killed
        Tom was killed
        was killed
    */


    //以上是对于Student对象的创建的一些概念
    //接下来创建Teacher对象，使用初始化列表进行构造函数定义
    
    //静态成员变量和静态成员函数
    //静态成员变量
    //1.所有成员共享一份变量
    //2.类内声明，类外初始化
    //3.在编译阶段分配内存

    //静态成员函数
    //1.所有对象共享一个函数
    //2.静态成员函数只能访问静态成员变量
    //Ps:原因：静态成员函数属于类，所有的对象共享一个成员函数，对于普通成员函数在创建时会为其创建一个this指针，使得其可以通过该指针区分调用它的对象
    //但是对于静态成员函数，并没有创建属于它的this指针，也就不能区分调用他的对象，从而也就不能在静态成员函数内部使用非静态成员变量
    
    Student stu7=Student("Tom",17,'M');
    Teacher teach1=Teacher("Lily",stu7);
    cout<<teach1.getStudent().getName()<<endl;
    
    //调用静态变量
    cout<<Teacher::teacherCount<<endl;
    //使用类名对静态成员方法进行调用，在函数中仅调用了静态成员变量
    Teacher::teachInfo();
    

    //常成员函数
    //常成员函数就是在funcName()后加上const,例如Student.h中的声明,普通成员函数相当于创建了一个指针常量(指向不能变)
    //在常成员函数中相当于加入const在this前加入const,即变成了常量指针常量(指向与值都不可变)
    //存在例外的是，假如一个成员属性被mutable修饰，那么它也可以更改
    
    stu7.showAge();
    
    //常对象
    //常对象只能调用常成员函数，因为常对象的属性值不可变
    //对于被mutable修饰的成员属性，也可以存在被修改的情况
    const Student stu8=Student("Lisa",22,'W');
    stu8.showAge();

    //友元
    //使用友元的目的：让一个函数或者类访问另一个类的私有变量
    //友元的三种实现
    //1.全局函数做友元
    //2.类做友元
    //3.成员函数做友元
    


        
    



}