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

#include<iostream>

//#include"base.hpp"
//#include"dynamic.hpp"
//#include"static.hpp"
#include"template.hpp"

int main(int argc, char *argv[])
{
	#define DECLARE(p0, p1, p2, p3)											\
		Adverb<p0, p1, p2, p3> adv##p0##p1##p2##p3

	#define CUBE(p0, p1, p2, p3)											\
		cube(adv##p0##p1##p2##p3, 3)

	#define CALL4(p0, p1, p2, p3)											\
		DECLARE(p0,p1,p2,p3);											\
		CUBE(p0,p1,p2,p3);

	#define CALL3(p0, p1, p2)											\
		CALL4(p0,p1,p2,0);											\
		CALL4(p0,p1,p2,1);											\
		CALL4(p0,p1,p2,2);											\
		CALL4(p0,p1,p2,3);

	#define CALL2(p0, p1)												\
		CALL3(p0,p1,0);												\
		CALL3(p0,p1,1);												\
		CALL3(p0,p1,2);												\
		CALL3(p0,p1,3);

	#define CALL1(p0)												\
		CALL2(p0,0);												\
		CALL2(p0,1);												\
		CALL2(p0,2);												\
		CALL2(p0,3);

	#define CALL()													\
		CALL1(0);												\
		CALL1(1);												\
		CALL1(2);												\
		CALL1(3);

	CALL()

	return 0;
}

