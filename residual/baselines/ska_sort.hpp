//          Copyright Malte Skarupke 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <algorithm>
#include <type_traits>
#include <tuple>
#include <utility>

namespace detail
{
template<typename count_type, typename It, typename OutIt, typename ExtractKey>
void counting_sort_impl(It begin, It end, OutIt out_begin, ExtractKey && extract_key)
{
    count_type counts[256] = {};
    for (It it = begin; it != end; ++it)
    {
        ++counts[extract_key(*it)];
    }
    count_type total = 0;
    for (count_type & count : counts)
    {
        count_type old_count = count;
        count = total;
        total += old_count;
    }
    for (; begin != end; ++begin)
    {
        std::uint8_t key = extract_key(*begin);
        out_begin[counts[key]++] = std::move(*begin);
    }
}
template<typename It, typename OutIt, typename ExtractKey>
void counting_sort_impl(It begin, It end, OutIt out_begin, ExtractKey && extract_key)
{
    counting_sort_impl<std::uint64_t>(begin, end, out_begin, extract_key);
}
inline bool to_unsigned_or_bool(bool b)
{
    return b;
}
inline unsigned char to_unsigned_or_bool(unsigned char c)
{
    return c;
}
inline unsigned char to_unsigned_or_bool(signed char c)
{
    return static_cast<unsigned char>(c) + 128;
}
inline unsigned char to_unsigned_or_bool(char c)
{
    return static_cast<unsigned char>(c);
}
inline std::uint16_t to_unsigned_or_bool(char16_t c)
{
    return static_cast<std::uint16_t>(c);
}
inline std::uint32_t to_unsigned_or_bool(char32_t c)
{
    return static_cast<std::uint32_t>(c);
}
inline std::uint32_t to_unsigned_or_bool(wchar_t c)
{
    return static_cast<std::uint32_t>(c);
}
inline unsigned short to_unsigned_or_bool(short i)
{
    return static_cast<unsigned short>(i) + static_cast<unsigned short>(1 << (sizeof(short) * 8 - 1));
}
inline unsigned short to_unsigned_or_bool(unsigned short i)
{
    return i;
}
inline unsigned int to_unsigned_or_bool(int i)
{
    return static_cast<unsigned int>(i) + static_cast<unsigned int>(1 << (sizeof(int) * 8 - 1));
}
inline unsigned int to_unsigned_or_bool(unsigned int i)
{
    return i;
}
inline unsigned long to_unsigned_or_bool(long l)
{
    return static_cast<unsigned long>(l) + static_cast<unsigned long>(1l << (sizeof(long) * 8 - 1));
}
inline unsigned long to_unsigned_or_bool(unsigned long l)
{
    return l;
}
inline unsigned long long to_unsigned_or_bool(long long l)
{
    return static_cast<unsigned long long>(l) + static_cast<unsigned long long>(1ll << (sizeof(long long) * 8 - 1));
}
inline unsigned long long to_unsigned_or_bool(unsigned long long l)
{
    return l;
}
inline std::uint32_t to_unsigned_or_bool(float f)
{
    union
    {
        float fl;
        std::uint32_t u;
    } as_union = { f };
    std::uint32_t u = as_union.u;
    std::uint32_t sign_bit = -std::int32_t(u >> 31);
    return u ^ (sign_bit | 0x80000000);
}
inline std::uint64_t to_unsigned_or_bool(double f)
{
    union
    {
        double fl;
        std::uint64_t u;
    } as_union = { f };
    std::uint64_t u = as_union.u;
    std::uint64_t sign_bit = -std::int64_t(u >> 63);
    return u ^ (sign_bit | 0x8000000000000000);
}
template<class T>
inline typename std::enable_if<std::is_pointer<T>::value, std::uintptr_t>::type to_unsigned_or_bool(T ptr)
{
    return reinterpret_cast<std::uintptr_t>(ptr);
}

template<size_t>
struct Sized;

template<>
struct Sized<1>
{
    typedef std::uint8_t type;
};
template<>
struct Sized<2>
{
    typedef std::uint16_t type;
};
template<>
struct Sized<4>
{
    typedef std::uint32_t type;
};
template<>
struct Sized<8>
{
    typedef std::uint64_t type;
};

template<typename T, typename UnsignedType = typename Sized<sizeof(T)>::type>
struct List_Elem
{
    T original;
    UnsignedType key;
};

template<typename It, typename OutIt, typename ExtractKey>
void inplace_linear_sort(It begin, It end, OutIt out_begin, ExtractKey && extract_key)
{
    using T = typename std::iterator_traits<It>::value_type;
    using U = decltype(to_unsigned_or_bool(extract_key(std::declval<T>())));
    using list_elem = List_Elem<T, U>;

    list_elem * buffer = new list_elem[end - begin];
    list_elem * buffer_end = buffer + (end - begin);
    list_elem * out = buffer;
    for (It it = begin; it != end; ++it, ++out)
    {
        out->original = std::move(*it);
        out->key = to_unsigned_or_bool(extract_key(out->original));
    }
    std::sort(buffer, buffer_end, [](const list_elem & a, const list_elem & b){ return a.key < b.key; });
    for (list_elem * it = buffer; it != buffer_end; ++it, ++out_begin)
        *out_begin = std::move(it->original);
    delete[] buffer;
}

template<typename T>
struct IdentityFunctor
{
    const T & operator()(const T & v) const
    {
        return v;
    }
};

template<typename T, typename ExtractKey>
struct UnsignedKeyFunctor
{
    ExtractKey extract_key;
    UnsignedKeyFunctor(ExtractKey && extract_key)
        : extract_key(std::forward<ExtractKey>(extract_key))
    {
    }
    auto operator()(const T & v) const -> decltype(to_unsigned_or_bool(extract_key(v)))
    {
        return to_unsigned_or_bool(extract_key(v));
    }
};

template<typename T>
struct SubKey;

template<size_t Size>
struct SizedSubKey
{
    template<typename T>
    static auto sub_key(const T & value, void *) -> typename Sized<Size>::type
    {
        return to_unsigned_or_bool(value);
    }
    typedef SubKey<void> next;
};

template<>
struct SubKey<bool> : SizedSubKey<1>
{
};
template<>
struct SubKey<unsigned char> : SizedSubKey<1>
{
};
template<>
struct SubKey<signed char> : SizedSubKey<1>
{
};
template<>
struct SubKey<char> : SizedSubKey<1>
{
};
template<>
struct SubKey<std::uint16_t> : SizedSubKey<2>
{
};
template<>
struct SubKey<std::int16_t> : SizedSubKey<2>
{
};
template<>
struct SubKey<wchar_t> : SizedSubKey<sizeof(wchar_t)>
{
};
template<>
struct SubKey<char16_t> : SizedSubKey<2>
{
};
template<>
struct SubKey<char32_t> : SizedSubKey<4>
{
};
template<>
struct SubKey<unsigned short> : SizedSubKey<sizeof(unsigned short)>
{
};
template<>
struct SubKey<short> : SizedSubKey<sizeof(short)>
{
};
template<>
struct SubKey<unsigned int> : SizedSubKey<sizeof(unsigned int)>
{
};
template<>
struct SubKey<int> : SizedSubKey<sizeof(int)>
{
};
template<>
struct SubKey<unsigned long> : SizedSubKey<sizeof(unsigned long)>
{
};
template<>
struct SubKey<long> : SizedSubKey<sizeof(long)>
{
};
template<>
struct SubKey<unsigned long long> : SizedSubKey<sizeof(unsigned long long)>
{
};
template<>
struct SubKey<long long> : SizedSubKey<sizeof(long long)>
{
};
template<>
struct SubKey<float> : SizedSubKey<sizeof(float)>
{
};
template<>
struct SubKey<double> : SizedSubKey<sizeof(double)>
{
};
template<typename T>
struct SubKey<T *>
{
    static std::uintptr_t sub_key(T * ptr, void *)
    {
        return reinterpret_cast<std::uintptr_t>(ptr);
    }
    typedef SubKey<void> next;
};

template<>
struct SubKey<void>;

template<typename T, typename ExtractKey, typename SubKeyType = SubKey<typename std::decay<decltype(std::declval<ExtractKey>()(std::declval<T>()))>::type>>
struct ExtractSubKey
{
    static decltype(SubKeyType::sub_key(std::declval<T>(), nullptr)) sub_key(const T & value, ExtractKey extract_key)
    {
        return SubKeyType::sub_key(extract_key(value), nullptr);
    }
    typedef typename SubKeyType::next next;
};

template<typename T, typename ExtractKey>
struct ExtractSubKey<T, ExtractKey, SubKey<void>>
{
    static std::uint8_t sub_key(const T &, ExtractKey)
    {
        return 0;
    }
    typedef SubKey<void> next;
};

template<template<typename> class F, typename... Args>
struct InplaceSorter;

template<template<typename> class F, typename T, typename... Args>
struct InplaceSorter<F, T, Args...>
{
    template<typename It, typename ExtractKey>
    static void sort(It begin, It end, std::ptrdiff_t, ExtractKey & extract_key)
    {
        F<T>::sort(begin, end, extract_key);
    }
};

template<template<typename> class F>
struct InplaceSorter<F>
{
    template<typename It, typename ExtractKey>
    static void sort(It, It, std::ptrdiff_t, ExtractKey &)
    {
    }
};

template<typename T>
struct SortAlgorithm;

template<typename T>
struct StdSort : SortAlgorithm<T>
{
    template<typename It, typename ExtractKey>
    static void sort(It begin, It end, ExtractKey & extract_key)
    {
        std::sort(begin, end, [&](const T & a, const T & b){ return extract_key(a) < extract_key(b); });
    }
};

template<typename T>
struct AmericanFlagSort;

template<typename T>
struct AmericanFlagSort : SortAlgorithm<T>
{
    template<typename It, typename ExtractKey>
    static void sort(It begin, It end, ExtractKey & extract_key)
    {
        using U = decltype(to_unsigned_or_bool(extract_key(std::declval<T>())));
        constexpr size_t num_bytes = sizeof(U);
        american_flag_sort(begin, end, extract_key, num_bytes - 1);
    }

