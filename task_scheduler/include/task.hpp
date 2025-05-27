#pragma once

struct Task{
  int id;
  int priority;
  std::function<void()> execute;

  bool operator==(const Task&) const;
};