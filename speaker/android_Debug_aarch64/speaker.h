#ifndef _SPEAKER_H
#define _SPEAKER_H
#include <AEEStdDef.h>
#include "AEEStdDef.h"
#ifndef __QAIC_HEADER
#define __QAIC_HEADER(ff) ff
#endif //__QAIC_HEADER

#ifndef __QAIC_HEADER_EXPORT
#define __QAIC_HEADER_EXPORT
#endif // __QAIC_HEADER_EXPORT

#ifndef __QAIC_HEADER_ATTRIBUTE
#define __QAIC_HEADER_ATTRIBUTE
#endif // __QAIC_HEADER_ATTRIBUTE

#ifndef __QAIC_IMPL
#define __QAIC_IMPL(ff) ff
#endif //__QAIC_IMPL

#ifndef __QAIC_IMPL_EXPORT
#define __QAIC_IMPL_EXPORT
#endif // __QAIC_IMPL_EXPORT

#ifndef __QAIC_IMPL_ATTRIBUTE
#define __QAIC_IMPL_ATTRIBUTE
#endif // __QAIC_IMPL_ATTRIBUTE
#ifdef __cplusplus
extern "C" {
#endif
#if !defined(__QAIC_STRING1_OBJECT_DEFINED__) && !defined(__STRING1_OBJECT__)
#define __QAIC_STRING1_OBJECT_DEFINED__
#define __STRING1_OBJECT__
typedef struct _cstring1_s {
   char* data;
   int dataLen;
} _cstring1_t;

#endif /* __QAIC_STRING1_OBJECT_DEFINED__ */
__QAIC_HEADER_EXPORT int __QAIC_HEADER(speaker_sum)(const int* vec, int vecLen, int64* res) __QAIC_HEADER_ATTRIBUTE;
__QAIC_HEADER_EXPORT int __QAIC_HEADER(speaker_static_sum)(const int* vec, int vecLen, int64* res1, int64* res2) __QAIC_HEADER_ATTRIBUTE;
__QAIC_HEADER_EXPORT int __QAIC_HEADER(speaker_iostream_sum)(const char* filename, int64* res) __QAIC_HEADER_ATTRIBUTE;
#ifdef __cplusplus
}
#endif
#endif //_SPEAKER_H
