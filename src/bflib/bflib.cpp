#include "bflib.h"

#include "ast.h"
#include "error_stream.h"
#include "tokenizer.h"

#include <exception>

bf_tokens *bf_tokenize(const char *program, const bf_tokenizer_options *options, bf_user_data user_data, bf_error_func error_func) {
    if (program == nullptr) {
        return nullptr;
    }

    bf::error_stream error_stream(user_data, error_func);
    const bf::tokenizer_options &resolved_options = options ? *options : bf::default_tokenizer_options();
    return bf::tokenize(program, resolved_options, error_stream);
}

bool bf_tokens_destroy(bf_tokens *tokens) {
    if (tokens == nullptr) {
        return false;
    }

    bf::tokens *tokens_object = reinterpret_cast<bf::tokens *>(tokens);
    delete tokens_object;

    return true;
}

bool bf_tokens_get_token_count(const bf_tokens *tokens, int *count) {
    if (tokens == nullptr || count == nullptr) {
        return false;
    }

    const bf::tokens *tokens_object = reinterpret_cast<const bf::tokens *>(tokens);
    *count = tokens_object->get_tokens_count();
    return true;
}

bool bf_tokens_get_tokens(const bf_tokens *tokens, int first, int count, bf_token *token_ptr) {
    if (tokens == nullptr || first < 0 || count < 0 || token_ptr == nullptr) {
        return false;
    }

    const bf::tokens *tokens_object = reinterpret_cast<const bf::tokens *>(tokens);
    if (first + count > tokens_object->get_tokens_count()) {
        return false;
    }

    for (int i = 0; i < count; i++) {
        token_ptr[i] = tokens_object->get_token(first + i);
    }

    return true;
}

bf_ast *bf_generate_ast(const bf_tokens *tokens, const bf_ast_options *options, bf_user_data user_data, bf_error_func error_func) {
    if (tokens == nullptr) {
        return nullptr;
    }

    const bf::tokens *tokens_object = reinterpret_cast<const bf::tokens *>(tokens);

    bf::error_stream error_stream(user_data, error_func);
    const bf::ast_options &resolved_options = options ? *options : bf::default_ast_options();
    return bf::generate_ast(tokens_object, resolved_options, error_stream);
}

bool bf_ast_destroy(bf_ast *ast) {
    if (ast == nullptr) {
        return false;
    }

    bf::ast *ast_object = reinterpret_cast<bf::ast *>(ast);
    delete ast_object;

    return true;
}

bool bf_ast_get_root_node(const bf_ast *ast, const bf_ast_node **root) {
    if (ast == nullptr || root == nullptr) {
        return false;
    }

    const bf::ast *ast_object = reinterpret_cast<const bf::ast *>(ast);
    *root = ast_object->get_root();
    return true;
}

bool bf_ast_node_get_type(const bf_ast_node *node, bf_ast_node_type *type) {
    if (node == nullptr || type == nullptr) {
        return false;
    }

    const bf::ast_node *ast_node_object = reinterpret_cast<const bf::ast_node *>(node);
    *type = ast_node_object->get_type();
    return true;
}

bool bf_ast_node_get_location(const bf_ast_node *node, bf_source_location *location) {
    if (node == nullptr || location == nullptr) {
        return false;
    }

    const bf::ast_node *node_object = reinterpret_cast<const bf::ast_node *>(node);
    *location = node_object->get_location();
    return true;
}

bool bf_ast_node_sequence_get_child_count(const bf_ast_node *node, int *count) {
    if (node == nullptr || count == nullptr) {
        return false;
    }

    const bf::ast_node *node_object = reinterpret_cast<const bf::ast_node *>(node);
    if (node_object->get_type() != bf_ast_sequence) {
        return false;
    }

    const bf::sequence_node *sequence_node = static_cast<const bf::sequence_node *>(node_object);
    *count = sequence_node->get_child_count();
    return true;
}

bool bf_ast_node_sequence_get_children(const bf_ast_node *node, int first, int count, const bf_ast_node **children) {
    if (node == nullptr || first < 0 || count < 0 || children == nullptr) {
        return false;
    }

    const bf::ast_node *node_object = reinterpret_cast<const bf::ast_node *>(node);
    if (node_object->get_type() != bf_ast_sequence) {
        return false;
    }

    const bf::sequence_node *sequence_node = static_cast<const bf::sequence_node *>(node_object);
    if (first + count > sequence_node->get_child_count()) {
        return false;
    }

    for (int i = 0; i < count; i++) {
        children[i] = sequence_node->get_child(first + i);
    }

    return true;
}

bool bf_ast_node_operation_get_type(const bf_ast_node *node, bf_ast_operation_type *type) {
    if (node == nullptr || type == nullptr) {
        return false;
    }

    const bf::ast_node *node_object = reinterpret_cast<const bf::ast_node *>(node);
    if (node_object->get_type() != bf_ast_operation) {
        return false;
    }

    const bf::operation_node *operation_node = static_cast<const bf::operation_node *>(node_object);
    *type = operation_node->get_operation_type();
    return true;
}
