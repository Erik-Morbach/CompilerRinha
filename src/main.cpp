#include <fstream>
#include <iostream>
#include <json/reader.h>
#include <json/value.h>
#include "ast.h"
#include "context.h"

int main(int argc, char **argv) {
	std::cout << argv[1] << std::endl;
	std::ifstream fss(argv[1]);
	Json::Value	 json;
	Json::Reader reader;
	reader.parse(fss, json);
	//std::cout << json.asString() << std::endl;
	//auto t0 = std::chrono::system_clock::now();
	auto ast = Ast::Ast::createFromJson(json);
	//auto t1 = std::chrono::system_clock::now();

	//std::cout << "Create Ast in "<< std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count() << std::endl;

	Context<std::shared_ptr<Ast::Node>> ctx;
	ast.evaluate(ctx);
	return 0;
}
