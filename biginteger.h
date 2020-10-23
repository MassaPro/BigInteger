//
// Created by Nikita Mastinen on 21.10.2020.
//

#include <string>
#include <vector>
#include <iostream>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;

class BigInteger {
  static const int base = 100;
  static const int number_of_digits = 2;
  std::vector<int> integer = {};
  short sign = 0;

  void trim() {
    while(!integer.empty() && integer.back() == 0) {
      integer.pop_back();
    }
    if (integer.empty()) {
      sign = 0;
    }
  }

public:
  BigInteger() = default;

  BigInteger(long long other) {
    if (other < 0) {
      sign = -1;
    } else if (other == 0) {
      sign = 0;
    } else {
      sign = 1;
    }
    while (other > 0) {
      integer.push_back(other % base);
      other /= base;
    }
  }

  BigInteger(const std::string& other) {
    long long store = 0;
    sign = 1;
    size_t last = other.size();
    for (size_t i = 0; i < other.size(); i++) {
      char character = other[other.size() - 1 - i];
      size_t is_signed = 0;
      if (character == '-') {
        sign = -1;
        is_signed = 1;
      }
      if (character == '+') {
        sign = 1;
        is_signed = 1;
      }
      if (other.size() - 1 - i == 0 || other.size() - i - 1 + number_of_digits == last) {
        integer.push_back(0);
        for (size_t j = (last > number_of_digits ? last - number_of_digits : is_signed); j < last; j++) {
          integer.back() = integer.back() * 10 + other[j] - '0';
        }
        last -= number_of_digits;
      }
    }
    integer.push_back(store);
    trim();
    if (integer.empty()) {
      sign = 0;
    }
  }

  BigInteger(const BigInteger& other) : integer(other.integer), sign(other.sign) {}

  std::string toString() const {
    if (sign == 0) {
      return "0";
    }
    std::string result = (sign == -1 ? "-" : "");
    for (size_t i = 0; i < integer.size(); i++) {
      if (i == 0) {
        result += std::to_string(integer[integer.size() - i - 1]);
      } else {
        int number_of_nulls =
            number_of_digits - static_cast<int>(std::to_string(integer[integer.size() - i - 1]).size());
        result += std::string(number_of_nulls, '0');
        result += std::to_string(integer[integer.size() - i - 1]);
      }
    }
    return result;
  }

  explicit operator bool() const {
    return sign != 0;
  }
};

std::istream& operator>>(std::istream& in, BigInteger& input) {
  std::string s;
  in >> s;
  input = s;
  return in;
}

std::ostream& operator<<(std::ostream& out, const BigInteger& output) {
  out << output.toString();
  return out;
}