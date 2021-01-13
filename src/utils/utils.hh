#pragma once

#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <variant>
#include <vector>


template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

template<typename T, typename... Ts>
std::ostream& operator<<(std::ostream& os, const std::variant<T, Ts...> &v) {
	std::visit([&os](auto&& arg) {
		os << arg;
	}, v);
	return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& ost, const std::vector<T> &v) {
	bool first = true;
	for (auto &i : v) {
		if (!first)
			ost << ", ";
		ost << i;
		first = false;
	}
	return ost;
}

template<typename T>
std::ostream& operator<<(std::ostream& ost, const std::set<T> &s) { // TODO: avoid copying
	bool first = true;
	for (auto &i : s) {
		if (!first)
			ost << ", ";
		ost << i;
		first = false;
	}
	return ost;
}

template<typename ... Ts>
std::string concat(Ts&&...args) {
	std::stringstream ss;
	((ss << std::forward<Ts>(args)), ...);
	return ss.str();
}

inline std::string add_new_line_spaces(const std::string &str, size_t num) {
	std::string res_str;
	bool add = true;
	for (char c : str) {
		if (add) {
			res_str += std::string(num, ' ');
			add = false;
		}
		res_str.push_back(c);
		if (c == '\n')
			add = true;
	}
	return res_str;
}

