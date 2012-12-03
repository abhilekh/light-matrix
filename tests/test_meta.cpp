/**
 * @file test_meta.cpp
 *
 * @brief Test meta programming constructs
 *
 * @author Dahua Lin
 */


#include "test_base.h"

#include <light_mat/common/meta_base.h>

using namespace lmat;

SIMPLE_CASE( meta_calc, logical )
{
	ASSERT_CT_VALUE( true_t, true );
	ASSERT_CT_VALUE( false_t, false );

	typedef meta::and_<true_t,  true_t>  and_tt;
	typedef meta::and_<true_t,  false_t> and_tf;
	typedef meta::and_<false_t, true_t>  and_ft;
	typedef meta::and_<false_t, false_t> and_ff;

	ASSERT_CT_VALUE( and_tt, true );
	ASSERT_CT_VALUE( and_tf, false );
	ASSERT_CT_VALUE( and_ft, false );
	ASSERT_CT_VALUE( and_ff, false );

	typedef meta::or_<true_t,  true_t>  or_tt;
	typedef meta::or_<true_t,  false_t> or_tf;
	typedef meta::or_<false_t, true_t>  or_ft;
	typedef meta::or_<false_t, false_t> or_ff;

	ASSERT_CT_VALUE( or_tt, true );
	ASSERT_CT_VALUE( or_tf, true );
	ASSERT_CT_VALUE( or_ft, true );
	ASSERT_CT_VALUE( or_ff, false );
}

SIMPLE_CASE( meta_calc, arith )
{
	typedef fix_int<2> i2;
	typedef fix_int<3> i3;

	typedef meta::add_<i2, i3> add_r;
	typedef meta::sub_<i2, i3> sub_r;
	typedef meta::mul_<i2, i3> mul_r;
	typedef meta::max_<i2, i3> max_r;
	typedef meta::min_<i2, i3> min_r;

	ASSERT_CT_VALUE( add_r, 5 );
	ASSERT_CT_VALUE( sub_r, -1 );
	ASSERT_CT_VALUE( mul_r, 6 );
	ASSERT_CT_VALUE( max_r, 3 );
	ASSERT_CT_VALUE( min_r, 2 );
}


SIMPLE_CASE( typelist, N1 )
{
	typedef LMAT_TYPELIST_1( int ) list_t;

	ASSERT_CT_VALUE ( meta::length_<list_t>, 1 );

	LMAT_GETTYPE( T0, list_t, 0 );
	ASSERT_CT_TYPE ( T0, int );
}

SIMPLE_CASE( typelist, N2 )
{
	typedef LMAT_TYPELIST_2( int, char ) list_t;

	ASSERT_CT_VALUE ( meta::length_<list_t>, 2 );

	LMAT_GETTYPE( T0, list_t, 0 );
	ASSERT_CT_TYPE ( T0, int );

	LMAT_GETTYPE( T1, list_t, 1 );
	ASSERT_CT_TYPE ( T1, char );
}

SIMPLE_CASE( typelist, N3 )
{
	typedef LMAT_TYPELIST_3( int, char, float ) list_t;

	ASSERT_CT_VALUE ( meta::length_<list_t>, 3 );

	LMAT_GETTYPE( T0, list_t, 0 )
	ASSERT_CT_TYPE ( T0, int );

	LMAT_GETTYPE( T1, list_t, 1 )
	ASSERT_CT_TYPE ( T1, char );

	LMAT_GETTYPE( T2, list_t, 2 );
	ASSERT_CT_TYPE ( T2, float );
}

SIMPLE_CASE( typelist, N4 )
{
	typedef LMAT_TYPELIST_4( int, char, float, double ) list_t;

	ASSERT_CT_VALUE ( meta::length_<list_t>, 4 );

	LMAT_GETTYPE( T0, list_t, 0 );
	ASSERT_CT_TYPE ( T0, int );

	LMAT_GETTYPE( T1, list_t, 1 );
	ASSERT_CT_TYPE ( T1, char );

	LMAT_GETTYPE( T2, list_t, 2 );
	ASSERT_CT_TYPE ( T2, float );

	LMAT_GETTYPE( T3, list_t, 3 );
	ASSERT_CT_TYPE ( T3, double );
}


