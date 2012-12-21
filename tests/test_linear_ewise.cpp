/**
 * @file test_linear_ewise.cpp
 *
 * @brief Test Linear element-wise accesses
 *
 * @author Dahua Lin
 */


#include "test_base.h"

#define DEFAULT_M_VALUE 13
#define DEFAULT_N_VALUE 9

#include "multimat_supp.h"

#include <light_mat/matrix/matrix_classes.h>
#include <light_mat/math/math_functors.h>
#include <light_mat/mateval/ewise_eval.h>

#include <light_mat/math/sse_ops.h>
#ifdef LMAT_HAS_AVX
#include <light_mat/math/avx_ops.h>
#endif


using namespace lmat;
using namespace lmat::test;

// core functions

template<typename U, int M, int N>
void test_linear_ewise_cont_cont()
{
	const index_t m = M == 0 ? DM : M;
	const index_t n = N == 0 ? DN : N;

	typedef typename mat_host<cont, double, M, N>::cmat_t smat_t;
	typedef typename mat_host<cont, double, M, N>::mat_t dmat_t;

	mat_host<cont, double, M, N> src(m, n);
	src.fill_lin();
	mat_host<cont, double, M, N> dst(m, n);

	dense_matrix<double, M, N> rmat(m, n);

	smat_t smat = src.get_cmat();
	dmat_t dmat = dst.get_mat();

	copy_kernel<double> cpy_kernel;
	accum_kernel<double> upd_kernel;

	ewise(cpy_kernel, U())(smat.shape(), in_(smat), out_(dmat));

	ASSERT_MAT_EQ(m, n, smat, dmat);

	for (index_t i = 0; i < m * n; ++i) rmat[i] = smat[i] + dmat[i];

	ewise(upd_kernel, U())(smat.shape(), in_out_(dmat), in_(smat));

	ASSERT_MAT_EQ(m, n, dmat, rmat);
}

template<typename U, typename STag, typename DTag, int M>
void test_linear_ewise_col()
{
	const index_t m = M == 0 ? DM : M;

	typedef typename mat_host<STag, double, M, 1>::cmat_t smat_t;
	typedef typename mat_host<DTag, double, M, 1>::mat_t dmat_t;

	mat_host<STag, double, M, 1> src(m, 1);
	src.fill_lin();
	mat_host<DTag, double, M, 1> dst(m, 1);

	dense_matrix<double, M, 1> rmat(m, 1);

	smat_t smat = src.get_cmat();
	dmat_t dmat = dst.get_mat();

	copy_kernel<double> cpy_kernel;
	accum_kernel<double> upd_kernel;

	ewise(cpy_kernel, U())(m, in_(smat), out_(dmat));

	ASSERT_MAT_EQ(m, 1, smat, dmat);

	for (index_t i = 0; i < m; ++i) rmat[i] = smat[i] + dmat[i];

	ewise(upd_kernel, U())(m, in_out_(dmat), in_(smat));
	ASSERT_MAT_EQ(m, 1, dmat, rmat);
}

template<typename U, typename STag, typename DTag, int N>
void test_linear_ewise_row()
{
	const index_t n = N == 0 ? DN : N;

	typedef typename mat_host<STag, double, 1, N>::cmat_t smat_t;
	typedef typename mat_host<DTag, double, 1, N>::mat_t dmat_t;

	mat_host<STag, double, 1, N> src(1, n);
	src.fill_lin();
	mat_host<DTag, double, 1, N> dst(1, n);

	dense_matrix<double, 1, N> rmat(1, n);

	smat_t smat = src.get_cmat();
	dmat_t dmat = dst.get_mat();

	copy_kernel<double> cpy_kernel;
	accum_kernel<double> upd_kernel;

	ewise(cpy_kernel, U())(n, in_(smat), out_(dmat));

	ASSERT_MAT_EQ(1, n, smat, dmat);

	for (index_t i = 0; i < n; ++i) rmat[i] = smat[i] + dmat[i];

	ewise(upd_kernel, U())(n, in_out_(dmat), in_(smat));
	ASSERT_MAT_EQ(1, n, dmat, rmat);
}


