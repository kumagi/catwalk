#ifndef CATWALK_CLASSIFIER_HPP_
#define CATWALK_CLASSIFIER_HPP_
#include <vector>
#include "dataset.hpp"
namespace catwalk {

class classifier {
public:
  virtual void train(const sample& s) = 0;
  virtual int classify(const std::vector<feature>& v) const = 0;
  virtual ~classifier() {}
};

}

#endif  // CATWALK_CLASSIFIER_HPP_