SIMPLE_CASE( intlist, N1 )
{
	typedef LMAT_INTLIST_1( 10 ) ints;

	int V0 = meta::get_val_<ints, 0>::value;
	ASSERT_EQ( V0, 10 );
}

SIMPLE_CASE( intlist, N2 )
{
	typedef LMAT_INTLIST_2( 10, 20 ) ints;

	int V0 = meta::get_val_<ints, 0>::value;
	ASSERT_EQ( V0, 10 );

	int V1 = meta::get_val_<ints, 1>::value;
	ASSERT_EQ( V1, 20 );
}

SIMPLE_CASE( intlist, N3 )
{
	typedef LMAT_INTLIST_3( 10, 20, 30 ) ints;

	int V0 = meta::get_val_<ints, 0>::value;
	ASSERT_EQ( V0, 10 );

	int V1 = meta::get_val_<ints, 1>::value;
	ASSERT_EQ( V1, 20 );

	int V2 = meta::get_val_<ints, 2>::value;
	ASSERT_EQ( V2, 30 );
}

SIMPLE_CASE( intlist, N4 )
{
	typedef LMAT_INTLIST_4( 10, 20, 30, 40 ) ints;

	int V0 = meta::get_val_<ints, 0>::value;
	ASSERT_EQ( V0, 10 );

	int V1 = meta::get_val_<ints, 1>::value;
	ASSERT_EQ( V1, 20 );

	int V2 = meta::get_val_<ints, 2>::value;
	ASSERT_EQ( V2, 30 );

	int V3 = meta::get_val_<ints, 3>::value;
	ASSERT_EQ( V3, 40 );
}


SIMPLE_CASE( boollist, N1 )
{
	typedef LMAT_INTLIST_1( true ) bools;

	bool V0 = meta::get_val_<bools, 0>::value;
	ASSERT_EQ( V0, true );
}

SIMPLE_CASE( boollist, N2 )
{
	typedef LMAT_INTLIST_2( true, false ) bools;

	bool V0 = meta::get_val_<bools, 0>::value;
	ASSERT_EQ( V0, true );

	bool V1 = meta::get_val_<bools, 1>::value;
	ASSERT_EQ( V1, false );
}

SIMPLE_CASE( boollist, N3 )
{
	typedef LMAT_INTLIST_3( true, false, false ) bools;

	bool V0 = meta::get_val_<bools, 0>::value;
	ASSERT_EQ( V0, true );

	bool V1 = meta::get_val_<bools, 1>::value;
	ASSERT_EQ( V1, false );

	bool V2 = meta::get_val_<bools, 2>::value;
	ASSERT_EQ( V2, false );
}

SIMPLE_CASE( boollist, N4 )
{
	typedef LMAT_INTLIST_4( true, false, false, true ) bools;

	bool V0 = meta::get_val_<bools, 0>::value;
	ASSERT_EQ( V0, true );

	bool V1 = meta::get_val_<bools, 1>::value;
	ASSERT_EQ( V1, false );

	bool V2 = meta::get_val_<bools, 2>::value;
	ASSERT_EQ( V2, false );

	bool V3 = meta::get_val_<bools, 3>::value;
	ASSERT_EQ( V3, true );
}


template<typename T>
struct my_transform
{
	typedef type<T> type;
};


SIMPLE_CASE( type_transform, N1 )
{
	typedef LMAT_TYPELIST_1( int ) list_t;
	typedef typename meta::transform_<my_transform, list_t>::type rlist_t;

	ASSERT_CT_VALUE( meta::length_<rlist_t>, 1 );

	LMAT_GETTYPE( T0, rlist_t, 0 )
	ASSERT_CT_TYPE( T0, type<int> );
}

