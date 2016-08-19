/************************************************************************************************************************
**
** Copyright 2016 Daniel Nikpayuk
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


#include<math.h>

#include"template.h"


/***********************************************************************************************************************/


#define declare5(N1, N2, N3, N4, N5)											\
															\
template<>														\
size_type godel<N1,N2,N3,N4,N5>::encoding()										\
{															\
	return pow(2,N1) * pow(3,N2) * pow(5,N3) * pow(7,N4) * pow(11,N5);						\
}


/***********************************************************************************************************************/


#define declare4(N1, N2, N3, N4)											\
	declare5(N1, N2, N3, N4, 1)											\
	declare5(N1, N2, N3, N4, 2)											\
	declare5(N1, N2, N3, N4, 3)											\
	declare5(N1, N2, N3, N4, 4)											\
	declare5(N1, N2, N3, N4, 5)											\


/***********************************************************************************************************************/


#define declare3(N1, N2, N3)												\
	declare4(N1, N2, N3, 1)												\
	declare4(N1, N2, N3, 2)												\
	declare4(N1, N2, N3, 3)												\
	declare4(N1, N2, N3, 4)												\
	declare4(N1, N2, N3, 5)												\


/***********************************************************************************************************************/


#define declare2(N1, N2)												\
	declare3(N1, N2, 1)												\
	declare3(N1, N2, 2)												\
	declare3(N1, N2, 3)												\
	declare3(N1, N2, 4)												\
	declare3(N1, N2, 5)												\


/***********************************************************************************************************************/


#define declare1(N1)													\
	declare2(N1, 1)													\
	declare2(N1, 2)													\
	declare2(N1, 3)													\
	declare2(N1, 4)													\
	declare2(N1, 5)													\


/***********************************************************************************************************************/


#define declare()													\
	declare1(1)													\
	declare1(2)													\
	declare1(3)													\
	declare1(4)													\
	declare1(5)													\


/***********************************************************************************************************************/


declare()


