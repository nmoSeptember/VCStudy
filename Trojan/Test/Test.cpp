// Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

typedef struct TestStruct {
	int num;
	char ch;
}Stu;

void method(const struct TestStruct * p) {

}

int main()
{
	Stu stu;
	method(&stu);
	std::cout << "Hello World!\n";
}

#include <iostream>
//
//using namespace std;
//
//int main()
//{
//	const int a = 5;
//	const int* p = &a;
//	int* p_var = NULL;
//
//	p_var = const_cast <int*>(p);   //ǿתΪ��constָ��
//	cout << a << endl;
//	*p_var = 10;    //���¸�ֵ
//	cout << "*p=" << *p << endl;         //���10
//	cout << "*p_var=" << *p_var << endl; //���10
//	cout << "a=" << a << endl;           //���5
//	system("pause");
//
//	return 0;
//}
//
