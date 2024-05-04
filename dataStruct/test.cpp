
#include<iostream>
using namespace std;
void sort(int a, int b)
{
	if (a < b)
	{
		int temp = a;
		a = b;
		b = temp;
	}
	cout << a << " " << b << endl;
}

void sort(int a, int b, int c)
{
	if (a < b)
	{
		int temp = a;
		a = b;
		b = temp;
	}
	if (a< c)
	{
		int temp = a;
		a = c;
		c = temp;
	}
	if (b< c)
	{
		int temp = b;
		b= c;
		c = temp;
	}
	cout << a << " " << b <<" "<<c << endl;
}
int main()
{
	int x, y, z;
	cout << "请输入两个数：";
	cin >> x >> y;
	cout << "从大到小排列后的结果为：";
	sort(x, y);
	cout << "请输入三个数：";
	cin >> x >> y>>z;
	cout << "从大到小排列后的结果为：";
	sort(x, y,z);
	return 0;
}