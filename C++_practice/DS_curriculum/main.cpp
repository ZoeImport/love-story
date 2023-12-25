#include "App.h"
#include "HashTable.h"
#include "UserInfo.h"
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

int sti(std::string str) {
  int uniqueInteger = 0;
    std::unordered_set<char> uniqueChars;
    for (char ch : str) {
        // 判断字符是否已经出现过
        if (uniqueChars.find(ch) == uniqueChars.end()) {
            // 字符第一次出现，将其ASCII值累加
            uniqueInteger += static_cast<int>(ch);
            uniqueChars.insert(ch);
        }
    }
    return uniqueInteger;
}

int main() {
  // HashTable hashTableByPhoneNumber;
  // HashTable hashTableByUserName;
  // vector<UserInfo> user_info_list;
  // ifstream ifs("../user_info.txt");
  // AppFunction(ifs, hashTableByPhoneNumber,hashTableByUserName,user_info_list);

  cout<<sti("lisa")<<endl;
}
