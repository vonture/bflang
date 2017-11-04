#include "stream.h"

#include <string>

namespace bf {
error_stream::error_stream(bf_user_data user_data, bf_error_func error_func)
    : user_data_(user_data)
    , error_func_(error_func) {}

void error_stream::emit_error(const source_location &start_loc, const source_location &end_loc, const std::string &msg) {
    if (error_func_) {
        error_func_(user_data_, start_loc, end_loc, msg.c_str());
    }
}

input_stream::input_stream(bf_user_data user_data, bf_input_function input_func)
    : user_data_(user_data)
    , input_func_(input_func) {}

char input_stream::read() {
    if (input_func_) {
        return input_func_(user_data_);
    } else {
        return std::char_traits<char>::eof();
    }
}

output_stream::output_stream(bf_user_data user_data, bf_output_function output_func)
    : user_data_(user_data)
    , output_func_(output_func) {}

void output_stream::write(char c) {
    if (output_func_) {
        output_func_(user_data_, c);
    }
}

} // namespace bf
