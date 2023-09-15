#include "term.h"
#include "ast.h"
#include "context.h"
#include <memory>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

std::string getOpString(Ast::BinaryOp op) {
	switch (op) {
		case Ast::Add:
			return "add";
		case Ast::Sub:
			return "sub";
		case Ast::Mul:
			return "mul";
		case Ast::Div:
			return "Div";
		case Ast::Rem:
			return "rem";
		case Ast::Eq:
			return "eq";
		case Ast::Neq:
			return "Not eq";
		case Ast::Lt:
			return "<";
		case Ast::Gt:
			return ">";
		case Ast::Lte:
			return "<=";
		case Ast::Gte:
			return ">=";
		case Ast::And:
			return "&";
		case Ast::Or:
			return "|";
	}
	return "";
}

std::string getStringValueOfTerm(Ast::Term value, Context<Ast::Term>& ctx) {
	std::string response = "";
	Ast::Term	aux;
	bool		first = true;
	switch (value->specification.kind) {
		case Ast::IntKind:
			response = std::to_string(std::static_pointer_cast<Ast::Int>(value)->value);
			break;
		case Ast::BoolKind:
			response = std::static_pointer_cast<Ast::Bool>(value)->value ? "true" : "false";
			break;
		case Ast::StrKind:
			response = std::static_pointer_cast<Ast::Str>(value)->value;
			break;
		case Ast::TupleKind:
			aux		 = std::static_pointer_cast<Ast::Tuple>(value)->first->evaluate(ctx);
			response = "(";
			response += getStringValueOfTerm(aux, ctx);
			aux = std::static_pointer_cast<Ast::Tuple>(value)->second->evaluate(ctx);
			response += ", ";
			response += getStringValueOfTerm(aux, ctx);
			response += ")";
			break;
		case Ast::FunctionKind:
			response = "<#";
			for (Ast::Parameter par : std::static_pointer_cast<Ast::Function>(value)->parameters) {
				if (!first)
					response += ", ";
				first = false;
				response += par.text;
			}
			response += ">";
			break;
		case Ast::CallKind:
			response = "Call("+getStringValueOfTerm(std::static_pointer_cast<Ast::Call>(value)->callee, ctx) + ")";
			break;
		case Ast::BinaryKind:
			response += getStringValueOfTerm(std::static_pointer_cast<Ast::Binary>(value)->lhs, ctx);
			response += getOpString(std::static_pointer_cast<Ast::Binary>(value)->op);
			response += getStringValueOfTerm(std::static_pointer_cast<Ast::Binary>(value)->rhs, ctx);
			break;
		default:
			break;
	}
	return response;
}
bool Ast::Int::check(Context<Term>& ctx) {
	return true;
}
Ast::Term Ast::Int::evaluate(Context<Term>& ctx) {
	/*qnt++;
	//printLog("Evaluate Int with " + std::to_string(this->value));
	qnt--;*/
	return std::make_shared<Int>(value, location);
}
bool Ast::Str::check(Context<Term>& ctx) {
	return true;
}
Ast::Term Ast::Str::evaluate(Context<Term>& ctx) {
	/*qnt++;
	//printLog("Evaluate Str with " + this->value);
	qnt--;*/
	return std::make_shared<Str>(value, location);
}
bool Ast::Bool::check(Context<Term>& ctx) {
	return true;
}
Ast::Term Ast::Bool::evaluate(Context<Term>& ctx) {
	/*qnt++;
	//printLog(std::string("Evaluate Bool with ") + (this->value ? "y" : "n"));
	qnt--;*/
	return std::make_shared<Bool>(value, location);
}

bool Ast::Call::check(Context<Term>& ctx)  // TODO:Do better
{
	bool ok = true;
	for (Term& w : arguments) {
		if (w)
			ok &= w->check(ctx);
	}
	if (this->callee)
		ok &= callee->check(ctx);
	return ok;
}
Ast::Term Ast::Call::evaluate(Context<Term>& ctx) {
	Context closureCtx = ctx;
	std::shared_ptr<Function> fun;
	if (this->callee->specification.kind == FunctionKind)
		fun = std::static_pointer_cast<Function>(this->callee);
	else
		fun = std::static_pointer_cast<Function>(recursiveEvaluate(this->callee, closureCtx));
	for (int i = 0; i < this->arguments.size(); i++) {
		Term resp;
		resp = recursiveEvaluate(this->arguments[i], closureCtx);
		closureCtx.setVar(fun->parameters[i].text, resp);
	}
	Term response;
	response = recursiveEvaluate(fun, closureCtx, true);
	return response;
}

