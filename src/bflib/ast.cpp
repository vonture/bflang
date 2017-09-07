#include "ast.h"

#include <assert.h>

namespace bf {

bf_ast_options default_ast_options() {
    bf_ast_options options;
    return options;
}

ast::ast(ast_type type, const source_location &location)
    : type_(type)
    , location_(location) {}

ast_type ast::get_type() const { return type_; }

const source_location &ast::get_location() const { return location_; }

sequence::sequence(const source_location &location)
    : ast(bf_ast_sequence, location) {}

void sequence::add_child(std::unique_ptr<ast> child) { children_.push_back(std::move(child)); }

const ast *sequence::get_child(int index) const {
    assert(index > 0 && index < children_.size());
    return children_[index].get();
}

int sequence::get_child_count() const { return children_.size(); }

branch::branch(const source_location &location)
    : ast(bf_ast_branch, location) {}

void branch::set_child(std::unique_ptr<sequence> child) { child_ = std::move(child); }

const sequence *branch::get_child() const { return child_.get(); }

operation::operation(const source_location &location, ast_operation_type type)
    : ast(bf_ast_operation, location)
    , operation_type_(type) {}

void operation::set_operation_type(ast_operation_type type) { operation_type_ = type; }

ast_operation_type operation::get_operation_type() const { return operation_type_; }

ast *generate_ast(tokens *tokens, const ast_options &options, error_stream &error_stream) { return nullptr; }
} // namespace bf
