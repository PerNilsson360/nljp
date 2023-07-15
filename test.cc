// MIT license
//
// Copyright 2023 Per Nilsson
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include <memory>
#include <cassert>
#include <iostream>

#include "nljp.hh"

void
testNumbers() {
    // NaN
    {
        nlohmann::json json;
        XpathData r(eval("'NO'", json));
        assert(std::isnan(r.getNumber()));
    }
    // +
    {
        nlohmann::json json;
        XpathData r(eval("1 + 2", json));
        assert(r.getNumber() == 3);
    }
    {
        // <a>3</a>
        const char* j = R"({"a":3})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("1 + /a", json));
        assert(r.getNumber() == 4);
    }
    {
        // <a><b>1</b><c>2</c></a>
        const char* j = R"({"a":{"b":3,"c":1}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("/a/b + /a/c", json));
        assert(r.getNumber() == 4);
    }
    // -
    {
        nlohmann::json json;
        XpathData r(eval("1 - 2", json));
        assert(r.getNumber() == -1);
    }
    {
        // <a>3</a>
        const char* j = R"({"a":3})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("1 - /a", json));
        assert(r.getNumber() == -2);
    }
    {
        // <a><b>1</b><c>2</c></a>
        const char* j = R"({"a":{"b":3,"c":1}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("/a/b - /a/c", json));
        assert(r.getNumber() == 2);
    }
    // *
    {
        nlohmann::json json;
        XpathData r(eval("3 * 2", json));
        assert(r.getNumber() == 6);
    }
    {
        // <a>3</a>
        const char* j = R"({"a":3})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("1 * /a", json));
        assert(r.getNumber() == 3);
    }
    {
        // <a><b>1</b><c>2</c></a>
        const char* j = R"({"a":{"b":3,"c":1}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("/a/b * /a/c", json));
        assert(r.getNumber() == 3);
    }
    // div
    {
        nlohmann::json json;
        XpathData r(eval("4 div 2", json));
        assert(r.getNumber() == 2);
    }
    {
        // <a>3</a>
        const char* j = R"({"a":3})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("6 div /a", json));
        assert(r.getNumber() == 2);
    }
    {
        // <a><b>1</b><c>2</c></a>
        const char* j = R"({"a":{"b":3,"c":1}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("/a/b div /a/c", json));
        assert(r.getNumber() == 3);
    }
    // mod
    {
        nlohmann::json json;
        XpathData r(eval("5 mod 2", json));
        assert(r.getNumber() == 1);
    }
    {
        nlohmann::json json;
        XpathData r(eval("5 mod -2", json));
        assert(r.getNumber() == 1);
    }
    // {
    //  nlohmann::json json;
    //  XpathData r(eval("5 mod -2", json));
    //  assert(r.getNumber() == -1); // TODO value is 1 should be -1 according to spec
    // }
    {
        nlohmann::json json;
        XpathData r(eval("-5 mod -2", json));
        assert(r.getNumber() == -1);
    }
    // unary -
    {
        nlohmann::json json;
        XpathData r(eval("4 div (- 2)", json));
        assert(r.getNumber() == -2);
    }
    {
        // <a>3</a>
        const char* j = R"({"a":3})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("-6 div /a", json));
        assert(r.getNumber() == -2);
    }
    {
        // <a><b>1</b><c>2</c></a>
        const char* j = R"({"a":{"b":3,"c":1}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("-/a/b div /a/c", json));
        assert(r.getNumber() == -3);
    }
}

void
testLogic() {
    {
        nlohmann::json json;
        XpathData r(eval("true() and true()", json));
        assert(r.getBool());
    }
    {
        nlohmann::json json;
        XpathData r(eval("true() and false()", json));
        assert(!r.getBool());
    }
    {
        // <a>3</a>
        const char* j = R"({"a":3})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("/a and 1", json));
        assert(r.getBool());
    }
    {
        nlohmann::json json;
        XpathData r(eval("true() or true()", json));
        assert(r.getBool());
    }
    {
        nlohmann::json json;
        XpathData r(eval("false() or false()", json));
        assert(!r.getBool());
    }
    {
        nlohmann::json json;
        XpathData r(eval("1 or false()", json));
        assert(r.getBool());
    }
    {
        nlohmann::json json;
        XpathData r(eval("'foo' or false()", json));
        assert(r.getBool());
    }
    {
        nlohmann::json json;
        XpathData r(eval("not(false())", json));
        assert(r.getBool());
    }
    {
        nlohmann::json json;
        XpathData r(eval("not(true())", json));
        assert(!r.getBool());
    }
    {
        // <a>3</a>
        const char* j = R"({"a":3})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("not(/a and 1)", json));
        assert(!r.getBool());
    }
    {
        // <a><b>1</b><c>true</c><d>foo</d></a>
        const char* j = R"({"a":{"b":1,"c":true,"d":"foo"}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("/a and /a/b and /a/c and /a/d", json));
        assert(r.getBool());
    }
}