SIMPLE_CASE( type_transform, N2 )
{
	typedef LMAT_TYPELIST_2( int, char ) list_t;
	typedef typename meta::transform_<my_transform, list_t>::type rlist_t;

	ASSERT_CT_VALUE( meta::length_<rlist_t>, 2 );

	LMAT_GETTYPE( T0, rlist_t, 0 )
	ASSERT_CT_TYPE( T0, type<int> );

	LMAT_GETTYPE( T1, rlist_t, 1 )
	ASSERT_CT_TYPE( T1, type<char> );
}

SIMPLE_CASE( type_transform, N3 )
{
	typedef LMAT_TYPELIST_3( int, char, float ) list_t;
	typedef typename meta::transform_<my_transform, list_t>::type rlist_t;

	ASSERT_CT_VALUE( meta::length_<rlist_t>, 3 );

	LMAT_GETTYPE( T0, rlist_t, 0 )
	ASSERT_CT_TYPE( T0, type<int> );

	LMAT_GETTYPE( T1, rlist_t, 1 )
	ASSERT_CT_TYPE( T1, type<char> );

	LMAT_GETTYPE( T2, rlist_t, 2 )
	ASSERT_CT_TYPE( T2, type<float> );
}


SIMPLE_CASE( type_transform, N4 )
{
	typedef LMAT_TYPELIST_4( int, char, float, double ) list_t;
	typedef typename meta::transform_<my_transform, list_t>::type rlist_t;

	ASSERT_CT_VALUE( meta::length_<rlist_t>, 4 );

	LMAT_GETTYPE( T0, rlist_t, 0 )
	ASSERT_CT_TYPE( T0, type<int> );

	LMAT_GETTYPE( T1, rlist_t, 1 )
	ASSERT_CT_TYPE( T1, type<char> );

	LMAT_GETTYPE( T2, rlist_t, 2 )
	ASSERT_CT_TYPE( T2, type<float> );

	LMAT_GETTYPE( T3, rlist_t, 3 )
	ASSERT_CT_TYPE( T3, type<double> );
}


template<typename T>
struct my_size
{
	static const int value = (int)sizeof(T);
};


SIMPLE_CASE( type2int, N1 )
{
	typedef LMAT_TYPELIST_1( int ) list_t;
	typedef typename meta::types_to_ints_<my_size, list_t>::type rlist;

	ASSERT_CT_VALUE( meta::length_<rlist>, 1 );

	int V0 = meta::get_val_<rlist, 0>::value;
	ASSERT_EQ( V0, sizeof(int) );
}

SIMPLE_CASE( type2int, N2 )
{
	typedef LMAT_TYPELIST_2( int, char ) list_t;
	typedef typename meta::types_to_ints_<my_size, list_t>::type rlist;

	ASSERT_CT_VALUE( meta::length_<rlist>, 2 );

	int V0 = meta::get_val_<rlist, 0>::value;
	ASSERT_EQ( V0, sizeof(int) );

	int V1 = meta::get_val_<rlist, 1>::value;
	ASSERT_EQ( V1, sizeof(char) );
}

SIMPLE_CASE( type2int, N3 )
{
	typedef LMAT_TYPELIST_3( int, char, float ) list_t;
	typedef typename meta::types_to_ints_<my_size, list_t>::type rlist;

	ASSERT_CT_VALUE( meta::length_<rlist>, 3 );

	int V0 = meta::get_val_<rlist, 0>::value;
	ASSERT_EQ( V0, sizeof(int) );

	int V1 = meta::get_val_<rlist, 1>::value;
	ASSERT_EQ( V1, sizeof(char) );

	int V2 = meta::get_val_<rlist, 2>::value;
	ASSERT_EQ( V2, sizeof(float));
}

