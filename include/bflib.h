#ifndef BFLIB_H_
#define BFLIB_H_

typedef void (*bf_error_stream)(const char *);

enum bf_token_type {
    bf_token_inc_data_ptr,
    bf_token_dec_data_ptr,
    bf_token_inc_data_value,
    bf_token_dec_data_value,
    bf_token_input,
    bf_token_output,
    bf_token_loop_open,
    bf_token_loop_close,
};

struct bf_token {
    bf_token_type type;
    int line;
    int character;
};

typedef void *bf_tokens;

bf_tokens bf_tokenize(const char *program, bf_error_stream error_stream);
int bf_tokens_get_token_count(bf_tokens tokens);
bool bf_tokens_get_token_count(bf_tokens tokens, int count, bf_token *token_ptr);
bool bf_tokens_destroy(bf_tokens tokens);

#endif // BFLIB_H_
