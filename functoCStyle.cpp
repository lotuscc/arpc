#include <cassert>
#include <functional>
#include <iostream>
#include <memory>


// copy from https://stackoverflow.com/questions/13238050/convert-stdbind-to-function-pointer


// Raw Bind - simulating auto storage behavior for static storage data
template <typename BindFunctor, typename FuncWrapper>
class scoped_raw_bind {
public:
    typedef scoped_raw_bind<BindFunctor, FuncWrapper> this_type;

    // Make it Move-Constructible only
    scoped_raw_bind(const this_type &) = delete;
    this_type &operator=(const this_type &) = delete;
    this_type &operator=(this_type &&rhs) = delete;

    scoped_raw_bind(this_type &&rhs) : m_owning(rhs.m_owning) {
        rhs.m_owning = false;
    }

    scoped_raw_bind(BindFunctor b) : m_owning(false) {
        // Precondition - check that we don't override static data for another
        // raw bind instance
        if (get_bind_ptr() != nullptr) {
            assert(false);
            return;
        }
        // Smart pointer is required because bind expression is
        // copy-constructible but not copy-assignable
        get_bind_ptr().reset(new BindFunctor(b));
        m_owning = true;
    }

    ~scoped_raw_bind() {
        if (m_owning) {
            assert(get_bind_ptr() != nullptr);
            get_bind_ptr().reset();
        }
    }

    decltype(&FuncWrapper::call) get_raw_ptr() {
        return &FuncWrapper::call;
    }

    static BindFunctor &get_bind() {
        return *get_bind_ptr();
    }

private:
    bool m_owning;

    static std::unique_ptr<BindFunctor> &get_bind_ptr() {
        static std::unique_ptr<BindFunctor> s_funcPtr;
        return s_funcPtr;
    }
};

// Handy macro for creating raw bind object
// W is target function wrapper, B is source bind expression
#define RAW_BIND(W, B)                                                         \
    std::move(scoped_raw_bind<decltype(B), W<decltype(B), __COUNTER__>>(B));

// Usage
///////////////////////////////////////////////////////////////////////////

// Target raw function signature
typedef void (*TargetFuncPtr)(double, int, const char *);

// Function that need to be called via bind
void f(double d, int i, const char *s1, const char *s2) {
    std::cout << "f(" << d << ", " << i << ", " << s1 << ", " << s2 << ")"
              << std::endl;
}

// Wrapper for bound function
// id is required to generate unique type with static data for
// each raw bind instantiation.
// THE ONLY THING THAT YOU NEED TO WRITE MANUALLY!
template <typename BindFunc, int id = 0>
struct fWrapper {
    static void call(double d, int i, const char *s) {
        scoped_raw_bind<BindFunc, fWrapper<BindFunc, id>>::get_bind()(d, i, s);
    }
};

int main() {
    using namespace std::placeholders;

    auto rf1 =
        RAW_BIND(fWrapper, std::bind(&f, _1, _2, _3, "This is f trail - 1"));
    TargetFuncPtr f1 = rf1.get_raw_ptr();
    f1(1.2345, 42, "f1: Bind! Bind!");

    auto rf2 =
        RAW_BIND(fWrapper, std::bind(&f, _1, _2, _3, "This is f trail - 2"));
    TargetFuncPtr f2 = rf2.get_raw_ptr();
    f2(10.2345, 420, "f2: Bind! Bind!");

    auto rf3 =
        RAW_BIND(fWrapper, std::bind(&f, _1, _2, _3, "This is f trail - 3"));
    TargetFuncPtr f3 = rf3.get_raw_ptr();
    f3(100.2345, 4200, "f3: Bind! Bind!");
}