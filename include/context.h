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
	int scope = 0;
	std::vector<std::vector<std::string>> scopedVars;
	std::unordered_map<std::string, std::vector<std::pair<T, int>>> vars;
	void removeScopedVars(std::vector<std::pair<T, int>>& buff);

public:
	Context() { scopedVars.push_back({}); }
	void enterScope();
	void endScope();
	void setVar(const std::string& varName, T value);
	bool varExist(const std::string& varName);
	T getVar(const std::string& varName);
	void removeVar(const std::string& varName);
};
template <typename T>
void Context<T>::enterScope() {
	scopedVars.push_back({});
	this->scope++;
}
template <typename T>
void Context<T>::endScope() {
	if(this->scope == 0) throw std::runtime_error("Ending not scope that does not exist");
	this->scope--;
	for (auto& var : scopedVars.back()) {
		removeScopedVars(vars[var]);
	}
}
template <typename T>
void Context<T>::removeScopedVars(std::vector<std::pair<T, int>>& buff) {
	while (buff.size() && buff.back().second > this->scope)
		buff.pop_back();
}
template <typename T>
void Context<T>::setVar(const std::string& var, T value) {
	if (vars[var].size() == 0 || vars[var].back().second < this->scope) {
		vars[var].push_back({ value, this->scope });
		scopedVars.back().push_back(var);
		return;
	}
	vars[var][vars[var].size() - 1].first = value;
}
template <typename T>
bool Context<T>::varExist(const std::string& var) {
	return vars.count(var) != 0;
}
template <typename T>
void Context<T>::removeVar(const std::string& var) {
	if (vars[var].size() == 0)
		throw std::runtime_error("Removing non existing var");
	vars[var].pop_back();
};

template <typename T>
T Context<T>::getVar(const std::string& var) {
	if(vars[var].size()==0) throw std::runtime_error("Var "+var+" not declared in scope");
	return vars[var].back().first;
}
