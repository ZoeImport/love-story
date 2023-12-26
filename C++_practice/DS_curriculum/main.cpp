#include "App.h"
#include "HashTable.h"
#include "UserInfo.h"
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;
int main() {
  string filePath("../user_info.txt"); // 用户信息文件路径
  //  AppFunction(filePath);
  AppFunctionOpen(filePath);
}
