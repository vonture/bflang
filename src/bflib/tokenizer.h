#ifndef BFLIB_TOKENIZER_H_
#define BFLIB_TOKENIZER_H_

#include "bflib.h"
#include "error_stream.h"

#include <vector>

namespace bf {
using token_type = bf_token_type;
using token = bf_token;
using tokenizer_options = bf_tokenizer_options;

tokenizer_options default_tokenizer_options();

class tokens {
  public:
    void add_token(token_type type, source_location location);
    const std::vector<token> &get_tokens() const;

  private:
    std::vector<token> tokens_;
};

tokens *tokenize(const char *program, const tokenizer_options &options, error_stream &error_stream);
} // namespace bf

#endif // BFLIB_TOKENIZER_H_
