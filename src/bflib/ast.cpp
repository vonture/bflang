#include "ast.h"

#include <assert.h>

namespace bf {

bf_ast_options default_ast_options() {
    bf_ast_options options;
    return options;
}
ast_node::ast_node(ast_node_type type, const source_location &location)
    : type_(type)
    , location_(location) {}

ast_node_type ast_node::get_type() const { return type_; }

const source_location &ast_node::get_location() const { return location_; }

sequence_node::sequence_node(const source_location &location)
    : ast_node(bf_ast_sequence, location) {}

void sequence_node::add_child(std::unique_ptr<ast_node> child) { children_.push_back(std::move(child)); }

const ast_node *sequence_node::get_child(int index) const {
    assert(index > 0 && index < children_.size());
    return children_[index].get();
}

int sequence_node::get_child_count() const { return children_.size(); }

branch_node::branch_node(const source_location &location)
    : ast_node(bf_ast_branch, location) {}

void branch_node::set_child(std::unique_ptr<sequence_node> child) { child_ = std::move(child); }

const sequence_node *branch_node::get_child() const { return child_.get(); }

operation_node::operation_node(const source_location &location, ast_operation_type type)
    : ast_node(bf_ast_operation, location)
    , operation_type_(type) {}

void operation_node::set_operation_type(ast_operation_type type) { operation_type_ = type; }

ast_operation_type operation_node::get_operation_type() const { return operation_type_; }

ast::ast(std::unique_ptr<sequence_node> root)
    : root_(std::move(root)) {}

sequence_node *ast::get_root() const { return root_.get(); }

ast *generate_ast(const tokens *tokens, const ast_options &options, error_stream &error_stream) { return nullptr; }

} // namespace bf
