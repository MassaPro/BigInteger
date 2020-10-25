//
// Created by Nikita Mastinen on 21.10.2020.
//

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

class BigInteger;

BigInteger operator+(const BigInteger& first_number, const BigInteger& second_number);

BigInteger operator*(const BigInteger& first_number, const BigInteger& second_number);

BigInteger operator/(const BigInteger& first_number, const BigInteger& second_number);

class BigInteger {
  static const int base = 100;
  static const int number_of_digits = 2;
  std::vector<int> integer = {};
  char sign = 0;

  void trim() {
    while(!integer.empty() && integer.back() == 0) {
      integer.pop_back();
    }
    if (integer.empty()) {
      sign = 0;
    }
  }

  void normalize() {
    for (size_t i = 0; i < integer.size(); ++i) {
      if (integer[i] >= base) {
        if (i + 1 == integer.size()) {
          integer.push_back(integer[i] / base);
        } else {
          integer[i + 1] += integer[i] / base;
        }
        integer[i] %= base;
      } else if (integer[i] < 0) {
        integer[i + 1] += integer[i] / base;
        integer[i] %= base;
        if (integer[i] != 0) {
          integer[i] += base;
          integer[i + 1]--;
        }
      }
    }
  }

  bool absolute_lower(const BigInteger& other) const {
    if (integer.size() == other.integer.size()) {
      for (size_t i = 0; i < integer.size(); i++) {
        if (integer[integer.size() - 1 - i] < other.integer[integer.size() - 1 - i]) {
          return true;
        } else if (integer[integer.size() - 1 - i] > other.integer[integer.size() - 1 - i]) {
          return false;
        }
      }
      return false;
    }
    return integer.size() < other.integer.size();
  }
public:

  BigInteger() = default;

