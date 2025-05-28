#include "../include/task.hpp"
#include <iostream>
#include <functional>

int main(){
  std::function<void()> func = [](){std::cout << "result: " << 3+5 << std::endl;};
  Task t = Task(func, 4);
  std::cout << t.id << std::endl;
  std::cout << t.priority << std::endl;
  t.execute();
}