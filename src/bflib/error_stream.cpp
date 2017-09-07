#include "error_stream.h"

namespace bf {
error_stream::error_stream(bf_user_data user_data, bf_error_func error_func)
    : user_data_(user_data_)
    , error_func_(error_func) {}

void error_stream::emit_error(const source_location &start_loc, const source_location &end_loc, const std::string &msg) {
    if (error_func_) {
        error_func_(user_data_, start_loc, end_loc, msg.c_str());
    }
}
} // namespace bf
