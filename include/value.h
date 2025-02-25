#pragma once

#include <iostream>
#include <vector>

template <class Value = double>
struct ValueArray {
	std::vector<Value> values;  // 使用 std::vector 自动管理内存

	ValueArray() = default;
	ValueArray(const Value& value) { addValue(value); }
	~ValueArray() = default;

	void addValue(const Value& value) {
		values.push_back(value); 
	}

	void printValue(const Value& value) const {
		std::cout << value;
	}

	int decValue() {
		if (values.empty()) return -1;  
		values.pop_back();
		return values.size();
	}

	friend std::ostream& operator<<(std::ostream& os, const ValueArray<Value>& valueArray) {
		os << "count: " << valueArray.values.size() << std::endl;  // 使用 vector 的 size()
		os << "values: ";
		for (const auto& val : valueArray.values) {
			os << val << "  ";
		}
		os << std::endl;
		return os;
	}
};
