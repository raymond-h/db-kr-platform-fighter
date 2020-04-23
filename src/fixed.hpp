#ifndef __FIXED_HPP__
#define __FIXED_HPP__

#include <boost/operators.hpp>

int32_t fixed_mul_32(const int32_t &a, const int32_t &b);
int32_t fixed_div_32(const int32_t &a, const int32_t &b);

class Fixed : boost::totally_ordered<Fixed>, boost::arithmetic<Fixed>, boost::arithmetic<Fixed, int32_t>
{
public:
    static const int FRACTION_BITS = 8;
    static const int ONE_VALUE = 1 << FRACTION_BITS;

    constexpr Fixed() : _value(0) {}
    constexpr Fixed(int32_t v) : _value(v << FRACTION_BITS) {}
    constexpr Fixed(const Fixed &fixed) : _value(fixed._value) {}

    int32_t const raw() const { return _value; }
    int32_t const int_value() const { return _value >> FRACTION_BITS; }
    double const double_value() const { return (double)_value / (double)ONE_VALUE; }

    explicit operator int32_t() const { return int_value(); }

    constexpr static Fixed from_int(int32_t v) { return Fixed(v); }
    constexpr static Fixed from_raw(int32_t v)
    {
        Fixed f;
        f._value = v;
        return f;
    }
    constexpr static Fixed from_fraction(int32_t t, int32_t b)
    {
        return from_raw((int64_t)t * ONE_VALUE / b);
    }

    bool operator==(Fixed const &rhs) const { return _value == rhs._value; }
    bool operator<(Fixed const &rhs) const { return _value < rhs._value; }

    Fixed operator-() { return from_raw(-_value); }

    Fixed operator+=(Fixed const &rhs)
    {
        _value += rhs._value;
        return *this;
    }
    Fixed operator-=(Fixed const &rhs)
    {
        _value -= rhs._value;
        return *this;
    }
    Fixed operator*=(Fixed const &rhs)
    {
        _value = fixed_mul_32(_value, rhs._value);
        return *this;
    }
    Fixed operator/=(Fixed const &rhs)
    {
        _value = fixed_div_32(_value, rhs._value);
        return *this;
    }

    Fixed abs() const { return _value > 0 ? *this : from_raw(-_value); }

private:
    int32_t _value;
};

std::ostream &operator<<(std::ostream &os, Fixed const &value);

inline Fixed abs(const Fixed &f) { return f.abs(); }

inline int32_t fixed_mul_32(const int32_t &a, const int32_t &b)
{
    return ((int64_t)a * (int64_t)b) / Fixed::ONE_VALUE;
}

inline int32_t fixed_div_32(const int32_t &a, const int32_t &b)
{
    return ((int64_t)a * Fixed::ONE_VALUE) / b;
}

constexpr Fixed operator"" _f(const char *s, std::size_t len)
{
    Fixed acc = 0;
    size_t i = 0;
    int32_t negaFactor = (len > 0 && s[0] == '-') ? -1 : 1;
    for (; i < len; i++)
    {
        const char c = s[i];
        if (c >= '0' && c <= '9')
        {
            int32_t n = c - '0';
            acc = (acc * 10) + n;
        }
        else if (c == '.') break;
    }

    Fixed accFrac = 0;
    for (size_t iRev = len-1; iRev > i; iRev--)
    {
        const char c = s[iRev];
        if (c >= '0' && c <= '9')
        {
            int32_t n = c - '0';
            accFrac = (accFrac + n) / 10;
        }
    }

    return Fixed::from_raw(negaFactor * (acc.raw() + accFrac.raw()));
}

#endif // __FIXED_HPP__
