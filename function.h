#ifndef __FUNCTION_H__
#define __FUNCTION_H__

template <size_t Size, typename returnType, typename... Types>
std::array<returnType, Size> map(returnType f(Types...),
                                 std::array<Types, Size>... args)
{
    std::array<returnType, Size> result;
    for (size_t i = 0; i < Size; ++i)
    {
        result[i] = f(args[i]...);
    }
    return result;
}

#endif
