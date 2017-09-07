#ifndef BFLIB_AST_H_
#define BFLIB_AST_H_

#include "bflib.h"
#include "error_stream.h"
#include "tokenizer.h"

#include <memory>

namespace bf {
using ast_type = bf_ast_type;
using ast_operation_type = bf_ast_operation_type;
using ast_options = bf_ast_options;

bf_ast_options default_ast_options();

class ast {
  public:
    ast(ast_type type, const source_location &location);

    ast_type get_type() const;
    const source_location &get_location() const;

  private:
    ast_type type_;
    source_location location_;
};

class sequence : public ast {
  public:
    sequence(const source_location &location);

    void add_child(std::unique_ptr<ast> child);

    const ast *get_child(int index) const;
    int get_child_count() const;

  private:
    std::vector<std::unique_ptr<ast>> children_;
};

class branch : public ast {
  public:
    branch(const source_location &location);

    void set_child(std::unique_ptr<sequence> child);
    const sequence *get_child() const;

  private:
    std::unique_ptr<sequence> child_;
};

class operation : public ast {
  public:
    operation(const source_location &location, ast_operation_type type);

    void set_operation_type(ast_operation_type type);
    ast_operation_type get_operation_type() const;

  private:
    ast_operation_type operation_type_;
};

ast *generate_ast(tokens *tokens, const ast_options &options, error_stream &error_stream);
} // namespace bf

#endif // BFLIB_AST_H_