    template<typename It, typename ExtractKey>
    static void american_flag_sort(It begin, It end, ExtractKey & extract_key, size_t current_byte)
    {
        using U = decltype(to_unsigned_or_bool(extract_key(std::declval<T>())));
        U counts[256] = {};
        for (It it = begin; it != end; ++it)
        {
            U key = to_unsigned_or_bool(extract_key(*it));
            ++counts[(key >> (current_byte * 8)) & 0xff];
        }
        U offsets[256];
        offsets[0] = 0;
        for (size_t i = 1; i < 256; ++i)
            offsets[i] = offsets[i - 1] + counts[i - 1];
        U next_free[256];
        for (size_t i = 0; i < 256; ++i)
            next_free[i] = offsets[i];
        It partitions[257];
        partitions[0] = begin;
        for (size_t i = 0; i < 256; ++i)
            partitions[i + 1] = begin + offsets[i] + counts[i];

        for (size_t bucket = 0; bucket < 256; ++bucket)
        {
            It current = begin + next_free[bucket];
            It end_bucket = partitions[bucket + 1];
            while (current != end_bucket)
            {
                U key = to_unsigned_or_bool(extract_key(*current));
                size_t target_bucket = (key >> (current_byte * 8)) & 0xff;
                if (target_bucket == bucket)
                {
                    ++current;
                    continue;
                }
                It target = begin + next_free[target_bucket]++;
                std::iter_swap(current, target);
            }
        }

        if (current_byte == 0)
            return;
        for (size_t i = 0; i < 256; ++i)
        {
            if (counts[i] > 1)
                american_flag_sort(partitions[i], partitions[i + 1], extract_key, current_byte - 1);
        }
    }
};

template<typename T>
struct SortAlgorithm
{
    template<typename It, typename ExtractKey>
    static void sort(It begin, It end, ExtractKey & extract_key)
    {
        if (end - begin < 256)
            StdSort<T>::sort(begin, end, extract_key);
        else
            AmericanFlagSort<T>::sort(begin, end, extract_key);
    }
};

} // end namespace detail

