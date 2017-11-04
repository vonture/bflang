#include "interpreter.h"

#include <assert.h>

namespace bf {

interpreter_options default_interpreter_options() {
    interpreter_options options;
    return options;
}

namespace {
template <typename data_type, typename ptr_type> class interpreter_array {
  public:
    interpreter_array()
        : data_{0} {}

    data_type &operator[](ptr_type position) {
        ensure_entry_exists(position);
        return data_[position];
    }

    const data_type &operator[](ptr_type position) const {
        ensure_entry_exists(position);
        return data_[position];
    }

  private:
    void ensure_entry_exists(ptr_type location) const {
        assert(location >= 0);
        while (location >= data_.size()) {
            data_.resize(data_.size() * resizing_factor_, 0);
        }
    }

    static constexpr size_t resizing_factor_ = 2;
    mutable std::vector<data_type> data_;
};

class interpreter_instance_impl : public interpreter_instance {
  public:
    interpreter_instance_impl(const ast *ast)
        : ast_(ast) {}

    void execute(input_stream &input_stream, output_stream &output_stream, error_stream &error_stream) override {
        using data_type = int8_t;
        using ptr_type = size_t;

        ptr_type ptr = 0;
        interpreter_array<data_type, ptr_type> data;

        struct stack_entry {
            const sequence_node *sequence = nullptr;
            int instruction = 0;
        };
        std::vector<stack_entry> stack{{ast_->get_root(), 0}};

        while (!stack.empty()) {
            stack_entry &cur_entry = stack.back();
            if (cur_entry.instruction < cur_entry.sequence->get_child_count()) {
                // Execute the current instruction
                const ast_node *cur_node = cur_entry.sequence->get_child(cur_entry.instruction);
                cur_entry.instruction++;

                if (cur_node->get_type() == bf_ast_sequence) {
                    if (data[ptr] != 0) {
                        stack.push_back({static_cast<const sequence_node *>(cur_node), 0});
                    }
                } else if (cur_node->get_type() == bf_ast_operation) {
                    switch (static_cast<const operation_node *>(cur_node)->get_operation_type()) {
                    case bf_ast_operation_inc_data_ptr:
                        ptr++;
                        break;

                    case bf_ast_operation_dec_data_ptr:
                        ptr--;
                        break;

                    case bf_ast_operation_inc_data_value:
                        data[ptr]++;
                        break;

                    case bf_ast_operation_dec_data_value:
                        data[ptr]--;
                        break;

                    case bf_ast_operation_input:
                        data[ptr] = input_stream.read();
                        break;

                    case bf_ast_operation_output:
                        output_stream.write(data[ptr]);
                        break;
                    }
                }
            } else if (stack.size() > 1) {
                // Evaluate the sequence condition
                if (data[ptr] != 0) {
                    cur_entry.instruction = 0;
                } else {
                    stack.pop_back();
                }
            } else {
                // Last instruction of the root sequence, program is finished.
                stack.pop_back();
            }
        }
    }

  private:
    const ast *ast_;
};
} // namespace

interpreter::interpreter(const interpreter_options &options)
    : options_(options) {}

std::unique_ptr<interpreter_instance> interpreter::create_instance(const ast *ast) const {
    return std::make_unique<interpreter_instance_impl>(ast);
}

std::unique_ptr<interpreter> create_interpreter(const interpreter_options &options) { return std::make_unique<interpreter>(options); }

} // namespace bf
