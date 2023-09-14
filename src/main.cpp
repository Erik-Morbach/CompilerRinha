#include <bits/chrono.h>
#include <fstream>
#include <iostream>
#include <json/reader.h>
#include <json/value.h>
#include "ast.h"
#include "context.h"

int main(void) {
	Json::Value	 json;
	Json::Reader reader;
	reader.parse(std::cin, json);
	//std::cout << json.asString() << std::endl;
	//auto t0 = std::chrono::system_clock::now();
	auto ast = Ast::Ast::createFromJson(json);
	//auto t1 = std::chrono::system_clock::now();

	//std::cout << "Create Ast in "<< std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count() << std::endl;

	Context<std::shared_ptr<Ast::Node>> ctx;
	//	ast.check(ctx); TODO
	ast.evaluate(ctx);
	return 0;
}