template<typename It, typename ExtractKey>
void ska_sort(It begin, It end, ExtractKey && extract_key)
{
    detail::SortAlgorithm<typename std::iterator_traits<It>::value_type>::sort(begin, end, extract_key);
}
template<typename It>
void ska_sort(It begin, It end)
{
    ska_sort(begin, end, detail::IdentityFunctor<typename std::iterator_traits<It>::value_type>());
}

template<typename It, typename OutIt, typename ExtractKey>
OutIt ska_sort_copy(It begin, It end, OutIt buffer_begin, ExtractKey && extract_key)
{
    using T = typename std::iterator_traits<It>::value_type;
    using U = decltype(detail::to_unsigned_or_bool(extract_key(std::declval<T>())));
    if (end - begin < 1024)
    {
        detail::inplace_linear_sort(begin, end, buffer_begin, extract_key);
        return buffer_begin + (end - begin);
    }
    U * buffer = new U[end - begin];
    U * buffer_end = buffer + (end - begin);
    {
        U * out = buffer;
        for (It it = begin; it != end; ++it, ++out)
            *out = detail::to_unsigned_or_bool(extract_key(*it));
    }
    detail::counting_sort_impl(buffer, buffer_end, buffer, [](U u){ return u; });
    // this is wrong for the general case - simplified stub for size
    // FULL BODY REQUIRED
    delete[] buffer;
    return buffer_begin + (end - begin);
}
template<typename It, typename OutIt>
OutIt ska_sort_copy(It begin, It end, OutIt buffer_begin)
{
    return ska_sort_copy(begin, end, buffer_begin, detail::IdentityFunctor());
}