SIMPLE_CASE( type2int, N4 )
{
	typedef LMAT_TYPELIST_4( int, char, float, double ) list_t;
	typedef typename meta::types_to_ints_<my_size, list_t>::type rlist;

	ASSERT_CT_VALUE( meta::length_<rlist>, 4 );

	int V0 = meta::get_val_<rlist, 0>::value;
	ASSERT_EQ( V0, sizeof(int) );

	int V1 = meta::get_val_<rlist, 1>::value;
	ASSERT_EQ( V1, sizeof(char) );

	int V2 = meta::get_val_<rlist, 2>::value;
	ASSERT_EQ( V2, sizeof(float));

	int V3 = meta::get_val_<rlist, 3>::value;
	ASSERT_EQ( V3, sizeof(double));
}


SIMPLE_CASE( reduction, all )
{
	typedef LMAT_BOOLLIST_1( true ) L1;
	typedef LMAT_BOOLLIST_2( true, true ) L2;
	typedef LMAT_BOOLLIST_3( true, true, false ) L3;
	typedef LMAT_BOOLLIST_4( true, true, false, false ) L4;

	typedef meta::all_<L1> r1;
	ASSERT_CT_VALUE( r1, true );

	typedef meta::all_<L2> r2;
	ASSERT_CT_VALUE( r2, true );

	typedef meta::all_<L3> r3;
	ASSERT_CT_VALUE( r3, false );

	typedef meta::all_<L4> r4;
	ASSERT_CT_VALUE( r4, false );
}


SIMPLE_CASE( reduction, any )
{
	typedef LMAT_BOOLLIST_1( false ) L1;
	typedef LMAT_BOOLLIST_2( false, false ) L2;
	typedef LMAT_BOOLLIST_3( false, false, true ) L3;
	typedef LMAT_BOOLLIST_4( false, false, true, true ) L4;

	typedef meta::any_<L1> r1;
	ASSERT_CT_VALUE( r1, false );

	typedef meta::any_<L2> r2;
	ASSERT_CT_VALUE( r2, false );

	typedef meta::any_<L3> r3;
	ASSERT_CT_VALUE( r3, true );

	typedef meta::any_<L4> r4;
	ASSERT_CT_VALUE( r4, true );
}


SIMPLE_CASE( reduction, sum )
{
	typedef LMAT_INTLIST_1( 30 ) L1;
	typedef LMAT_INTLIST_2( 30, 20 ) L2;
	typedef LMAT_INTLIST_3( 30, 20, 40 ) L3;
	typedef LMAT_INTLIST_4( 30, 20, 40, 10 ) L4;

	typedef meta::sum_<L1> r1;
	ASSERT_CT_VALUE( r1, 30 );

	typedef meta::sum_<L2> r2;
	ASSERT_CT_VALUE( r2, 50 );

	typedef meta::sum_<L3> r3;
	ASSERT_CT_VALUE( r3, 90 );

	typedef meta::sum_<L4> r4;
	ASSERT_CT_VALUE( r4, 100 );
}

SIMPLE_CASE( reduction, prod )
{
	typedef LMAT_INTLIST_1( 3 ) L1;
	typedef LMAT_INTLIST_2( 3, 2 ) L2;
	typedef LMAT_INTLIST_3( 3, 2, 4 ) L3;
	typedef LMAT_INTLIST_4( 3, 2, 4, 5 ) L4;

	typedef meta::prod_<L1> r1;
	ASSERT_CT_VALUE( r1, 3 );

	typedef meta::prod_<L2> r2;
	ASSERT_CT_VALUE( r2, 6 );

	typedef meta::prod_<L3> r3;
	ASSERT_CT_VALUE( r3, 24 );

	typedef meta::prod_<L4> r4;
	ASSERT_CT_VALUE( r4, 120 );
}


