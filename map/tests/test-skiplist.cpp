#include <iostream>
#include <string>
#include "SkipList.hpp"

int main() {
  cs540::SkipList<int, 32> sl{};
  sl.insert(2);
  sl.insert(1);
  sl.insert(6);
  sl.insert(12);
  sl.insert(8);
  sl.insert(18);
  sl.insert(13);
  sl.insert(17);
  for (auto it : sl) {
    logd("%d", it.data);
    for (auto jt : it.skips) {
      std::string s{};
      if (jt == sl.end())
        s = "NIL";
      else
        s = std::to_string(jt->data);
      logd("--> %s", s.c_str());
    }
  }
  std::cout << (*sl.find(6)).data << std::endl;
  std::cout << (*sl.find(1)).data << std::endl;
  std::cout << (*sl.find(6)).data << std::endl;
  std::cout << (*sl.find(17)).data << std::endl;
  sl.begin();
  sl.end();
}
