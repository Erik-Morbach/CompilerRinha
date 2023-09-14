#include "ast.h"
#include "term.h"
#include <bits/fs_fwd.h>
#include <iostream>
#include <exception>
#include <json/value.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "context.h"

/*
static int qnt=0;
static void addTab(){ qnt++; }
static void printLog(std::string s) {
	for(int i=0;i<qnt;i++) std::cout <<"\t";
	std::cout << s << std::endl;
}
*/

std::unordered_map<std::string, Ast::Kind> termLookupTable = {
	{ "Int", Ast::IntKind },	   { "Str", Ast::StrKind },			  { "Call", Ast::CallKind },
	{ "Binary", Ast::BinaryKind }, { "Function", Ast::FunctionKind }, { "Let", Ast::LetKind },
	{ "If", Ast::IfKind },		   { "Print", Ast::PrintKind },		  { "First", Ast::FirstKind },
	{ "Second", Ast::SecondKind }, { "Bool", Ast::BoolKind },		  { "Tuple", Ast::TupleKind },
	{ "Var", Ast::VarKind }
};
std::unordered_map<std::string, Ast::BinaryOp> binaryOpLookupTable = { { "Add", Ast::Add }, { "Sub", Ast::Sub }, { "Mul", Ast::Mul },
																	   { "Div", Ast::Div }, { "Rem", Ast::Rem }, { "Eq", Ast::Eq },
																	   { "Neq", Ast::Neq }, { "Lt", Ast::Lt },	 { "Gt", Ast::Gt },
																	   { "Lte", Ast::Lte }, { "Gte", Ast::Gte }, { "And", Ast::And },
																	   { "Or", Ast::Or } };

Ast::Location createLocationFromJson(const Json::Value& json) {
	if (!json.isMember("start") || !json.isMember("end") || !json.isMember("filename"))
		throw std::runtime_error("Location illformed");
	return { json["start"].asUInt(), json["end"].asUInt(), json["filename"].asString() };
}
Ast::BinaryOp createBinaryOpFromJson(const Json::Value& json) {
	//printLog("Binary op " + json.asString() + " -> "+std::to_string((int)(binaryOpLookupTable[json.asString()])));
	return binaryOpLookupTable[json.asString()];
}
Ast::Parameter createParameterFromJson(const Json::Value& json) {
	if (!json.isMember("text"))
		throw std::runtime_error("Parameter illformed");
	//printLog("Parameter " + json["text"].asString());
	return { json["text"].asString(), createLocationFromJson(json["location"]) };
}