template<typename U, int M, int N>
void test_linear_ewise_single_cont()
{
	const index_t m = M == 0 ? DM : M;
	const index_t n = N == 0 ? DN : N;

	typedef typename mat_host<cont, double, M, N>::mat_t dmat_t;

	double v = 2.56;
	mat_host<cont, double, M, N> dst(m, n);

	dmat_t dmat = dst.get_mat();

	dense_matrix<double, M, N> rmat(m, n);
	fill(rmat, v);

	dimension<M * N> dim(m * n);

	ewise(copy_kernel<double>(), U())(dim, in_(v, atags::single()), out_(dmat));

	ASSERT_MAT_EQ(m, n, dmat, rmat);
}


template<typename U, int M, int N>
void test_linear_ewise_map()
{
	const index_t m = M == 0 ? DM : M;
	const index_t n = N == 0 ? DN : N;

	dense_matrix<double, M, N> sa(m, n);
	dense_matrix<double, M, N> sb(m, n);
	dense_matrix<double, M, N> dst(m, n, zero());
	dense_matrix<double, M, N> r1(m, n);
	dense_matrix<double, M, N> r2(m, n);

	for (index_t i = 0; i < m * n; ++i)
	{
		sa[i] = double(i + 2);
		sb[i] = double(i * i + 1);

		r1[i] = math::sqr(sa[i]);
		r2[i] = sa[i] + sb[i];
	}

	matrix_shape<M, N> shape(m, n);

	map(math::sqr_fun<double>(), U())(shape, out_(dst), in_(sa));
	ASSERT_MAT_EQ( m, n, dst, r1 );

	map(math::add_fun<double>(), U())(shape, out_(dst), in_(sa), in_(sb));
	ASSERT_MAT_EQ( m, n, dst, r2 );
}



// Specific test cases


MN_CASE( linear_ewise, scalar_cont_cont  )
{
	test_linear_ewise_cont_cont<atags::scalar, M, N>();
}

N_CASE( linear_ewise, scalar_cont_stepcol  )
{
	test_linear_ewise_col<atags::scalar, cont, grid, N>();
}

N_CASE( linear_ewise, scalar_stepcol_cont  )
{
	test_linear_ewise_col<atags::scalar, grid, cont, N>();
}

N_CASE( linear_ewise, scalar_stepcol_stepcol  )
{
	test_linear_ewise_col<atags::scalar, grid, grid, N>();
}

N_CASE( linear_ewise, scalar_cont_steprow  )
{
	test_linear_ewise_row<atags::scalar, cont, bloc, N>();
}

N_CASE( linear_ewise, scalar_steprow_cont  )
{
	test_linear_ewise_row<atags::scalar, bloc, cont, N>();
}

N_CASE( linear_ewise, scalar_steprow_steprow  )
{
	test_linear_ewise_row<atags::scalar, bloc, bloc, N>();
}


MN_CASE( linear_ewise, sse_cont_cont  )
{
	test_linear_ewise_cont_cont<atags::simd<sse_t>, M, N>();
}

#ifdef LMAT_HAS_AVX

MN_CASE( linear_ewise, avx_cont_cont  )
{
	test_linear_ewise_cont_cont<atags::simd<avx_t>, M, N>();
}

#endif


MN_CASE( linear_ewise, scalar_single_cont )
{
	test_linear_ewise_single_cont<atags::scalar, M, N>();
}

MN_CASE( linear_ewise, sse_single_cont )
{
	test_linear_ewise_single_cont<atags::simd<sse_t>, M, N>();
}

#ifdef LMAT_HAS_AVX

MN_CASE( linear_ewise, avx_single_cont )
{
	test_linear_ewise_single_cont<atags::simd<avx_t>, M, N>();
}

#endif


MN_CASE( linear_ewise, scalar_map )
{
	test_linear_ewise_map<atags::scalar, M, N>();
}

MN_CASE( linear_ewise, sse_map )
{
	test_linear_ewise_map<atags::simd<sse_t>, M, N>();
}

#ifdef LMAT_HAS_AVX

