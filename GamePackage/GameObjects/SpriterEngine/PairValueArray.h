#pragma once
#include<vector>
template<class T>
class PairValueArray {
	std::vector<T>m_array;
	T m_intialValue;
public:
	void Init(int terminalNum, T intialValue) {
		int n = (terminalNum-1)*(terminalNum) / 2;
		m_array = std::vector<T>(n,intialValue);
		m_intialValue = intialValue;
	}
	void SetValue(int t1, int t2, T value) {
		if (t1 == t2) 
			return;
		if (t2 < t1) {
			T temp = t2;
			t2 = t1;
			t1 = temp;
		}
		//out of range checking
		int temp = t2*(t2 + 1) / 2;
		if (temp > m_array.size())
			return ;

		int n = m_array.size();
		int index = t1*n - (t1*(t1 + 1)) / 2 + (t2 - t1 - 1);
		m_array[index] = value;
	}
	T GetValue(int t1, int t2) {
		if (t1 == t2)
			return m_intialValue;
		if (t2 < t1) {
			T temp = t2;
			t2 = t1;
			t1 = temp;
		}
		//out of range checking
		int temp = t2*(t2 + 1) / 2;
		if (temp > m_array.size()) 
			return m_intialValue;

		int n = m_array.size();
		int index = t1*n - (t1*(t1 + 1)) / 2 + (t2 - t1 - 1);
		return m_array[index];
	}
};
