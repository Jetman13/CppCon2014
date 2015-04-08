#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <memory>
#include <Windows.h>

//Cheap, Simple, and Safe Logging Using Expression Templates
//可变模板参数，这种LOG形式，虽然快，方便，但是不可控，同时代码会庞大；
//不过，方便是主要的。
#define LOG(...) Log(__FILE__, __LINE__, __VA_ARGS__)

template<typename... Args>
void Log(const char* file, int line, const Args&... args)
{
	std::cout << file << "(" << line << "): ";
	LogRecursive(std::cout, args...);
	std::cout << std::endl;
}

template<typename T, typename... Args>
void LogRecursive(std::ostream& os, T first, const Args&... rest)
{
	os << first;
	LogRecursive(os, rest...);
}
inline void LogRecursive(std::ostream& os){}

// Less Code = More Software
// One way is to make it so simple that there are obviously no deficiencies, and
// the other way is to make it so complicated that there are no obvious deficiencies.

namespace Cppcon14
{

	// Test-Driven Performance
	// 实验测试
	template <typename Container> void tester(int iterations)
	{
		srand(1337);
		Container nodesToProcess(1);
		for (auto i = 0; i < iterations; ++i){
			auto rand2 = rand() % 128;//rand() srand() >> stdlib.h (windows)  srandom() random()
			std::vector<typename Container::value_type> nodes(rand2);

			if (!nodesToProcess.empty()) {
				nodesToProcess.pop_back();
			}

			std::for_each(nodes.rbegin(), nodes.rend(), [&nodesToProcess](typename Container::value_type & v) {
				nodesToProcess.push_back(v);
			});
		}
	}

	// Move semantics
	// 1, Using move assignment operator to implement move constructor.
	// 2, Don't forget release resource.
	// 3, Care about self assignment.

	class MemoryBlock
	{
	public:
		// Simple constructor that initializes the resource.
		explicit MemoryBlock(size_t length)
			: _length(length)
			, _data(new int[length])
		{
			std::cout << "In MemoryBlock(size_t). length = "
				<< _length << "." << std::endl;
		}

		// Destructor.
		~MemoryBlock()
		{
			std::cout << "In ~MemoryBlock(). length = " << _length << ".";
			if (_data != nullptr)
			{
				std::cout << "Deleting resource.";
				// Delete the resource.
				delete[] _data;
			}

			std::cout << std::endl;
		}

		// Copy constructor.
		MemoryBlock(const MemoryBlock& other)
			: _length(other._length)
			, _data(new int[other._length])
		{
			std::cout << "In MemoryBlock(const MemoryBlock& ). length = "
				<< _length << ". Copying resource." << std::endl;
			//std::copy(other._data, other._data + _length, _data);
		}

		// Copy assignment operator.
		MemoryBlock& operator=(const MemoryBlock& other)
		{
			std::cout << "In operator=(const MemoryBlock&). length = "
				<< other._length << ". Copying resource." << std::endl;

			if (this != &other)
			{
				// Free the existing resource
				delete[] _data;

				_length = other._length;
				_data = new int[_length];
				//std::copy(other._data, other._data + _length, _data);
			}
			return *this;
		}

		// Move constructor.
		/*
		MemoryBlock(MemoryBlock&& other)
			: _data(other._data)
			, _length(other._length)
		{
			other._data = nullptr;
			other._length = 0;
		}*/
		MemoryBlock(MemoryBlock&& other)
			: _data(nullptr)
			, _length(0)
		{
			// Using the move assignment operator
			*this = std::move(other);
		}

		// Move assignment operator.
		MemoryBlock& operator=(MemoryBlock&& other)
		{
			if (this != &other)
			{
				// Free the existing resource.
				delete[] _data;

				// Copy the data pointer and its lenght from the 
				// source object.
				_data = other._data;
				_length = other._length;

				// Release the data pointer from the source object so that
				// the destructor does not free the memory multiple times.
				other._data = nullptr;
				other._length = 0;
			}
			return *this;
		}

		// Retrieves the length of the data resource.
		size_t Length() const
		{
			return _length;
		}

	private:
		size_t _length; // The Length of the resource.
		int* _data; // The resource.

	};

	void howAboutSmartPtr()
	{
		MemoryBlock mb0{ 100 }; //common constructor
		auto mb1 = std::move(mb0); //move constructor
		//std::unique_ptr<MemoryBlock> uptr = std::make_unique<MemoryBlock>(100);
		//auto uptr2 = std::move(uptr); // Using unique_ptr's move assignment operator
	}//mb0, mb1 destroy


	void cppcon14(void)
	{
		std::cout << "hello world!" << std::endl;
		//可变参数模板
		LOG("sdfsdf", 123);
		tester<std::vector<int>>(1);
		howAboutSmartPtr();
	}
}

int main(void)
{
	Cppcon14::cppcon14();
	system("PAUSE");
	return 0;
}