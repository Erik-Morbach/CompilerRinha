#include "ast.h"
#include <memory>

std::shared_ptr<Ast::Node> evaluateOperation(std::shared_ptr<Ast::Node> lhs,
											 Ast::BinaryOp				op,
											 std::shared_ptr<Ast::Node> rhs,
											 Ast::Location				location);
