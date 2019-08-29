#ifndef BASE_NONCOPYABLE_H_
#define BASE_NONCOPYABLE_H_
namespace dachun {
class NonCopyable {
protected:
    NonCopyable() {}
    ~NonCopyable() {}
private:
    NonCopyable(const NonCopyable&);
    const NonCopyable& operator=(const NonCopyable&);
};
} // namespace dachun
#endif // BASE_NONCOPYABLE_H_