#ifndef FMT_CONFIG_H
#define FMT_CONFIG_H

#ifdef __GNUC__
# define FMT_GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)
# define FMT_GCC_EXTENSION __extension__
# if __cplusplus >= 201103L || defined __GXX_EXPERIMENTAL_CXX0X__
#  define FMT_HAS_GXX_CXX11 1
# endif
#else
# define FMT_GCC_EXTENSION
#endif

#ifdef __clang__
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif

#ifdef __GNUC_LIBSTD__
# define FMT_GNUC_LIBSTD_VERSION (__GNUC_LIBSTD__ * 100 + __GNUC_LIBSTD_MINOR__)
#endif

#ifdef __has_feature
# define FMT_HAS_FEATURE(x) __has_feature(x)
#else
# define FMT_HAS_FEATURE(x) 0
#endif

#ifdef __has_builtin
# define FMT_HAS_BUILTIN(x) __has_builtin(x)
#else
# define FMT_HAS_BUILTIN(x) 0
#endif

#ifdef __has_cpp_attribute
# define FMT_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
# define FMT_HAS_CPP_ATTRIBUTE(x) 0
#endif

#ifndef FMT_USE_VARIADIC_TEMPLATES
// Variadic templates are available in GCC since version 4.4
// (http://gcc.gnu.org/projects/cxx0x.html) and in Visual C++
// since version 2013.
# define FMT_USE_VARIADIC_TEMPLATES \
   (FMT_HAS_FEATURE(cxx_variadic_templates) || \
       (FMT_GCC_VERSION >= 404 && FMT_HAS_GXX_CXX11) || _MSC_VER >= 1800)
#endif

#ifndef FMT_USE_RVALUE_REFERENCES
// Don't use rvalue references when compiling with clang and an old libstdc++
// as the latter doesn't provide std::move.
# if defined(FMT_GNUC_LIBSTD_VERSION) && FMT_GNUC_LIBSTD_VERSION <= 402
#  define FMT_USE_RVALUE_REFERENCES 0
# else
#  define FMT_USE_RVALUE_REFERENCES \
    (FMT_HAS_FEATURE(cxx_rvalue_references) || \
        (FMT_GCC_VERSION >= 403 && FMT_HAS_GXX_CXX11) || _MSC_VER >= 1600)
# endif
#endif


// Define FMT_USE_NOEXCEPT to make C++ Format use noexcept (C++11 feature).
#if FMT_USE_NOEXCEPT || FMT_HAS_FEATURE(cxx_noexcept) || \
  (FMT_GCC_VERSION >= 408 && FMT_HAS_GXX_CXX11)
# define FMT_NOEXCEPT noexcept
#else
# define FMT_NOEXCEPT throw()
#endif

// Check if exceptions are disabled.
#if (__GNUC__ && !__EXCEPTIONS) || (_MSC_VER && !_HAS_EXCEPTIONS)
# define FMT_EXCEPTIONS 0
#else
# define FMT_EXCEPTIONS 1
#endif

#if FMT_EXCEPTIONS
# define FMT_TRY try
# define FMT_CATCH(x) catch (x)
#else
# define FMT_TRY if (true)
# define FMT_CATCH(x) if (false)
#endif

#ifndef FMT_THROW
# if FMT_EXCEPTIONS
#  define FMT_THROW(x) throw x
# else
#  define FMT_THROW(x) std::terminate()
# endif
#endif

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#if FMT_USE_DELETED_FUNCTIONS || FMT_HAS_FEATURE(cxx_deleted_functions) || \
  (FMT_GCC_VERSION >= 404 && FMT_HAS_GXX_CXX11) || _MSC_VER >= 1800
# define FMT_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&) = delete; \
    TypeName& operator=(const TypeName&) = delete
#else
# define FMT_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&); \
    TypeName& operator=(const TypeName&)
#endif

#if defined __GNUC__
# define FMT_DEPRECATED(msg) __attribute__((__deprecated__(msg)))
#elif defined _MSC_VER
# define FMT_DEPRECATED(msg) __declspec(deprecated(msg))
#elif __cplusplus >= 201405L
# define FMT_DEPRECATED(msg) [[deprecated(msg)]]
#else
# define FMT_DEPRECATED(msg)
#endif

#if defined __GNUC__
# define FMT_NORETURN __attribute__((__noreturn__))
#elif defined _MSC_VER
# define FMT_NORETURN __declspec(noreturn)
#elif __cplusplus >= 201103L
# define FMT_NORETURN [[noreturn]]
#else
# define FMT_NORETURN
#endif

#endif // FMT_CONFIG_H