MN_CASE( linear_ewise, avx_map )
{
	test_linear_ewise_map<atags::simd<avx_t>, M, N>();
}

#endif


MN_CASE( linear_ewise, map_to )
{
	const index_t m = M == 0 ? DM : M;
	const index_t n = N == 0 ? DN : N;

	dense_matrix<double, M, N> a(m, n);
	dense_matrix<double, M, N> s1(m, n);
	dense_matrix<double, M, N> s2(m, n);

	do_fill_rand( s1.ptr_data(), m * n );
	do_fill_rand( s2.ptr_data(), m * n );

	matrix_shape<M, N> shape(m, n);
	dense_matrix<double, M, N> r(m, n);

	for (index_t i = 0; i < m * n; ++i) r[i] = s1[i] + s2[i];

	map_to(a, math::add_fun<double>(), in_(s1), in_(s2));

	ASSERT_MAT_EQ(m, n, a, r);
}


MN_CASE( linear_ewise, accum_to )
{
	const index_t m = M == 0 ? DM : M;
	const index_t n = N == 0 ? DN : N;

	dense_matrix<double, M, N> a(m, n);
	dense_matrix<double, M, N> s(m, n);
	dense_matrix<double, M, N> c(m, n);

	do_fill_rand( a.ptr_data(), m * n );
	do_fill_rand( s.ptr_data(), m * n );
	do_fill_rand( c.ptr_data(), m * n );

	double cv = c[0] * 2.0;

	matrix_shape<M, N> shape(m, n);
	dense_matrix<double, M, N> r(m, n);

	for (index_t i = 0; i < m * n; ++i) r[i] = a[i] + s[i];
	accum_to(a, s);

	ASSERT_MAT_EQ( m, n, a, r );

	for (index_t i = 0; i < m * n; ++i) r[i] = a[i] + cv * s[i];
	accum_to(a, cv, s);

	ASSERT_MAT_EQ( m, n, a, r );

	for (index_t i = 0; i < m * n; ++i) r[i] = a[i] + c[i] * s[i];
	accum_to(a, c, s);

	ASSERT_MAT_EQ(m, n, a, r );
}


MN_CASE( linear_ewise, accumf_to )
{
	const index_t m = M == 0 ? DM : M;
	const index_t n = N == 0 ? DN : N;

	dense_matrix<double, M, N> a(m, n);
	dense_matrix<double, M, N> s(m, n);
	dense_matrix<double, M, N> c(m, n);

	do_fill_rand( a.ptr_data(), m * n );
	do_fill_rand( s.ptr_data(), m * n );
	do_fill_rand( c.ptr_data(), m * n );

	double cv = c[0] * 2.0;

	matrix_shape<M, N> shape(m, n);
	dense_matrix<double, M, N> r(m, n);

	math::sqr_fun<double> f;

	for (index_t i = 0; i < m * n; ++i) r[i] = a[i] + f(s[i]);
	accumf_to(a, f, in_(s));

	ASSERT_MAT_APPROX( m, n, a, r, 1.0e-12 );

	for (index_t i = 0; i < m * n; ++i) r[i] = a[i] + cv * f(s[i]);
	accumf_to(a, cv, f, in_(s));

	ASSERT_MAT_APPROX( m, n, a, r, 1.0e-12 );

}


// Test packs

BEGIN_TPACK( linear_ewise_scalar_cont_cont )
	ADD_MN_CASE_3X3( linear_ewise, scalar_cont_cont, DM, DN )
END_TPACK

BEGIN_TPACK( linear_ewise_scalar_cont_stepcol )
	ADD_N_CASE_3( linear_ewise, scalar_cont_stepcol, DM )
END_TPACK

BEGIN_TPACK( linear_ewise_scalar_stepcol_cont )
	ADD_N_CASE_3( linear_ewise, scalar_stepcol_cont, DM )
END_TPACK

BEGIN_TPACK( linear_ewise_scalar_stepcol_stepcol )
	ADD_N_CASE_3( linear_ewise, scalar_stepcol_stepcol, DM )
END_TPACK

