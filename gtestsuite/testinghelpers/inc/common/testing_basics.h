#pragma once

#include <stdio.h>
#include <iostream>
#include "common/type_info.h"

namespace testinghelpers {

void char_to_blis_trans( char trans, trans_t* blis_trans );
void char_to_blis_conj( char conj, conj_t* blis_conj );
void char_to_blis_side( char side, side_t* blis_side );
void char_to_blis_uplo( char uplo, uplo_t* blis_uplo );
void char_to_blis_diag( char diag, diag_t* blis_diag );

/**
 * @brief Returns the size of a buffer which has strides.
 *
 * @param n length of vector
 * @param incx increment
 * @return gtint_t dimension of the buffer that stored a vector with length n and increment incx
 */
gtint_t buff_dim(gtint_t n, gtint_t incx);

/**
 * @brief Returns the size of matrix.
 *
 * @param storage specifies the storage format of matrix in memory.
 * @param trns    specifies the form of given matrix.
 * @param m       specifies the number of rows of given matrix.
 * @param n       specifies the number of columns of given matrix.
 * @param ldm     specifies the leading dimension of given matrix.
  * @return gtint_t  Size of the matrix for dimension (m,n) and strides(rs,cs).
 */
gtint_t matsize(char storage, char trans, gtint_t m, gtint_t n, gtint_t ldm );

/**
 * Returns the leading dimension of a matrix depending on the storage type,
 * whether it is transpose or not, and the size of rows and columns.
 *
 * @param storage specifies the storage format of matrix in memory.
 * @param trns    specifies the form of given matrix.
 * @param m       specifies the number of rows of given matrix.
 * @param n       specifies the number of columns of given matrix.
 * @param inc     specifies the increment of the leading dimension.
*/
gtint_t get_leading_dimension(char storage, char trans, gtint_t m, gtint_t n, gtint_t inc);

/**
 * @brief Returns the conjugate of a scalar x.
 *
 * @tparam T float, double, scomplex, dcomplex
 * @param x scalar of type T
 * @return T conjugate of x
 */
template<typename T>
static T conj(T &x){
    if constexpr (testinghelpers::type_info<T>::is_real)
        return x;
    else
        return {x.real, -x.imag};
}

template <typename T>
void conj(T* x, gtint_t len, gtint_t inx)
{
    gtint_t i, ix;
    ix = 0;
    for( i = 0 ; i < len ; i++ )
    {
      x[ix] = conj<T>(x[ix]);
      ix = ix + inx;
    }
    return;
}

template <typename T>
void conj(char storage, T* X, gtint_t m, gtint_t n, gtint_t ldm )
{
    gtint_t i,j;
    gtint_t rs, cs;
    rs=cs=1;
    if( (storage == 'c') || (storage == 'C') )
        cs = ldm ;
    else
        rs = ldm ;

    for( i = 0 ; i < m ; i++ )
    {
        for( j = 0 ; j < n ; j++ )
        {
            X[i*rs + j*cs] = conj<T>( X[i*rs + j*cs] );
        }
    }
    return;
}

template<typename T>
static void initone(T &x) {
    if constexpr (testinghelpers::type_info<T>::is_real)
        x = 1.0;
    else
        x = {1.0, 0.0};
}

template<typename T>
static void initzero(T &x) {
    if constexpr (testinghelpers::type_info<T>::is_real)
        x = 0.0;
    else
        x = {0.0, 0.0};
}

template<typename T>
static void alphax( gtint_t n, T alpha, T *xp, gtint_t incx )
{
    gtint_t i = 0;
    gtint_t ix = 0;
    for(i = 0 ; i < n ; i++) {
        xp[ix] = (alpha * xp[ix]);
        ix = ix + incx;
    }
}

/**
 * @brief Returns the boolean form of a trans value.
 *
 * @param trans specifies the form of matrix stored in memory.
 * @return boolean of the transform of the matrix.
 */
bool chktrans( char trans );
bool chknotrans( char trans );
bool chkconjtrans( char trans );
bool chktransconj( char trans );
bool chkconj( char trans );


/**
 * @brief Returns the boolean form of a matrix triangular form.
 *
 * @param uplo specifies whether matrix is upper or lower triangular stored in memory.
 * @return boolean of the triangular form of the matrix.
 */
bool chkupper( char uplo );
bool chklower( char uplo );

/**
 * @brief Returns the boolean form of a matrix unit/non-unit diagonal form.
 *
 * @param diag specifies whether matrix is unit or non-unit diagonal form.
 * @return boolean of the diagonal form of the matrix.
 */
bool chkunitdiag( char diag );
bool chknonunitdiag( char diag );

/**
 * @brief Returns the boolean form of a matrix left/right side.
 *
 * @param side specifies whether matrix is left or right side form.
 * @return boolean of the side of the matrix.
 */
bool chksideleft( char side );
bool chksideright( char side );

/**
 * @brief swap the dimensions and strides of the matrix based on trans
 *
 * @param trans specifies the form of matrix stored in memory.
 * @param m       specifies the number of rows of given matrix.
 * @param n       specifies the number of columns of given matrix.
 * @param rs      specifies the row stride of given matrix.
 * @param cs      specifies the column stride of given matrix.
 * @param mt      pointer to the row number of given matrix.
 * @param nt      pointer to the column number of given matrix.
 * @param rst     pointer to the row stride of given matrix.
 * @param cst     pointer to the column stride of given matrix.
 */
void swap_dims_with_trans( char trans,
                           gtint_t  m,  gtint_t  n,  gtint_t  rs,  gtint_t  cs,
                           gtint_t* mt, gtint_t* nt, gtint_t* rst, gtint_t* cst );
/**
 * @brief swap the strides of the matrix based on trans
 *
 * @param trans specifies the form of matrix stored in memory.
 * @param rs      specifies the row stride of given matrix.
 * @param cs      specifies the column stride of given matrix.
 * @param rst     pointer to the row stride of given matrix.
 * @param cst     pointer to the column stride of given matrix.
 */
void swap_strides_with_trans( char trans,
                                     gtint_t  rs,  gtint_t  cs,
                                     gtint_t* rst, gtint_t* cst );

/**
 * @brief swap the dimensions
 *
 * @param trans specifies the form of matrix stored in memory.
 * @param x     pointer to the dimension of given vector/matrix.
 * @param y     pointer to the dimension of given vector/matrix.
 */
void swap_dims( gtint_t* x, gtint_t* y );

/**
 * @brief set the dimension of the matrix based on trans
 *
 * @param trans specifies the form of matrix stored in memory.
 * @param m       specifies the number of rows of given matrix.
 * @param n       specifies the number of columns of given matrix.
 * @param mt      pointer to the row number of given matrix.
 * @param nt      pointer to the column number of given matrix.
 */
void set_dims( char trans, gtint_t m, gtint_t n, gtint_t* mt, gtint_t* nt );

/**
 * @brief set the dimension of the matrix based on side
 *
 * @param side    specifies the side of matrix selected in memory.
 * @param m       specifies the number of rows of given matrix.
 * @param n       specifies the number of columns of given matrix.
 * @param dim     pointer to the dimension based on side.
 */
void set_dim_with_side( char side, gtint_t m, gtint_t n, gtint_t* dim );

} //end of namespace testinghelpers
