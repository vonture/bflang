#ifndef BFLIB_AST_H_
#define BFLIB_AST_H_

#include "bflib.h"
#include "error_stream.h"
#include "tokenizer.h"

#include <memory>

namespace bf {
using ast_node_type = bf_ast_node_type;
using ast_operation_type = bf_ast_operation_type;
using ast_options = bf_ast_options;

bf_ast_options default_ast_options();

class ast_node {
  public:
    ast_node(ast_node_type type, const source_location &location);

    ast_node_type get_type() const;
    const source_location &get_location() const;

  private:
    ast_node_type type_;
    source_location location_;
};

class sequence_node : public ast_node {
  public:
    sequence_node(const source_location &location);

    void add_child(std::unique_ptr<ast_node> child);

    const ast_node *get_child(int index) const;
    int get_child_count() const;

  private:
    std::vector<std::unique_ptr<ast_node>> children_;
};

class branch_node : public ast_node {
  public:
    branch_node(const source_location &location);

    void set_child(std::unique_ptr<sequence_node> child);
    const sequence_node *get_child() const;

  private:
    std::unique_ptr<sequence_node> child_;
};

class operation_node : public ast_node {
  public:
    operation_node(const source_location &location, ast_operation_type type);

    void set_operation_type(ast_operation_type type);
    ast_operation_type get_operation_type() const;

  private:
    ast_operation_type operation_type_;
};

class ast {
  public:
    ast(std::unique_ptr<sequence_node> root);

    sequence_node *get_root() const;

  private:
    std::unique_ptr<sequence_node> root_;
};

ast *generate_ast(const tokens *tokens, const ast_options &options, error_stream &error_stream);
} // namespace bf

#endif // BFLIB_AST_H_
