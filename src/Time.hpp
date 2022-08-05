#ifndef __H_TIME__
#define __H_TIME__

#include <ostream>
#include <iomanip>
#include <cmath>

#include "Constants.hpp"

[[nodiscard]] constexpr inline long constexpr_labs(long x)
{
	return x >= 0 ? x : -x;
}

class Time
{
public:
	explicit constexpr Time() : timeVal(0) {}
	explicit constexpr Time(int milliseconds, int seconds = 0) : timeVal (milliseconds + (seconds * 1000)) {}
	explicit constexpr Time(long milliseconds, long seconds = 0) : timeVal(milliseconds + (seconds * 1000)) {}
	explicit constexpr Time(double milliseconds, double seconds = 0.0) : timeVal(((long) milliseconds) + ((long) seconds * 1000)) {}
	constexpr Time(const Time& existingTime) = default;
	~Time() = default;

	constexpr Time operator + (const Time& rhs) const { return Time(timeVal + rhs.timeVal); }
	constexpr Time operator - (const Time& rhs) const { return Time(timeVal - rhs.timeVal); }
	constexpr Time operator * (const Time& rhs) const { return Time(timeVal * rhs.timeVal); }
	constexpr Time operator / (const Time& rhs) const { return Time(timeVal / rhs.timeVal); }

	constexpr Time& operator = (const Time& rhs) = default;
	constexpr Time& operator += (const Time& rhs) { timeVal = operator+(rhs).timeVal; return *this; }
	constexpr Time& operator -= (const Time& rhs) { timeVal = operator-(rhs).timeVal; return *this; }
	constexpr Time& operator *= (const Time& rhs) { timeVal = operator*(rhs).timeVal; return *this; }
	constexpr Time& operator /= (const Time& rhs) { timeVal = operator/(rhs).timeVal; return *this; }

	constexpr Time& operator ++ () { timeVal = timeVal + 1; return *this; }
	constexpr const Time operator ++ (int)& { const Time tmp = *this; operator++(); return tmp; }
	constexpr Time& operator -- () { timeVal = timeVal - 1; return *this; }
	constexpr const Time operator -- (int) { const Time tmp = *this; operator--(); return tmp; }

	friend constexpr bool operator == (const Time& lhs, const Time& rhs) { return lhs.timeVal == rhs.timeVal; }
	friend constexpr bool operator != (const Time& lhs, const Time& rhs) { return !(operator==(lhs, rhs)); }
	friend constexpr bool operator <  (const Time& lhs, const Time& rhs) { return lhs.timeVal < rhs.timeVal; }
	friend constexpr bool operator >  (const Time& lhs, const Time& rhs) { return operator<(rhs, lhs); }
	friend constexpr bool operator <= (const Time& lhs, const Time& rhs) { return !(operator>(lhs, rhs)); }
	friend constexpr bool operator >= (const Time& lhs, const Time& rhs) { return !(operator<(lhs, rhs)); }

	friend std::ostream& operator << (std::ostream& os, const Time& dur) {
		std::streamsize ss = os.precision(4);
		os << std::setprecision(4) << (double) dur.timeVal / 1000.0;
		os.precision(ss);
		return os;
	}

	constexpr static inline Time fromMilliseconds(int milliseconds) { return Time(milliseconds, true); }
	constexpr static inline Time difference(const Time& lhs, const Time& rhs) { return lhs - rhs; }
	constexpr static inline Time differenceAbs(const Time& lhs, const Time& rhs) { return Time(constexpr_labs((lhs - rhs).timeVal)); }

	[[nodiscard]] constexpr inline Time differenceTo(const Time& to) const { return Time::difference(to, *this); }
	[[nodiscard]] constexpr inline Time differenceFrom(const Time& from) const { return Time::difference(*this, from); }
	[[nodiscard]] constexpr inline Time differenceAbs(const Time& to) const { return Time::differenceAbs(*this, to); }

	[[nodiscard]] constexpr inline long getSeconds() const { return timeVal / 1000; }
	[[nodiscard]] constexpr inline long getMilliseconds() const { return timeVal; }
	[[nodiscard]] constexpr inline double getSecondsDouble() const { return (double) timeVal / 1000.0; }

private:
	long timeVal; /* Stored as milliseconds */
};


#endif
