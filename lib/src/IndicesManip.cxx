//
// Created by phw on 22/01/18.
//
#include "otagr/IndicesManip.hxx"

namespace OTAGR {
Indice pos(const OT::Indices &X, Indice val) {
  for (Indice p = 0; p < X.getSize(); ++p) {
    if (X[p] == val)
      return p;
  }
  return X.getSize();
}

bool isIn(const OT::Indices &X, Indice y) { return (pos(X, y) != X.getSize()); }

OT::Indices operator+(const OT::Indices &X, Indice y) {
  OT::Indices res(X);
  if (!isIn(res, y)) {
    res.add(y);
  }
  return res;
}

OT::Indices operator-(const OT::Indices &X, Indice y) {
  OT::Indices res(X);
  auto p = pos(X, y);
  if (p < X.getSize()) {
    res.__delitem__(p);
  }
  return res;
}

IndicesCombinationIterator::IndicesCombinationIterator(const OT::Indices &base,
                                                       OT::UnsignedInteger n)
    : base_(base), size_(n) {
  if (n > base.getSize()) {
    throw OT::InvalidArgumentException(HERE)
        << "Error: cannot iterate on subsets of size " << n << " in "
        << base.__str__() << ".";
  }
  coord_.reserve(size_);
  setFirst();
}

void IndicesCombinationIterator::setFirst() {
  coord_.clear();
  current_.clear();
  for (int i = 0; i < size_; ++i) {
    coord_.push_back(i);
    current_ = current_ + base_[i];
  }
  carry_ = false;
}
bool IndicesCombinationIterator::isLast() const { return carry_; }

void IndicesCombinationIterator::next() {
  for (int i = size_ - 1; i >= 0; --i) {
    if (coord_[i] < base_.getSize() + i - size_) {
      set_(i, coord_[i] + 1);

      for (int j = i + 1; j < size_; ++j) {
        set_(j, coord_[j - 1] + 1);
      }
      return;
    }
  }
  carry_ = true;
}
const OT::Indices &IndicesCombinationIterator::current() const {
  return current_;
}

void IndicesCombinationIterator::set_(Indice pos, int val) {
  coord_[pos] = val;
  current_[pos] = base_[coord_[pos]];
}

std::string IndicesCombinationIterator::__str__(const std::string &delim) {
  std::stringstream ss;
  ss << delim << "CombinationIterator :\n";
  ss << delim << "  | base :" << static_cast<const OT::Indices>(base_).__str__()
     << "\n";
  ss << delim << "  | coord :";
  for (const auto &i : coord_)
    ss << i << ", ";
  ss << "\n";
  ss << delim << "  | current :" << current_.__str__() << "\n";
  return ss.str();
}
} // eo namespace
