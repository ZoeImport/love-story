#ifndef _ORDER_STACK_
#define _ORDER_STACK_

#include "ClothesOrder.h"
#include <iostream>
#include <stdexcept>
#include <vector>

// ����ջͷ�ļ�

template <typename T>
class Stack
{
public:
  // ����Ԫ��
  void push(const T &value) { data.push_back(value); }

  // ɾ��Ԫ��
  void pop()
  {
    if (empty())
    {
      throw std::out_of_range("Stack is empty");
    }
    data.pop_back();
  }

  // ��ȡջ��Ԫ��
  T &top()
  {
    if (empty())
    {
      throw std::out_of_range("Stack is empty");
    }
    return data.back();
  }

  // ջ�Ƿ�Ϊ��
  bool empty() const { return data.empty(); }

  // ��ȡջ�Ĵ�С
  size_t size() const { return data.size(); }

  // ����Ԫ�أ�����������
  int find(const T &value) const
  {
    for (int i = data.size() - 1; i >= 0; --i)
    {
      if (data[i] == value)
      {
        return i;
      }
    }
    return -1; // δ�ҵ�
  }

  // �޸�Ԫ��
  void modify(int index, const T &value)
  {
    if (index < 0 || index >= data.size())
    {
      throw std::out_of_range("Index out of range");
    }
    data[index] = value;
  }

  // ��ӡջ��Ԫ��
  void print() const
  {
    for (const auto &element : data)
    {
      std::cout << element << " ";
    }
    std::cout << std::endl;
  }

private:
  std::vector<T> data;
};
#endif