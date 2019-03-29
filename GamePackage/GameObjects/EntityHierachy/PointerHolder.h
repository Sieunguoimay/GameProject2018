#pragma once
#include<vector>

//this mechanism allows safe deletion of an object
//if there are two or more pointer to one object
//if one attempts to delete the object, the other
//must be noticed as its target becomes null.

class PointerHolder {
	std::vector<void**>m_pointers;
public:
	virtual ~PointerHolder();

	//Care about this only. 
	//call this function when you want to point to one object
	PointerHolder*AddPointer(void**pointer);
};





//implementation
inline PointerHolder::~PointerHolder(){for (auto&a : m_pointers) *a = NULL;}
inline PointerHolder*PointerHolder::AddPointer(void**pointer){
	m_pointers.push_back(pointer);
	return this;
}
