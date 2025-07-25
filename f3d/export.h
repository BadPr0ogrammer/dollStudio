
#ifndef F3D_EXPORT_H
#define F3D_EXPORT_H

#ifdef F3D_STATIC_DEFINE
#  define F3D_EXPORT
#  define F3D_NO_EXPORT
#else
#  ifndef F3D_EXPORT
#    ifdef libf3d_EXPORTS
        /* We are building this library */
#      define F3D_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define F3D_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef F3D_NO_EXPORT
#    define F3D_NO_EXPORT 
#  endif
#endif

#ifndef F3D_DEPRECATED
#  define F3D_DEPRECATED __declspec(deprecated)
#endif

#ifndef F3D_DEPRECATED_EXPORT
#  define F3D_DEPRECATED_EXPORT F3D_EXPORT F3D_DEPRECATED
#endif

#ifndef F3D_DEPRECATED_NO_EXPORT
#  define F3D_DEPRECATED_NO_EXPORT F3D_NO_EXPORT F3D_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef F3D_NO_DEPRECATED
#    define F3D_NO_DEPRECATED
#  endif
#endif

#endif /* F3D_EXPORT_H */
