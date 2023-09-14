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
	std::unordered_map<std::string, std::vector<std::pair<T, int>>> vars;

	void removeScopedVars(std::vector<std::pair<T, int>>& buff);

public:
	int scope = 0;
	Context() {}
	void enterScope();
	void endScope();
	void shadowVar(const std::string& var, T value);
	void setVar(const std::string& varName, T value);
	bool varExist(const std::string& varName);
	T	 getVar(const std::string& varName);
	void removeVar(const std::string& varName);
};
template <typename T>
void Context<T>::enterScope() {
	this->scope++;
}
template <typename T>
void Context<T>::endScope() {
	this->scope--;
	for (auto& buff : vars) {
		removeScopedVars(buff.second);
	}
}
template <typename T>
void Context<T>::removeScopedVars(std::vector<std::pair<T, int>>& buff) {
	while (buff.size() && buff.back().second > this->scope)
		buff.pop_back();
}

template <typename T>
void Context<T>::shadowVar(const std::string& var, T value) {
	vars[var].push_back({ value, this->scope });
}
template <typename T>
void Context<T>::setVar(const std::string& var, T value) {
	while (vars[var].size() != 0 && vars[var].back().second > this->scope) {
		vars[var].pop_back();
	}
	if (vars[var].size() == 0 || vars[var].back().second < this->scope) {
		vars[var].push_back({ value, this->scope });
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
	while (vars[var].size() && vars[var].back().second > this->scope) {
		vars[var].pop_back();
	}
	return vars[var].back().first;
}
