#pragma once

/*
 * A simplified version of Howard Hinnant's combinations.hpp
 */

#include <algorithm>

namespace detail {

template <class Function, class It> class bound_range {
  Function f_;
  It first_;
  It last_;

public:
  bound_range(Function f, It first, It last)
      : f_(f), first_(first), last_(last) {}

  bool operator()() { return f_(first_, last_); }

  bool operator()(It, It) { return f_(first_, last_); }
};

template <class BidirIter>
void rotate_discontinuous(
    BidirIter first1, BidirIter last1,
    typename std::iterator_traits<BidirIter>::difference_type d1,
    BidirIter first2, BidirIter last2,
    typename std::iterator_traits<BidirIter>::difference_type d2) {
  using std::swap;
  if (d1 <= d2)
    std::rotate(first2, std::swap_ranges(first1, last1, first2), last2);
  else {
    BidirIter i1 = last1;
    while (first2 != last2)
      swap(*--i1, *--last2);
    std::rotate(first1, i1, last1);
  }
}

// A binder for binding arguments to call permute
template <class Function, class It> class call_permute {
  typedef typename std::iterator_traits<It>::difference_type D;
  Function f_;
  It first_;
  It last_;
  D d_;

public:
  call_permute(Function f, It first, It last, D d)
      : f_(f), first_(first), last_(last), d_(d) {}

  bool operator()() { return permute(first_, last_, d_, f_); }
};

template <class BidirIter, class Function>
bool combine_discontinuous(
    BidirIter first1, BidirIter last1,
    typename std::iterator_traits<BidirIter>::difference_type d1,
    BidirIter first2, BidirIter last2,
    typename std::iterator_traits<BidirIter>::difference_type d2, Function &f,
    typename std::iterator_traits<BidirIter>::difference_type d = 0) {
  typedef typename std::iterator_traits<BidirIter>::difference_type D;
  using std::swap;
  if (d1 == 0 || d2 == 0)
    return f();
  if (d1 == 1) {
    for (BidirIter i2 = first2; i2 != last2; ++i2) {
      if (f())
        return true;
      swap(*first1, *i2);
    }
  } else {
    BidirIter f1p = std::next(first1);
    BidirIter i2 = first2;
    for (D d22 = d2; i2 != last2; ++i2, --d22) {
      if (combine_discontinuous(f1p, last1, d1 - 1, i2, last2, d22, f, d + 1))
        return true;
      swap(*first1, *i2);
    }
  }
  if (f())
    return true;
  if (d != 0)
    rotate_discontinuous(first1, last1, d1, std::next(first2), last2, d2 - 1);
  else
    rotate_discontinuous(first1, last1, d1, first2, last2, d2);
  return false;
}

// A binder for binding arguments to call combine_discontinuous
template <class Function, class BidirIter> class call_combine_discontinuous {
  typedef typename std::iterator_traits<BidirIter>::difference_type D;
  Function f_;
  BidirIter first1_;
  BidirIter last1_;
  D d1_;
  BidirIter first2_;
  BidirIter last2_;
  D d2_;

public:
  call_combine_discontinuous(BidirIter first1, BidirIter last1, D d1,
                             BidirIter first2, BidirIter last2, D d2,
                             Function &f)
      : f_(f), first1_(first1), last1_(last1), d1_(d1), first2_(first2),
        last2_(last2), d2_(d2) {}

  bool operator()() {
    return combine_discontinuous(first1_, last1_, d1_, first2_, last2_, d2_,
                                 f_);
  }
};

} // namespace detail

template <class BidirIter, class Function>
Function for_each_combination(BidirIter first, BidirIter mid, BidirIter last,
                              Function f) {
  detail::bound_range<Function &, BidirIter> wfunc(f, first, mid);
  detail::combine_discontinuous(first, mid, std::distance(first, mid), mid,
                                last, std::distance(mid, last), wfunc);
  return std::move(f);
}
