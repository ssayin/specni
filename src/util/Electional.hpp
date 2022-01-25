#pragma once

#include <vector>

template <typename T> struct Attribute {
  virtual auto is_satisfied(T *item) -> bool = 0;
};

template <typename T> struct ChooserBase {
  virtual auto choose(std::vector<T *> items,
                                  std::vector<Attribute<T>> &attrs) -> std::vector<T *> = 0;
};

struct DateChooser : ChooserBase<double> {
  auto choose(std::vector<double *> items,
                               std::vector<Attribute<double>> &attrs) override; -> std::vector<double *> {
    std::vector<double *> ret;
    for (auto &d : items) {
      for (auto &a : attrs) {
        if (!a.is_satisfied(d))
          continue;
      }
      ret.push_back(d);
    }
    return ret;
  }
};
