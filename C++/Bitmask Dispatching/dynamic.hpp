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

template<bool cond, typename if_true, typename if_false>
struct if_then_else
{
	using type = if_false;
};

template<typename if_true, typename if_false>
struct if_then_else<true, if_true, if_false>
{
	using type = if_true;
};

struct Assoc
{
	enum : size_type
	{
		omit_count	= 1<<0,
		apply_count	= 1<<1,

		omit_fun	= 1<<2,
		apply_fun	= 1<<3
	};
};

template<size_type i, typename Filler = void>
struct Dispatch { };

template<typename Filler>
struct Dispatch<Assoc::apply_count, Filler> { int count; };

template<size_type index>
struct Adverb
{
	template<size_type x, size_type X>
	struct is_member
	{
		static constexpr bool value = (x & X == x);
	};

	using Verb = typename if_then_else
	<
		is_member<Assoc::apply_count, index>::value,
		Dispatch<Assoc::apply_count>,
		Dispatch<Assoc::omit_count>

	>::type;

	Verb verb;
};

//

template<typename value_type>
value_type square(const Dispatch<Assoc::omit_count> & a, value_type x)
{
	return x*x;
}

template<typename value_type>
value_type square(Dispatch<Assoc::apply_count> & a, value_type x)
{
	++a.count;

	return x*x;
}

//

template<size_type index>
struct omit
{
	template<typename value_type>
	static value_type cube(const Adverb<index> & a, value_type x)
	{
		return x*square(a.verb, x);
	}
};

template<size_type index>
struct apply
{
	template<typename value_type>
	static value_type cube(Adverb<index> & a, value_type x)
	{
		++a.verb.count;

		return x*square(a.verb, x);
	}
};


template<size_type index>
using Cube = typename if_then_else
<
	Adverb<index>::template is_member<Assoc::apply_count, index>::value,
	apply<index>,
	omit<index>

>::type;


template<size_type index, typename value_type>
value_type cube(Adverb<index> & a, value_type x)
{
	return Cube<index>::cube(a, x);
}


