#ifndef KANSO_OPERATORS_H
#define KANSO_OPERATORS_H 1

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

/* If `iso646.h` is not included (it shouldn't be) */
#if !defined(__KANSO_XOR_DEFINED)
#define __KANSO_XOR_DEFINED 1
#define XOR(x, y) (x ^ y)
#endif

  enum kso_bool
  {
#if defined(__STDBOOL_H)
    KSO_FALSE = false,
    KSO_TRUE = true,
#else
  KSO_FALSE = 0,
  KSO_TRUE = 1,
};
#define kanso_bool enum kso_bool
#endif

#define null_ptr(ptr) (!ptr || ptr == NULL)

#if defined(__cplusplus)
  }
#endif // __cplusplus

#endif /* !KANSO_OPERATORS_H */

  /// vim:ts=4:sts=4:sw=4:et:ai:si:sta:noci:nopi:
