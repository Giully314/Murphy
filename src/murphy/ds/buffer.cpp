/*
* murphy/ds/buffer.cpp
*/


module murphy.ds.buffer;

import <cstring>;

namespace murphy::ds
{
	auto DynamicBuffer::Append(const non_owned_res<const byte> p, const u64 s) -> void
	{
		// data is non null, allocate new memory and copy all.
		if (ShouldRealloc(size + s)) 
		{
			Reserve(size + s);
		}
		std::memcpy(&data[size], p, s);
		size += s;
	}


	auto DynamicBuffer::Reserve(const u64 new_capacity) -> void
	{
		if (new_capacity <= capacity)
		{
			return;
		}

		AllocateAndCopy(new_capacity);
	}


	auto DynamicBuffer::AllocateAndCopy(const u64 new_capacity) -> void
	{
		auto new_data = CreateUniqueRes<byte[]>(new_capacity);
		
		if (data)
		{
			std::memcpy(new_data.get(), data.get(), size);
		}
		data = std::move(new_data);
		capacity = new_capacity;
	}
} // namespace murphy::ds