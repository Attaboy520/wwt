#ifndef _STDDEF_H_
#define _STDDEF_H_

#if !(defined (__GNUG__) && defined (size_t))
typedef __SIZE_TYPE__ size_t;
#ifdef __BEOS__
typedef long ssize_t;
#endif /* __BEOS__ */
#endif /* !(defined (__GNUG__) && defined (size_t)) */

#endif /* _STD_DEF_H_ */