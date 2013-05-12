#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <exception>
#include <boost/chrono/chrono.hpp>
#include "dataset.hpp"
#include "perceptron.hpp"

using namespace std;
using namespace boost::chrono;

int main(int argcc, char** argv) {
  cout << "data load..." << flush;
  catwalk::dataset dataset("dataset/rcv1_test.multiclass");
  cout << " ok." << endl;
  const int train_percent = 75;
  const int test_percent = 25;
  catwalk::perceptron perc;

  for (;;) {
    steady_clock::time_point start
      = high_resolution_clock::now();
    for (size_t i = 0; i < ((train_percent + test_percent) / test_percent); ++i) {
      const size_t size = dataset.samples.size();

      const size_t train_size
        = size * train_percent / (train_percent + test_percent);
      const size_t test_size
        = size * test_percent / (train_percent + test_percent);
      const int offset = test_size * i;

      // train
      for (size_t j = offset; j < train_size + offset; ++j) {
        perc.train(dataset.samples[j % size]);
      }

      int correct = 0;
      int failed = 0;
      for (size_t j = train_size + offset;
           j < train_size + test_size + offset;
           ++j) {
        const int predicted = perc.classify(dataset.samples[j % size].features);
        if (predicted == dataset.samples[j % size].label) {
          ++correct;
        } else {
          ++failed;
        }
      }
      cout << ((double)correct * 100 / (correct + failed)) << "% " << flush;
    }
    steady_clock::time_point end
      = high_resolution_clock::now();
    duration<double> sec = end - start;
    cout << sec.count() << "sec" << endl;
  }
}
