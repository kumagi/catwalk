#include <algorithm>
#include <vector>
#include <boost/unordered_map.hpp>
//#include <google/sparse_hash_map>
//#include <google/dense_hash_map>
#include "dataset.hpp"
#include "classifier.hpp"
namespace catwalk {

class perceptron : public classifier {
public:
  perceptron() { }
  void train(const sample& s) {
    const int predicted = classify(s.features);
    if (s.label != predicted) {
      /*
      std::cout << "predicted :" << predicted
        << " but it is " << s.label << " so update" << std::endl;
      //*/
      for (sample::features_t::const_iterator it = s.features.begin();
           it != s.features.end();
           ++it) {
        model_update(it->name, s.label, it->data, 1);
        model_update(it->name, predicted, it->data, -1);
      }
    }
  }
  int classify(const std::vector<feature>& v) const {
    scoremap scores;
    //scores.set_empty_key(0);
    for (std::vector<feature>::const_iterator it = v.begin();
         it != v.end();
         ++it) {
      feature2scoremap::const_iterator localmap = model.find(it->name);
      if (localmap != model.end()) {
        const scoremap& label_score = localmap->second;
        for (scoremap::const_iterator score = label_score.begin();
             score != label_score.end();
             ++score) {
          scoremap::iterator target
            = scores.find(score->first);
          const double inner = score->second * it->data;
          if (target == scores.end()) {
            scores[score->first] = inner;
          } else {
            target->second += inner;
          }
        }
      }
    }

    double max = 0;
    int maybe = 0;
    for (scoremap::const_iterator it = scores.begin();
         it != scores.end();
         ++it) {
      if (max < it->second) {
        maybe = it->first;
        max = it->second;
      }
    }
    return maybe;
  }
private:
  void model_update(int feature, int label, double score, double factor) {
    feature2scoremap::iterator it = model.find(feature);
    if (it != model.end()) {
      scoremap& update_target = it->second;
      scoremap::iterator update_label = update_target.find(label);
      if (update_label != update_target.end()) {
        update_label->second += score * factor;
      }
    } else {
      scoremap new_map;
      //new_map.set_empty_key(0);
      new_map[label] = score * factor;
      model.insert(std::make_pair(feature, new_map));
    }
  }
  //typedef google::dense_hash_map<int, double> scoremap;
  //typedef google::sparse_hash_map<int, scoremap> feature2scoremap;
  typedef boost::unordered_map<int, double> scoremap;
  typedef boost::unordered_map<int, scoremap> feature2scoremap;
  feature2scoremap model;
};

}