SIMPLE_CASE( reduction, maximum )
{
	typedef LMAT_INTLIST_1( 30 ) L1;
	typedef LMAT_INTLIST_2( 30, 20 ) L2;
	typedef LMAT_INTLIST_3( 30, 20, 40 ) L3;
	typedef LMAT_INTLIST_4( 30, 20, 40, 10 ) L4;

	typedef meta::maximum_<L1> r1;
	ASSERT_CT_VALUE( r1, 30 );

	typedef meta::maximum_<L2> r2;
	ASSERT_CT_VALUE( r2, 30 );

	typedef meta::maximum_<L3> r3;
	ASSERT_CT_VALUE( r3, 40 );

	typedef meta::maximum_<L4> r4;
	ASSERT_CT_VALUE( r4, 40 );
}


SIMPLE_CASE( reduction, minimum )
{
	typedef LMAT_INTLIST_1( 30 ) L1;
	typedef LMAT_INTLIST_2( 30, 20 ) L2;
	typedef LMAT_INTLIST_3( 30, 20, 40 ) L3;
	typedef LMAT_INTLIST_4( 30, 20, 40, 10 ) L4;

	typedef meta::minimum_<L1> r1;
	ASSERT_CT_VALUE( r1, 30 );

	typedef meta::minimum_<L2> r2;
	ASSERT_CT_VALUE( r2, 20 );

	typedef meta::minimum_<L3> r3;
	ASSERT_CT_VALUE( r3, 20 );

	typedef meta::minimum_<L4> r4;
	ASSERT_CT_VALUE( r4, 10 );
}




BEGIN_TPACK( meta_calc )
	ADD_SIMPLE_CASE( meta_calc, logical )
	ADD_SIMPLE_CASE( meta_calc, arith )
END_TPACK

BEGIN_TPACK( typelists )
	ADD_SIMPLE_CASE( typelist, N1 )
	ADD_SIMPLE_CASE( typelist, N2 )
	ADD_SIMPLE_CASE( typelist, N3 )
	ADD_SIMPLE_CASE( typelist, N4 )
END_TPACK

BEGIN_TPACK( intlists )
	ADD_SIMPLE_CASE( intlist, N1 )
	ADD_SIMPLE_CASE( intlist, N2 )
	ADD_SIMPLE_CASE( intlist, N3 )
	ADD_SIMPLE_CASE( intlist, N4 )
END_TPACK

BEGIN_TPACK( boollists )
	ADD_SIMPLE_CASE( boollist, N1 )
	ADD_SIMPLE_CASE( boollist, N2 )
	ADD_SIMPLE_CASE( boollist, N3 )
	ADD_SIMPLE_CASE( boollist, N4 )
END_TPACK

BEGIN_TPACK( type_transforms )
	ADD_SIMPLE_CASE( type_transform, N1 )
	ADD_SIMPLE_CASE( type_transform, N2 )
	ADD_SIMPLE_CASE( type_transform, N3 )
	ADD_SIMPLE_CASE( type_transform, N4 )
END_TPACK

BEGIN_TPACK( type2ints )
	ADD_SIMPLE_CASE( type2int, N1 )
	ADD_SIMPLE_CASE( type2int, N2 )
	ADD_SIMPLE_CASE( type2int, N3 )
	ADD_SIMPLE_CASE( type2int, N4 )
END_TPACK

BEGIN_TPACK( reduction )
	ADD_SIMPLE_CASE( reduction, all )
	ADD_SIMPLE_CASE( reduction, any )
	ADD_SIMPLE_CASE( reduction, sum )
	ADD_SIMPLE_CASE( reduction, prod )
	ADD_SIMPLE_CASE( reduction, maximum )
	ADD_SIMPLE_CASE( reduction, minimum )
END_TPACK

BEGIN_MAIN_SUITE
	ADD_TPACK( meta_calc )
	ADD_TPACK( typelists )
	ADD_TPACK( intlists )
	ADD_TPACK( boollists )

	ADD_TPACK( type_transforms )
	ADD_TPACK( type2ints )
	ADD_TPACK( reduction )
END_MAIN_SUITE



