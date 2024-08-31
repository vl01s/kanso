#if !defined(KANSO_TYPES_H)
#define KANSO_TYPES_H 1

#if defined(__cplusplus)
extern "C"
{
#endif /* __cplusplus */

#define KANSO_CAST(type, value) (type)(value)

/* NOTE: Analog to `size_t` */
#define ko_size_t unsigned long

#if !defined(__KANSO_SIZE_CAST) || !defined(__KANSO_SIZEOF)
#define __KANSO_SIZE_CAST(t) KANSO_CAST(ko_size_t, t)
#define __KANSO_SIZEOF(t) __KANSO_SIZE_CAST(sizeof(t))
#endif /* !__KANSO_SIZE_CAST || __KANSO_SIZEOF */

/* Signed types */
#if !defined(__KANSO_SIGNED_DEFINED) || !defined(__KANSO_SIGNED_LEN_DEFINED)
#define __KANSO_SIGNED_DEFINED 1
#define kso_int8 signed char
#define kso_int16 signed short int
#define kso_int32 signed int
#define kso_int64 signed long

#define __KANSO_SIGNED_LEN_DEFINED 1
#define KSO_I8LEN __KANSO_SIZEOF(kso_int8)
#define KSO_I16LEN __KANSO_SIZEOF(kso_int16)
#define KSO_I32LEN __KANSO_SIZEOF(kso_int32)
#define KSO_I64LEN __KANSO_SIZEOF(kso_int64)

#define KSO_CHARLEN KSO_I8LEN

#endif /* !__KANSO_SIGNED_DEFINED || !__KANSO_SIGNED_LEN_DEFINED */

/* Unsigned types */
#if !defined(__KANSO_UNSIGNED_DEFINED) || !defined(__KANSO_UNSIGNED_LEN_DEFINED)
#define __KANSO_UNSIGNED_DEFINED 1
#define kso_uint8 unsigned char
#define kso_uint16 unsigned short int
#define kso_uint32 unsigned int
#define kso_uint64 unsigned long long

#define __KANSO_SIGNED_LEN_DEFINED 1
#define KSO_UI8LEN __KANSO_SIZEOF(kso_uint8)
#define KSO_UI16LEN __KANSO_SIZEOF(kso_uint16)
#define KSO_UI32LEN __KANSO_SIZEOF(kso_uint32)
#define KSO_UI64LEN __KANSO_SIZEOF(kso_uint64)

#define KSO_UCHARLEN KSO_UI8LEN

#endif /* !__KANSO_UNSIGNED_DEFINED || !__KANSO_UNSIGNED_LEN_DEFINED */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* !KANSO_TYPES_H */
