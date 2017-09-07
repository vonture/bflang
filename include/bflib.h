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
bool bf_tokens_get_token_count(bf_tokens *tokens, int *count);
bool bf_tokens_get_tokens(bf_tokens *tokens, int first, int count, bf_token *token_ptr);
bool bf_tokens_destroy(bf_tokens *tokens);

enum bf_ast_type {
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

bf_ast *bf_generate_ast(bf_tokens *tokens, const bf_ast_options *options, bf_user_data user_data, bf_error_func error_func);
bool bf_ast_get_type(bf_ast *ast, bf_ast_type *type);
bool bf_ast_get_location(bf_ast *ast, bf_source_location *location);
bool bf_ast_sequence_get_child_count(bf_ast *ast, int *count);
bool bf_ast_sequence_get_children(bf_ast *ast, int first, int count, bf_ast *children);
bool bf_ast_branch_get_child(bf_ast *ast, bf_ast *child);
bool bf_ast_operation_get_operation_type(bf_ast *ast, bf_ast_operation_type *type);
bool bf_ast_destroy(bf_ast *ast);

#ifdef __cplusplus
}
#endif

#endif // BFLIB_H_
