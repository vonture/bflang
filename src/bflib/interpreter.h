#ifndef BFLIB_INTERPRETER_H_
#define BFLIB_INTERPRETER_H_

#include "ast.h"
#include "bflib.h"
#include "stream.h"

namespace bf {
using interpreter_options = bf_interpreter_options;

interpreter_options default_interpreter_options();

class interpreter_instance {
  public:
    virtual void execute(input_stream &input_stream, output_stream &output_stream, error_stream &error_stream) = 0;
};

class interpreter {
  public:
    interpreter(const interpreter_options &options);

    std::unique_ptr<interpreter_instance> create_instance(const ast *ast) const;

  private:
    interpreter_options options_;
};

std::unique_ptr<interpreter> create_interpreter(const interpreter_options &options);

} // namespace bf

#endif // BFLIB_INTERPRETER_H_
