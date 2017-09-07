#ifndef BFLIB_H_
#define BFLIB_H_

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

#endif // BFLIB_H_
