#ifndef UTIL_NON_COPYABLE_H
#define UTIL_NON_COPYABLE_H 1

class NonCopyable
{
public:
    NonCopyable() {}
    ~NonCopyable() {}

    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

#endif
