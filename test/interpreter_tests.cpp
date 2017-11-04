#include "bflib.h"
#include "bflib_raii.h"

#include <sstream>

#include <gtest/gtest.h>

namespace bf {

namespace {
class input_output_stream {
  public:
    input_output_stream(const char *input)
        : input_(input)
        , output_() {}

    char read() { return input_.get(); }
    void write(char c) { output_.put(c); }
    std::string str() const { return output_.str(); }

    static char input_function(bf_user_data user_data) {
        input_output_stream *stream = reinterpret_cast<input_output_stream *>(user_data);
        return stream->read();
    }

    static void output_function(bf_user_data user_data, char c) {
        input_output_stream *stream = reinterpret_cast<input_output_stream *>(user_data);
        return stream->write(c);
    }

  private:
    std::istringstream input_;
    std::ostringstream output_;
};

std::string interpret_source(const char *source, const char *input, const bf_tokenizer_options *token_options,
                             const bf_ast_options *ast_options, const bf_interpreter_options *interpreter_options) {
    unique_tokens_ptr tokens(bf_tokenize(source, token_options, nullptr, nullptr));
    EXPECT_NE(nullptr, tokens);
    if (tokens == nullptr) {
        return "";
    }

    unique_ast_ptr ast(bf_generate_ast(tokens.get(), ast_options, nullptr, nullptr));
    EXPECT_NE(nullptr, ast);
    if (ast == nullptr) {
        return "";
    }

    unique_interpreter_ptr interpreter(bf_create_interpreter(interpreter_options));
    EXPECT_NE(nullptr, interpreter);
    if (interpreter == nullptr) {
        return "";
    }

    input_output_stream stream(input);
    EXPECT_TRUE(bf_interpreter_execute(interpreter.get(), ast.get(), &stream, input_output_stream::input_function,
                                       input_output_stream::output_function, nullptr));
    return stream.str();
}
} // namespace

TEST(interpreter, adder) {
    constexpr const char *program = ",>,<[->+<]>.";
    constexpr const char input[] = {
        1, 2, 0,
    };
    constexpr const char expected_output[] = {
        3, 0,
    };
    EXPECT_EQ(expected_output, interpret_source(program, input, nullptr, nullptr, nullptr));
}

TEST(interpreter, hello_world) {
    constexpr const char *program =
        "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";
    EXPECT_EQ("Hello World!\n", interpret_source(program, "", nullptr, nullptr, nullptr));
}

} // namespace bf
