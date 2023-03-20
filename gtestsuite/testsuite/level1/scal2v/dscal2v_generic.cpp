#include <gtest/gtest.h>
#include "test_scal2v.h"

class dscal2vGenericTest :
        public ::testing::TestWithParam<std::tuple<char,
                                                   gtint_t,
                                                   gtint_t,
                                                   gtint_t,
                                                   double,
                                                   char>> {};

GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(dscal2vGenericTest);

// Tests using random integers as vector elements.
TEST_P( dscal2vGenericTest, RandomData )
{
  using T = double;
  //----------------------------------------------------------
  // Initialize values from the parameters passed through
  // test suite instantiation (INSTANTIATE_TEST_SUITE_P).
  //----------------------------------------------------------
  // denotes whether alpha or conj(alpha) will be used:
  char conj_alpha = std::get<0>(GetParam());
  // vector length:
  gtint_t n = std::get<1>(GetParam());
  // stride size for x:
  gtint_t incx = std::get<2>(GetParam());
  // stride size for y:
  gtint_t incy = std::get<3>(GetParam());
  // alpha
  T alpha = std::get<4>(GetParam());
  // specifies the datatype for randomgenerators
  char datatype = std::get<5>(GetParam());

  // Set the threshold for the errors:
  float thresh = testinghelpers::getEpsilon<T>();
  //----------------------------------------------------------
  //     Call generic test body using those parameters
  //----------------------------------------------------------
  test_scal2v<T>(conj_alpha, n, incx, incy, alpha, thresh, datatype);
}

// Used to generate a test case with a sensible name.
// Beware that we cannot use fp numbers (e.g., 2.3) in the names,
// so we are only printing int(2.3). This should be enough for debugging purposes.
// If this poses an issue, please reach out.
class dscal2vGenericTestPrint {
public:
    std::string operator()(
        testing::TestParamInfo<std::tuple<char, gtint_t, gtint_t, gtint_t, double, char>> str) const {
        char conj = std::get<0>(str.param);
        gtint_t n = std::get<1>(str.param);
        gtint_t incx = std::get<2>(str.param);
        gtint_t incy = std::get<3>(str.param);
        double alpha = std::get<4>(str.param);
        char datatype = std::get<5>(str.param);
        std::string str_name = "bli_dscal2v";
        str_name += "_" + std::to_string(n);
        str_name += "_" + std::string(&conj, 1);
        std::string incx_str = ( incx > 0) ? std::to_string(incx) : "m" + std::to_string(std::abs(incx));
        str_name += "_" + incx_str;
        std::string incy_str = ( incy > 0) ? std::to_string(incy) : "m" + std::to_string(std::abs(incy));
        str_name += "_" + incy_str;
        std::string alpha_str = ( alpha > 0) ? std::to_string(int(alpha)) : "m" + std::to_string(int(std::abs(alpha)));
        str_name = str_name + "_a" + alpha_str;
        str_name = str_name + "_" + datatype;
        return str_name;
    }
};
#ifdef TEST_BLIS_TYPED
// Black box testing for generic and main use of dscal2.
INSTANTIATE_TEST_SUITE_P(
        Blackbox,
        dscal2vGenericTest,
        ::testing::Combine(
            ::testing::Values('n'),                                          // n: use x, not conj(x) (since it is real)
            ::testing::Range(gtint_t(10), gtint_t(101), 10),                 // m size of vector takes values from 10 to 100 with step size of 10.
            ::testing::Values(gtint_t(1)),                                   // stride size for x
            ::testing::Values(gtint_t(1)),                                   // stride size for y
            ::testing::Values(double(2.0), double(-3.0)),                    // alpha
            ::testing::Values(ELEMENT_TYPE)                                  // i : integer, f : float  datatype type tested
        ),
        ::dscal2vGenericTestPrint()
    );

// Test when conjugate of x is used as an argument. This option is BLIS-api specific.
// Only test very few cases as sanity check since conj(x) = x for real types.
// We can modify the values using implementantion details.
INSTANTIATE_TEST_SUITE_P(
        Conjalpha,
        dscal2vGenericTest,
        ::testing::Combine(
            ::testing::Values('c'),                                          // c: use conjugate
            ::testing::Values(gtint_t(3), gtint_t(30), gtint_t(112)),        // m size of vector takes values from 10 to 100 with step size of 10.
            ::testing::Values(gtint_t(1)),                                   // stride size for x
            ::testing::Values(gtint_t(1)),                                   // stride size for y
            ::testing::Values(double(-3.0)),                                 // alpha
            ::testing::Values(ELEMENT_TYPE)                                  // i : integer, f : float  datatype type tested
        ),
        ::dscal2vGenericTestPrint()
    );

// Test for non-unit increments.
// Only test very few cases as sanity check.
// We can modify the values using implementantion details.
INSTANTIATE_TEST_SUITE_P(
        NonUnitIncrements,
        dscal2vGenericTest,
        ::testing::Combine(
            ::testing::Values('n'),                                          // n: use x
            ::testing::Values(gtint_t(3), gtint_t(30), gtint_t(112)),        // m size of vector takes values from 10 to 100 with step size of 10.
            ::testing::Values(gtint_t(2), gtint_t(11)),                      // stride size for x
            ::testing::Values(gtint_t(5)),                                   // stride size for y
            ::testing::Values(double(3.0)),                                  // alpha
            ::testing::Values(ELEMENT_TYPE)                                  // i : integer, f : float  datatype type tested
        ),
        ::dscal2vGenericTestPrint()
    );
#endif