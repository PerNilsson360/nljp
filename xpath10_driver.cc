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

#include "xpath10_driver.hh"
#include "xpath10_parser.tab.h"

xpath10_driver::xpath10_driver() :
    trace_scanning(false), trace_parsing(false) {
}

xpath10_driver::~xpath10_driver() {}

int
xpath10_driver::parse (const std::string& s) {
    xpath = s;
    scan_begin ();
    yy::xpath10_parser parser (*this);
    parser.set_debug_level (trace_parsing);
    int res = parser.parse ();
    scan_end ();
    return res;
}

void
xpath10_driver::error (const yy::location& l, const std::string& m) {
    std::cerr << l << ": " << m << std::endl;
}

void
xpath10_driver::error (const std::string& m) {
    std::cerr << m << std::endl;
}