std::shared_ptr<Ast::Node> createTermFromJson(const Json::Value& json) {
	//int q = qnt;
	if (!json.isMember("kind") || !json.isMember("location"))
		throw std::runtime_error("Term Illformed");
	//addTab();
	std::string kind = json["kind"].asString();
	if (termLookupTable.count(kind) == 0)
		throw std::runtime_error("Term kind not recognized");

	Ast::Kind	  termKind = termLookupTable.find(kind)->second;
	Ast::Location location = createLocationFromJson(json["location"]);
	//printLog("Term with kind = "+ kind);

	std::shared_ptr<Ast::Node> node;
	std::shared_ptr<Ast::Node> aux;
	std::shared_ptr<Ast::Node> aux2;
	std::shared_ptr<Ast::Node> aux3;
	// Auxiliar vector to get argument list
	std::vector<std::shared_ptr<Ast::Node>> argVec;
	// Auxiliar vector to get Parameter list
	std::vector<Ast::Parameter> paramVec;
	bool						error = false;

	switch (termKind) {
		case Ast::IntKind:
			error = !json.isMember("value");
			if (error)
				break;
			//printLog("Value = "+std::to_string(json["value"].asInt()));
			node = std::make_shared<Ast::Int>(json["value"].asInt(), location);
			break;
		case Ast::StrKind:
			error = !json.isMember("value");
			if (error)
				break;
			//printLog("Value = "+json["value"].asString());
			node = std::make_shared<Ast::Str>(json["value"].asString(), location);
			break;
		case Ast::CallKind:
			error = !json.isMember("arguments");
			error |= !json.isMember("callee");
			if (error)
				break;
			for (const Json::Value& js : json["arguments"]) {
				//printLog("Arg += ");
				argVec.push_back(createTermFromJson(js));
			}
			//printLog("Callee ");
			node = std::make_shared<Ast::Call>(createTermFromJson(json["callee"]), argVec, location);
			break;
		case Ast::BinaryKind:
			error = !json.isMember("lhs");
			error |= !json.isMember("op");
			error |= !json.isMember("rhs");
			if (error)
				break;
			//printLog("Lhs = ");
			aux = createTermFromJson(json["lhs"]);
			//printLog("Rhs = ");
			aux2 = createTermFromJson(json["rhs"]);
			node = std::make_shared<Ast::Binary>(aux, createBinaryOpFromJson(json["op"]), aux2, location);
			break;
		case Ast::FunctionKind:
			error = !json.isMember("parameters");
			error |= !json.isMember("value");
			error |= !json.isMember("location");
			if (error)
				break;
			for (Json::Value js : json["parameters"]) {
				paramVec.push_back(createParameterFromJson(js));
			}
			//printLog("Function -> ");
			node = std::make_shared<Ast::Function>(paramVec, createTermFromJson(json["value"]), createLocationFromJson(json["location"]));
			break;
		case Ast::LetKind:
			error = !json.isMember("name");
			error |= !json.isMember("value");
			error |= !json.isMember("next");
			if (error)
				break;
			//printLog("let "+json["name"]["text"].asString() + " = ");
			aux = createTermFromJson(json["value"]);
			//printLog("Then next -> ");
			aux2 = createTermFromJson(json["next"]);
			node = std::make_shared<Ast::Let>(createParameterFromJson(json["name"]), aux, aux2, location);
			break;
		case Ast::IfKind:
			error = !json.isMember("condition");
			error |= !json.isMember("then");
			error |= !json.isMember("otherwise");
			if (error)
				break;
			//printLog("Cond = ");
			aux = createTermFromJson(json["condition"]);
			//printLog("Then = ");
			aux2 = createTermFromJson(json["then"]);
			//printLog("otherwise = ");
			aux3 = createTermFromJson(json["otherwise"]);
			node = std::make_shared<Ast::If>(aux, aux2, aux3, location);
			break;
		case Ast::PrintKind:
			error = !json.isMember("value");
			if (error)
				break;
			//printLog("Print = ");
			node = std::make_shared<Ast::Print>(createTermFromJson(json["value"]), location);
			break;
		case Ast::FirstKind:
			error = !json.isMember("value");
			if (error)
				break;
			//printLog("First of - ");
			node = std::make_shared<Ast::First>(createTermFromJson(json["value"]), location);
			break;
		case Ast::SecondKind:
			error = !json.isMember("value");
			if (error)
				break;
			//printLog("Second of - ");
			node = std::make_shared<Ast::Second>(createTermFromJson(json["value"]), location);
			break;
		case Ast::BoolKind:
			error = !json.isMember("value");
			if (error)
				break;
			//printLog(std::string("Bool -> ") + (json["value"].asBool()?"y":"n"));
			node = std::make_shared<Ast::Bool>(json["value"].asBool(), location);
			break;
		case Ast::TupleKind:
			error = !json.isMember("first");
			error |= !json.isMember("second");
			if (error)
				break;
			//printLog("first = ");
			aux = createTermFromJson(json["first"]);
			//printLog("second = ");
			aux2 = createTermFromJson(json["second"]);
			node = std::make_shared<Ast::Tuple>(aux, aux2, location);
			break;
		case Ast::VarKind:
			error = !json.isMember("text");
			if (error)
				break;
			//printLog("Var = "+json["text"].asString());
			node = std::make_shared<Ast::Var>(json["text"].asString(), location);
			break;
		default:
			throw std::runtime_error("Term not found");
	}

	if (error)
		throw std::runtime_error("Term illformed on " + location.filename + "(" + std::to_string(location.start) + ", " +
								 std::to_string(location.end) + ")");
	//qnt = q;
	return node;
}

std::shared_ptr<Ast::Node> createNodeFromJson(const Json::Value& json) {
	if (!(json.isMember("name") && json.isMember("expression") && json.isMember("location")))
		throw std::runtime_error("Node illformed");
	return std::make_shared<Ast::File>(
		json["name"].asString(), createTermFromJson(json["expression"]), createLocationFromJson(json["location"]));
}
Ast::Ast Ast::Ast::createFromJson(const Json::Value& json) {
	return Ast(createNodeFromJson(json));
}

bool Ast::Ast::check(Context<std::shared_ptr<Node>>& ctx) {
	return std::static_pointer_cast<File>(this->head)->expression->check(ctx);
}
void Ast::Ast::evaluate(Context<std::shared_ptr<Node>>& ctx) {
	std::static_pointer_cast<File>(this->head)->expression->evaluate(ctx);
}
std::shared_ptr<Ast::Node> Ast::recursiveEvaluate(std::shared_ptr<Node> node, Context<std::shared_ptr<Node>>& ctx, bool stopOnFunction) {
	int count = 0;
	while (count < 10000000) {
		node = node->evaluate(ctx);
		if (!node)
			return node;
		if (node->specification.kind == IntKind)
			return node;
		if (node->specification.kind == StrKind)
			return node;
		if (node->specification.kind == BoolKind)
			return node;
		if (node->specification.kind == TupleKind)
			return node;
		if (stopOnFunction && node->specification.kind == FunctionKind)
			return node;
		count++;
	}
	throw std::runtime_error("Exceed maximum evaluate");
}
