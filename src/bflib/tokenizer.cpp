#include "tokenizer.h"

#include <memory>

namespace bf {
void tokens::add_token(token_type type, source_location location) {
    token t;
    t.type = type;
    t.location = location;
    tokens_.emplace_back(t);
}

const std::vector<token> &tokens::get_tokens() const { return tokens_; }

tokens *tokenize(const char *program, const tokenizer_options *options, error_stream error_func) {
    if (program == nullptr) {
        return nullptr;
    }

    std::unique_ptr<bf::tokens> tokens = std::make_unique<bf::tokens>();

    const int source_length = std::strlen(program);

    int character_index = 0;
    source_location cur_source_location{0, 0};
    for (int character_index = 0; character_index < source_length; character_index++) {
        switch (program[character_index]) {
        case '>':
            tokens->add_token(bf_token_inc_data_ptr, cur_source_location);
            cur_source_location.character++;
            break;

        case '<':
            tokens->add_token(bf_token_dec_data_ptr, cur_source_location);
            cur_source_location.character++;
            break;

        case '+':
            tokens->add_token(bf_token_inc_data_value, cur_source_location);
            cur_source_location.character++;
            break;

        case '-':
            tokens->add_token(bf_token_dec_data_value, cur_source_location);
            cur_source_location.character++;
            break;

        case '.':
            tokens->add_token(bf_token_input, cur_source_location);
            cur_source_location.character++;
            break;

        case ',':
            tokens->add_token(bf_token_output, cur_source_location);
            cur_source_location.character++;
            break;

        case '[':
            tokens->add_token(bf_token_loop_open, cur_source_location);
            cur_source_location.character++;
            break;

        case ']':
            tokens->add_token(bf_token_loop_close, cur_source_location);
            cur_source_location.character++;
            break;

        case '\n':
            cur_source_location.line++;
            cur_source_location.character = 0;
            break;

        default:
            cur_source_location.character++;
            break;
        }
    }

    return tokens.release();
}

} // namespace bf
