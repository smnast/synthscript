#include "cout_redirect.h"
#include <functional>
#include <iostream>
#include <sstream>

CoutRedirect::CoutRedirect() : old_buf(std::cout.rdbuf()), ss() {}

CoutRedirect::~CoutRedirect() {
    end();
}

void CoutRedirect::begin() {
    std::cout.rdbuf(ss.rdbuf());
}

void CoutRedirect::end() {
    std::cout.rdbuf(old_buf);
}

std::string CoutRedirect::get_string() const {
    return ss.str();
}

void CoutRedirect::clear() {
    ss.str("");
    ss.clear();
}

void CoutRedirect::run(std::function<void()> func) {
    begin();
    func();
    end();
}