bool Ast::Binary::check(Context<Term>& ctx) {
	bool ok = true;
	if (lhs)
		ok &= lhs->check(ctx);
	else
		ok = false;
	// the OP is always Ok
	if (rhs)
		ok &= rhs->check(ctx);
	else
		ok = false;
	return ok;
}
Ast::Term Ast::Binary::evaluate(Context<Term>& ctx) {
	//qnt++;
	//printLog("Evaluating operation");
	//printLog("lhs = ");
	Term l = recursiveEvaluate(lhs, ctx);
	//printLog("operation = " + getOpString(op));
	//printLog("rhs = ");
	Term r = recursiveEvaluate(rhs, ctx);
	Term t = evaluateOperation(l, op, r, location);
	//qnt--;
	return t;
}
bool Ast::Tuple::check(Context<Term>& ctx) {
	return this->first->check(ctx) and this->second->check(ctx);
}
Ast::Term Ast::Tuple::evaluate(Context<Term>& ctx) {
	//qnt++;
	//printLog("Evaluating tuple");
	//qnt--;
	return std::make_shared<Tuple>(first, second, location);
}
bool Ast::Var::check(Context<Term>& ctx) {
	return ctx.varExist(this->text);
}
Ast::Term Ast::Var::evaluate(Context<Term>& ctx) {
	//qnt++;
	//printLog("Evaluating Var " + text + " -> " + getStringValueOfTerm(ctx.getVar(text), ctx));
	//qnt--;
	return ctx.getVar(text);  //recursiveEvaluate(ctx.getVar(text), ctx, false);
}
bool Ast::Function::check(Context<Term>& ctx) {
	// for(Parameter p: this->parameters) ctx.setVar(p, );
	return true;
}
Ast::Term Ast::Function::evaluate(Context<Term>& ctx) {
	//qnt++;
	//printLog("Evaluating Func");
	for (Parameter& p : this->parameters) {
		Term resp = ctx.getVar(p.text);
		//printLog("Parameter " + p.text + " = " + getStringValueOfTerm(resp, ctx));
	}
	Term t = recursiveEvaluate(this->value, ctx);
	//qnt--;
	return t;
}
bool Ast::Let::check(Context<Term>& ctx) {
	return this->value->check(ctx);
}
Ast::Term Ast::Let::evaluate(Context<Term>& ctx) {
	//qnt++;
	//printLog("Evaluating Let");

	//printLog("var " + name.text + " set to " + getStringValueOfTerm(value, ctx));
	ctx.setVar(name.text, value);

	//printLog("Evaluate next");
	Term t = recursiveEvaluate(next, ctx);
	//qnt--;
	return t;
}
bool Ast::If::check(Context<Term>& ctx) {
	return this->condition->check(ctx) and this->then->check(ctx) && this->otherwise->check(ctx);
}
Ast::Term Ast::If::evaluate(Context<Term>& ctx) {
	//qnt++;
	//printLog("Evaluate Condition");
	Term cond = recursiveEvaluate(condition, ctx);
	if (cond->specification.kind != BoolKind)
		throw std::runtime_error("Condition didn't evaluate properly on " + location.str());
	bool condValue = std::static_pointer_cast<Bool>(cond)->value;
	//printLog("Get response " + getStringValueOfTerm(cond, ctx));
	if (condValue) {
		//printLog("Evaluate Then");
		Term t = recursiveEvaluate(then, ctx);
		//qnt--;
		return t;
	}
	//printLog("Evaluate Otherwise");

	Term t = recursiveEvaluate(otherwise, ctx);
	//qnt--;
	return t;
}

bool Ast::Print::check(Context<Term>& ctx) {
	return this->value->check(ctx);
}
Ast::Term Ast::Print::evaluate(Context<Term>& ctx) {
	//qnt++;
	//printLog("Evaluate Print");
	Term val = recursiveEvaluate(this->value, ctx);
	std::cout << getStringValueOfTerm(val, ctx) << std::endl;
	//qnt--;
	return this->value;
}
bool Ast::First::check(Context<Term>& ctx) {
	return std::static_pointer_cast<Tuple>(this->value)->first->check(ctx);
}
Ast::Term Ast::First::evaluate(Context<Term>& ctx) {
	//qnt++;
	//printLog("Evaluate First");
	auto tu = std::static_pointer_cast<Tuple>(value);
	Term t	= recursiveEvaluate(tu->first, ctx);
	//qnt--;
	return t;
}
bool Ast::Second::check(Context<Term>& ctx) {
	return std::static_pointer_cast<Tuple>(this->value)->second->check(ctx);
}
Ast::Term Ast::Second::evaluate(Context<Term>& ctx) {
	//qnt++;
	//printLog("Evaluate Second");
	auto tu = std::static_pointer_cast<Tuple>(value);
	Term t	= recursiveEvaluate(tu->second, ctx);
	//qnt--;
	return t;
}
