/************************************************************************************************************************
**
** Copyright 2016, 2017 Daniel Nikpayuk
**
** This file is part of POCC.
**
** nik_debug is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
**
** nik_debug is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
** of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License along with nik_debug. If not, see
** <http://www.gnu.org/licenses/>.
**
************************************************************************************************************************/

typedef unsigned long size_type;

//

template<size_type t0, typename Filler = void>
struct Dispatch { };

template<typename Filler>
struct Dispatch<1, Filler>
{
	int count;

	Dispatch(int c) : count(c) { }
};

//

template<typename value_type>
value_type square(const Dispatch<0> & a, value_type x)
{
	return x*x;
}

template<typename value_type>
value_type square(Dispatch<1> & a, value_type x)
{
	++a.count;

	return x*x;
}

//

template<size_type p0, size_type p1, size_type p2, size_type p3, typename Filler = void>
struct Adverb { };

template<typename Filler>
struct Adverb<0, 0, 0, 0, Filler> { int count; };

//

template<size_type p0, size_type p1, size_type p2, size_type p3, typename value_type>
value_type cube(const Adverb<p0, p1, p2, p3> & a, value_type x)
{
	Dispatch<0> d;

	return x*square(d, x);
}

template<typename value_type>
value_type cube(Adverb<0, 0, 0, 0> & a, value_type x)
{
	Dispatch<1> d(a.count+1);

	value_type rtn = x*square(d, x);

	a.count = d.count;

	return rtn;
}



