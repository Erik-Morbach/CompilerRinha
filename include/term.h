#pragma once
#include "ast.h"
#include "context.h"
#include "operations.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace Ast {
	typedef std::shared_ptr<Node> Term;
	struct Int : public Node {
		int32_t value;
		Int(int32_t value, Location location) : value(value), Node(location, { TermType, IntKind }) {}
		virtual ~Int() {}
		virtual bool check(Context<Term>& ctx);
		virtual Term evaluate(Context<Term>& ctx);
	};
	struct Str : public Node {
		std::string value;
		Str(std::string value, Location location) : value(value), Node(location, { TermType, StrKind }) {}
		virtual ~Str() {}
		virtual bool check(Context<Term>& ctx);
		virtual Term evaluate(Context<Term>& ctx);
	};
	struct Bool : public Node {
		bool value;
		Bool(bool value, Location location) : value(value), Node(location, { TermType, BoolKind }) {}
		virtual ~Bool() {}
		virtual bool check(Context<Term>& ctx);
		virtual Term evaluate(Context<Term>& ctx);
	};

	struct Call : public Node {
		Term			  callee;
		std::vector<Term> arguments;
		Call(Term callee, std::vector<Term> arguments, Location location) :
			callee(callee), arguments(arguments), Node(location, { TermType, CallKind }) {}
		virtual ~Call() {}
		virtual bool check(Context<Term>& ctx);
		virtual Term evaluate(Context<Term>& ctx);
	};
	struct Binary : public Node {
		Term	 lhs;
		BinaryOp op;
		Term	 rhs;
		Binary(Term lhs, BinaryOp op, Term rhs, Location location) : lhs(lhs), op(op), rhs(rhs), Node(location, { TermType, BinaryKind }) {}
		virtual ~Binary() {}
		virtual bool check(Context<Term>& ctx);
		virtual Term evaluate(Context<Term>& ctx);
	};
	struct Tuple : public Node {
		Term first;
		Term second;
		Tuple(Term first, Term second, Location location) : first(first), second(second), Node(location, { TermType, TupleKind }) {}
		virtual ~Tuple() {}
		virtual bool check(Context<Term>& ctx);
		virtual Term evaluate(Context<Term>& ctx);
	};
	struct Var : public Node {
		std::string text;
		Var(std::string text, Location location) : text(text), Node(location, { TermType, VarKind }) {}
		virtual ~Var() {}
		virtual bool check(Context<Term>& ctx);
		virtual Term evaluate(Context<Term>& ctx);
	};
	struct Function : public Node {
		std::vector<Parameter> parameters;
		Term				   value;
		Function(std::vector<Parameter> parameters, Term value, Location location) :
			parameters(parameters), value(value), Node(location, { TermType, FunctionKind }) {}
		virtual ~Function() {}
		virtual bool check(Context<Term>& ctx);
		virtual Term evaluate(Context<Term>& ctx);
	};
	struct Let : public Node {
		Parameter name;
		Term	  value;
		Term	  next;
		Let(Parameter name, Term value, Term next, Location location) :
			name(name), value(value), next(next), Node(location, { TermType, LetKind })

		{}
		virtual ~Let() {}
		virtual bool check(Context<Term>& ctx);
		virtual Term evaluate(Context<Term>& ctx);
	};
	struct If : public Node {
		Term condition, then, otherwise;
		If(Term condition, Term then, Term otherwise, Location location) :
			condition(condition), then(then), otherwise(otherwise), Node(location, { TermType, IfKind }) {}
		virtual ~If() {}
		virtual bool check(Context<Term>& ctx);
		virtual Term evaluate(Context<Term>& ctx);
	};
	struct Print : public Node {
		Term value;
		Print(Term value, Location location) : value(value), Node(location, { TermType, PrintKind }) {}
		virtual ~Print() {}
		virtual bool check(Context<Term>& ctx);
		virtual Term evaluate(Context<Term>& ctx);
	};
	struct First : public Node {
		Term value;
		First(Term value, Location location) : Node(location, { TermType, FirstKind }), value(value) {}
		virtual ~First() {}
		virtual bool check(Context<Term>& ctx);
		virtual Term evaluate(Context<Term>& ctx);
	};
	struct Second : public Node {
		Term value;
		Second(Term value, Location location) : Node(location, { TermType, SecondKind }), value(value) {}
		virtual ~Second() {}
		virtual bool check(Context<Term>& ctx);
		virtual Term evaluate(Context<Term>& ctx);
	};
};	// namespace Ast