  BigInteger(long long other) {
    if (other < 0) {
      sign = -1;
      other = -other;
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

  BigInteger& operator=(const BigInteger& other) = default;

  bool operator==(const BigInteger& other) const {
    return (sign == other.sign && integer == other.integer);
  }

  bool operator!=(const BigInteger& other) const {
    return !(*this == other);
  }

  bool operator<(const BigInteger& other) const {
    if (sign == other.sign) {
      if (integer.size() == other.integer.size()) {
        for (size_t i = 0; i < integer.size(); i++) {
          if (integer[integer.size() - 1 - i] < other.integer[integer.size() - 1 - i]) {
            return true;
          } else if (integer[integer.size() - 1 - i] > other.integer[integer.size() - 1 - i]) {
            return false;
          }
        }
        return false;
      }
      return integer.size() < other.integer.size();
    }
    return sign < other.sign;
  }

  bool operator>=(const BigInteger& other) const {
    return !(*this < other);
  }

  bool operator<=(const BigInteger& other) const {
    return (*this < other || *this == other);
  }

  bool operator>(const BigInteger& other) const {
    return !(*this <= other);
  }

  BigInteger operator-() const {
    BigInteger negation = *this;
    negation.sign *= -1;
    return negation;
  }

  BigInteger& operator+=(const BigInteger& other) {
    if (other.integer.size() > integer.size()) {
      integer.resize(other.integer.size(), 0);
    }
    if (sign == other.sign || sign == 0 || other.sign == 0) {
      if (sign == 0) {
        sign = other.sign;
      }
      for (size_t i = 0; i < other.integer.size(); i++) {
        integer[i] += other.integer[i];
      }
    } else if (absolute_lower(other)) {
      if (other.sign == -1) {
        sign = -1;
      } else {
        sign = 1;
      }
      for (size_t i = 0; i < other.integer.size(); i++) {
        integer[i] = other.integer[i] - integer[i];
      }
    } else {
      if (other.sign == -1) {
        sign = 1;
      } else {
        sign = -1;
      }
      for (size_t i = 0; i < other.integer.size(); i++) {
        integer[i] -= other.integer[i];
      }
    }
    normalize();
    trim();
    return *this;
  }

  BigInteger& operator++() {
      *this += 1;
      return *this;
  }

  BigInteger operator++(int) {
    const BigInteger copy = *this;
    *this += 1;
    return copy;
  }

  BigInteger& operator-=(const BigInteger& other) {
    if (other.integer.size() > integer.size()) {
      integer.resize(other.integer.size(), 0);
    }
    if (sign != other.sign || sign == 0 || other.sign == 0) {
      if (sign == 0) {
        sign = -other.sign;
      }
      for (size_t i = 0; i < other.integer.size(); i++) {
        integer[i] += other.integer[i];
      }
    } else if (absolute_lower(other)) {
      if (sign == -1) {
        sign = 1;
      } else {
        sign = -1;
      }
      for (size_t i = 0; i < other.integer.size(); i++) {
        integer[i] = other.integer[i] - integer[i];
      }
    } else {
      if (sign == 1) {
        sign = 1;
      } else {
        sign = -1;
      }
      for (size_t i = 0; i < other.integer.size(); i++) {
        integer[i] -= other.integer[i];
      }
    }
    normalize();
    trim();
    return *this;
  }

  BigInteger& operator*=(const BigInteger& other) {
    BigInteger copy = *this;
    integer.clear();
    integer.resize(copy.integer.size() + other.integer.size());
    sign = copy.sign * other.sign;
    for (size_t i = 0; i < copy.integer.size(); ++i) {
      for (size_t j = 0; j < other.integer.size(); ++j) {
        integer[i + j] += copy.integer[i] * other.integer[j];
      }
    }
    normalize();
    trim();
    return *this;
  }

  BigInteger& operator/=(BigInteger other) {
    if (other == 0) return *this;
    char new_sign = sign * other.sign;
    BigInteger copy = *this;
    copy.sign = ::abs(copy.sign);
    other.sign = ::abs(other.sign);
    *this = 0;
    while (other.absolute_lower(copy + 1)) {
      BigInteger store = other, factor = 1;
      while ((store * base).absolute_lower(copy)) {
        store *= base;
        factor *= base;
      }
      while (store.absolute_lower(copy + 1)) {
        copy -= store;
        *this += factor;
      }
    }
    sign = new_sign;
    normalize();
    trim();
    return *this;
  }

  BigInteger& operator%=(const BigInteger& other) {
    *this -= *this / other * other;
    return *this;
  }

  BigInteger abs() {
    BigInteger copy = *this;
    if (copy.sign < 0) {
      copy.sign = 1;
    }
    return copy;
  }
};

BigInteger operator*(const BigInteger& first_number, const BigInteger& second_number) {
  BigInteger result = first_number;
  result *= second_number;
  return result;
}

BigInteger operator+(const BigInteger& first_number, const BigInteger& second_number) {
  BigInteger result = first_number;
  result += second_number;
  return result;
}

BigInteger operator-(const BigInteger& first_number, const BigInteger& second_number) {
  BigInteger result = first_number;
  result -= second_number;
  return result;
}

BigInteger operator%(const BigInteger& first_number, const BigInteger& second_number) {
  BigInteger result = first_number;
  result %= second_number;
  return result;
}

BigInteger operator/(const BigInteger& first_number, const BigInteger& second_number) {
  BigInteger result = first_number;
  result /= second_number;
  return result;
}

BigInteger operator ""_bi(const unsigned long long other) {
  return BigInteger(static_cast<long long>(other));
}

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

BigInteger gcd(const BigInteger& x, const BigInteger& y) {
  return (y == 0 ? x : gcd(y, x % y));
}

class Rational {
  static const int base = 10;
  static const int number_of_digits = 1;
  BigInteger numerator = {};
  BigInteger denominator = {};

  void normalize() {
    BigInteger g = gcd(denominator, numerator).abs();
    numerator /= g;
    denominator /= g;
    if (denominator < 0) {
      denominator = -denominator;
      numerator = -numerator;
    }
  }
public:
  Rational(): numerator(0), denominator(1) {};

  Rational(const BigInteger& other): numerator(other), denominator(1) {};

  Rational(const long long other): numerator(other), denominator(1) {};

  Rational(const Rational& other):
      numerator(other.numerator),
      denominator(other.denominator) {};

  std::string toString() const {
    std::string output_numerator = numerator.toString();
    std::string output_denominator = denominator.toString();
    if (denominator != 1) {
      return output_numerator + "/" + output_denominator;
    } else {
      return output_numerator;
    }
  }

  Rational& operator=(const Rational& other) = default;

  bool operator==(const Rational& other) const {
    return numerator == other.numerator && denominator == other.denominator;
  }

  bool operator!=(const Rational& other) const {
    return !(*this == other);
  }

  bool operator<(const Rational& other) const {
    return numerator * other.denominator < denominator * other.numerator;
  }

  bool operator<=(const Rational& other) const {
    return numerator * other.denominator <= denominator * other.numerator;
  }

  bool operator>(const Rational& other) const {
    return numerator * other.denominator > denominator * other.numerator;
  }

  bool operator>=(const Rational& other) const {
    return numerator * other.denominator >= denominator * other.numerator;
  }

  Rational operator-() const {
    Rational negation = *this;
    negation.numerator = -negation.numerator;
    return negation;
  }

  Rational& operator+=(const Rational& other) {
    numerator = numerator * other.denominator + other.numerator * denominator;
    denominator = denominator * other.denominator;
    normalize();
    return *this;
  }

  Rational& operator-=(const Rational& other) {
    numerator = numerator * other.denominator - other.numerator * denominator;
    denominator = denominator * other.denominator;
    normalize();
    return *this;
  }

  Rational& operator*=(const Rational& other) {
    numerator = numerator * other.numerator;
    denominator = denominator * other.denominator;
    normalize();
    return *this;
  }

  Rational& operator/=(const Rational& other) {
    numerator = numerator * other.denominator;
    denominator = denominator * other.numerator;
    normalize();
    return *this;
  }

  std::string asDecimal(const size_t precision) const {
    Rational copy = *this;
    for (size_t i = 0; i < precision; i++) {
      copy *= 10;
    }
    std::string current = (copy.numerator / copy.denominator).toString();
    std::string result;
    int is_negative = 0;
    if (current[0] == '-') {
      result += '-';
      is_negative = 2;
    }
    while (result.size() + current.size() - is_negative <= precision) {
      result += '0';
    }
    for (size_t i = (is_negative == 2 ? 1 : 0); i < current.size(); i++) {
      result += current[i];
    }
    result.insert(result.begin() + result.size() - precision, '.');
    return result;
  }

  explicit operator double() {
    std::stringstream copy;
    copy << asDecimal(100);
    double result;
    copy >> result;
    return result;
  }
};

Rational operator*(const Rational& first_number, const Rational& second_number) {
  Rational result = first_number;
  result *= second_number;
  return result;
}

Rational operator+(const Rational& first_number, const Rational& second_number) {
  Rational result = first_number;
  result += second_number;
  return result;
}

Rational operator-(const Rational& first_number, const Rational& second_number) {
  Rational result = first_number;
  result -= second_number;
  return result;
}

Rational operator/(const Rational& first_number, const Rational& second_number) {
  Rational result = first_number;
  result /= second_number;
  return result;
}

std::ostream& operator<<(std::ostream& out, const Rational& output) {
  out << output.toString();
  return out;
}


