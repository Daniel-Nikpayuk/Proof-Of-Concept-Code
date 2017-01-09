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

template<size_type p0, typename Adverb>
struct BaseAdverb { };

template<typename Adverb>
struct BaseAdverb<1, Adverb>
{
	void increment() { ++static_cast<Adverb*>(this)->count; }
};

//

template<typename Adverb, typename value_type>
value_type square(const BaseAdverb<0, Adverb> & a, value_type x)
{
	return x*x;
}

template<typename Adverb, typename value_type>
value_type square(BaseAdverb<1, Adverb> & a, value_type x)
{
	a.increment();

	return x*x;
}

//

template<size_type p0, size_type p1, size_type p2, size_type p3, typename Filler = void>
struct Adverb : public BaseAdverb<0, Adverb<p0, p1, p2, p3>> { };

template<typename Filler>
struct Adverb<0, 0, 0, 0, Filler> : public BaseAdverb<1, Adverb<0, 0, 0, 0, Filler>> { int count; };

//

template<size_type p0, size_type p1, size_type p2, size_type p3, typename value_type>
value_type cube(const Adverb<p0, p1, p2, p3> & a, value_type x)
{
	return x*square(a, x);
}

template<typename value_type>
value_type cube(Adverb<0, 0, 0, 0> & a, value_type x)
{
	++a.count;

	return x*square(a, x);
}



