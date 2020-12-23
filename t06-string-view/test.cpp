#include "string_view.hpp"
#include <cassert>
#include <string>
#include <cstring>

const char *OrigData = "foobar";


void testOrigDataStringView(StringView sv) {
    assert(sv.data() == OrigData);
    assert(sv.size() == 6);
    assert(sv[3] == 'b');

    const char *a = "";
    StringView sv2(a);
    assert(sv2.size() == 0);
    const char *b = "a";
    StringView sv3(b);
    assert(sv3.size() == 1);

    StringView subSv = sv.substr(2);
    assert(subSv.data() == OrigData + 2);
    assert(subSv.size() == 4);
    assert(subSv[0] == 'o');

    StringView subShortSv = sv.substr(3, 4);
    assert(subShortSv[0] == 'b');
    assert(subShortSv.size() == 1);

    assert(sv.find(StringView("foo")) == 0);
    assert(sv.find(StringView("foo")) == 0);
    assert(sv.find(StringView("bar")) == 3);
    assert(sv.find('m') == StringView::npos);
    assert(sv.find(StringView("moo")) == StringView::npos);

    assert(sv.startsWith('f'));
    assert(sv.startsWith(StringView("foo")));
    assert(!sv.startsWith(StringView("Foo")));
    assert(!sv.startsWith(StringView("ofo")));


    assert(sv.endsWith('r'));
    assert(sv.endsWith("bar"));
    assert(!sv.endsWith("abr"));


    assert(sv.findFirstOf('o') == 1);
    assert(sv.findFirstOf('o') == sv.findFirstOf('o', 1));
    assert(sv.findFirstOf('o', 2) == 2);
    assert(sv.findFirstOf('m') == StringView::npos);
    assert(sv.findFirstOf('m', 4) == StringView::npos);
    assert(sv.findFirstNotOf('f') == 1);
    assert(sv.findFirstNotOf('m') == 0);

    assert(sv.findFirstOf("oof") == 0);
    assert(sv.findFirstOf("fm") == 0);
    assert(sv.findFirstOf("A") == StringView::npos);
    assert(sv.findFirstNotOf("oof") == 3);
    assert(sv.findFirstNotOf("fo") == 3);
    assert(sv.findFirstNotOf("abrfo") == StringView::npos);
}

static void testFindFirstOf() {
    std::string s("foobar\0baz", 10);
    StringView sv = s;
    assert(sv.startsWith(sv));
    assert(sv.find(sv) == 0);
    assert(sv.find("baz") == 7);

    char symbols[] = {'Z', '\0', 'z'};
    StringView searched_symbols(symbols, 3);
    assert(sv.findFirstOf(searched_symbols) == 6);

    s = "foobaz";
    sv = s;
    assert(sv.findFirstOf(searched_symbols) == 5);

    s = std::string("foobaz\0baz", 10);
    sv = s;
    assert(sv.findFirstOf(searched_symbols) == 5);
}

void testCopiesAndTrims() {
    auto sv = static_cast<StringView>(OrigData);
    StringView svCopy = sv;

    assert(sv.data() == OrigData);
    assert(sv.size() == 6);

    sv.removePrefix(0);
    assert(sv.data() == OrigData);
    assert(sv.size() == 6);

    sv.removePrefix(2);
    assert(sv.data() == OrigData + 2);
    assert(sv.size() == 4);
    assert(sv[0] == 'o');
    assert(sv[1] == 'b');

    assert(svCopy.data() == OrigData);
    assert(svCopy.size() == 6);

    {
        StringView svCopyCopy = svCopy;
        assert(svCopyCopy.data() == OrigData);
        assert(svCopyCopy.size() == 6);
        svCopyCopy = sv;
        assert(svCopyCopy.data() == OrigData + 2);
        assert(svCopyCopy.size() == 4);
    }

    assert(svCopy.data() == OrigData);
    assert(svCopy.size() == 6);
}


static void testNonCStringView() {
    std::string s("foobar\0baz", 10);
    StringView sv = s;

    assert(s.size() == 10);
    assert(sv.data() == s.c_str());
    assert(sv.size() == 10);

    assert(sv.findFirstOf('z') == 9);
    assert(sv.findFirstOf('z', 6) == 9);
    assert(sv.findFirstOf("mz") == 9);
}

void testOrigDataStringViewFromString(StringView sv) {
    assert(sv.size() == 6);
    assert(sv[3] == 'b');
    StringView subSv = sv.substr(2);
    assert(subSv.data() == sv.data() + 2);
    assert(subSv.size() == 4);
    assert(subSv[0] == 'o');
    StringView subShortSv = sv.substr(3, 4);
    assert(subShortSv[0] == 'b');
    assert(subShortSv.size() == 1);
    assert(sv.find(StringView("foo")) == 0);
    assert(sv.find(StringView("foo")) == 0);
    assert(sv.find(StringView("bar")) == 3);
    assert(sv.find('m') == StringView::npos);
    assert(sv.find(StringView("moo")) == StringView::npos);
    assert(sv.startsWith('f'));
    assert(sv.startsWith(StringView("foo")));
    assert(!sv.startsWith(StringView("Foo")));
    assert(!sv.startsWith(StringView("ofo")));
    assert(sv.endsWith('r'));
    assert(sv.endsWith("bar"));
    assert(!sv.endsWith("abr"));
    assert(sv.findFirstOf('o') == 1);
    assert(sv.findFirstOf('o') == sv.findFirstOf('o', 1));
    assert(sv.findFirstOf('o', 2) == 2);
    assert(sv.findFirstOf('m') == StringView::npos);
    assert(sv.findFirstOf('m', 4) == StringView::npos);
    assert(sv.findFirstNotOf('f') == 1);
    assert(sv.findFirstNotOf('m') == 0);
    assert(sv.findFirstOf("oof") == 0);
    assert(sv.findFirstOf("fm") == 0);
    assert(sv.findFirstOf("A") == StringView::npos);
    assert(sv.findFirstNotOf("oof") == 3);
    assert(sv.findFirstNotOf("fo") == 3);
    assert(sv.findFirstNotOf("abrfo") == StringView::npos);
}

static void testSubString() {
    StringView sv = "foobar";
    StringView empty = sv.substr(0, 0);
    assert(empty.size() == 0);

    StringView whole = sv.substr(0, 100500);
    assert(whole.size() == sv.size());
    assert(!std::memcmp(sv.data(), whole.data(), whole.size()));
}

static void testStringViews() {
    testOrigDataStringView(OrigData);
    testNonCStringView();
    testCopiesAndTrims();
    const std::string s = OrigData;
    testOrigDataStringViewFromString(std::string(OrigData));
    testFindFirstOf();
    testSubString();
}


int main() {
    testStringViews();
}
