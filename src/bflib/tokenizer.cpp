#include "tokenizer.h"

#include <assert.h>
#include <memory>

namespace bf {
void tokens::add_token(token_type type, source_location location) {
    token t;
    t.type = type;
    t.location = location;
    tokens_.emplace_back(t);
}

const token &tokens::get_token(int index) const {
    assert(index >= 0 && static_cast<size_t>(index) < tokens_.size());
    return tokens_[index];
}

int tokens::get_tokens_count() const { return tokens_.size(); }

tokens::const_iterator tokens::begin() const { return tokens_.begin(); }

tokens::const_iterator tokens::end() const { return tokens_.end(); }

tokenizer_options default_tokenizer_options() {
    tokenizer_options options;
    return options;
}

tokens *tokenize(const char *program, const tokenizer_options &options, error_stream &error_stream) {
    assert(program != nullptr);

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
            tokens->add_token(bf_token_output, cur_source_location);
            cur_source_location.character++;
            break;

        case ',':
            tokens->add_token(bf_token_input, cur_source_location);
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
