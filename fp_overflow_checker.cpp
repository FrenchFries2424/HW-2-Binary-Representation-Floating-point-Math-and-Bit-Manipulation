// fp_overflow_checker.cpp
// CS3339 Spring 2026 - HW2

#include <bitset>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

//Wrapper around a 32-bit float that exposes IEEE 754 fields and bit manipulation
class FloatBits {
 public:
  //Construct from a float value using memcpy to safely access its bits
  explicit FloatBits(float value) {
    memcpy(&bits_, &value, sizeof(bits_));
  }

  //Construct directly from a raw bit pattern
  static FloatBits FromRaw(uint32_t raw) {
    FloatBits fb(0.0f);
    fb.bits_ = raw;
    return fb;
  }

  //Convert stored bits back to a float
  float ToFloat() const {
    float value;
    memcpy(&value, &bits_, sizeof(value));
    return value;
  }

  //Extract the sign, exponent, and mantissa fields from the bit pattern
  uint32_t Sign() const {
    return (bits_ >> 31) & 0x1;
  }
  uint32_t Exponent() const {
    return (bits_ >> 23) & 0xFF;
  }
  uint32_t Mantissa() const {
    return bits_ & 0x7FFFFF;
  }
  uint32_t Raw() const {
    return bits_;
  }

  //Return a formatted IEEE bit string: "S EEEEEEEE MMMMMMMMMMMMMMMMMMMMMMM"
  string BitString() const {
    bitset<32> bs(bits_);
    string full = bs.to_string();
    return full.substr(0, 1) + " " + full.substr(1, 8) + " " + full.substr(9, 23);
  }

 private:
  uint32_t bits_;
};

//Print correct usage when wrong number of arguments are given
void PrintUsage(const char* prog_name) {
  cout << "usage:\n";
  cout << "\t" << prog_name << " loop_bound loop_counter\n\n";
  cout << "\tloop_bound is a positive floating-point value\n";
  cout << "\tloop_counter is a positive floating-point value\n";
}

//Find the smallest value where adding the counter becomes a no-op
//Uses the counter's biased exponent: threshold exponent = E_c + 24
FloatBits FindOverflowThreshold(const FloatBits& counter) {
  uint32_t E_thresh = counter.Exponent() + 24;
  uint32_t raw_thresh = (E_thresh & 0xFF) << 23;
  return FloatBits::FromRaw(raw_thresh);
}

//Overflow occurs if the loop bound can reach the threshold value
//Raw bit comparison is valid for positive floats since IEEE 754 is ordered
bool OverflowWillOccur(const FloatBits& bound, const FloatBits& threshold) {
  return bound.Raw() >= threshold.Raw();
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    PrintUsage(argv[0]);
    return 1;
  }

  float loop_bound   = atof(argv[1]);
  float loop_counter = atof(argv[2]);

  FloatBits fb_bound(loop_bound);
  FloatBits fb_counter(loop_counter);

  //Print IEEE bit representations of both inputs
  cout << "Loop bound:   " << fb_bound.BitString()   << "\n";
  cout << "Loop counter: " << fb_counter.BitString() << "\n";
  cout << "\n";

  //Check for overflow and print result
  FloatBits threshold = FindOverflowThreshold(fb_counter);

  if (!OverflowWillOccur(fb_bound, threshold)) {
    cout << "There is no overflow!\n";
  } else {
    cout << "Warning: Possible overflow!\n";
    cout << "Overflow threshold:\n";
    cout << "\t" << threshold.ToFloat() << "\n";
    cout << "\t" << threshold.BitString() << "\n";
  }

  return 0;
}
