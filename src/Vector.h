#pragma once
#include <memory>
#include <type_traits>
#include <iterator>
namespace structs
{

	template <typename T, typename Alloc = std::allocator<T>>
	class Vector
	{
		size_t size = 0;
		size_t capacity = 0;
		T*	   arr = nullptr;
		Alloc  alloc;

		template <bool isConst>
		class BaseIterator
		{
			using difference_type = int;
			using value_type = std::conditional_t<isConst, const T, T>;
			using pointer = std::conditional_t<isConst, const T*, T*>;
			using reference = std::conditional_t<isConst, const T&, T&>;
			using iterator_category = std::random_access_iterator_tag;

			pointer ptr = nullptr;

			friend class Vector<T>;

			BaseIterator(pointer _ptr)
				: ptr(_ptr)
			{
			}

		public:
			reference operator*()
			{
				return *ptr;
			}

			pointer operator->()
			{
				return ptr;
			}

			BaseIterator& operator++()
			{
				++ptr;
				return *this;
			}

			BaseIterator operator++(int)
			{
				pointer oldptr = ptr;
				++ptr;

				return BaseIterator(oldptr);
			}

			BaseIterator& operator--()
			{
				--ptr;
				return *this;
			}

			BaseIterator operator--(int)
			{
				pointer oldptr = ptr;
				--ptr;

				return BaseIterator(oldptr);
			}

			BaseIterator& operator+=(difference_type value)
			{
				ptr += value;
				return *this;
			}

			BaseIterator& operator-=(difference_type value)
			{
				ptr -= value;
				return *this;
			}

			template <bool Cond>
			bool operator!=(BaseIterator<Cond>& other)
			{
				return ptr != other.ptr;
			}

			template <bool Cond>
			bool operator==(BaseIterator<Cond>& other)
			{
				return ptr == other.ptr;
			}

			operator BaseIterator<true>()
			{
				return { ptr };
			}
		};

		void destroy()
		{
			for (size_t i = 0; i < size; ++i)
			{
				std::allocator_traits<Alloc>::destroy(alloc, arr + i);
			}
			std::allocator_traits<Alloc>::deallocate(alloc, arr, capacity);
		}

	public:
		using ConstIterator = BaseIterator<true>;
		using Iterator = BaseIterator<false>;

		Vector() = default;

		Vector(const Vector& other)
		{
			arr = std::allocator_traits<Alloc>::allocate(alloc, other.capacity);
			capacity = other.capacity;
			for (size_t i = 0; i < other.size; ++i)
			{
				std::allocator_traits<Alloc>::construct(alloc, arr + i, *(other.arr + i));
				size++;
			}
		}

		Vector(Vector&& other) noexcept
		{
			std::swap(arr, other.arr);
			std::swap(size, other.size);
			std::swap(capacity, other.capacity);
		}

		~Vector()
		{
			if (capacity)
			{
				destroy();
			}
		}

		Vector& operator=(const Vector& other)
		{
			if (capacity)
			{
				destroy();
			}
			arr = std::allocator_traits<Alloc>::allocate(alloc, other.capacity);
			capacity = other.capacity;
			for (size_t i = 0; i < other.size; ++i)
			{
				std::allocator_traits<Alloc>::construct(alloc, arr + i, *(other.arr + i));
				size++;
			}
		}

		Vector& operator=(Vector&& other)
		{
			if (capacity)
			{
				destroy();
			}

			size = 0;
			capacity = 0;
			arr = nullptr;

			std::swap(arr, other.arr);
			std::swap(size, other.size);
			std::swap(capacity, other.capacity);
			return *this;
		}

		T& operator[](size_t index)
		{
			return *(arr + index);
		}

		const T& operator[](size_t index) const
		{
			return *(arr + index);
		}

		T& at(size_t index)
		{
			if (index > size - 1)
			{
				throw std::out_of_range("out of range");
			}
			return *(arr + index);
		}

		const T& at(size_t index) const
		{
			if (index > size - 1)
			{
				throw std::out_of_range("out of range");
			}
			return *(arr + index);
		}

		void reserve(size_t amount)
		{
			if (capacity >= amount)
			{
				return;
			}
			T*	   newarr = std::allocator_traits<Alloc>::allocate(alloc, amount);
			size_t i = 0;
			if constexpr (!std::is_nothrow_move_constructible_v<T>)
			{
				if constexpr (std::is_copy_constructible_v<T>)
				{
					try
					{
						for (; i < size; ++i)
						{
							std::allocator_traits<Alloc>::construct(alloc, newarr + i, *(arr + i));
						}
					}
					catch (...)
					{
						for (size_t j = 0; j <= i; ++j)
						{
							std::allocator_traits<Alloc>::destroy(alloc, newarr + j);
						}
						std::allocator_traits<Alloc>::deallocate(alloc, newarr, i + 1);
						throw;
					}
				}
				else
				{
					for (; i < size; ++i)
					{
						std::allocator_traits<Alloc>::construct(alloc, newarr + i, std::move(*(arr + i)));
					}
				}
			}
			else
			{
				for (; i < size; ++i)
				{
					std::allocator_traits<Alloc>::construct(alloc, newarr + i, std::move(*(arr + i)));
				}
			}

			std::allocator_traits<Alloc>::deallocate(alloc, arr, size);
			arr = newarr;
			capacity = amount;
		}

		void pushBack(const T& value)
		{
			emplaceBack(value);
		}

		void pushBack(T&& value)
		{
			emplaceBack(std::move(value));
		}

		template <typename... Args>
		void emplaceBack(Args&&... args)
		{
			if (size >= capacity)
			{
				reserve(capacity < 4 ? 4 : capacity * 2);
			}

			std::allocator_traits<Alloc>::construct(alloc, arr + size, std::forward<Args>(args)...);

			size++;
		}

		size_t getSize() const
		{
			return size;
		}

		size_t getCapacity() const
		{
			return capacity;
		}

		Iterator begin()
		{
			return Iterator(arr);
		}

		Iterator end()
		{
			return Iterator(arr + size);
		}

		ConstIterator cbegin() const
		{
			return ConstIterator(arr);
		}

		ConstIterator cend() const
		{
			return ConstIterator(arr + size);
		}
	};
} // namespace structs