/**
 * @file meta_base.h
 *
 * @brief Basic facilities for meta-programming.
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHTMAT_META_BASE_H_
#define LIGHTMAT_META_BASE_H_

#include <light_mat/config/config.h>
#include <light_mat/common/prim_types.h>
#include <light_mat/common/type_traits.h>

namespace lmat {  namespace meta {


	/********************************************
	 *
	 *  Compile-time evaluation
	 *
	 ********************************************/

	// logical operations

	template<typename CondT>
	struct not_
	{
		static const bool value = !CondT::value;
	};

	template<typename CondT1, typename CondT2>
	struct and_
	{
		static const bool value = CondT1::value && CondT2::value;
	};

	template<typename CondT1, typename CondT2>
	struct or_
	{
		static const bool value = CondT1::value || CondT2::value;
	};

	template<typename CondT1, typename CondT2>
	struct xor_
	{
		static const bool value =
				(CondT1::value && !CondT2::value) ||
				(CondT2::value && !CondT1::value);
	};


	// arithmetic calculations on integer

	template<typename ValT1, typename ValT2>
	struct add_
	{
		static const int value = ValT1::value + ValT2::value;
	};

	template<typename ValT1, typename ValT2>
	struct sub_
	{
		static const int value = ValT1::value - ValT2::value;
	};

	template<typename ValT1, typename ValT2>
	struct mul_
	{
		static const int value = ValT1::value * ValT2::value;
	};

	template<typename ValT1, typename ValT2>
	struct max_
	{
		static const int value = (ValT1::value > ValT2::value ? ValT1::value : ValT2::value);
	};

	template<typename ValT1, typename ValT2>
	struct min_
	{
		static const int value = (ValT1::value < ValT2::value ? ValT1::value : ValT2::value);
	};


	// comparison operations

	template<typename ValT1, typename ValT2>
	struct eq_
	{
		static const bool value = ValT1::value == ValT2::value;
	};

	template<typename ValT1, typename ValT2>
	struct ne_
	{
		static const bool value = ValT1::value != ValT2::value;
	};

	template<typename ValT1, typename ValT2>
	struct gt_
	{
		static const bool value = ValT1::value > ValT2::value;
	};

	template<typename ValT1, typename ValT2>
	struct ge_
	{
		static const bool value = ValT1::value >= ValT2::value;
	};

	template<typename ValT1, typename ValT2>
	struct lt_
	{
		static const bool value = ValT1::value < ValT2::value;
	};

	template<typename ValT1, typename ValT2>
	struct le_
	{
		static const bool value = ValT1::value <= ValT2::value;
	};


	/********************************************
	 *
	 *  Conditional statements
	 *
	 ********************************************/

	// meta if statement

	template<bool Cond, typename Ttrue, typename Tfalse>
	struct if_c;

	template<typename Ttrue, typename Tfalse>
	struct if_c<true, Ttrue, Tfalse>
	{
		typedef Ttrue type;
	};

	template<typename Ttrue, typename Tfalse>
	struct if_c<false, Ttrue, Tfalse>
	{
		typedef Tfalse type;
	};

	template<typename CondT, typename Ttrue, typename Tfalse>
	struct if_
	{
		typedef typename if_c<CondT::value, Ttrue, Tfalse>::type type;
	};

	// enable_if

	template<bool Cond, typename T>
	struct enable_if_c;

	template<typename T>
	struct enable_if_c<true, T>
	{
		typedef T type;
	};

	template<typename T>
	struct enable_if_c<false, T> { };

	template<typename CondT, typename T>
	struct enable_if
	{
		typedef typename enable_if_c<CondT::value, T>::type type;
	};

	template<bool Cond, int Val>
	struct enable_int_if_c;

	template<int Val>
	struct enable_int_if_c<true, Val>
	{
		static const int value = Val;
	};

	template<int Val>
	struct enable_int_if_c<false, Val> { };

	template<typename CondT, int Val>
	struct enable_int_if
	{
		static const int value = enable_int_if_c<CondT::value, Val>::value;
	};


	/********************************************
	 *
	 *  type list
	 *
	 ********************************************/

	template<typename Head, typename Tail=nil_t>
	struct type_list_
	{
		typedef Head head;
		typedef Tail tail;
	};

	// length (can be applied to both type list and integer list)

	template<class TList>
	struct length_
	{
		static const int value = 1 + length_<typename TList::tail>::value;
	};

	template<>
	struct length_<nil_t>
	{
		static const int value = 0;
	};

	// element retrieval

	template<class TList, int N>
	struct get_
	{
		typedef typename get_<typename TList::tail, N-1>::type type;
	};

	template<class TList>
	struct get_<TList, 0>
	{
		typedef typename TList::head type;
	};

	// list transformation

	template<template<typename X> class TFun, class TList>
	struct transform_
	{
		typedef type_list_<
				typename TFun<typename TList::head>::type,
				typename transform_<TFun, typename TList::tail>::type > type;
	};

	template<template<typename X> class TFun>
	struct transform_<TFun, nil_t>
	{
		typedef nil_t type;
	};


	/********************************************
	 *
	 *  value list
	 *
	 ********************************************/

	template<int V, typename Tail=nil_t>
	struct int_list_
	{
		typedef int value_type;
		static const int head = V;
		typedef Tail tail;
	};

	template<bool V, typename Tail=nil_t>
	struct bool_list_
	{
		typedef bool value_type;
		static const int head = V;
		typedef Tail tail;
	};

	template<class VList, int N>
	struct get_val_
	{
		static const typename VList::value_type value =
				get_val_<typename VList::tail, N-1>::value;
	};

	template<class VList>
	struct get_val_<VList, 0>
	{
		static const typename VList::value_type value = VList::head;
	};

	template<template<typename X> class TFun, class TList>
	struct types_to_bools_
	{
		typedef bool_list_<
				TFun<typename TList::head>::value,
				typename types_to_bools_<TFun, typename TList::tail>::type> type;
	};

	template<template<typename X> class TFun>
	struct types_to_bools_<TFun, nil_t>
	{
		typedef nil_t type;
	};

	template<template<typename X> class TFun, class TList>
	struct types_to_ints_
	{
		typedef int_list_<
				TFun<typename TList::head>::value,
				typename types_to_ints_<TFun, typename TList::tail>::type> type;
	};

	template<template<typename X> class TFun>
	struct types_to_ints_<TFun, nil_t>
	{
		typedef nil_t type;
	};

	template<template<typename X> class TFun, class VList>
	struct ints_to_bools_
	{
		typedef bool_list_<
				TFun<typename VList::head>::value,
				typename ints_to_bools_<TFun, typename VList::tail>::type> type;
	};

	template<template<typename X> class TFun>
	struct ints_to_bools_<TFun, nil_t>
	{
		typedef nil_t type;
	};


	/********************************************
	 *
	 *  reduction
	 *
	 ********************************************/

	template<class VList>
	struct all_
	{
		static const bool value = VList::head && all_<typename VList::tail>::value;
	};

	template<>
	struct all_<nil_t>
	{
		static const bool value = true;
	};

	template<class VList>
	struct any_
	{
		static const bool value = VList::head || any_<typename VList::tail>::value;
	};

	template<>
	struct any_<nil_t>
	{
		static const bool value = false;
	};

	template<class VList>
	struct sum_
	{
		static const int value = VList::head + sum_<typename VList::tail>::value;
	};

	template<>
	struct sum_<nil_t>
	{
		static const int value = 0;
	};

	template<class VList>
	struct prod_
	{
		static const int value = VList::head * prod_<typename VList::tail>::value;
	};

	template<>
	struct prod_<nil_t>
	{
		static const int value = 1;
	};

	template<class VList>
	struct maximum_
	{
		static const int tail_v = maximum_<typename VList::tail>::value;
		static const int value = VList::head > tail_v ? VList::head : tail_v;
	};

	template<int H>
	struct maximum_<int_list_<H, nil_t> >
	{
		static const int value = H;
	};

	template<class VList>
	struct minimum_
	{
		static const int tail_v = minimum_<typename VList::tail>::value;
		static const int value = VList::head < tail_v ? VList::head : tail_v;
	};

	template<int H>
	struct minimum_<int_list_<H, nil_t> >
	{
		static const int value = H;
	};


} } // lmat::meta


