#include "bflib.h"

#include <array>
#include <gtest/gtest.h>
#include <memory>

namespace {

struct tokens_deleter {
    void operator()(bf_tokens *tokens) {
        if (tokens) {
            EXPECT_TRUE(bf_tokens_destroy(tokens));
        }
    }
};

using unique_tokens_ptr = std::unique_ptr<bf_tokens, tokens_deleter>;

TEST(tokenizer, null_program) { ASSERT_EQ(nullptr, bf_tokenize(nullptr, nullptr, nullptr, nullptr)); }

TEST(tokenizer, null_tokens) {
    int count = 0;
    EXPECT_FALSE(bf_tokens_get_token_count(nullptr, &count));

    bf_token token;
    EXPECT_FALSE(bf_tokens_get_tokens(nullptr, 0, 1, &token));

    EXPECT_FALSE(bf_tokens_destroy(nullptr));
}

TEST(tokenizer, invalid_offsets) {
    unique_tokens_ptr tokens(bf_tokenize("++", nullptr, nullptr, nullptr));
    ASSERT_NE(nullptr, tokens);

    bf_token tokens_array[4];
    EXPECT_TRUE(bf_tokens_get_tokens(tokens.get(), 0, 1, tokens_array));
    EXPECT_TRUE(bf_tokens_get_tokens(tokens.get(), 0, 0, tokens_array));
    EXPECT_FALSE(bf_tokens_get_tokens(tokens.get(), -1, 1, tokens_array));
    EXPECT_FALSE(bf_tokens_get_tokens(tokens.get(), 0, 3, tokens_array));
    EXPECT_FALSE(bf_tokens_get_tokens(tokens.get(), 1, 2, tokens_array));
    EXPECT_FALSE(bf_tokens_get_tokens(tokens.get(), 2, 1, tokens_array));
    EXPECT_FALSE(bf_tokens_get_tokens(tokens.get(), 1, -1, tokens_array));
    EXPECT_FALSE(bf_tokens_get_tokens(tokens.get(), 1, 1, nullptr));
}

TEST(tokenizer, hello_world) {
    constexpr const char *program =
        "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";
    unique_tokens_ptr tokens(bf_tokenize(program, nullptr, nullptr, nullptr));
    ASSERT_NE(nullptr, tokens);

    int token_count = -1;
    ASSERT_TRUE(bf_tokens_get_token_count(tokens.get(), &token_count));
    ASSERT_EQ(strlen(program), token_count);

    std::array<bf_token, 10> token_array;
    ASSERT_TRUE(bf_tokens_get_tokens(tokens.get(), 0, token_array.size(), token_array.data()));

    EXPECT_EQ(bf_token_inc_data_value, token_array[0].type);
    EXPECT_EQ(0, token_array[0].location.line);
    EXPECT_EQ(0, token_array[0].location.character);

    EXPECT_EQ(bf_token_inc_data_value, token_array[5].type);
    EXPECT_EQ(0, token_array[5].location.line);
    EXPECT_EQ(5, token_array[5].location.character);

    EXPECT_EQ(bf_token_loop_open, token_array[8].type);
    EXPECT_EQ(0, token_array[8].location.line);
    EXPECT_EQ(8, token_array[8].location.character);
}

TEST(tokenizer, token_types) {
    constexpr const char *program = "><+-.,[]";
    unique_tokens_ptr tokens(bf_tokenize(program, nullptr, nullptr, nullptr));
    ASSERT_NE(nullptr, tokens);

    int token_count = -1;
    ASSERT_TRUE(bf_tokens_get_token_count(tokens.get(), &token_count));
    ASSERT_EQ(8, token_count);

    std::array<bf_token, 8> token_array;
    ASSERT_TRUE(bf_tokens_get_tokens(tokens.get(), 0, token_array.size(), token_array.data()));

    EXPECT_EQ(bf_token_inc_data_ptr, token_array[0].type);
    EXPECT_EQ(bf_token_dec_data_ptr, token_array[1].type);
    EXPECT_EQ(bf_token_inc_data_value, token_array[2].type);
    EXPECT_EQ(bf_token_dec_data_value, token_array[3].type);
    EXPECT_EQ(bf_token_input, token_array[4].type);
    EXPECT_EQ(bf_token_output, token_array[5].type);
    EXPECT_EQ(bf_token_loop_open, token_array[6].type);
    EXPECT_EQ(bf_token_loop_close, token_array[7].type);
}

TEST(tokenizer, other_characters_ignored) {
    constexpr const char *program = "saa>\nsah<bwer+c-rj      \t    d.f,gjh\nhdf[e]f";
    unique_tokens_ptr tokens(bf_tokenize(program, nullptr, nullptr, nullptr));
    ASSERT_NE(nullptr, tokens);

    int token_count = -1;
    ASSERT_TRUE(bf_tokens_get_token_count(tokens.get(), &token_count));
    ASSERT_EQ(8, token_count);

    std::array<bf_token, 8> token_array;
    ASSERT_TRUE(bf_tokens_get_tokens(tokens.get(), 0, token_array.size(), token_array.data()));

    EXPECT_EQ(bf_token_inc_data_ptr, token_array[0].type);
    EXPECT_EQ(bf_token_dec_data_ptr, token_array[1].type);
    EXPECT_EQ(bf_token_inc_data_value, token_array[2].type);
    EXPECT_EQ(bf_token_dec_data_value, token_array[3].type);
    EXPECT_EQ(bf_token_input, token_array[4].type);
    EXPECT_EQ(bf_token_output, token_array[5].type);
    EXPECT_EQ(bf_token_loop_open, token_array[6].type);
    EXPECT_EQ(bf_token_loop_close, token_array[7].type);
}

TEST(tokenizer, token_location) {
    constexpr const char *program = ">\n<\n+\n-\n.\n,\n[\n]\n";
    unique_tokens_ptr tokens(bf_tokenize(program, nullptr, nullptr, nullptr));
    ASSERT_NE(nullptr, tokens);

    int token_count = -1;
    ASSERT_TRUE(bf_tokens_get_token_count(tokens.get(), &token_count));
    ASSERT_EQ(8, token_count);

    std::array<bf_token, 8> token_array;
    ASSERT_TRUE(bf_tokens_get_tokens(tokens.get(), 0, token_array.size(), token_array.data()));

    for (int i = 0; i < token_array.size(); i++) {
        EXPECT_EQ(i, token_array[i].location.line);
        EXPECT_EQ(0, token_array[i].location.character);
    }
}

} // namespace
