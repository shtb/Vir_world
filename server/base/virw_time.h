#pragma once

#include"../../pub/STDHF.h"

using namespace std;

class virw_time
{
	friend ostream &operator<<(ostream & out, const virw_time& time)
	{
		out << time.big_t << " " << time.small_t;
		return out;
	}
	friend istream &operator>>(istream&in, virw_time& time)
	{
		in >> time.big_t >> time.small_t;
		return in;
	}

public:
	int small_t = 80000;
	int big_t = 0;

	string font_out()
	{
		stringstream out;
		out << big_t << ":" << small_t / 10800 << "-" << (small_t % 10800) / 180 << "-" << (small_t % 180) / 3.0 << endl;
		return out.str();
	}
	void time_go();
	int hour()
	{
		return small_t / 10800;
	}
};