/********************************************
 *
 *  Conditional macro constructs
 *
 ********************************************/

#define LMAT_CONDTYPE_2( CondT1, T1, TOther ) typename meta::enable_if< CondT1, T1, TOther >::type

#define LMAT_CONDTYPE_3( CondT1, T1, CondT2, T2, TOther ) \
	typename meta::enable_if< CondT1, T1, \
	typename meta::enable_if< CondT2, T2, TOther >::type >::type

#define LMAT_CONDTYPE_4( CondT1, T1, CondT2, T2, CondT3, T3, TOther ) \
	typename meta::enable_if< CondT1, T1, \
	typename meta::enable_if< CondT2, T2, \
	typename meta::enable_if< CondT3, T3, TOther >::type >::type >::type


/********************************************
 *
 *  List constructs
 *
 ********************************************/

#define LMAT_TYPELIST_1( T1 ) meta::type_list_<T1>
#define LMAT_TYPELIST_2( T1, T2 ) meta::type_list_<T1, meta::type_list_<T2> >
#define LMAT_TYPELIST_3( T1, T2, T3 ) meta::type_list_<T1, meta::type_list_<T2, meta::type_list_<T3> > >
#define LMAT_TYPELIST_4( T1, T2, T3, T4 ) meta::type_list_<T1, meta::type_list_<T2, meta::type_list_<T3, meta::type_list_<T4> > > >

#define LMAT_INTLIST_1( V1 ) meta::int_list_<V1>
#define LMAT_INTLIST_2( V1, V2 ) meta::int_list_<V1, meta::int_list_<V2> >
#define LMAT_INTLIST_3( V1, V2, V3 ) meta::int_list_<V1, meta::int_list_<V2, meta::int_list_<V3> > >
#define LMAT_INTLIST_4( V1, V2, V3, V4 ) meta::int_list_<V1, meta::int_list_<V2, meta::int_list_<V3, meta::int_list_<V4> > > >

#define LMAT_BOOLLIST_1( V1 ) meta::bool_list_<V1>
#define LMAT_BOOLLIST_2( V1, V2 ) meta::bool_list_<V1, meta::bool_list_<V2> >
#define LMAT_BOOLLIST_3( V1, V2, V3 ) meta::bool_list_<V1, meta::bool_list_<V2, meta::bool_list_<V3> > >
#define LMAT_BOOLLIST_4( V1, V2, V3, V4 ) meta::bool_list_<V1, meta::bool_list_<V2, meta::bool_list_<V3, meta::bool_list_<V4> > > >

// get types

#define LMAT_GETTYPE( Name, Lst, I ) typedef typename meta::get_<Lst, I>::type Name;

#endif


