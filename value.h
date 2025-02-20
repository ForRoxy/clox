#pragma once

#include <iostream>
#include <vector>

template <class Value = double>
struct ValueArray {
	std::vector<Value> values;  // ʹ�� std::vector �Զ������ڴ�

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
		os << "count: " << valueArray.values.size() << std::endl;  // ʹ�� vector �� size()
		os << "values: ";
		for (const auto& val : valueArray.values) {
			os << val << "  ";
		}
		os << std::endl;
		return os;
	}
};
