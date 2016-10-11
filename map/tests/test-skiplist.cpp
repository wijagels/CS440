#include "SkipList.hpp"

int main() {
  cs540::SkipList<int, 32> sl{};
  sl.insert(0);
  sl.find(0);
}
