#include "operations.h"
#include "ast.h"
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <term.h>

Ast::Term add(Ast::Term lhs, Ast::Term rhs, Ast::Location location) {
	bool lhsIsInt = lhs->specification.kind == Ast::IntKind;
	bool rhsIsInt = rhs->specification.kind == Ast::IntKind;
	if (lhsIsInt && rhsIsInt) {
		int32_t value = std::static_pointer_cast<Ast::Int>(lhs)->value;
		value += std::static_pointer_cast<Ast::Int>(rhs)->value;
		return std::make_shared<Ast::Int>(value, location);
	}
	std::string value = "";
	if (lhsIsInt)
		value += std::to_string(std::static_pointer_cast<Ast::Int>(lhs)->value);
	else
		value += std::static_pointer_cast<Ast::Str>(lhs)->value;

	if (rhsIsInt)
		value += std::to_string(std::static_pointer_cast<Ast::Int>(rhs)->value);
	else
		value += std::static_pointer_cast<Ast::Str>(rhs)->value;

	return std::make_shared<Ast::Str>(value, location);
}

Ast::Term sub(Ast::Term lhs, Ast::Term rhs, Ast::Location location) {
	int32_t value = std::static_pointer_cast<Ast::Int>(lhs)->value;
	value -= std::static_pointer_cast<Ast::Int>(rhs)->value;
	return std::make_shared<Ast::Int>(value, location);
}
Ast::Term mul(Ast::Term lhs, Ast::Term rhs, Ast::Location location) {
	int32_t value = std::static_pointer_cast<Ast::Int>(lhs)->value;
	value *= std::static_pointer_cast<Ast::Int>(rhs)->value;
	return std::make_shared<Ast::Int>(value, location);
}
Ast::Term div(Ast::Term lhs, Ast::Term rhs, Ast::Location location) {
	int32_t num = std::static_pointer_cast<Ast::Int>(lhs)->value;
	int32_t den = std::static_pointer_cast<Ast::Int>(rhs)->value;
	if(den==0) throw std::runtime_error("Dividing by zero not allowed");
	int32_t value = num / den;
	return std::make_shared<Ast::Int>(value, location);
}
Ast::Term rem(Ast::Term lhs, Ast::Term rhs, Ast::Location location) {
	int32_t value = std::static_pointer_cast<Ast::Int>(lhs)->value;
	value %= std::static_pointer_cast<Ast::Int>(rhs)->value;
	return std::make_shared<Ast::Int>(value, location);
}
template <typename T, typename C>
Ast::Term compare(Ast::Term l, Ast::Term r, Ast::Location location, C comp = C {}) {
	return std::make_shared<Ast::Bool>(comp(std::static_pointer_cast<T>(l)->value, std::static_pointer_cast<T>(r)->value), location);
}
Ast::Term eq(Ast::Term lhs, Ast::Term rhs, Ast::Location location) {
	if (lhs->specification.kind != rhs->specification.kind)
		return std::make_shared<Ast::Bool>(false, location);
	// Can use a lookup table to check the types
	if (lhs->specification.kind == Ast::IntKind)
		return compare<Ast::Int, std::equal_to<int32_t>>(lhs, rhs, location);
	if (lhs->specification.kind == Ast::BoolKind)
		return compare<Ast::Bool, std::equal_to<bool>>(lhs, rhs, location);
	if (lhs->specification.kind == Ast::StrKind)
		return compare<Ast::Str, std::equal_to<std::string>>(lhs, rhs, location);
	// TODO: check for function equality
	return std::make_shared<Ast::Bool>(false, location);
}
Ast::Term neq(Ast::Term lhs, Ast::Term rhs, Ast::Location location) {
	if (lhs->specification.kind != rhs->specification.kind)
		return std::make_shared<Ast::Bool>(true, location);
	// Can use a lookup table to check the types
	if (lhs->specification.kind == Ast::IntKind)
		return compare<Ast::Int, std::not_equal_to<int32_t>>(lhs, rhs, location);
	if (lhs->specification.kind == Ast::BoolKind)
		return compare<Ast::Bool, std::not_equal_to<bool>>(lhs, rhs, location);
	if (lhs->specification.kind == Ast::StrKind)
		return compare<Ast::Str, std::not_equal_to<std::string>>(lhs, rhs, location);
	// TODO: check for function equality
	return std::make_shared<Ast::Bool>(true, location);
}
Ast::Term lt(Ast::Term lhs, Ast::Term rhs, Ast::Location location) {
	if (lhs->specification.kind != rhs->specification.kind)	 // Throw?
		return std::make_shared<Ast::Bool>(false, location);
	// Can use a lookup table to check the types
	if (lhs->specification.kind == Ast::IntKind)
		return compare<Ast::Int, std::less<int32_t>>(lhs, rhs, location);
	if (lhs->specification.kind == Ast::BoolKind)
		return compare<Ast::Bool, std::less<bool>>(lhs, rhs, location);
	if (lhs->specification.kind == Ast::StrKind)
		return compare<Ast::Str, std::less<std::string>>(lhs, rhs, location);
	// TODO: check for function equality
	return std::make_shared<Ast::Bool>(false, location);  // Throw?
}
Ast::Term gt(Ast::Term lhs, Ast::Term rhs, Ast::Location location) {
	if (lhs->specification.kind != rhs->specification.kind)	 // Throw?
		return std::make_shared<Ast::Bool>(false, location);
	// Can use a lookup table to check the types
	if (lhs->specification.kind == Ast::IntKind)
		return compare<Ast::Int, std::greater<int32_t>>(lhs, rhs, location);
	if (lhs->specification.kind == Ast::BoolKind)
		return compare<Ast::Bool, std::greater<bool>>(lhs, rhs, location);
	if (lhs->specification.kind == Ast::StrKind)
		return compare<Ast::Str, std::greater<std::string>>(lhs, rhs, location);
	// TODO: check for function equality
	return std::make_shared<Ast::Bool>(false, location);  // Throw?
}
Ast::Term lte(Ast::Term lhs, Ast::Term rhs, Ast::Location location) {
	if (lhs->specification.kind != rhs->specification.kind)	 // Throw?
		return std::make_shared<Ast::Bool>(false, location);
	// Can use a lookup table to check the types
	if (lhs->specification.kind == Ast::IntKind)
		return compare<Ast::Int, std::less_equal<int32_t>>(lhs, rhs, location);
	if (lhs->specification.kind == Ast::BoolKind)
		return compare<Ast::Bool, std::less_equal<bool>>(lhs, rhs, location);
	if (lhs->specification.kind == Ast::StrKind)
		return compare<Ast::Str, std::less_equal<std::string>>(lhs, rhs, location);
	// TODO: check for function equality
	return std::make_shared<Ast::Bool>(false, location);  // Throw?
}
Ast::Term gte(Ast::Term lhs, Ast::Term rhs, Ast::Location location) {
	if (lhs->specification.kind != rhs->specification.kind)	 // Throw?
		return std::make_shared<Ast::Bool>(false, location);
	// Can use a lookup table to check the types
	if (lhs->specification.kind == Ast::IntKind)
		return compare<Ast::Int, std::greater_equal<int32_t>>(lhs, rhs, location);
	if (lhs->specification.kind == Ast::BoolKind)
		return compare<Ast::Bool, std::greater_equal<bool>>(lhs, rhs, location);
	if (lhs->specification.kind == Ast::StrKind)
		return compare<Ast::Str, std::greater_equal<std::string>>(lhs, rhs, location);
	// TODO: check for function equality
	return std::make_shared<Ast::Bool>(false, location);  // Throw?
}
Ast::Term andOp(Ast::Term lhs, Ast::Term rhs, Ast::Location location) {
	if (lhs->specification.kind == Ast::BoolKind) {
		bool value = std::static_pointer_cast<Ast::Bool>(lhs)->value;
		value &= std::static_pointer_cast<Ast::Bool>(rhs)->value;
		return std::make_shared<Ast::Bool>(value, location);
	}
	return std::make_shared<Ast::Bool>(false, location);  // Throw?
}
Ast::Term orOp(Ast::Term lhs, Ast::Term rhs, Ast::Location location) {
	if (lhs->specification.kind == Ast::BoolKind) {
		bool value = std::static_pointer_cast<Ast::Bool>(lhs)->value;
		value |= std::static_pointer_cast<Ast::Bool>(rhs)->value;
		return std::make_shared<Ast::Bool>(value, location);
	}
	return std::make_shared<Ast::Bool>(false, location);  // Throw?
}

Ast::Term evaluateOperation(Ast::Term lhs, Ast::BinaryOp op, Ast::Term rhs, Ast::Location location) {
	switch (op) {
		case Ast::Add:
			return add(lhs, rhs, location);
		case Ast::Sub:
			return sub(lhs, rhs, location);
		case Ast::Mul:
			return mul(lhs, rhs, location);
		case Ast::Div:
			return div(lhs, rhs, location);
		case Ast::Rem:
			return rem(lhs, rhs, location);
		case Ast::Eq:
			return eq(lhs, rhs, location);
		case Ast::Neq:
			return neq(lhs, rhs, location);
		case Ast::Lt:
			return lt(lhs, rhs, location);
		case Ast::Gt:
			return gt(lhs, rhs, location);
		case Ast::Lte:
			return lte(lhs, rhs, location);
		case Ast::Gte:
			return gte(lhs, rhs, location);
		case Ast::And:
			return andOp(lhs, rhs, location);
		case Ast::Or:
			return orOp(lhs, rhs, location);
	}
	throw std::runtime_error("operation not found");
}
