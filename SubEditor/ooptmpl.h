#pragma once
namespace oop {
	
	template <typename T>
	class vector
	{
		T* elems;
		int _size;
	public:
		vector()
			: elems(nullptr), _size(0)	{}
		~vector()
		{
			if (_size == 0) return;
			delete[] elems;
		}
		int size() const
		{
			return _size;
		}
		void add(const T& elem)
		{
			T* temp = elems;
			elems = (T*)malloc(sizeof(T) * (_size + 1));
			memcpy(elems, temp, _size * sizeof(T));
			memcpy(elems + _size, &elem, sizeof(T));
			if (_size > 0)
			delete[] temp;
			_size += 1;
		}
		void insert(int index, const T& elem)
		{
			T* temp = elems;
			elems = (T*)malloc(sizeof(T) * (_size + 1));
			memcpy(elems, temp, index * sizeof(T));
			memcpy(elems + index, &elem, sizeof(T));
			memcpy(elems + index + 1, temp + index, (_size - index) * sizeof(T));
			if (_size > 0)
				delete[] temp;
			_size += 1;
		}
		void remove(int index)
		{
			if (index < 0 || index >= _size)
				return;
// fix this !?!?!?!
//			delete (elems + index);
			memmove(elems + index, elems + index + 1, sizeof(T) * (_size - index - 1));
			_size -= 1;
		}
		T& operator[](int index) const
		{
			return *(elems + index);
		}
		void clear()
		{
			if (_size == 0) return;
			delete[] elems;
			_size = 0;
		}
		int contains(const T& elem) const
		{
			if (_size == 0) return -1;
			for (int i = 0; i < size(); i++)
				if (*(elems + i) == elem)
					return i;
			return -1;
		}
	};

	template<typename A, typename B>
	class pair
	{
	public:
		A a;
		B b;
		pair(A a, B b)
			: a(a), b(b) {}
	};

	template<typename T>
	class unique_ptr
	{
		T* ptr;
	public:
		unique_ptr(T* ptr)
			: ptr(ptr) {}
		unique_ptr(const unique_ptr& other) = delete;
		~unique_ptr()
		{
			delete ptr;
		}
		T& get() const
		{
			return *ptr;
		}
	};

	template<typename T>
	class shared_ptr
	{
		T* ptr;
	public:
		static vector<pair<T*, int>> sp_table;
		shared_ptr(T* ptr) : ptr(ptr)
		{
			for (int i = 0; i < sp_table.size(); i++) {
				if (sp_table[i].a == ptr) {
					sp_table[i].b++;
					return;
				}
			}
			sp_table.add(pair<T*, int>(ptr, 1));
		}

		shared_ptr(const shared_ptr<T>& other) : ptr(other.ptr)
		{
			for (int i = 0; i < sp_table.size(); i++) {
				if (sp_table[i].a == ptr) 
					sp_table[i].b++;
			}
		}
		~shared_ptr()
		{
			for (int i = 0; i < sp_table.size(); i++) {
				if (sp_table[i].a == ptr) {
					sp_table[i].b--;
					if (sp_table[i].b == 0)
						delete ptr;
					break;
				}
			}
		}
		T& get() const
		{
			return *ptr;
		}
	};

	template<typename T>
	vector<pair<T*, int>> shared_ptr<T>::sp_table = vector<pair<T*, int>>();
}