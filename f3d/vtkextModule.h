
#ifndef VTKEXT_EXPORT_H
#define VTKEXT_EXPORT_H

#ifdef VTKEXT_STATIC_DEFINE
#  define VTKEXT_EXPORT
#  define VTKEXT_NO_EXPORT
#else
#  ifndef VTKEXT_EXPORT
#    ifdef vtkext_EXPORTS
        /* We are building this library */
#      define VTKEXT_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKEXT_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKEXT_NO_EXPORT
#    define VTKEXT_NO_EXPORT 
#  endif
#endif

#ifndef VTKEXT_DEPRECATED
#  define VTKEXT_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKEXT_DEPRECATED_EXPORT
#  define VTKEXT_DEPRECATED_EXPORT VTKEXT_EXPORT VTKEXT_DEPRECATED
#endif

#ifndef VTKEXT_DEPRECATED_NO_EXPORT
#  define VTKEXT_DEPRECATED_NO_EXPORT VTKEXT_NO_EXPORT VTKEXT_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKEXT_NO_DEPRECATED
#    define VTKEXT_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkextModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKEXT_EXPORT_H */