BEGIN_TPACK( linear_ewise_scalar_cont_steprow )
	ADD_N_CASE_3( linear_ewise, scalar_cont_steprow, DN )
END_TPACK

BEGIN_TPACK( linear_ewise_scalar_steprow_cont )
	ADD_N_CASE_3( linear_ewise, scalar_steprow_cont, DN )
END_TPACK

BEGIN_TPACK( linear_ewise_scalar_steprow_steprow )
	ADD_N_CASE_3( linear_ewise, scalar_steprow_steprow, DN )
END_TPACK


BEGIN_TPACK( linear_ewise_sse_cont_cont )
	ADD_MN_CASE_3X3( linear_ewise, sse_cont_cont, DM, DN )
END_TPACK

#ifdef LMAT_HAS_AVX

BEGIN_TPACK( linear_ewise_avx_cont_cont )
	ADD_MN_CASE_3X3( linear_ewise, avx_cont_cont, DM, DN )
END_TPACK

#endif

BEGIN_TPACK( linear_ewise_scalar_single_cont )
	ADD_MN_CASE_3X3( linear_ewise, scalar_single_cont, DM, DN )
END_TPACK

BEGIN_TPACK( linear_ewise_sse_single_cont )
	ADD_MN_CASE_3X3( linear_ewise, sse_single_cont, DM, DN )
END_TPACK

#ifdef LMAT_HAS_AVX

BEGIN_TPACK( linear_ewise_avx_single_cont )
	ADD_MN_CASE_3X3( linear_ewise, avx_single_cont, DM, DN )
END_TPACK

#endif


BEGIN_TPACK( linear_ewise_scalar_map )
	ADD_MN_CASE_3X3( linear_ewise, scalar_map, DM, DN )
END_TPACK

BEGIN_TPACK( linear_ewise_sse_map )
	ADD_MN_CASE_3X3( linear_ewise, sse_map, DM, DN )
END_TPACK

#ifdef LMAT_HAS_AVX

BEGIN_TPACK( linear_ewise_avx_map )
	ADD_MN_CASE_3X3( linear_ewise, avx_map, DM, DN )
END_TPACK

#endif

BEGIN_TPACK( linear_ewise_map_to )
	ADD_MN_CASE_3X3( linear_ewise, map_to, DM, DN )
END_TPACK

BEGIN_TPACK( linear_ewise_accum_to )
	ADD_MN_CASE_3X3( linear_ewise, accum_to, DM, DN )
END_TPACK

BEGIN_TPACK( linear_ewise_accumf_to )
	ADD_MN_CASE_3X3( linear_ewise, accumf_to , DM, DN )
END_TPACK



BEGIN_MAIN_SUITE
	ADD_TPACK( linear_ewise_scalar_cont_cont )
	ADD_TPACK( linear_ewise_scalar_cont_stepcol )
	ADD_TPACK( linear_ewise_scalar_stepcol_cont )
	ADD_TPACK( linear_ewise_scalar_stepcol_stepcol )
	ADD_TPACK( linear_ewise_scalar_cont_steprow )
	ADD_TPACK( linear_ewise_scalar_steprow_cont )
	ADD_TPACK( linear_ewise_scalar_steprow_steprow )

	ADD_TPACK( linear_ewise_sse_cont_cont )
#ifdef LMAT_HAS_AVX
	ADD_TPACK( linear_ewise_avx_cont_cont )
#endif

	ADD_TPACK( linear_ewise_scalar_single_cont )
	ADD_TPACK( linear_ewise_sse_single_cont )
#ifdef LMAT_HAS_AVX
	ADD_TPACK( linear_ewise_avx_single_cont )
#endif

	ADD_TPACK( linear_ewise_scalar_map )
	ADD_TPACK( linear_ewise_sse_map )
#ifdef LMAT_HAS_AVX
	ADD_TPACK( linear_ewise_avx_map )
#endif

	ADD_TPACK( linear_ewise_map_to )
	ADD_TPACK( linear_ewise_accum_to )
	ADD_TPACK( linear_ewise_accumf_to )
END_MAIN_SUITE


