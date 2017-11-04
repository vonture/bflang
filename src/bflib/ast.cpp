#include "ast.h"

#include <assert.h>
#include <unordered_map>

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
    assert(index >= 0 && index < children_.size());
    return children_[index].get();
}

int sequence_node::get_child_count() const { return children_.size(); }

operation_node::operation_node(const source_location &location, ast_operation_type type)
    : ast_node(bf_ast_operation, location)
    , operation_type_(type) {}

void operation_node::set_operation_type(ast_operation_type type) { operation_type_ = type; }

ast_operation_type operation_node::get_operation_type() const { return operation_type_; }

ast::ast(std::unique_ptr<sequence_node> root)
    : root_(std::move(root)) {}

sequence_node *ast::get_root() const { return root_.get(); }

ast *generate_ast(const tokens *tokens, const ast_options &options, error_stream &error_stream) {
    std::unique_ptr<sequence_node> root_sequence = std::make_unique<sequence_node>(source_location{0, 0});

    const std::unordered_map<token_type, ast_operation_type> operation_type_mapping = {
        {bf_token_inc_data_ptr, bf_ast_operation_inc_data_ptr},
        {bf_token_dec_data_ptr, bf_ast_operation_dec_data_ptr},
        {bf_token_inc_data_value, bf_ast_operation_inc_data_value},
        {bf_token_dec_data_value, bf_ast_operation_dec_data_value},
        {bf_token_input, bf_ast_operation_input},
        {bf_token_output, bf_ast_operation_output},
    };

    std::vector<sequence_node *> sequence_stack{root_sequence.get()};

    for (const token &token : *tokens) {
        auto operation_type_mapping_iter = operation_type_mapping.find(token.type);
        if (operation_type_mapping_iter != operation_type_mapping.end()) {
            sequence_stack.back()->add_child(std::make_unique<operation_node>(token.location, operation_type_mapping_iter->second));
        } else if (token.type == bf_token_loop_open) {
            sequence_node *current_sequence = sequence_stack.back();
            std::unique_ptr<sequence_node> new_sequence = std::make_unique<sequence_node>(token.location);
            sequence_stack.push_back(new_sequence.get());
            current_sequence->add_child(std::move(new_sequence));
        } else if (token.type == bf_token_loop_close) {
            if (sequence_stack.size() <= 1) {
                error_stream.emit_error(token.location, token.location, "unmatched loop closing brace.");
                return nullptr;
            }
            sequence_stack.pop_back();
        } else {
            assert(false);
        }
    }

    if (sequence_stack.size() > 1) {
        error_stream.emit_error(source_location{0, 0}, source_location{0, 0}, "uneven loop braces.");
        return nullptr;
    }

    return new ast(std::move(root_sequence));
}

} // namespace bf
