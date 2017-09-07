#ifndef BFLIB_ERROR_STREAM_H_
#define BFLIB_ERROR_STREAM_H_

#include "bflib.h"

#include <string>

namespace bf {
using source_location = bf_source_location;

class error_stream {
  public:
    error_stream(bf_user_data user_data, bf_error_func error_func);

    void emit_error(const source_location &start_loc, const source_location &end_loc, const std::string &msg);

  private:
    bf_user_data user_data_;
    bf_error_func error_func_;
};

} // namespace bf

#endif // BFLIB_ERROR_STREAM_H_
