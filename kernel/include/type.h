#ifndef _KERNEL_TYPE_H_
#define _KERNEL_TYPE_H_

#ifndef IN
#define IN
#endif /* IN */

#ifndef OUT
#define OUT
#endif /* OUT */

#if !defined(GNU_EFI_USE_EXTERNAL_STDARG) && !defined(_MSC_VER)
typedef __builtin_va_list va_list;

# define va_start(v,l)	__builtin_va_start(v,l)
# define va_end(v)	__builtin_va_end(v)
# define va_arg(v,l)	__builtin_va_arg(v,l)
# define va_copy(d,s)	__builtin_va_copy(d,s)
#else
# include <stdarg.h>
#endif

#endif /* _KERNEL_TYPE_H_ */