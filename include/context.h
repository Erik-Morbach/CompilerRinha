#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>

template <typename T>
class Context {
private:
	std::unordered_map<std::string, T> vars;
public:
	Context() {}
	void setVar(const std::string& varName, T value);
	bool varExist(const std::string& varName);
	T getVar(const std::string& varName);
	void removeVar(const std::string& varName);
};
template <typename T>
void Context<T>::setVar(const std::string& var, T value) {
	vars[var] = value;
}
template <typename T>
bool Context<T>::varExist(const std::string& var) {
	return vars.count(var) != 0;
}
template <typename T>
void Context<T>::removeVar(const std::string& var) {
	if (vars.count(var) == 0)
		throw std::runtime_error("Removing non existing var");
	vars.erase(vars.find(var));
};

template <typename T>
T Context<T>::getVar(const std::string& var) {
	auto it = vars.find(var);
	if(it == vars.end()) throw std::runtime_error("Var "+var+" not declared in scope");
	return (*it).second;
}
