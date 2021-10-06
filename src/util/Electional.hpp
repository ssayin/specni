#pragma once

#include <vector>

template <typename T> struct Attribute {
  virtual bool is_satisfied(T *item) = 0;
};

template <typename T> struct ChooserBase {
  virtual std::vector<T *> choose(std::vector<T *> items,
                                  std::vector<Attribute<T>> &attrs) = 0;
};

struct DateChooser : ChooserBase<double> {
  std::vector<double *> choose(std::vector<double *> items,
                               std::vector<Attribute<double>> &attrs) {
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
