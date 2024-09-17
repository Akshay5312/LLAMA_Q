#pragma once

namespace LLAMA{
namespace Lists {
	/// <summary>
	///	A Dynamic list. O(n) time for insertion and O(1) for fetching.
	///		changed from a linked list for simplicity. Will be changed to a more efficient data structure.
	/// </summary>
	/// <typeparam name="T"> the type of object</typeparam>
	template<class T>
	class AList{
	private:
		T** _elements = nullptr;//new T*[0];
		int* _indeces = nullptr;//new int[0];
		int _size = 0;

	public:
		AList();

		~AList();
		
		AList(const AList& AL);
		AList& operator = (const AList& AL);


		/// <summary>
		/// Determines whether this instance is empty.
		/// </summary>
		/// <returns>
		///   <c>true</c> if this instance is empty; otherwise, <c>false</c>.
		/// </returns>
		bool isEmpty() { return _size == 0; }

		/// <summary>
		/// Gets the size.
		/// </summary>
		/// <returns> the size of the list</returns>
		int getSize();

		/// <summary>
		/// Appends the specified append value.
		/// </summary>
		/// <param name="appendVal">The append value.</param>
		/// <returns> the index it is appended at</returns>
		int append(T* appendVal);

		/// <summary>
		/// Empties this instance.
		/// </summary>
		void empty();


		/// <summary>
		/// Gets the value at index index
		/// </summary>
		/// <param name="index">The index.</param>
		/// <returns>the pointer stored at the index</returns>
		T* getValue(int index);

		/// <summary>
		/// gets the index of the passed pointer in the list. returns -1 if it soes not exist
		/// </summary>
		/// <param name="lookForPtr">The ptr of the object to look for.</param>
		/// <returns> the index of the passed pointer. returns -1 if it does not exist</returns>
		int indexOf(T* lookForPtr);

		/// <summary>
		/// Determines whether the specified object exists in the list.
		/// </summary>
		/// <param name="lookForPtr">The ptr of the object to look for.</param>
		/// <returns>
		///   <c>true</c> if the specified object exists in the list; otherwise, <c>false</c>.
		/// </returns>
		bool contains(T* lookForPtr);
								

		///// <summary>
		///// Determines whether an equivalent to specified object exists in the list. operator == must be defined for T
		///// </summary>
		///// <param name="lookForEquivalent">The equivalent to the object to look for.</param>
		///// <returns>
		/////   <c>true</c> if an equivalent to the specified object exists in the list; otherwise, <c>false</c>.
		///// </returns>
		//bool contains(T lookForEquivalent);
	};
}


template<class T>
int Lists::AList<T>::getSize() { return _size; }

template<class T>
Lists::AList<T>::AList() {
}

template<class T>
inline Lists::AList<T>::~AList() {
	if(_elements == nullptr){}else{
		if(_size == 0){
		}else{
			delete[] _elements;
		}
	}
	if(_indeces == nullptr){}else{
		if(_size == 0){
		}else{
			delete[] _indeces;
		}
	}
	
}

template<class T>
Lists::AList<T>::AList(const AList& AL)
{
	_size = AL._size;


	_elements = new T * [_size];
	_indeces = new int[_size];
	for (int i = 0; i < _size; i++) {
		_elements[i] = AL._elements[i];
		_indeces[i] = AL._indeces[i];
	}
}

template<class T>
Lists::AList<T>& Lists::AList<T>::operator=(const Lists::AList<T>& AL)
{
	_size = AL._size;


	_elements = new T * [_size];
	_indeces = new int[_size];

	for (int i = 0; i < _size; i++) {
		_elements[i] = AL._elements[i];
		_indeces[i] = AL._indeces[i];
	}
	return *this;
}

/*template<class T>
AList<T>::~Alist()
{
}*/

template<class T>
int Lists::AList<T>::append(T* appendVal)
{
	if (_size == 0) {
		_size++;

		delete _elements;
		delete _indeces;
		_elements = new T * [_size];
		_indeces = new int[_size];

		_elements[0] = appendVal;
		_indeces[0] = 0;

		return 0;
	}

	//otherwise, elements are already added.

	T** tempEl = new T * [_size];
	int* tempIndeces = new int[_size];

	for (int i = 0; i < _size; i++) {
		tempEl[i] = _elements[i];
		tempIndeces[i] = _indeces[i];
	}

	delete[] _elements;
	delete[] _indeces;

	_size++;

	_elements = new T * [_size];
	_indeces = new int[_size];

	for (int i = 0; i < _size - 1; i++) {
		_elements[i] = tempEl[i];
		_indeces[i] = tempIndeces[i];
	}
	delete[] tempEl;
	delete[] tempIndeces;


	_elements[_size - 1] = appendVal;
	_indeces[_size - 1] = _indeces[_size - 2] + 1;

	return _indeces[_size - 1];
}

template<class T>
void Lists::AList<T>::empty()
{
	for (int i = 0; i < _size; i++) {
		delete _elements[i];
	}
}

template<class T>
T* Lists::AList<T>::getValue(int index)
{
	int retIndex = -1;

	//if index is out of bounds, less than 0
	if ((index < 0)) {
		for (int i = 0; i < _size; i++) {
			//check each index for it 
			if (_indeces[i] == index) {
				retIndex = i; i = _size;
			}
		}

		if (retIndex == -1) { return nullptr; }
		else { return _elements[retIndex]; }
	}

	//if index is out of bounds, greater than _size
	if (index >= _size) {
		for (int i = _size - 1; i >= 0; i--) {
			//check each index for it 
			if (_indeces[i] == index) {
				retIndex = i; i = -1;
			}
		}

		if (retIndex == -1) { return nullptr; }
		else { return _elements[retIndex]; }
	}


	//otherwise look around.
	retIndex = index;
	if (_indeces[index] > index) {
		for (int i = index - 1; i >= 0; i--) {
			if (_indeces[i] == index) {
				return _elements[i];
			}
		}
		return nullptr;
	}

	for (int i = index; i < _size; i++) {
		if (_indeces[i] == index) {
			return _elements[i];
		}
	}

	return nullptr;
}

template<class T>
int Lists::AList<T>::indexOf(T* lookForPtr)
{
	for (int i = 0; i < _size; i++) {
		if (_elements[i] == lookForPtr) {
			return i;
		}
	}
	return -1;
}

template<class T>
inline bool Lists::AList<T>::contains(T* lookForPtr)
{
	return !(indexOf(lookForPtr) == -1);
}

}