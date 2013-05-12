
#ifndef CATWALK_DATASET_HPP_
#define CATWALK_DATASET_HPP_
#include <vector>
#include <boost/lexical_cast.hpp>
namespace catwalk {
struct feature {
	int name;
	double data;
	feature(int n, double d) :name(n), data(d) {}
	feature(const std::string& datapair) {
		const std::size_t splitter = datapair.find_first_of(":");
		if (splitter == std::string::npos) {
			throw std::exception();
		}
    name = boost::lexical_cast<int>(datapair.substr(0, splitter));
    data = boost::lexical_cast<double>(datapair.substr(splitter+1, -1));
	}
	friend std::ostream& operator<<(std::ostream& os, const feature& rhs) {
		os << "[" << rhs.name << " -> " << rhs.data << "]" << std::flush;
		return os;
	}
};

struct sample {
	int label;
	typedef std::vector<feature> features_t;
	features_t features;
	sample(const std::string& line) {
		size_t idx = line.find_first_of(" ");
    label = boost::lexical_cast<int>(line.substr(0, idx));
		while(true) {
			const size_t next_idx = line.find(" ", idx + 1);
			if (next_idx == std::string::npos) { break; }
			features.push_back(feature(line.substr(idx + 1, next_idx - idx - 1)));
			idx = next_idx;
		}
	}
	friend std::ostream& operator<<(std::ostream& os, const sample& rhs) {
		os << "{" << rhs.label << " ";
		for (features_t::const_iterator it = rhs.features.begin();
				 it != rhs.features.end();
				 ++it) {
			os << *it << " ";
		}
		os << "}" << std::flush;
		return os;
	}
};

struct dataset {
	typedef std::vector<sample> samples_t;
	samples_t samples;
	dataset(const std::string& filename) {
    std::ifstream src(filename.data());
		while(true) {
      std::string line;
			getline(src, line);
      if (src.bad()) {
        // IO error
      } else if (!src.eof()) {
        samples.push_back(sample(line));
      } else {
        break;
      }
		}
	}
	friend std::ostream& operator<<(std::ostream& os, const dataset& rhs) {
		for (samples_t::const_iterator it = rhs.samples.begin();
				 it != rhs.samples.end();
				 ++it) {
			os << *it << std::endl;
		}
	}
};
}
#endif // CATWALK_DATASET_HPP_
