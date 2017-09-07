#include "bflib.h"
#include "bflib_raii.h"

#include <gtest/gtest.h>

namespace bf {

TEST(ast, null_tokens) { ASSERT_EQ(nullptr, bf_generate_ast(nullptr, nullptr, nullptr, nullptr)); }

} // namespace bf
