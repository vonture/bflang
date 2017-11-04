#ifndef BFLIB_RAII_H_
#define BFLIB_RAII_H_

#include "bflib.h"

#ifdef __cplusplus

#include <memory>

namespace bf {
namespace impl {
template <typename object_type> using destroy_function = bool (*)(object_type *);

template <typename object_type, destroy_function<object_type> func> struct deleter {
    void operator()(object_type *object) {
        if (object) {
            func(object);
        }
    }
};
} // namespace impl

using unique_tokens_ptr = std::unique_ptr<bf_tokens, impl::deleter<bf_tokens, bf_tokens_destroy>>;
using unique_ast_ptr = std::unique_ptr<bf_ast, impl::deleter<bf_ast, bf_ast_destroy>>;
using unique_interpreter_ptr = std::unique_ptr<bf_interpreter, impl::deleter<bf_interpreter, bf_interpreter_destroy>>;
} // namespace bf

#endif // __cplusplus

#endif // BFLIB_RAII_H_
