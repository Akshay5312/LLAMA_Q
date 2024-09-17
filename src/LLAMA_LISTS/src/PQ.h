#pragma once
namespace LLAMA{
namespace Lists {
	/// <summary>
	/// Priority queue
	/// O(n) time insert, O(n) time to pop
	/// </summary>
	template<class T>
	class PQ{
	private:
		T** _elements = new T*[0];
		int* _costs = new int[0];
		int _size = 0;
	public:		
		/// <summary>
		///		Initializes a new instance of the <see cref="PQ"/> class.
		/// </summary>
		PQ();

		//delete lists		
		/// <summary>
		///		Finalizes an instance of the <see cref="PQ"/> class.
		/// </summary>
		~PQ();
				

		/// <summary>
		/// Adds the specified value to add.
		/// </summary>
		/// <param name="valueToAdd">The value to add.</param>
		/// <param name="cost">The cost.</param>
		void add(T* valueToAdd, int cost);

		/// <summary>
		/// Pops the element with the leat cost.
		/// </summary>
		/// <returns> the element at the top of the queue</returns>
		T* pop();

		//
		/// <summary>
		/// Gets the cost of the next element popped.
		/// </summary>
		/// <returns></returns>
		int getPopCost();

		/// <summary>
		/// Determines whether this instance is empty.
		/// </summary>
		/// <returns>
		///   <c>true</c> if this instance is empty; otherwise, <c>false</c>.
		/// </returns>
		bool isEmpty();

	};
}



template<class T>
inline Lists::PQ<T>::PQ()
{
}
template<class T>
inline Lists::PQ<T>::~PQ()
{
	delete[] _elements; delete[] _costs;
}
template<class T>
inline void Lists::PQ<T>::add(T* valueToAdd, int cost)
{

	T** tempEl = new T * [_size];
	int* tempCosts = new int[_size];

	for (int i = 0; i < _size; i++) {
		tempEl[i] = _elements[i];
		tempCosts[i] = _costs[i];
	}

	delete[]_elements;
	delete[]_costs;

	_size++;

	_elements = new T * [_size];
	_costs = new int[_size];

	int pauseVal = _size - 1;
	for (int i = 0; i < _size - 1; i++) {
		if (tempCosts[i] < cost) {
			_elements[i] = tempEl[i];
			_costs[i] = tempCosts[i];
		}
		else {
			pauseVal = i;
			i = _size;
		}
	}

	_elements[pauseVal] = valueToAdd;
	_costs[pauseVal] = cost;

	if (pauseVal + 1 < _size) {
		for (int i = pauseVal + 1; i < _size; i++) {
			_elements[i] = tempEl[i - 1];
			_costs[i] = tempCosts[i - 1];
		}
	}
	delete[] tempEl;
	delete[] tempCosts;

}
template<class T>
inline T* Lists::PQ<T>::pop()
{
	if (_size <= 0) { return nullptr; }

	T* retVal = _elements[0];

	_size--;

	T** tempEl = new T * [_size];
	int* tempCosts = new int[_size];
	for (int i = 0; i < _size; i++) {
		tempEl[i] = _elements[i + 1];
		tempCosts[i] = _costs[i + 1];
	}


	delete[]_elements;
	delete[]_costs;

	_elements = new T * [_size];
	_costs = new int[_size];

	for (int i = 0; i < _size; i++) {
		_elements[i] = tempEl[i];
		_costs[i] = tempCosts[i];
	}

	delete[] tempEl;
	delete[] tempCosts;

	return retVal;
}
template<class T>
inline int Lists::PQ<T>::getPopCost()
{
	if (_size < 0)
		return -1;

	return _costs[0];
}
template<class T>
inline bool Lists::PQ<T>::isEmpty()
{
	return _size == 0;
}

}