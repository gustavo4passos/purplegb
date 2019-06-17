#pragma once

namespace pgb
{

class BitManip
{
public:
	template<typename T>
	static bool BitTest(T data, unsigned bit)
	{
		return (bool)(data & (0b1 << bit));
	}

	template <typename T>
	static T SetBitmask(unsigned bit)
	{
		T data = 0x0;
		return data | ((T)(0x1 << bit));
	}

	template <typename T>
	static T UnsetBitmask(unsigned bit)
	{
		T data = 0x0;
		data = data | ((T)(0x1 << bit));
		data = ~data;
		return data;
	}

	template<typename T>
	static void SetBit(T& data, unsigned bit)
	{
		data |= SetBitmask<T>(bit);
	}

	template<typename T>
	static void UnsetBit(T& data, unsigned bit)
	{
		data &= UnsetBitmask<T>(bit);
	}
};

}