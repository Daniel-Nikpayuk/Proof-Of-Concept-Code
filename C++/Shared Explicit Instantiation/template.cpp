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


#include"template.h"


/***********************************************************************************************************************/


#define declare5(N0, N1, N2, N3, N4, N5)										\
															\
template<>														\
size_type sum<N0,N1,N2,N3,N4,N5>::function(size_type x)									\
{															\
	return (N0+N1+N2+N3+N4+N5)*x;											\
}


/***********************************************************************************************************************/


#define declare4(N0, N1, N2, N3, N4)											\
	declare5(N0, N1, N2, N3, N4, 0)											\
	declare5(N0, N1, N2, N3, N4, 1)											\
	declare5(N0, N1, N2, N3, N4, 2)											\
	declare5(N0, N1, N2, N3, N4, 3)											\
	declare5(N0, N1, N2, N3, N4, 4)											\
	declare5(N0, N1, N2, N3, N4, 5)											\


/***********************************************************************************************************************/


#define declare3(N0, N1, N2, N3)											\
	declare4(N0, N1, N2, N3, 0)											\
	declare4(N0, N1, N2, N3, 1)											\
	declare4(N0, N1, N2, N3, 2)											\
	declare4(N0, N1, N2, N3, 3)											\
	declare4(N0, N1, N2, N3, 4)											\
	declare4(N0, N1, N2, N3, 5)											\


/***********************************************************************************************************************/


#define declare2(N0, N1, N2)												\
	declare3(N0, N1, N2, 0)												\
	declare3(N0, N1, N2, 1)												\
	declare3(N0, N1, N2, 2)												\
	declare3(N0, N1, N2, 3)												\
	declare3(N0, N1, N2, 4)												\
	declare3(N0, N1, N2, 5)												\


/***********************************************************************************************************************/


#define declare1(N0, N1)												\
	declare2(N0, N1, 0)												\
	declare2(N0, N1, 1)												\
	declare2(N0, N1, 2)												\
	declare2(N0, N1, 3)												\
	declare2(N0, N1, 4)												\
	declare2(N0, N1, 5)												\


/***********************************************************************************************************************/


#define declare0(N0)													\
	declare1(N0, 0)													\
	declare1(N0, 1)													\
	declare1(N0, 2)													\
	declare1(N0, 3)													\
	declare1(N0, 4)													\
	declare1(N0, 5)													\


/***********************************************************************************************************************/


#define declare()													\
	declare0(0)													\
	declare0(1)													\
	declare0(2)													\
	declare0(3)													\
	declare0(4)													\
	declare0(5)													\


/***********************************************************************************************************************/


declare()


