#include "stream_redirect.h"
#include <functional>
#include <iostream>
#include <sstream>

StreamRedirect::StreamRedirect() : old_cout_buf(nullptr), old_cin_buf(nullptr) {}

StreamRedirect::~StreamRedirect() {
    end();
}

void StreamRedirect::begin() {
    old_cout_buf = std::cout.rdbuf();
    old_cin_buf = std::cin.rdbuf();

    std::cout.rdbuf(cout_ss.rdbuf());
    std::cin.rdbuf(cin_ss.rdbuf());
}

void StreamRedirect::end() {
    if (old_cout_buf && old_cin_buf) {
        std::cout.rdbuf(old_cout_buf);
        std::cin.rdbuf(old_cin_buf);

        old_cout_buf = nullptr;
        old_cin_buf = nullptr;
    }
}

std::string StreamRedirect::get_string() const {
    return cout_ss.str();
}

void StreamRedirect::give_string(std::string str) {
    cin_ss.str(str);
    cin_ss.clear();
    cin_ss.seekg(0);
}

void StreamRedirect::clear() {
    cout_ss.str("");
    cout_ss.clear();
    cin_ss.str("");
    cin_ss.clear();
}

void StreamRedirect::run(std::function<void()> func) {
    begin();
    func();
    end();
}
