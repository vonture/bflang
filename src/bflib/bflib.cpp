#include "bflib.h"

#include "error_stream.h"
#include "tokenizer.h"

#include <exception>

bf_tokens *bf_tokenize(const char *program, const bf_tokenizer_options *options, bf_user_data user_data, bf_error_func error_func) {
    if (program == nullptr) {
        return nullptr;
    }

    bf::error_stream error_stream(user_data, error_func);
    const bf::tokenizer_options &resolved_options = options ? *options : bf::default_tokenizer_options();
    return bf::tokenize(program, resolved_options, error_stream);
}

bool bf_tokens_get_token_count(bf_tokens *tokens, int *count) {
    if (tokens == nullptr || count == nullptr) {
        return false;
    }

    bf::tokens *tokens_object = reinterpret_cast<bf::tokens *>(tokens);
    *count = static_cast<int>(tokens_object->get_tokens().size());
    return true;
}

bool bf_tokens_get_tokens(bf_tokens *tokens, int first, int count, bf_token *token_ptr) {
    if (tokens == nullptr || first < 0 || count < 0 || token_ptr == nullptr) {
        return false;
    }

    const auto &tokens_vector = reinterpret_cast<bf::tokens *>(tokens)->get_tokens();
    if (first + count > static_cast<int>(tokens_vector.size())) {
        return false;
    }

    std::copy(tokens_vector.begin() + first, tokens_vector.begin() + first + count, token_ptr);
    return true;
}

bool bf_tokens_destroy(bf_tokens *tokens) {
    if (tokens == nullptr) {
        return false;
    }

    bf::tokens *tokens_object = reinterpret_cast<bf::tokens *>(tokens);
    delete tokens_object;

    return true;
}
