#include "./mhdr.h"
#include <vector>

using namespace std;

namespace cisco::common {
const vector<byte> MHDR::serialize() const {
  vector<byte> result{};

  return result;
}

const MHDR MHDR::deserialize(const vector<byte> &bytes) const {}
} // namespace cisco::common