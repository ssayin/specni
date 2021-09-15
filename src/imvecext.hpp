#pragma once
#include "imgui.h"

#include <math.h>
static inline ImVec2 operator+(const ImVec2 &lhs, const ImVec2 &rhs) {
  return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

static inline ImVec2 operator-(const ImVec2 &l, const ImVec2 &r) {
  return {l.x - r.x, l.y - r.y};
}

static inline ImVec2 operator/(const ImVec2 &l, const int r) {
  return {l.x / 2, l.y / 2};
}

inline bool operator<(const ImVec2 &lhs, const ImVec2 &rhs) {
  return (rhs.x > lhs.x) && (rhs.y > lhs.x);
}

inline bool operator>(ImVec2 &lhs, const ImVec2 &rhs) {
  return (rhs.x < lhs.x) && (rhs.y < lhs.x);
}

static inline ImVec2 ImRotate(const ImVec2 &v, float cos_a, float sin_a) {
  return ImVec2(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a);
}
