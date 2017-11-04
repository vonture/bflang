#include "bflib.h"
#include "bflib_raii.h"

#include <array>

#include <gtest/gtest.h>

namespace bf {

namespace {

unique_ast_ptr create_ast_from_source(const char *source, const bf_tokenizer_options *token_options, const bf_ast_options *ast_options,
                                      bf_user_data user_data, bf_error_func error_func) {
    unique_tokens_ptr tokens(bf_tokenize(source, token_options, user_data, error_func));
    if (tokens == nullptr) {
        return nullptr;
    }

    return unique_ast_ptr(bf_generate_ast(tokens.get(), ast_options, user_data, error_func));
}

} // namespace

TEST(ast, null_tokens) { ASSERT_EQ(nullptr, bf_generate_ast(nullptr, nullptr, nullptr, nullptr)); }

TEST(ast, null_ast) {
    unique_ast_ptr valid_ast = create_ast_from_source("", nullptr, nullptr, nullptr, nullptr);
    ASSERT_NE(nullptr, valid_ast);

    EXPECT_FALSE(bf_ast_destroy(nullptr));

    const bf_ast_node *root = nullptr;
    EXPECT_FALSE(bf_ast_get_root_node(nullptr, &root));
    EXPECT_FALSE(bf_ast_get_root_node(valid_ast.get(), nullptr));

    bf_ast_node_type type;
    EXPECT_FALSE(bf_ast_node_get_type(nullptr, &type));
    EXPECT_FALSE(bf_ast_node_get_type(valid_ast.get(), nullptr));
}

TEST(ast, unmatched_braces) {
    EXPECT_EQ(nullptr, create_ast_from_source("[", nullptr, nullptr, nullptr, nullptr));
    EXPECT_EQ(nullptr, create_ast_from_source("[[]", nullptr, nullptr, nullptr, nullptr));
    EXPECT_EQ(nullptr, create_ast_from_source("[][[][][]", nullptr, nullptr, nullptr, nullptr));
    EXPECT_EQ(nullptr, create_ast_from_source("][][][", nullptr, nullptr, nullptr, nullptr));
    EXPECT_EQ(nullptr, create_ast_from_source("[[[]]][][", nullptr, nullptr, nullptr, nullptr));
}

TEST(ast, basic_functionality) {
    unique_ast_ptr ast(create_ast_from_source(".,", nullptr, nullptr, nullptr, nullptr));

    const bf_ast_node *root = nullptr;
    EXPECT_TRUE(bf_ast_get_root_node(ast.get(), &root));
    EXPECT_NE(nullptr, root);

    {
        bf_ast_node_type root_type;
        EXPECT_TRUE(bf_ast_node_get_type(root, &root_type));
        EXPECT_EQ(bf_ast_sequence, root_type);
    }

    {
        bf_source_location location;
        EXPECT_TRUE(bf_ast_node_get_location(root, &location));
        EXPECT_EQ(0, location.line);
        EXPECT_EQ(0, location.character);
    }

    {
        constexpr int expected_child_count = 2;
        int child_count = 0;
        EXPECT_TRUE(bf_ast_node_sequence_get_child_count(root, &child_count));
        ASSERT_EQ(expected_child_count, child_count);

        std::array<const bf_ast_node *, expected_child_count> children;
        EXPECT_TRUE(bf_ast_node_sequence_get_children(root, 0, expected_child_count, children.data()));

        std::array<std::pair<bf_ast_operation_type, bf_source_location>, expected_child_count> expected_child_operation_types{{
            {bf_ast_operation_output, {0, 0}}, {bf_ast_operation_input, {0, 1}},
        }};
        for (int i = 0; i < expected_child_count; i++) {
            bf_ast_node_type node_type;
            EXPECT_TRUE(bf_ast_node_get_type(children[i], &node_type));
            EXPECT_EQ(bf_ast_operation, node_type);

            bf_ast_operation_type operation_type;
            EXPECT_TRUE(bf_ast_node_operation_get_type(children[i], &operation_type));
            EXPECT_EQ(expected_child_operation_types[i].first, operation_type);

            bf_source_location location;
            EXPECT_TRUE(bf_ast_node_get_location(children[i], &location));
            EXPECT_EQ(expected_child_operation_types[i].second.line, location.line);
            EXPECT_EQ(expected_child_operation_types[i].second.character, location.character);
        }
    }
}

} // namespace bf
