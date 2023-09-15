#pragma once
#include <cmath>
#include <json/json.h>
#include <json/value.h>
#include <shared_mutex>
#include <string>
#include <vector>
#include <memory>
#include "context.h"

namespace Ast {

	enum Kind {
		IntKind,
		StrKind,
		CallKind,
		BinaryKind,
		FunctionKind,
		LetKind,
		IfKind,
		PrintKind,
		FirstKind,
		SecondKind,
		BoolKind,
		TupleKind,
		VarKind,
		NoneKind
	};
	enum BinaryOp { Add, Sub, Mul, Div, Rem, Eq, Neq, Lt, Gt, Lte, Gte, And, Or };
	struct Location {
		uint32_t start, end;
		std::string filename;
		std::string str() { return filename + "(" + std::to_string(start) + ":" + std::to_string(end) + ")"; }
	};
	struct Parameter {
		std::string text;
		Location location;
	};
	enum NodeType { FileType, TermType };
	struct NodeSpecification {
		NodeType type;
		Kind kind;
	};
	struct Node {
		NodeSpecification specification;
		Location location;
		Node(Location location, NodeSpecification specification) : location(location), specification(specification) {}
		virtual ~Node() {}

		virtual bool check(Context<std::shared_ptr<Node>>& ctx) { return true; }
		virtual std::shared_ptr<Node> evaluate(Context<std::shared_ptr<Node>>& ctx) { return nullptr; }
	};
	struct File : public Node {
		std::string name;
		std::shared_ptr<Node> expression;
		File(std::string name, std::shared_ptr<Node> expression, Location location) :
			name(name), expression(expression), Node(location, { FileType, NoneKind }) {}
		virtual ~File() {}
		virtual bool check(Context<std::shared_ptr<Node>>& ctx) { return this->expression->check(ctx); }
		virtual std::shared_ptr<Node> evaluate(Context<std::shared_ptr<Node>>& ctx) { return this->expression->evaluate(ctx); }
	};

	class Ast {
	private:
		std::shared_ptr<Node> head;

		Ast(std::shared_ptr<Node> head) : head(head) {}

	public:
		static Ast createFromJson(const Json::Value& json);
		bool check(Context<std::shared_ptr<Node>>& ctx);
		void evaluate(Context<std::shared_ptr<Node>>& ctx);
	};
	std::shared_ptr<Node> recursiveEvaluate(std::shared_ptr<Node> node, Context<std::shared_ptr<Node>>& ctx, bool evaluateFunctions=0);
};
