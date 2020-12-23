#include <cassert>
#include <string>
#include <sstream>

// Include guard should prevent double inclusion.
#include "bignum.hpp"
#include "bignum.hpp"

namespace {
    void check_constructors() {
        mp::bignum n1;
        assert(n1.to_string() == "0");

        mp::bignum n2(42);
        assert(n2.to_string() == "42");

        mp::bignum n3(0xffffffff);
        assert(n3.to_string() == "4294967295");

        mp::bignum n4(n2);
        assert(n2.to_string() == "42");
        assert(n4.to_string() == "42");

        mp::bignum n5("42");
        assert(n5.to_string() == "42");

        mp::bignum n6("18446744073709551615");
        assert(n6.to_string() == "18446744073709551615");
    }

    void check_assignment() {
        mp::bignum n1;
        assert(n1.to_string() == "0");

        n1 = 1;
        assert(n1.to_string() == "1");

        n1 = 0xffffffff;
        assert(n1.to_string() == "4294967295");

        mp::bignum n2;
        assert(n2.to_string() == "0");

        n2 = n1;
        assert(n1.to_string() == "4294967295");
        assert(n2.to_string() == "4294967295");

        mp::bignum n3("18446744073709551615"), n4, n5;
        assert(n3.to_string() == "18446744073709551615");
        assert(n4.to_string() == "0");
        n4 = n3;
        assert(n3.to_string() == "18446744073709551615");
        assert(n4.to_string() == "18446744073709551615");

        n3 = n5;
        assert(n3.to_string() == "0");
        assert(n5.to_string() == "0");

    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wself-assign-overloaded"

        n4 = n4;
    #pragma clang diagnostic pop
        assert(n4.to_string() == "18446744073709551615");
    }

    void check_bool() {
        assert(!mp::bignum(0));
        assert(mp::bignum(123));
    }

    void check_bignum_binary_operators_with_conversion() {
        mp::bignum const a(10);

        assert((a + 10).to_string() == "20");
        assert((10 + a).to_string() == "20");
        assert((a * 10).to_string() == "100");
        assert((10 * a).to_string() == "100");
    }

    void check_bignum_zero_multiplication() {
        mp::bignum a(0);
        mp::bignum b(0);

        assert(!(a * b));
        assert((a * b).to_string() == "0");
    }

    void check_long_bignum_bool() {
        mp::bignum a(4294967295);
        assert(a);
        a += 1;
        assert(a);
    }

    void check_io() {
        mp::bignum const n1(1234);

        std::ostringstream ostr;
        ostr << n1;
        assert(ostr.str() == "1234");

        std::istringstream istr1("5678");
        mp::bignum n2(42);
        istr1 >> n2;
        assert(n2.to_string() == "5678");

        // Optional
        std::istringstream istr2("0001234");
        mp::bignum n3;
        istr2 >> n3;
        assert(n3.to_string() == "1234");

        std::istringstream istr3("+1234");
        mp::bignum n4;
        istr3 >> n4;
        assert(n4.to_string() == "1234");

        std::istringstream istr4("+01234");
        mp::bignum n5;
        istr4 >> n5;
        assert(n5.to_string() == "1234");

        mp::bignum n6("+012349384287592834693825972398570293485702345");
        std::ostringstream os;
        os << n6;
        assert(os.str() == "12349384287592834693825972398570293485702345");
    }

    void check_to_int() {
        mp::bignum n1(42);
        assert(std::uint32_t(n1) == 42);

        mp::bignum n2("1311768467284833366");  // 0x1234567890123456
        assert(std::uint32_t(n2) == 0x90123456);
        n2 = mp::bignum("1311768987986086000013487619238403249172634467284833366");
        assert(std::uint32_t(n2) == 3466875990);
        n2 = mp::bignum("675968765");
        assert(std::uint32_t(n2) == 675968765);
        n2 = mp::bignum("96587657654768995437647654659760000000000000000000");
        assert(std::uint32_t(n2) == 3972005888);
    }

