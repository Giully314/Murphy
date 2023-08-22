/*
* murphy/ds/buffer.ixx
* 
* PURPOSE: manage buffer of bytes.
* 
* CLASSES:
*	DynamicBuffer: dynamic memory buffer of bytes.
*	NonOwnedBuffer: buffer of bytes without ownership.
* 
*/

export module murphy.ds.buffer;

import murphy.types;

import <utility>;

export
namespace murphy::ds
{
	class DynamicBuffer
	{
	public:
		DynamicBuffer() = default;

			
		DynamicBuffer(const DynamicBuffer& other) : data(CreateUniqueRes<byte[]>(other.size)),
			size(other.size)
		{
			std::memcpy(data.get(), other.data.get(), size);
		}

		[[nodiscard]]
		auto operator=(const DynamicBuffer& other) -> DynamicBuffer&
		{
			data = CreateUniqueRes<byte[]>(other.size);
			std::memcpy(data.get(), other.data.get(), size);
			size = other.size;
			return *this;
		}

		DynamicBuffer(DynamicBuffer&& other) noexcept : data(std::move(other.data)), 
			size(std::exchange(other.size, 0))
		{
		}

		[[nodiscard]]
		auto operator=(DynamicBuffer&& other) noexcept -> DynamicBuffer&
		{
			data = std::move(other.data);
			size = std::exchange(other.size, 0);
			return *this;
		}

		// Append s bytes at the end. Always allocates additional memory.
		// The memory allocated is always the stricly necessary to fit the data,
		// no growth factor is used (like in std::vector).
		auto Append(const non_owned_res<const byte> p, const u64 s) -> void;

		// Reserve new_capacity bytes and if needed reallocate. 
		// If new capacity <= current capacity do nothing.
		auto Reserve(const u64 new_capacity) -> void;


		auto Free() -> void
		{
			data.release();
			size = 0;
			capacity = 0;
		}


		// Could be nullptr.
		[[nodiscard]]
		auto Data() const noexcept -> non_owned_res<byte>
		{
			return data.get();
		}

		[[nodiscard]]
		auto Size() const noexcept -> u64
		{
			return size;
		}

		[[nodiscard]]
		auto Capacity() const noexcept -> u64
		{
			return capacity;
		}

	private:
		// Allocate new memory and copy all the the data into the new memory.
		// Precondition: s > size.
		auto AllocateAndCopy(const u64 new_size) -> void;

		[[nodiscard]]
		auto ShouldRealloc(const u64 check_size) -> bool
		{
			return check_size > capacity;
		}

	private:
		unique_res<byte[]> data;
		u64 size = 0; 
		u64 capacity = 0;
	};


	class NonOwnedBuffer
	{
	public:
		NonOwnedBuffer() = default;

		NonOwnedBuffer(const DynamicBuffer& b) : data(b.Data()), size(b.Size())
		{

		}

		NonOwnedBuffer(non_owned_res<byte> d, const u64 s) :
			data(d), size(s)
		{

		}
		
		[[nodiscard]]
		auto Data(const u64 idx = 0) const noexcept -> non_owned_res<byte>
		{
			return &data[idx];
		}

		[[nodiscard]]
		auto Size() const noexcept -> u64
		{
			return size;
		}


	private:
		non_owned_res<byte> data = nullptr;
		u64 size = 0;
	};


	inline
	[[nodiscard]]
	auto operator==(const NonOwnedBuffer& lhs, const NonOwnedBuffer& rhs) -> bool
	{
		return lhs.Size() == rhs.Size() && (std::memcmp(lhs.Data(), rhs.Data(), lhs.Size()) == 0);
	}


	inline
	[[nodiscard]]
	auto operator==(const DynamicBuffer& lhs, const DynamicBuffer& rhs) -> bool
	{
		return lhs.Size() == rhs.Size() && (std::memcmp(lhs.Data(), rhs.Data(), lhs.Size()) == 0);
	}


	inline
	[[nodiscard]]
	auto operator==(const NonOwnedBuffer& lhs, const DynamicBuffer& rhs) -> bool
	{
		return lhs.Size() == rhs.Size() && (std::memcmp(lhs.Data(), rhs.Data(), lhs.Size()) == 0);
	}

	inline
	[[nodiscard]]
	auto operator==(const DynamicBuffer& lhs, const NonOwnedBuffer& rhs) -> bool
	{
		return lhs.Size() == rhs.Size() && (std::memcmp(lhs.Data(), rhs.Data(), lhs.Size()) == 0);
	}

} // namespace murphy::ds