#include "./mhdr.h"

using namespace std;

namespace cisco::common {
const vector<byte> MHDR::serialize(const MHDR &mhdr) const {
    vector<byte> result{};

    return result;
}

void MHDR::deserialize(const vector<byte> &bytes) {}
} // namespace cisco::common