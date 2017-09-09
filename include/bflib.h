#ifndef BFLIB_H_
#define BFLIB_H_

#ifdef __cplusplus
extern "C" {
#endif

enum bf_token_type {
    bf_token_inc_data_ptr,   // >
    bf_token_dec_data_ptr,   // <
    bf_token_inc_data_value, // +
    bf_token_dec_data_value, // -
    bf_token_input,          // .
    bf_token_output,         // ,
    bf_token_loop_open,      // [
    bf_token_loop_close      // ]
};

struct bf_source_location {
    int line;
    int character;
};

struct bf_token {
    bf_token_type type;
    bf_source_location location;
};

typedef void *bf_user_data;
typedef void (*bf_error_func)(bf_user_data user_data, bf_source_location start_location, bf_source_location end_location,
                              const char *message);

struct bf_tokenizer_options {};

typedef void bf_tokens;

bf_tokens *bf_tokenize(const char *program, const bf_tokenizer_options *options, bf_user_data user_data, bf_error_func error_func);
bool bf_tokens_destroy(bf_tokens *tokens);
bool bf_tokens_get_token_count(const bf_tokens *tokens, int *count);
bool bf_tokens_get_tokens(const bf_tokens *tokens, int first, int count, bf_token *token_ptr);

enum bf_ast_node_type {
    bf_ast_sequence,
    bf_ast_branch,
    bf_ast_operation,
};

enum bf_ast_operation_type {
    bf_ast_operation_inc_data_ptr,
    bf_ast_operation_dec_data_ptr,
    bf_ast_operation_inc_data_value,
    bf_ast_operation_dec_data_value,
    bf_ast_operation_input,
    bf_ast_operation_output,
};

struct bf_ast_options {};

typedef void bf_ast;
typedef void bf_ast_node;

bf_ast *bf_generate_ast(const bf_tokens *tokens, const bf_ast_options *options, bf_user_data user_data, bf_error_func error_func);
bool bf_ast_destroy(bf_ast *ast);
bool bf_ast_get_root_node(const bf_ast *ast, bf_ast_node **root);

bool bf_ast_node_get_type(const bf_ast_node *node, bf_ast_node_type *type);
bool bf_ast_node_get_location(const bf_ast_node *node, bf_source_location *location);
bool bf_ast_node_sequence_get_child_count(const bf_ast_node *node, int *count);
bool bf_ast_node_sequence_get_children(const bf_ast_node *node, int first, int count, bf_ast_node const **children);
bool bf_ast_node_branch_get_child(const bf_ast_node *node, bf_ast_node const **child);
bool bf_ast_node_operation_get_operation_type(const bf_ast_node *node, bf_ast_operation_type *type);

#ifdef __cplusplus
}
#endif

#endif // BFLIB_H_