    void check_adding_operators() {
        mp::bignum n1(1234);
        mp::bignum n2(5678);
        assert((n1 + n2).to_string() == std::to_string(1234 + 5678));
        mp::bignum n3(0xffffffff);
        mp::bignum n4(0xaaaaaaaa);
        assert((n3 + n4).to_string() == "7158278825");
        n3 = "1231209210398210938029809281039812";
        n4 = "10920193821098401298301298412";
        assert((n3 + n4).to_string() == "1231220130592032036431107582338224");
        n3 = "000000001029347029387501923847150329471520397512093847092374";
        n4 = "+00013798709287409387409821730";
        assert((n3 + n4).to_string() ==
               "1029347029387501923847150343270229684921481256914104");
        n3 = "999999999999999999999999999999999";
        n4 = "1";
        assert((n3 + n4).to_string() == "1000000000000000000000000000000000");

        n3 = "12878237641";
        n4 = "123123";
        assert((n3 + n4).to_string() == "12878360764");

        n3 = "999999999";
        n4 = "999999999";
        assert((n3 + n4).to_string() == "1999999998");
        n3 = "999999999";
        n4 = "999999999";
        n3 += n4;
        assert(n3.to_string() == "1999999998");

        n3 = "12878237641";
        n4 = "123123";
        n3 += n4;
        assert(n3.to_string() == "12878360764");

        n3 = "000000001029347029387501923847150329471520397512093847092374";
        n4 = "+00013798709287409387409821730";
        assert((n3 += n4).to_string() ==
               "1029347029387501923847150343270229684921481256914104");
    }

    void check_multiply_operators() {
        mp::bignum n1(1234), n2(5678);
        assert((n1 * n2).to_string() == std::to_string(1234 * 5678));
        mp::bignum n3{"999999999"};
        mp::bignum n4{"999999999"};
        assert((n3 * n4).to_string() == "999999998000000001");

        n3 = "9999999999";
        n4 = "9999999999";
        assert((n3 * n4).to_string() == "99999999980000000001");
        n3 *= n4;
        assert(n3.to_string() == "99999999980000000001");


        n3 = "000000001029347029387501923847150329471520397512093847092374";
        n4 = "+00013798709287409387409821730";
        assert((n3 * n4).to_string() == "14203660414376586432488681623808670452458821977753857663150940052490182487020");

        n3 *= 2;
        assert(n3.to_string() == "2058694058775003847694300658943040795024187694184748");
    }


    void check_const() {
        mp::bignum const n1(42);
        mp::bignum n2;
        n2 = n1;
        assert(n1.to_string() == "42");
        assert(n2.to_string() == "42");

        mp::bignum const n3(10);
        assert(std::uint32_t(n3) == 10);

        assert(n3);

        assert((n1 + n3).to_string() == "52");
        assert((n1 * n3).to_string() == "420");

        n2 *= n3;
        assert(n2.to_string() == "420");
        assert(n3.to_string() == "10");

        n2 += n3;
        assert(n2.to_string() == "430");
        assert(n3.to_string() == "10");
    }

    namespace long_bignums {
        void check_constructor() {
            std::string str{"1" + std::string(150, '0')};
            mp::bignum a(str);
            assert(a.to_string() == str);
        }

        void check_long_operators() {
            mp::bignum a("1" + std::string(50, '0'));
            mp::bignum b("1" + std::string(77, '0'));

            std::string const aplusb("1" + std::string(26, '0') + "1" + std::string(50, '0'));
            std::string const amultb("1" + std::string(127, '0'));

            assert((a + b).to_string() == aplusb);
            assert((a * b).to_string() == amultb);
            assert((b + a).to_string() == aplusb);
            assert((b * a).to_string() == amultb);
        }
    }
}

namespace check_polynomial_nm {
    void check_polynomial()
    {
        mp::polynomial const p("2*x^2+10*x^1+1*x^0");
        assert(p(0) == 1);
        assert(std::abs(p(1.0) - 13.) < std::numeric_limits<double>::epsilon());
        assert(p(mp::bignum(2)).to_string() == "29");
    }

    void check_long_coefficient()
    {
        mp::polynomial p("100*x^0");
        assert(p.at(0) == 100);
    }

    void check_at()
    {
        mp::polynomial p("1*x^0");

        assert(p.at(0) == 1);
        assert(p.at(1) == 0);
        assert(p(1) == 1);

        p.at(1) = 2;

        assert(p.at(0) == 1);
        assert(p.at(1) == 2);
        assert(p(1) == 3);
    }

    void check_const_at()
    {
        mp::polynomial const p("2*x^2+10*x^1+1*x^0");

        assert(p.at(0) == 1);
        assert(p.at(1) == 10);
        assert(p.at(2) == 2);
    }
}

int main() {
    check_bignum_binary_operators_with_conversion();
    check_constructors();
    check_assignment();
    check_bool();
    check_io();
    check_to_int();
    check_adding_operators();
    check_multiply_operators();
    check_const();
    check_bignum_zero_multiplication();
    check_long_bignum_bool();
    long_bignums::check_constructor();
    long_bignums::check_long_operators();
    check_polynomial_nm::check_polynomial();
    check_polynomial_nm::check_long_coefficient();
    check_polynomial_nm::check_at();
    check_polynomial_nm::check_const_at();
}
