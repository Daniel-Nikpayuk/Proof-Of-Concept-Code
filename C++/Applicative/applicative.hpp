/************************************************************************************************************************
**
** Copyright 2016, 2017 Daniel Nikpayuk
**
** This file is part of POCC.
**
** POCC is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
**
** POCC is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
** of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License along with POCC. If not, see
** <http://www.gnu.org/licenses/>.
**
************************************************************************************************************************/

using size_type = unsigned long long;

//

template<typename, typename...> struct apply;

template<typename function, typename first, typename... params>
struct apply<function, first, params...>
{
	struct partial
	{
		template<size_type... args>
		using lambda = typename function::template lambda<args..., first::value>;
	};

	static constexpr size_type value = apply<partial, params...>::value;
};

template<typename function>
struct apply<function> { static constexpr size_type value = function::template lambda<>::value; };

//

template<size_type v>
struct constant { static constexpr size_type value = v; };

//

struct add
{
	template<size_type...> struct lambda;

	template<size_type x, size_type y>
	struct lambda<x, y>
	{
		static constexpr size_type value = x + y;
	};
};

