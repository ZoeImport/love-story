#include "HashTable.h"
#include "UserInfo.h"
#include <string>
#include <vector>

int HashTable::hashFUnction(int key){
    return key%TableSize;
}
int HashTable::insert(int key,const UserInfo& value){
    int index=hashFUnction(key);
    for (auto &atom :table[index]) {
        if(atom.first==key)atom.second=value;
        return 2;
    }
    table[index].emplace_back(key,value);
    return 1;
}
UserInfo HashTable::get(int key){
    int index=hashFUnction(key);
    for (auto& atom :table[index]) {
        if(atom.first==key)return atom.second;
    }
    return UserInfo();
}


UserInfo& HashTable::operator[](int key){
    int index=hashFUnction(key);
    for (auto& atom :table[index]) {
        if (atom.first==key) {
            return atom.second;
        }
    }

    table[index].emplace_back(key,UserInfo());
    return table[index].back().second;
}

// UserInfo& HashTable::operator[](std::string key){
//     int index=hashFUnction(stringToInt(key));
//     for (auto& atom :table[index]) {
//         if (atom.first==index) {
//             return atom.second;
//         }
//     }

//     table[index].emplace_back(key,UserInfo());
//     return table[index].back().second;
// }

HashTable& initHashTableByPhoneNumber(std::ifstream& ifs, HashTable &hashTable,std::vector<UserInfo>& user_info_list){
    for (auto index=0;index<user_info_list.size();++index) {
        user_info_list.pop_back();
    }
    readUserInfo(ifs,user_info_list);
    for (auto atom :user_info_list) {
        hashTable[stringToInt(atom.getPhoneNumber())]=atom;
    }
    return hashTable;
}

HashTable& initHashTableByUserName(std::ifstream& ifs, HashTable &hashTable,std::vector<UserInfo>& user_info_list){
    for (auto index=0;index<user_info_list.size();++index) {
        user_info_list.pop_back();
    }
    readUserInfo(ifs,user_info_list);
    for (auto atom :user_info_list) {
        hashTable[stringToInt(atom.getUserName())]=atom;
    }
    return hashTable;
}