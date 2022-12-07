/**********************************************************************************************************************/
// ArtiCore
// File: axsc.cpp
/**********************************************************************************************************************/

//Include files
#include "arti.h"

//ArtiInputSourceContainer constructor function
ArtiInputSourceContainer::ArtiInputSourceContainer() :
	source(NULL),
	size(0)
{

}

//ArtiInputSourceContainer destructor function
ArtiInputSourceContainer::~ArtiInputSourceContainer() {
	if(size > 0)
		free(source);
	size = 0;
}

//ArtiInputSourceContainer subscript operator overload function
ArtiInputSource*& ArtiInputSourceContainer::operator[](size_t index) {
	if (index < size)
		return source[index];
	else
		throw ArtiException(std::format("Arti Exception: input source container index out of bounds ({})", size));
}

//ArtiInputSourceContainer add function
void ArtiInputSourceContainer::add(ArtiInputSource* s) {
	if (size == 0) {
		source = (ArtiInputSource**)malloc(sizeof(ArtiInputSource*));

		if (source == NULL)
			throw ArtiException("Arti Exception: failed to allocate memory for input sources\n");

		size = 1;
	}
	else {
		ArtiInputSource** temp = (ArtiInputSource**)realloc(source, (size + 1) * sizeof(ArtiInputSource*));

		if (temp == NULL)
			throw ArtiException("Arti Exception: failed to allocate memory for input sources\n");

		source = temp;

		source[0] = s;

		size++;
	}
}

//ArtiInputSourceContainer get function
ArtiInputSource*& ArtiInputSourceContainer::get(size_t index) {
	if (index < size)
		return source[index];
	else
		throw ArtiException(std::format("Arti Exception: input source container index out of bounds ({})", size));
}

//ArtiInputSourceContainer remove by index function
bool ArtiInputSourceContainer::remove(size_t index) {
	if (size == 0 || index >= size) {
		return false;
	}
	else if (size == 1) {
		free(source);
	}
	else {
		ArtiInputSource** temp = (ArtiInputSource**)malloc((size - 1) * sizeof(ArtiInputSource*));

		if (temp == NULL)
			throw ArtiException("Arti Exception: failed to allocate memory for input sources\n");

		int index = 0;

		for (int i = 0; i < size; ++i) {
			if (i != index) {
				temp[index] = source[i];
				index++;
			}
		}

		free(source);
		source = temp;

		size--;
	}
}

//ArtiInputSourceContainer set function
bool ArtiInputSourceContainer::set(ArtiInputSource* s, size_t index) {
	if (index < size) {
		source[index] = s;
		return true;
	}

	return false;
}