void
testPaths() {
    {
        // <a><b>1</b><b>2</b><b>3</b></a>
        const char* j = R"({"a":{"b":[1, 2, 3]}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("count(/a/b)", json));
        assert(r.getNumber() == 3);
        r = eval("count(/a/b/ancestor::a)", json);
        assert(r.getNumber() == 1);
        r = eval("local-name(/a/b/ancestor::a)", json);
        assert(r.getString() == "a");        
        r = eval("count(/child::a/child::b)", json);
        assert(r.getNumber() == 3);
        r = eval("count(/a/..)", json);
        assert(r.getNumber() == 1);
        r = eval("count(/child::a/..)", json);
        assert(r.getNumber() == 1);
        r = eval("local-name(/a)", json);
        assert(r.getString() == "a");
        r = eval("local-name(/child::a)", json);
        assert(r.getString() == "a");
        r = eval("local-name(/a/.)", json);
        assert(r.getString() == "a");
        r = eval("local-name(/a/self::*)", json);
        assert(r.getString() == "a");
        r = eval("local-name(/a/self::a)", json);
        assert(r.getString() == "a");
        r = eval("count(/a/self::b)", json);
        assert(r.getNumber() == 0);
        r = eval("count(/a/child::*)", json);
        assert(r.getNumber() == 3);
    }
    {
        // <a><b><c><e>1</e></c></b><d><c><e>1</e></c></d></a>
        const char* j = R"({"a":{"b":{"c":{"e":1}},"d":{"c":{"e":1}}}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("count(/a)", json));
        assert(r.getNumber() == 1);
        r = eval("count(/a/b)", json);
        assert(r.getNumber() == 1);
        r = eval("count(/a/b/parent::a)", json);
        assert(r.getNumber() == 1);
        r = eval("count(/a/b/parent::*)", json);
        assert(r.getNumber() == 1);
        r = eval("count(/a/b/parent::c)", json);
        assert(r.getNumber() == 0);
        r = eval("count(/a/b/c)", json);
        assert(r.getNumber() == 1);
        r = eval("count(/a/b/c/e)", json);
        assert(r.getNumber() == 1);
        r = eval("count(//e)", json);
        assert(r.getNumber() == 2);
        r = eval("count(//e/ancestor::c)", json);
        assert(r.getNumber() == 2);
        r = eval("count(//e/ancestor::b)", json);
        assert(r.getNumber() == 1);
        r = eval("count(//e/ancestor::a)", json);
        assert(r.getNumber() == 1);
        r = eval("count(//e/ancestor::*)", json);
        assert(r.getNumber() == 6);
        r = eval("count(/descendant::e)", json);
        assert(r.getNumber() == 2);
    }
    // * tests
    {
        // <a><b>1</b><c>true</c><d>foo</d></a>
        const char* j = R"({"a":{"b":1,"c":true,"d":"foo"}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("count(/*)", json));
        assert(r.getNumber() == 1);
        r = eval("/*", json);
        assert(r.getString() == "1truefoo");
        r = eval("count(/a/*)", json);
        assert(r.getNumber() == 3);
        r = eval("/a/*", json);
        assert(r.getStringValue() == "1truefoo");
    }
    {
        // <a><b><b>1</b></b><b><b>2</b></b><b><c>3</c></b></a>
        const char* j = R"({"a":{"b": [{"b": 1},{"b": 2},{"c":3}]}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("count(/a/*)", json));
        assert(r.getNumber() == 3);
        r = (eval("/a/*", json));
        assert(r.getStringValue() == "123");
        r = (eval("count(/a/b/*)", json));
        assert(r.getNumber() == 3);
        r = (eval("count(/a/b/following-sibling::*)", json));
        assert(r.getNumber() == 2);
        r = eval("/a/b/following-sibling::*", json);
        assert(r.getStringValue() == "23");
        r = eval("/a/b/following-sibling::*[2]", json);
        assert(r.getStringValue() == "3");
        r = eval("count(/a/b[b = 2]/following-sibling::*)", json);
        assert(r.getNumber() == 1);
        r = eval("/a/b[b = 2]/following-sibling::*", json);
        assert(r.getStringValue() == "3");
        r = (eval("count(/a/b/following-sibling::b)", json));
        assert(r.getNumber() == 2);
        r = (eval("/a/b/following-sibling::b", json));
        assert(r.getStringValue() == "23");
        r = eval("/a/b/following-sibling::b[1]", json);
        assert(r.getStringValue() == "2");
        r = eval("count(/a/b[b = 2]/following-sibling::b)", json);
        assert(r.getNumber() == 1);
        r = eval("/a/b[b = 2]/following-sibling::b", json);
        assert(r.getStringValue() == "3");
    }
    // Descendant tests
    {
        // <a>3</a>
        const char* j = R"({"a":3})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("count(//a)", json));
        assert(r.getNumber() == 1);
        r = eval("count(//*)", json);
        assert(r.getNumber() == 1);
        r = eval("count(/descendant::*)", json);
        assert(r.getNumber() == 1);
        r = eval("count(//.)", json);
        assert(r.getNumber() == 1);
    }
    {
        // <a><b>1</b><c>2</c></a>
        const char* j = R"({"a":{"b":3,"c":1}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("count(//a)", json));
        assert(r.getNumber() == 1);
        r = eval("count(/descendant::a)", json);
        assert(r.getNumber() == 1);
        r = eval("count(//*)", json);
        assert(r.getNumber() == 3);
        r = eval("count(/descendant::*)", json);
        assert(r.getNumber() == 3);
        r = eval("count(//.)", json);
        assert(r.getNumber() == 3);
        r = eval("count(//b)", json);
        assert(r.getNumber() == 1);
        r = eval("local-name(//b/..)", json);
        assert(r.getString() == "a");
    }   
    {
        //<a><b><c><e>1</e></c></b><d><c><e>1</e></c></d></a>
        const char* j = R"({"a":{"b":{"c":{"e":1}},"d":{"c":{"e":1}}}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("count(//a)", json));
        assert(r.getNumber() == 1);
        r = eval("count(//b)", json);
        assert(r.getNumber() == 1);
        r = eval("count(/descendant::b)", json);
        assert(r.getNumber() == 1);
        r = eval("count(//b/c)", json);
        assert(r.getNumber() == 1);
        r = eval("count(/descendant::b/c)", json);
        assert(r.getNumber() == 1);
        r = eval("count(//c)", json);
        assert(r.getNumber() == 2);
        r = eval("count(/descendant::c)", json);
        assert(r.getNumber() == 2);
        r = eval("count(//c/e)", json);
        assert(r.getNumber() == 2);
        r = eval("count(/descendant::c/e)", json);
        assert(r.getNumber() == 2);
        r = eval("count(//e)", json);
        assert(r.getNumber() == 2);
        r = eval("count(/descendant::e)", json);
        assert(r.getNumber() == 2);
        r = eval("count(/a//e)", json);
        assert(r.getNumber() == 2);
        r = eval("count(/a/descendant::e)", json);
        assert(r.getNumber() == 2);
        r = eval("count(//e/..)", json);
        assert(r.getNumber() == 2);
        r = eval("count(/descendant::e/..)", json);
        assert(r.getNumber() == 2);
        r = eval("count(//e/../../..)", json);
        assert(r.getNumber() == 1);
        r = eval("count(/descendant::e/../../..)", json);
        assert(r.getNumber() == 1);
    }
    {
        // <a><b>1</b><b>2</b><b>3</b><b>4</b></a>
        const char* j = R"({"a":{"b":[1,2,3,4]}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("count(//b)", json));
        assert(r.getNumber() == 4);
        r = eval("count(/descendant::b)", json);
        assert(r.getNumber() == 4);
        r = eval("count(//*)", json);
        assert(r.getNumber() == 5);
        r = eval("count(/descendant::*)", json);
        assert(r.getNumber() == 5);
        r = eval("count(//.)", json);
        assert(r.getNumber() == 5);
    }
    {
        // <a><a><a></a>1</a></a>
        const char* j = R"({"a":{"a":{"a":1}}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("count(//a)", json));
        assert(r.getNumber() == 3);
         r = eval("count(/descendant::a)", json);
        assert(r.getNumber() == 3);
        r = eval("count(/a/descendant-or-self::a)", json);
        assert(r.getNumber() == 3);
        r = eval("count(/a/descendant-or-self::*)", json);
        assert(r.getNumber() == 3);
        r = eval("count(//a/a)", json);
        assert(r.getNumber() == 2);
        r = eval("count(/descendant::a/a)", json);
        assert(r.getNumber() == 2);
        r = eval("count(//*)", json);
        assert(r.getNumber() == 3);
        r = eval("count(/descendant::*)", json);
        assert(r.getNumber() == 3);
        r = eval("count(//.)", json);
        assert(r.getNumber() == 3);
        r = eval("count(/a/a/a/ancestor-or-self::a)", json);
        assert(r.getNumber() == 3);
    }
    {
        const char* j = R"({"a":[{"a":1},{"a":2},{"b":3}]})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("count(//a)", json));
        assert(r.getNumber() == 5);
        r = eval("count(/descendant::a)", json);
        assert(r.getNumber() == 5);
        r = eval("count(/a/descendant-or-self::a)", json);
        assert(r.getNumber() == 5);
        r = eval("count(//*)", json);
        assert(r.getNumber() == 6);
        r = eval("count(/descendant::*)", json);
        assert(r.getNumber() == 6);
        r = eval("count(//.)", json);
        assert(r.getNumber() == 6);
        r = eval("//a", json);
        assert(r.getStringValue() == "12312");
        r = eval("/descendant::a", json);
        assert(r.getStringValue() == "12312");
        r = eval("count(/a/a/ancestor-or-self::a)", json);
        assert(r.getNumber() == 4);
    }
    {
        // TODO replace above with following
        // <a><b><b>1</b></b><b><b>2</b></b><b><c>3</c></b></a>
    }
}

void
testRelations() {
    // =
    {
        nlohmann::json json;
        XpathData r(eval("1 = 1", json));
        assert(r.getBool());
        r = (eval("1 = 2", json));
        assert(!r.getBool());
        r = eval("true() = true()", json);
        assert(r.getBool());
        r = eval("false() = false()", json);
        assert(r.getBool());
        r = eval("true() = false()", json);
        assert(!r.getBool());
        r = eval("'a' = 'a'", json);
        assert(r.getBool());
        r = eval("\"a\" = \"a\"", json);
        assert(r.getBool());
        r = eval("'a' = 'b'", json);
        assert(!r.getBool());
    }
    {
        //<a><b><c><e>1</e></c></b><d><c><e>1</e></c></d></a>
        const char* j = R"({"a":{"b":{"c":{"e":1}},"d":{"c":{"e":1}}}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("/a/b/c/e = 1", json));
        assert(r.getBool());
        r = eval("/a/b/c/e = '1'", json);
        assert(r.getBool());
        r = eval("/a/b/c/e = '2'", json);
        assert(!r.getBool());
        r = eval("/a/b/c/e = true()", json);
        assert(r.getBool());
        r = eval("/a/b/c/e = false()", json);
        assert(!r.getBool());
        r = eval("/a/b/c = /a/d/c", json);
        assert(r.getBool());
        r = eval("/a/b/c = /a/d", json);
        assert(r.getBool());
    }
    // !=
    {
        nlohmann::json json;
        XpathData r(eval("1 != 1", json));
        assert(!r.getBool());
        r = (eval("1 != 2", json));
        assert(r.getBool());
        r = eval("true() != true()", json);
        assert(!r.getBool());
        r = eval("false() != false()", json);
        assert(!r.getBool());
        r = eval("true() != false()", json);
        assert(r.getBool());
        r = eval("'a' != 'a'", json);
        assert(!r.getBool());
        r = eval("\"a\" != \"a\"", json);
        assert(!r.getBool());
        r = eval("'a' != 'b'", json);
        assert(r.getBool());
    }
    {
        //<a><b><c><e>1</e></c></b><d><c><e>1</e></c></d></a>
        const char* j = R"({"a":{"b":{"c":{"e":1}},"d":{"c":{"e":1}}}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("/a/b/c/e != 1", json));
        assert(!r.getBool());
        r = eval("/a/b/c/e != '1'", json);
        assert(!r.getBool());
        r = eval("/a/b/c/e != '2'", json);
        assert(r.getBool());
        r = eval("/a/b/c/e != true()", json);
        assert(!r.getBool());
        r = eval("/a/b/c/e != false()", json);
        assert(r.getBool());
        r = eval("/a/b/c != /a/d/c", json);
        assert(!r.getBool());
        r = eval("/a/b/c != /a/d", json);
        assert(!r.getBool());
    }
    // <
    {
        nlohmann::json json;
        XpathData r(eval("1 < 1", json));
        assert(!r.getBool());
        r = (eval("1 < 2", json));
        assert(r.getBool());
        r = eval("true()  < true()", json);
        assert(!r.getBool());
        r = eval("false() < false()", json);
        assert(!r.getBool());
        r = eval("false() < true()", json);
        assert(r.getBool());
        r = eval("'a' < 1", json);
        assert(!r.getBool());
    }
    {
        //<a><b><c><e>1</e></c></b><d><c><e>1</e></c></d></a>
        const char* j = R"({"a":{"b":{"c":{"e":1}},"d":{"c":{"e":1}}}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("/a/b/c/e < 2", json));
        assert(r.getBool());
        r = eval("/a/b/c/e < '1'", json);
        assert(!r.getBool());
        r = eval("/a/b/c/e < '2'", json);
        assert(r.getBool());
        r = eval("/a/b/c/e < true()", json);
        assert(!r.getBool());
        r = eval("/a/b/c/e < false()", json);
        assert(!r.getBool());
        bool exception(false);
        try {
            r = eval("/a/b/c < /a/d/c", json);
        } catch (const std::runtime_error& e) {
            exception = true;
        }
        assert(exception);
    }
    // <=
    {
        nlohmann::json json;
        XpathData r(eval("1 <= 1", json));
        assert(r.getBool());
        r = (eval("1 <= 2", json));
        assert(r.getBool());
        r = eval("true()  <= true()", json);
        assert(r.getBool());
        r = eval("false() <= false()", json);
        assert(r.getBool());
        r = eval("true() <= false()", json);
        assert(!r.getBool());
        r = eval("'a' <= 1", json);
        assert(!r.getBool());
    }
    {
        //<a><b><c><e>1</e></c></b><d><c><e>1</e></c></d></a>
        const char* j = R"({"a":{"b":{"c":{"e":1}},"d":{"c":{"e":1}}}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("/a/b/c/e <= 2", json));
        assert(r.getBool());
        r = eval("/a/b/c/e <= '0.5'", json);
        assert(!r.getBool());
        r = eval("/a/b/c/e <= '2'", json);
        assert(r.getBool());
        r = eval("/a/b/c/e <= true()", json);
        assert(r.getBool());
        r = eval("/a/b/c/e <= false()", json);
        assert(!r.getBool());
        bool exception(false);
        try {
            r = eval("/a/b/c <= /a/d/c", json);
        } catch (const std::runtime_error& e) {
            exception = true;
        }
        assert(exception);
    }
    // >
        {
        nlohmann::json json;
        XpathData r(eval("1 > 1", json));
        assert(!r.getBool());
        r = (eval("2 > 1", json));
        assert(r.getBool());
        r = eval("true()  > true()", json);
        assert(!r.getBool());
        r = eval("false() > false()", json);
        assert(!r.getBool());
        r = eval("true() > false()", json);
        assert(r.getBool());
        r = eval("'a' > 1", json);
        assert(!r.getBool());
    }
    {
        //<a><b><c><e>1</e></c></b><d><c><e>1</e></c></d></a>
        const char* j = R"({"a":{"b":{"c":{"e":1}},"d":{"c":{"e":1}}}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("/a/b/c/e > 2", json));
        assert(!r.getBool());
        r = eval("/a/b/c/e > '0.5'", json);
        assert(r.getBool());
        r = eval("/a/b/c/e > '2'", json);
        assert(!r.getBool());
        r = eval("/a/b/c/e > true()", json);
        assert(!r.getBool());
        r = eval("/a/b/c/e > false()", json);
        assert(r.getBool());
        bool exception(false);
        try {
            r = eval("/a/b/c > /a/d/c", json);
        } catch (const std::runtime_error& e) {
            exception = true;
        }
        assert(exception);
    }
    // >=
        {
        nlohmann::json json;
        XpathData r(eval("1 >= 1", json));
        assert(r.getBool());
        r = (eval("2 >= 1", json));
        assert(r.getBool());
        r = eval("true()  >= true()", json);
        assert(r.getBool());
        r = eval("false() >= false()", json);
        assert(r.getBool());
        r = eval("false() >= true()", json);
        assert(!r.getBool());
        r = eval("'a' >= 1", json);
        assert(!r.getBool());
    }
    {
        //<a><b><c><e>1</e></c></b><d><c><e>1</e></c></d></a>
        const char* j = R"({"a":{"b":{"c":{"e":1}},"d":{"c":{"e":1}}}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("/a/b/c/e >= 0.5", json));
        assert(r.getBool());
        r = eval("/a/b/c/e >= '2'", json);
        assert(!r.getBool());
        r = eval("/a/b/c/e >= '0.5'", json);
        assert(r.getBool());
        r = eval("/a/b/c/e >= true()", json);
        assert(r.getBool());
        r = eval("/a/b/c/e >= false()", json);
        assert(r.getBool());
        bool exception(false);
        try {
            r = eval("/a/b/c >= /a/d/c", json);
        } catch (const std::runtime_error& e) {
            exception = true;
        }
        assert(exception);
    }
}

void 
testFilter() {
    {
        // <a><b>1</b><b>2</b><b>3</b><b>4</b></a>
        const char* j = R"({"a":{"b":[1,2,3,4]}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("count(/a/b[. = 1])", json));
        assert(r.getNumber() == 1);
        r = eval("count(/a/b[not(. = 1)])", json);
        assert(r.getNumber() == 3);
        r = eval("count(/a/b[not(. = 1)][not(. = 2)])", json);
        assert(r.getNumber() == 2);
        r = eval("count(/a/b[not(. = 1)][not(. = 2)][not(. = 3)])", json);
        assert(r.getNumber() == 1);
        r = eval("count(/a/b[not(. = 1)][not(. = 2)][not(. = 3)][not(. = 4)])", json);
        assert(r.getNumber() == 0);
        r = eval("/a/b[1]", json);
        assert(r.getNumber() == 1);
        r = eval("/a/b[2]", json);
        assert(r.getNumber() == 2);
        r = eval("/a/b[2 + 1]", json);
        assert(r.getNumber() == 3);
        r = eval("/a/b[1 + 3]", json);
        assert(r.getNumber() == 4);
        r = eval("count(/a/b[0])", json);
        assert(r.getNumber() == 0);
        r = eval("count(/a/b[5])", json);
        assert(r.getNumber() == 0);

    }
    {
        //<a><b><c><e>1</e></c></b><d><f><e>1</e></f></d></a>
        const char* j = R"({"a":{"b":{"c":{"e":1}},"d":{"f":{"e":1}}}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("count(/a/*[count(c) > 0])", json));
        assert(r.getNumber() == 1);
        r  = eval("count(/a/*/*[local-name(..) = 'b'])", json);
        assert(r.getNumber() == 1);
        r = eval("count(/a/*[count(*[local-name(.) = 'c']) > 0])", json);
        assert(r.getNumber() == 1);
    }
    {
        // <a><b><c><e>1</e></c></b><b><d><e>2</e></d></b></a>
        const char* j = R"({"a":{"b":[{"c":{"e":1}},{"d":{"e":2}}]}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("count(/a/b[count(.//e) = 1])", json));
        assert(r.getNumber() == 2);
        r = eval("count(/a/b[count(//e) = 2])", json);
        assert(r.getNumber() == 2);
        r = eval("count(//*[local-name(.) = 'd'])", json);
        assert(r.getNumber() == 1);
        r = eval("count(/a/*[count(following-sibling::*) = 1])", json);
        assert(r.getNumber() == 1);
        r = eval("/a/*[count(following-sibling::*) = 1]", json);
        assert(r.getStringValue() == "1");
    }
}

void
testNodeSetFunctions() {
    {
        // <a>3</a>
        const char* j = R"({"a":3})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("/a[position()=1]", json));
        assert(r.getStringValue() == "3");
        r = eval("/a[position()=last()]", json);
        assert(r.getStringValue() == "3");
        r = eval("count(/a[position()=last()])", json);
        assert(r.getNumber() == 1);
        r = eval("local-name(/a[position()=last()])", json);
        assert(r.getStringValue() == "a");
        r = eval("count(/a[position()=0])", json);
        assert(r.getNumber() == 0);
        r = eval("count(/a[position()=2])", json);
        assert(r.getNumber() == 0);
    }
}

void
testStringFunctions() {
    {
        // <a>3</a>
        const char* j = R"({"a":3})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("string(/)", json));
        assert(r.getString() == "3");
        assert(r.getStringValue() == "3");
        r = eval("string(/a)", json);
        assert(r.getString() == "3");
    }
    {
        // <a><b>1</b><c>2</c></a>
        const char* j = R"({"a":{"b":3,"c":1}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("string(/)", json));
        assert(r.getString() == "31");
        r = eval("string(/a)", json);
        assert(r.getString() == "31");
        r = eval("string(/a/b)", json);
        assert(r.getString() == "3");
        r = eval("string(/a/c)", json);
        assert(r.getString() == "1");
    }
    {
        //<a><b><c><e>1</e></c></b><d><c><e>1</e></c></d></a>
        const char* j = R"({"a":{"b":{"c":{"e":1}},"d":{"c":{"e":1}}}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("string(/)", json));
        assert(r.getString() == "11");
        r = (eval("string(/a)", json));
        assert(r.getString() == "11");
        r = (eval("string(/a/b)", json));
        assert(r.getString() == "1");
        r = (eval("string(/a/b)", json));
        assert(r.getString() == "1");
        r = (eval("string(/a/b/c)", json));
        assert(r.getString() == "1");
        r = (eval("string(/a/b/c/e)", json));
        assert(r.getString() == "1");
        r = (eval("string(/a/b/c/e/z)", json));
        assert(r.getString() == "");
    }
    {
        // <a><b>1</b><c>true</c><d>foo</d></a>
        const char* j = R"({"a":{"b":1,"c":true,"d":"foo"}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("string(/)", json));
        assert(r.getString() == "1truefoo");
        r = eval("string(/a)", json);
        assert(r.getString() == "1truefoo");
        r = eval("string(/a/b)", json);
        assert(r.getString() == "1");
        r = eval("string(/a/c)", json);
        assert(r.getString() == "true");
        r = eval("string(/a/d)", json);
        assert(r.getString() == "foo");
    }
    {
        // <a><b>1</b><b>2</b><b>3</b><b>4</b></a>
        const char* j = R"({"a":{"b":[1,2,3,4]}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("string(/)", json));
        assert(r.getString() == "1234");
        r = eval("string(/a)", json);
        assert(r.getString() == "1234");
        r = eval("string(/a/b)", json);
        assert(r.getString() == "1");
        r = eval("string(//b)", json);
        assert(r.getString() == "1");
    }
}

void
testStringValue() {
    {
        // <a><b>1</b><b>2</b><b>3</b><b>4</b></a>
        const char* j = R"({"a":{"b":[1,2,3,4]}})";
        nlohmann::json json = nlohmann::json::parse(j);
        XpathData r(eval("/", json));
        assert(r.getStringValue() == "1234");
        r = eval("/a", json);
        assert(r.getString() == "1234");
        r = eval("/a/b", json);
        assert(r.getStringValue() == "1234");
        r = eval("//b", json);
        assert(r.getStringValue() == "1234");
    }
}

int
main (int argc, char *argv[])
{
    testNumbers();
    testLogic();
    testPaths();
    testRelations();
    testFilter();
    testNodeSetFunctions();
    testStringFunctions();
    testStringValue();
    return 0;
}
