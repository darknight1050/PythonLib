#pragma once
#include <string>
#include <beatsaber-hook/shared/utils/typedefs-wrappers.hpp>

#define DECLARE_DLSYM(retval, name, ...) \
extern retval(*name)(__VA_ARGS__);

#define DECLARE_DLSYM_TYPE(name) \
extern PyTypeObject* name;

/* Disallow creating instances of the type: set tp_new to NULL and don't create
 * the "__new__" key in the type dictionary. */
#define Py_TPFLAGS_DISALLOW_INSTANTIATION (1UL << 7)

/* Set if the type object is immutable: type attributes cannot be set nor deleted */
#define Py_TPFLAGS_IMMUTABLETYPE (1UL << 8)

/* Set if the type object is dynamically allocated */
#define Py_TPFLAGS_HEAPTYPE (1UL << 9)

/* Set if the type allows subclassing */
#define Py_TPFLAGS_BASETYPE (1UL << 10)

/* Set if the type implements the vectorcall protocol (PEP 590) */
#ifndef Py_LIMITED_API
#define Py_TPFLAGS_HAVE_VECTORCALL (1UL << 11)
// Backwards compatibility alias for API that was provisional in Python 3.8
#define _Py_TPFLAGS_HAVE_VECTORCALL Py_TPFLAGS_HAVE_VECTORCALL
#endif

/* Set if the type is 'ready' -- fully initialized */
#define Py_TPFLAGS_READY (1UL << 12)

/* Set while the type is being 'readied', to prevent recursive ready calls */
#define Py_TPFLAGS_READYING (1UL << 13)

/* Objects support garbage collection (see objimpl.h) */
#define Py_TPFLAGS_HAVE_GC (1UL << 14)

/* These two bits are preserved for Stackless Python, next after this is 17 */
#ifdef STACKLESS
#define Py_TPFLAGS_HAVE_STACKLESS_EXTENSION (3UL << 15)
#else
#define Py_TPFLAGS_HAVE_STACKLESS_EXTENSION 0
#endif

/* Objects behave like an unbound method */
#define Py_TPFLAGS_METHOD_DESCRIPTOR (1UL << 17)

/* Object has up-to-date type attribute cache */
#define Py_TPFLAGS_VALID_VERSION_TAG  (1UL << 19)

/* Type is abstract and cannot be instantiated */
#define Py_TPFLAGS_IS_ABSTRACT (1UL << 20)

// This undocumented flag gives certain built-ins their unique pattern-matching
// behavior, which allows a single positional subpattern to match against the
// subject itself (rather than a mapped attribute on it):
#define _Py_TPFLAGS_MATCH_SELF (1UL << 22)

/* These flags are used to determine if a type is a subclass. */
#define Py_TPFLAGS_LONG_SUBCLASS        (1UL << 24)
#define Py_TPFLAGS_LIST_SUBCLASS        (1UL << 25)
#define Py_TPFLAGS_TUPLE_SUBCLASS       (1UL << 26)
#define Py_TPFLAGS_BYTES_SUBCLASS       (1UL << 27)
#define Py_TPFLAGS_UNICODE_SUBCLASS     (1UL << 28)
#define Py_TPFLAGS_DICT_SUBCLASS        (1UL << 29)
#define Py_TPFLAGS_BASE_EXC_SUBCLASS    (1UL << 30)
#define Py_TPFLAGS_TYPE_SUBCLASS        (1UL << 31)

#define Py_TPFLAGS_DEFAULT  ( \
                 Py_TPFLAGS_HAVE_STACKLESS_EXTENSION | \
                0)

#define METH_VARARGS  0x0001
#define METH_KEYWORDS 0x0002
#define METH_NOARGS   0x0004
#define METH_O        0x0008

#define METH_CLASS    0x0010
#define METH_STATIC   0x0020

#define METH_COEXIST   0x0040

#define PyMODINIT_FUNC extern "C" PyObject*

#define PyObject_HEAD          PyObject ob_base;

#define _PyObject_HEAD_EXTRA
#define _PyObject_EXTRA_INIT

#define PyObject_HEAD_INIT(type)        \
    { _PyObject_EXTRA_INIT              \
    1, type },

#define PyModuleDef_HEAD_INIT { \
    PyObject_HEAD_INIT(NULL)    \
    NULL, /* m_init */          \
    0,    /* m_index */         \
    NULL, /* m_copy */          \
  }
  
#define PyObject_VAR_HEAD      PyVarObject ob_base;

#define _PyObject_CAST(op) ((PyObject*)(op))
#define _PyObject_CAST_CONST(op) ((const PyObject*)(op))

#define _PyVarObject_CAST(op) ((PyVarObject*)(op))
#define _PyVarObject_CAST_CONST(op) ((const PyVarObject*)(op))

#define Py_TYPE(ob) (_PyObject_CAST(ob)->ob_type)

#define Py_SIZE(ob) (_PyVarObject_CAST(ob)->ob_size)

#define Py_IS_TYPE(ob, type) _Py_IS_TYPE(_PyObject_CAST_CONST(ob), type)

#define PyObject_TypeCheck(ob, type) _PyObject_TypeCheck(_PyObject_CAST(ob), type)

#define PyType_FastSubclass(type, flag) PyType_HasFeature(type, flag)

#define Py_NewRef(obj) _Py_NewRef(_PyObject_CAST(obj))
#define Py_RETURN_NONE return Py_NewRef(Py_None)

#define Py_INCREF(op) Py_IncRef(_PyObject_CAST(op))
#define Py_DECREF(op) Py_DecRef(_PyObject_CAST(op))

#define PyLong_Check(op) \
        PyType_FastSubclass(Py_TYPE(op), Py_TPFLAGS_LONG_SUBCLASS)
#define PyLong_CheckExact(op) Py_IS_TYPE(op, PyLong_Type)

#define PyFloat_Check(op) PyObject_TypeCheck(op, PyFloat_Type)
#define PyFloat_CheckExact(op) Py_IS_TYPE(op, PyFloat_Type)

#define PyBool_Check(x) Py_IS_TYPE(x, PyBool_Type)

#define PyBytes_Check(op) \
                 PyType_FastSubclass(Py_TYPE(op), Py_TPFLAGS_BYTES_SUBCLASS)
#define PyBytes_CheckExact(op) Py_IS_TYPE(op, PyBytes_Type)

#define PyUnicode_Check(op) \
                 PyType_FastSubclass(Py_TYPE(op), Py_TPFLAGS_UNICODE_SUBCLASS)
#define PyUnicode_CheckExact(op) Py_IS_TYPE(op, PyUnicode_Type)

namespace Python {

    extern UnorderedEventCallback<int, char*> PythonWriteEvent;

    typedef ssize_t Py_ssize_t;
    typedef Py_ssize_t Py_hash_t;
    typedef struct _typeobject PyTypeObject;
    typedef struct _object {
        _PyObject_HEAD_EXTRA
        Py_ssize_t ob_refcnt;
        PyTypeObject *ob_type;
    } PyObject;

    typedef PyObject *(*vectorcallfunc)(PyObject *callable, PyObject *const *args,
                                    size_t nargsf, PyObject *kwnames);
    typedef PyObject * (*unaryfunc)(PyObject *);
    typedef PyObject * (*binaryfunc)(PyObject *, PyObject *);
    typedef PyObject * (*ternaryfunc)(PyObject *, PyObject *, PyObject *);
    typedef int (*inquiry)(PyObject *);
    typedef Py_ssize_t (*lenfunc)(PyObject *);
    typedef PyObject *(*ssizeargfunc)(PyObject *, Py_ssize_t);
    typedef PyObject *(*ssizessizeargfunc)(PyObject *, Py_ssize_t, Py_ssize_t);
    typedef int(*ssizeobjargproc)(PyObject *, Py_ssize_t, PyObject *);
    typedef int(*ssizessizeobjargproc)(PyObject *, Py_ssize_t, Py_ssize_t, PyObject *);
    typedef int(*objobjargproc)(PyObject *, PyObject *, PyObject *);
    typedef int (*objobjproc)(PyObject *, PyObject *);
    typedef int (*visitproc)(PyObject *, void *);
    typedef int (*traverseproc)(PyObject *, visitproc, void *);
    typedef void (*freefunc)(void *);
    typedef void (*destructor)(PyObject *);
    typedef PyObject *(*getattrfunc)(PyObject *, char *);
    typedef PyObject *(*getattrofunc)(PyObject *, PyObject *);
    typedef int (*setattrfunc)(PyObject *, char *, PyObject *);
    typedef int (*setattrofunc)(PyObject *, PyObject *, PyObject *);
    typedef PyObject *(*reprfunc)(PyObject *);
    typedef Py_hash_t (*hashfunc)(PyObject *);
    typedef PyObject *(*richcmpfunc) (PyObject *, PyObject *, int);
    typedef PyObject *(*getiterfunc) (PyObject *);
    typedef PyObject *(*iternextfunc) (PyObject *);
    typedef PyObject *(*descrgetfunc) (PyObject *, PyObject *, PyObject *);
    typedef int (*descrsetfunc) (PyObject *, PyObject *, PyObject *);
    typedef int (*initproc)(PyObject *, PyObject *, PyObject *);
    typedef PyObject *(*newfunc)(PyTypeObject *, PyObject *, PyObject *);
    typedef PyObject *(*allocfunc)(PyTypeObject *, Py_ssize_t);

    typedef enum {
        PYGEN_RETURN = 0,
        PYGEN_ERROR = -1,
        PYGEN_NEXT = 1,
    } PySendResult;

    typedef struct {
        lenfunc mp_length;
        binaryfunc mp_subscript;
        objobjargproc mp_ass_subscript;
    } PyMappingMethods;

    typedef PySendResult (*sendfunc)(PyObject *iter, PyObject *value, PyObject **result);
    typedef struct {
        unaryfunc am_await;
        unaryfunc am_aiter;
        unaryfunc am_anext;
        sendfunc am_send;
    } PyAsyncMethods;
    typedef struct {
        /* Number implementations must check *both*
        arguments for proper type and implement the necessary conversions
        in the slot functions themselves. */

        binaryfunc nb_add;
        binaryfunc nb_subtract;
        binaryfunc nb_multiply;
        binaryfunc nb_remainder;
        binaryfunc nb_divmod;
        ternaryfunc nb_power;
        unaryfunc nb_negative;
        unaryfunc nb_positive;
        unaryfunc nb_absolute;
        inquiry nb_bool;
        unaryfunc nb_invert;
        binaryfunc nb_lshift;
        binaryfunc nb_rshift;
        binaryfunc nb_and;
        binaryfunc nb_xor;
        binaryfunc nb_or;
        unaryfunc nb_int;
        void *nb_reserved;  /* the slot formerly known as nb_long */
        unaryfunc nb_float;

        binaryfunc nb_inplace_add;
        binaryfunc nb_inplace_subtract;
        binaryfunc nb_inplace_multiply;
        binaryfunc nb_inplace_remainder;
        ternaryfunc nb_inplace_power;
        binaryfunc nb_inplace_lshift;
        binaryfunc nb_inplace_rshift;
        binaryfunc nb_inplace_and;
        binaryfunc nb_inplace_xor;
        binaryfunc nb_inplace_or;

        binaryfunc nb_floor_divide;
        binaryfunc nb_true_divide;
        binaryfunc nb_inplace_floor_divide;
        binaryfunc nb_inplace_true_divide;

        unaryfunc nb_index;

        binaryfunc nb_matrix_multiply;
        binaryfunc nb_inplace_matrix_multiply;
    } PyNumberMethods;
    typedef struct {
        lenfunc sq_length;
        binaryfunc sq_concat;
        ssizeargfunc sq_repeat;
        ssizeargfunc sq_item;
        void *was_sq_slice;
        ssizeobjargproc sq_ass_item;
        void *was_sq_ass_slice;
        objobjproc sq_contains;

        binaryfunc sq_inplace_concat;
        ssizeargfunc sq_inplace_repeat;
    } PySequenceMethods;


    /* buffer interface */
    typedef struct bufferinfo {
        void *buf;
        PyObject *obj;        /* owned reference */
        Py_ssize_t len;
        Py_ssize_t itemsize;  /* This is Py_ssize_t so it can be
                                pointed to by strides in simple case.*/
        int readonly;
        int ndim;
        char *format;
        Py_ssize_t *shape;
        Py_ssize_t *strides;
        Py_ssize_t *suboffsets;
        void *internal;
    } Py_buffer;

    typedef int (*getbufferproc)(PyObject *, Py_buffer *, int);
    typedef void (*releasebufferproc)(PyObject *, Py_buffer *);
    typedef struct {
        getbufferproc bf_getbuffer;
        releasebufferproc bf_releasebuffer;
    } PyBufferProcs;

    typedef struct {
        PyObject ob_base;
        Py_ssize_t ob_size; /* Number of items in variable part */
    } PyVarObject;

    typedef struct _typeobject {
        PyObject_VAR_HEAD
        const char *tp_name; /* For printing, in format "<module>.<name>" */
        Py_ssize_t tp_basicsize, tp_itemsize; /* For allocation */

        /* Methods to implement standard operations */

        destructor tp_dealloc;
        Py_ssize_t tp_vectorcall_offset;
        getattrfunc tp_getattr;
        setattrfunc tp_setattr;
        PyAsyncMethods *tp_as_async; /* formerly known as tp_compare (Python 2)
                                        or tp_reserved (Python 3) */
        reprfunc tp_repr;

        /* Method suites for standard classes */

        PyNumberMethods *tp_as_number;
        PySequenceMethods *tp_as_sequence;
        PyMappingMethods *tp_as_mapping;

        /* More standard operations (here for binary compatibility) */

        hashfunc tp_hash;
        ternaryfunc tp_call;
        reprfunc tp_str;
        getattrofunc tp_getattro;
        setattrofunc tp_setattro;

        /* Functions to access object as input/output buffer */
        PyBufferProcs *tp_as_buffer;

        /* Flags to define presence of optional/expanded features */
        unsigned long tp_flags;

        const char *tp_doc; /* Documentation string */

        /* Assigned meaning in release 2.0 */
        /* call function for all accessible objects */
        traverseproc tp_traverse;

        /* delete references to contained objects */
        inquiry tp_clear;

        /* Assigned meaning in release 2.1 */
        /* rich comparisons */
        richcmpfunc tp_richcompare;

        /* weak reference enabler */
        Py_ssize_t tp_weaklistoffset;

        /* Iterators */
        getiterfunc tp_iter;
        iternextfunc tp_iternext;

        /* Attribute descriptor and subclassing stuff */
        struct PyMethodDef *tp_methods;
        struct PyMemberDef *tp_members;
        struct PyGetSetDef *tp_getset;
        // Strong reference on a heap type, borrowed reference on a static type
        struct _typeobject *tp_base;
        PyObject *tp_dict;
        descrgetfunc tp_descr_get;
        descrsetfunc tp_descr_set;
        Py_ssize_t tp_dictoffset;
        initproc tp_init;
        allocfunc tp_alloc;
        newfunc tp_new;
        freefunc tp_free; /* Low-level free-memory routine */
        inquiry tp_is_gc; /* For PyObject_IS_GC */
        PyObject *tp_bases;
        PyObject *tp_mro; /* method resolution order */
        PyObject *tp_cache;
        PyObject *tp_subclasses;
        PyObject *tp_weaklist;
        destructor tp_del;

        /* Type attribute cache version tag. Added in version 2.6 */
        unsigned int tp_version_tag;

        destructor tp_finalize;
        vectorcallfunc tp_vectorcall;
    } PyTypeObject;

    struct PyFrameObject { };
    

    typedef void (*PyCapsule_Destructor)(PyObject *);
    typedef void *PyThread_type_lock;
    typedef struct {
        double real;
        double imag;
    } Py_complex;
    typedef
        enum { PyGILState_LOCKED, PyGILState_UNLOCKED }
            PyGILState_STATE;
    typedef enum {} PyLockStatus;
    typedef Py_ssize_t Py_hash_t;
    typedef PyObject *(*PyCFunction)(PyObject *, PyObject *);
    typedef void (*PyOS_sighandler_t)(int);
    typedef void *PyThread_type_lock;
    typedef struct {
        enum {
            _PyStatus_TYPE_OK=0,
            _PyStatus_TYPE_ERROR=1,
            _PyStatus_TYPE_EXIT=2
        } _type;
        const char *func;
        const char *err_msg;
        int exitcode;
    } PyStatus;
    typedef enum {
        /* PyMem_RawMalloc(), PyMem_RawRealloc() and PyMem_RawFree() */
        PYMEM_DOMAIN_RAW,

        /* PyMem_Malloc(), PyMem_Realloc() and PyMem_Free() */
        PYMEM_DOMAIN_MEM,

        /* PyObject_Malloc(), PyObject_Realloc() and PyObject_Free() */
        PYMEM_DOMAIN_OBJ
    } PyMemAllocatorDomain;

    typedef int (*Py_tracefunc)(PyObject *, PyFrameObject *, int, PyObject *);
    typedef int(*Py_AuditHookFunction)(const char *, PyObject *, void *);
    typedef PyObject * (*Py_OpenCodeHookFunction)(PyObject *, void *);
    #define PY_TIMEOUT_T long long

    struct PyMethodDef {
        const char  *ml_name;   /* The name of the built-in function/method */
        PyCFunction ml_meth;    /* The C function that implements it */
        int         ml_flags;   /* Combination of METH_xxx flags, which mostly
                                describe the args expected by the C func */
        const char  *ml_doc;    /* The __doc__ attribute, or NULL */
    };
    typedef struct PyMethodDef PyMethodDef;
    typedef struct PyModuleDef_Base {
        PyObject_HEAD
        PyObject* (*m_init)(void);
        Py_ssize_t m_index;
        PyObject* m_copy;
    } PyModuleDef_Base;
    typedef struct PyModuleDef{
        PyModuleDef_Base m_base;
        const char* m_name;
        const char* m_doc;
        Py_ssize_t m_size;
        PyMethodDef *m_methods;
        struct PyModuleDef_Slot* m_slots;
        traverseproc m_traverse;
        inquiry m_clear;
        freefunc m_free;
    } PyModuleDef;

    typedef wchar_t Py_UNICODE;
    
    extern PyObject* Py_None;

    struct PyPreConfig { };
    struct PyConfig { };
    struct PyWideStringList { };
    struct PyCompilerFlags { };
    struct PyMemAllocatorEx { };
    struct PyInterpreterState { };
    struct PyThreadState { };
    struct PyHash_FuncDef { };
    struct PyStructSequence_Desc { };
    struct PyCodeObject { };
    struct Py_tss_t { };
    struct PyObjectArenaAllocator { };
    struct Py_UCS4 { };
    struct PyMemberDef { };
    struct PyTryBlock { };
    struct PyType_Spec { };
    struct PyGetSetDef { };
    DECLARE_DLSYM(PyObject *, PyMarshal_WriteObjectToString, PyObject *, int);
    DECLARE_DLSYM(void, PyThread_release_lock, PyThread_type_lock);
    DECLARE_DLSYM(void *, PyObject_Realloc, void *ptr, size_t new_size);
    DECLARE_DLSYM(PyObject *, PyNumber_Xor, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(PyObject *, PyBytes_DecodeEscape, const char *, Py_ssize_t,const char *, Py_ssize_t,const char *);
    DECLARE_DLSYM(PyObject *, PyImport_AddModule,const char *name            /* UTF-8 encoded string */);
    DECLARE_DLSYM(PyObject *, PyUnicodeTranslateError_GetObject, PyObject *);
    DECLARE_DLSYM(PyObject *, PyUnicodeDecodeError_GetEncoding, PyObject *);
    DECLARE_DLSYM(PyObject *, PyNumber_Negative, PyObject *o);
    DECLARE_DLSYM(Py_ssize_t, PyList_Size, PyObject *);
    DECLARE_DLSYM(int, PyModule_AddFunctions, PyObject *, PyMethodDef *);
    DECLARE_DLSYM(PyObject *, PyNumber_FloorDivide, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(void, PyErr_Restore, PyObject *, PyObject *, PyObject *);
    DECLARE_DLSYM(void, PyObject_CallFinalizer, PyObject *);
    DECLARE_DLSYM(PyObject *, PySequence_Tuple, PyObject *o);
    DECLARE_DLSYM(int, PyCapsule_SetContext, PyObject *capsule, void *context);
    DECLARE_DLSYM(int, PySlice_Unpack, PyObject *slice,Py_ssize_t *start, Py_ssize_t *stop, Py_ssize_t *step);
    DECLARE_DLSYM(int, PyErr_BadArgument, void);
    DECLARE_DLSYM(PyObject *, PyObject_Format, PyObject *obj,PyObject *format_spec);
    DECLARE_DLSYM(int, PyUnicodeDecodeError_GetEnd, PyObject *, Py_ssize_t *);
    DECLARE_DLSYM_TYPE(PyLongRangeIter_Type);
    DECLARE_DLSYM(int, PyNumber_Check, PyObject *o);
    DECLARE_DLSYM(long, PyOS_strtol, const char *, char **, int);
    DECLARE_DLSYM(PyObject *, PyFile_OpenCodeObject, PyObject *path);
    DECLARE_DLSYM(int, PyUnicode_FSConverter, PyObject*, void*);
    DECLARE_DLSYM(void, PyMarshal_WriteObjectToFile, PyObject *, FILE *, int);
    DECLARE_DLSYM(void, PyPreConfig_InitIsolatedConfig, PyPreConfig *config);
    DECLARE_DLSYM(PyCapsule_Destructor, PyCapsule_GetDestructor, PyObject *capsule);
    DECLARE_DLSYM(PyObject *, PyImport_Import, PyObject *name);
    DECLARE_DLSYM(PyStatus, PyStatus_Ok, void);
    DECLARE_DLSYM(int, PyOS_mystrnicmp, const char *, const char *, Py_ssize_t);
    DECLARE_DLSYM(const Py_buffer *, PyPickleBuffer_GetBuffer, PyObject *);
    DECLARE_DLSYM(PyObject*, PyThread_GetInfo, void);
    DECLARE_DLSYM(int, PyFile_WriteString, const char *, PyObject *);
    DECLARE_DLSYM(size_t, PyThread_get_stacksize, void);
    DECLARE_DLSYM(int, PyToken_TwoChars, int, int);
    DECLARE_DLSYM(void, Py_SetPath, const wchar_t *);
    DECLARE_DLSYM_TYPE(PyMethodDescr_Type);
    DECLARE_DLSYM(Py_ssize_t, PySequence_Length, PyObject *o);
    DECLARE_DLSYM(PyObject *, PyType_GenericNew, PyTypeObject *,PyObject *, PyObject *);
    DECLARE_DLSYM(PyObject *, PyTuple_GetItem, PyObject *, Py_ssize_t);
    DECLARE_DLSYM(PyObject*, PyUnicode_Split,PyObject *s,                /* String to split */PyObject *sep,              /* String separator */Py_ssize_t maxsplit         /* Maxsplit count */);
    DECLARE_DLSYM(PyObject *, PyLong_FromSize_t, size_t);
    DECLARE_DLSYM(int, PyBuffer_IsContiguous, const Py_buffer *view, char fort);
    DECLARE_DLSYM(PyStatus, Py_InitializeFromConfig,const PyConfig *config);
    DECLARE_DLSYM(PyStatus, PyWideStringList_Append, PyWideStringList *list,const wchar_t *item);
    DECLARE_DLSYM(char *, PyOS_Readline, FILE *, FILE *, const char *);
    DECLARE_DLSYM(PyObject *, PyDict_GetItem, PyObject *mp, PyObject *key);
    DECLARE_DLSYM(PyObject*, PyUnicode_EncodeLocale,PyObject *unicode,const char *errors);
    DECLARE_DLSYM(PyObject *, PyErr_SetFromErrnoWithFilenameObjects,PyObject *, PyObject *, PyObject *);
    DECLARE_DLSYM(PyObject *, PyUnicode_InternFromString,const char *u              /* UTF-8 encoded string */);
    DECLARE_DLSYM(void, PyErr_PrintEx, int);
    DECLARE_DLSYM(int, PyErr_ExceptionMatches, PyObject *);
    DECLARE_DLSYM(PyObject*, PyUnicode_FromWideChar,const wchar_t *w,           /* wchar_t buffer */Py_ssize_t size             /* size of buffer */);
    DECLARE_DLSYM(PyObject *, PyCodec_LookupError, const char *name);
    DECLARE_DLSYM(PyObject *, PyComplex_FromCComplex, Py_complex);
    DECLARE_DLSYM(Py_ssize_t, PyUnicode_Tailmatch,PyObject *str,              /* String */PyObject *substr,           /* Prefix or Suffix string */Py_ssize_t start,           /* Start index */Py_ssize_t end,             /* Stop index */int direction               /* Tail end: -1 prefix, +1 suffix */);
    DECLARE_DLSYM(char *, PyBytes_AsString, PyObject *);
    DECLARE_DLSYM(void *, PyCapsule_Import,const char *name,           /* UTF-8 encoded string */int no_block);
    DECLARE_DLSYM_TYPE(PyMemoryView_Type);
    DECLARE_DLSYM(PyObject *, PyMarshal_ReadObjectFromString, const char *,Py_ssize_t);
    DECLARE_DLSYM(PyObject *, PyLong_FromUnicodeObject, PyObject *u, int base);
    DECLARE_DLSYM(PyStatus, PyConfig_SetArgv, PyConfig *config,Py_ssize_t argc,wchar_t * const *argv);
    DECLARE_DLSYM(PyStatus, Py_PreInitializeFromArgs,const PyPreConfig *src_config,Py_ssize_t argc,wchar_t **argv);
    DECLARE_DLSYM(int, PyToken_ThreeChars, int, int, int);
    DECLARE_DLSYM(int, PyUnicodeTranslateError_GetStart, PyObject *, Py_ssize_t *);
    DECLARE_DLSYM(int, PyUnicode_Contains,PyObject *container,        /* Container string */PyObject *element           /* Element string */);
    DECLARE_DLSYM(PyObject *, PyRun_StringFlags, const char *, int, PyObject *,PyObject *, PyCompilerFlags *);
    DECLARE_DLSYM(void, PySys_FormatStdout, const char *format, ...);
    DECLARE_DLSYM(int, PyImport_ImportFrozenModuleObject,PyObject *name);
    DECLARE_DLSYM(int, PyRun_AnyFileFlags, FILE *, const char *, PyCompilerFlags *);
    DECLARE_DLSYM(PyObject *, PyDictProxy_New, PyObject *);
    DECLARE_DLSYM_TYPE(PyDictIterKey_Type);
    DECLARE_DLSYM_TYPE(PyODictKeys_Type);
    DECLARE_DLSYM(PyObject *, PyFloat_FromString, PyObject*);
    DECLARE_DLSYM(int, PyList_Insert, PyObject *, Py_ssize_t, PyObject *);
    DECLARE_DLSYM(int, PyPickleBuffer_Release, PyObject *);
    DECLARE_DLSYM(const char *, PyEval_GetFuncDesc, PyObject *);
    DECLARE_DLSYM(int, PyTraceBack_Print, PyObject *, PyObject *);
    DECLARE_DLSYM(void, PySys_AddXOption, const wchar_t *);
    DECLARE_DLSYM(PyObject *, PyBytes_Repr, PyObject *, int);
    DECLARE_DLSYM(PyObject *, Py_BuildValue, const char *, ...);
    DECLARE_DLSYM(PyObject *, PyNumber_Lshift, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(void, PyConfig_InitPythonConfig, PyConfig *config);
    DECLARE_DLSYM(PyObject*, PyUnicode_DecodeUTF8Stateful,const char *string,         /* UTF-8 encoded string */Py_ssize_t length,          /* size of string */const char *errors,         /* error handling */Py_ssize_t *consumed        /* bytes consumed */);
    DECLARE_DLSYM(int, PyBuffer_FromContiguous, Py_buffer *view, void *buf,Py_ssize_t len, char order);
    DECLARE_DLSYM(int, PyUnicodeDecodeError_SetReason,PyObject *exc,const char *reason          /* UTF-8 encoded string */);
    DECLARE_DLSYM(void, PyOS_AfterFork_Parent, void);
    DECLARE_DLSYM(int, PyDict_DelItemString, PyObject *dp, const char *key);
    DECLARE_DLSYM(PyObject *, PySequence_Repeat, PyObject *o, Py_ssize_t count);
    DECLARE_DLSYM(Py_ssize_t, PyTuple_Size, PyObject *);
    DECLARE_DLSYM(void, PyMem_SetAllocator, PyMemAllocatorDomain domain,PyMemAllocatorEx *allocator);
    DECLARE_DLSYM(PyObject*, PyUnicode_EncodeUTF7,PyObject *unicode,          /* Unicode object */int base64SetO,             /* Encode RFC2152 Set O characters in base64 */int base64WhiteSpace,       /* Encode whitespace (sp, ht, nl, cr) in base64 */const char *errors          /* error handling */);
    DECLARE_DLSYM(PyObject *, PyEval_EvalCode, PyObject *, PyObject *, PyObject *);
    DECLARE_DLSYM(PyObject *, PyNumber_InPlaceFloorDivide, PyObject *o1,PyObject *o2);
    DECLARE_DLSYM(int, PySet_Add, PyObject *set, PyObject *key);
    DECLARE_DLSYM(PyThread_type_lock, PyThread_allocate_lock, void);
    DECLARE_DLSYM(void, Py_ReprLeave, PyObject *);
    DECLARE_DLSYM(Py_ssize_t, PyUnicode_Fill,PyObject *unicode,Py_ssize_t start,Py_ssize_t length,Py_UCS4 fill_char);
    DECLARE_DLSYM(int, Py_AtExit, void (*func)(void));
    DECLARE_DLSYM(PyObject *, PyDict_New, void);
    DECLARE_DLSYM(int, PyCodec_RegisterError, const char *name, PyObject *error);
    DECLARE_DLSYM(int, PyIndex_Check, PyObject *);
    DECLARE_DLSYM(PyThreadState *, PyThreadState_New, PyInterpreterState *);
    DECLARE_DLSYM(int, PyUnicode_IsIdentifier, PyObject *s);
    DECLARE_DLSYM(PyObject *, PyUnicode_RichCompare,PyObject *left,             /* Left string */PyObject *right,            /* Right string */int op                      /* Operation: Py_EQ, Py_NE, Py_GT, etc. */);
    DECLARE_DLSYM(PyObject *, PyImport_ImportModuleLevelObject,PyObject *name,PyObject *globals,PyObject *locals,PyObject *fromlist,int level);
    DECLARE_DLSYM(PyObject *, PyObject_GetAttr, PyObject *, PyObject *);
    DECLARE_DLSYM_TYPE(PyUnicode_Type);
    DECLARE_DLSYM(PyStatus, PyStatus_Error, const char *err_msg);
    DECLARE_DLSYM(int, PyList_Sort, PyObject *);
    DECLARE_DLSYM(int, PyUnicodeDecodeError_SetEnd, PyObject *, Py_ssize_t);
    DECLARE_DLSYM(PyObject *, PyModule_FromDefAndSpec2, PyModuleDef *def,PyObject *spec,int module_api_version);
    DECLARE_DLSYM(int, PyRun_InteractiveOneFlags,FILE *fp,const char *filename,       /* decoded from the filesystem encoding */PyCompilerFlags *flags);
    DECLARE_DLSYM(PyObject *, PyNumber_InPlaceAnd, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(PyObject *, PyObject_CallMethod, PyObject *obj,const char *name,const char *format, ...);
    DECLARE_DLSYM(PyObject *, PyObject_Dir, PyObject *);
    DECLARE_DLSYM(void, PyThread_free_lock, PyThread_type_lock);
    DECLARE_DLSYM(int, PyObject_AsReadBuffer, PyObject *obj,const void **buffer,Py_ssize_t *buffer_len);
    DECLARE_DLSYM(const char *, PyCapsule_GetName, PyObject *capsule);
    DECLARE_DLSYM(PyObject *, PyCodec_StreamWriter,const char *encoding,PyObject *stream,const char *errors);
    DECLARE_DLSYM(int, PyArg_VaParse, PyObject *, const char *, va_list);
    DECLARE_DLSYM(PyObject *, PyBytes_FromStringAndSize, const char *, Py_ssize_t);
    DECLARE_DLSYM(PyObject *, PyLong_FromLong, long);
    DECLARE_DLSYM(PyObject *, PyList_New, Py_ssize_t size);
    DECLARE_DLSYM_TYPE(PyContextToken_Type);
    DECLARE_DLSYM(void, Py_Exit, int);
    DECLARE_DLSYM(void, PyUnicode_AppendAndDel,PyObject **pleft,           /* Pointer to left string */PyObject *right             /* Right string */);
    DECLARE_DLSYM(int, PySequence_In, PyObject *o, PyObject *value);
    DECLARE_DLSYM(PyObject *, PyUnicodeEncodeError_GetEncoding, PyObject *);
    DECLARE_DLSYM_TYPE(PyCoro_Type);
    DECLARE_DLSYM(int, PyList_SetItem, PyObject *, Py_ssize_t, PyObject *);
    DECLARE_DLSYM(PyStatus, PyStatus_Exit, int exitcode);
    DECLARE_DLSYM_TYPE(PyCFunction_Type);
    DECLARE_DLSYM(int, PyObject_GenericSetAttr, PyObject *, PyObject *, PyObject *);
    DECLARE_DLSYM(int, PySys_Audit,const char *event,const char *argFormat,...);
    DECLARE_DLSYM(PyHash_FuncDef*, PyHash_GetFuncDef, void);
    DECLARE_DLSYM(Py_ssize_t, PyMapping_Length, PyObject *o);
    DECLARE_DLSYM_TYPE(PyODictIter_Type);
    DECLARE_DLSYM(PyObject *, PyObject_Init, PyObject *, PyTypeObject *);
    DECLARE_DLSYM(PyObject*, PyUnicode_DecodeLatin1,const char *string,         /* Latin-1 encoded string */Py_ssize_t length,          /* size of string */const char *errors          /* error handling */);
    DECLARE_DLSYM(int, PyArg_VaParseTupleAndKeywords, PyObject *, PyObject *,const char *, char **, va_list);
    DECLARE_DLSYM(PyThreadState *, PyInterpreterState_ThreadHead, PyInterpreterState *);
    DECLARE_DLSYM_TYPE(PyFloat_Type);
    DECLARE_DLSYM(PyObject *, PyWrapper_New, PyObject *, PyObject *);
    DECLARE_DLSYM_TYPE(PyClassMethod_Type);
    DECLARE_DLSYM(void, PyThreadState_Clear, PyThreadState *);
    DECLARE_DLSYM(Py_ssize_t, PyUnicode_Find,PyObject *str,              /* String */PyObject *substr,           /* Substring to find */Py_ssize_t start,           /* Start index */Py_ssize_t end,             /* Stop index */int direction               /* Find direction: +1 forward, -1 backward */);
    DECLARE_DLSYM(int, PyStructSequence_InitType2, PyTypeObject *type,PyStructSequence_Desc *desc);
    DECLARE_DLSYM(void, PyErr_GetExcInfo, PyObject **, PyObject **, PyObject **);
    DECLARE_DLSYM(PyObject *, PyNumber_ToBase, PyObject *n, int base);
    DECLARE_DLSYM(PyObject *, PyNumber_Subtract, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(void, PySys_SetArgv, int, wchar_t **);
    DECLARE_DLSYM(PyObject *, PyType_GenericAlloc, PyTypeObject *, Py_ssize_t);
    DECLARE_DLSYM_TYPE(PyFrozenSet_Type);
    DECLARE_DLSYM_TYPE(PyListRevIter_Type);
    DECLARE_DLSYM_TYPE(PySetIter_Type);
    DECLARE_DLSYM(PyObject *, PyImport_ExecCodeModuleEx,const char *name,           /* UTF-8 encoded string */PyObject *co,const char *pathname        /* decoded from the filesystem encoding */);
    DECLARE_DLSYM(int, PyErr_WarnExplicitObject,PyObject *category,PyObject *message,PyObject *filename,int lineno,PyObject *module,PyObject *registry);
    DECLARE_DLSYM_TYPE(PyStringIO_Type);
    DECLARE_DLSYM(int, PySequence_DelSlice, PyObject *o, Py_ssize_t i1, Py_ssize_t i2);
    DECLARE_DLSYM(int, PySequence_Contains, PyObject *seq, PyObject *ob);
    DECLARE_DLSYM(int, PyDict_Update, PyObject *mp, PyObject *other);
    DECLARE_DLSYM(const char *, Py_GetCopyright, void);
    DECLARE_DLSYM_TYPE(PySuper_Type);
    DECLARE_DLSYM_TYPE(PyModuleDef_Type);
    DECLARE_DLSYM(void, PySys_ResetWarnOptions, void);
    DECLARE_DLSYM(const char *, PyUnicode_AsUTF8AndSize,PyObject *unicode,Py_ssize_t *size);
    DECLARE_DLSYM(int, PyObject_RichCompareBool, PyObject *, PyObject *, int);
    DECLARE_DLSYM(PyObject *, PyObject_Bytes, PyObject *);
    DECLARE_DLSYM(PyObject *, PyInstanceMethod_New, PyObject *);
    DECLARE_DLSYM(int, PyContext_Enter, PyObject *);
    DECLARE_DLSYM(PyObject*, PyUnicode_AsRawUnicodeEscapeString,PyObject *unicode           /* Unicode object */);
    DECLARE_DLSYM(PyStatus, PyConfig_SetBytesString,PyConfig *config,wchar_t **config_str,const char *str);
    DECLARE_DLSYM_TYPE(PyModule_Type);
    DECLARE_DLSYM(PyCodeObject *, PyCode_NewWithPosOnlyArgs,int, int, int, int, int, int, PyObject *, PyObject *,PyObject *, PyObject *, PyObject *, PyObject *,PyObject *, PyObject *, int, PyObject *);
    DECLARE_DLSYM(PyObject*, PyUnicode_EncodeCharmap,PyObject *unicode,          /* Unicode object */PyObject *mapping,          /* encoding mapping */const char *errors          /* error handling */);
    DECLARE_DLSYM(PyObject *, PyTuple_New, Py_ssize_t size);
    DECLARE_DLSYM(int, PyFile_WriteObject, PyObject *, PyObject *, int);
    DECLARE_DLSYM(int, PyCapsule_SetDestructor, PyObject *capsule, PyCapsule_Destructor destructor);
    DECLARE_DLSYM(int, PyStatus_Exception, PyStatus err);
    DECLARE_DLSYM(int, PyThread_set_stacksize, size_t);
    DECLARE_DLSYM_TYPE(PyBufferedIOBase_Type);
    DECLARE_DLSYM(double, PyLong_AsDouble, PyObject *);
    DECLARE_DLSYM(PyObject *, PyFunction_GetGlobals, PyObject *);
    DECLARE_DLSYM(PyObject *, PyLong_GetInfo, void);
    DECLARE_DLSYM(int, PyMarshal_ReadShortFromFile, FILE *);
    DECLARE_DLSYM(int, PySlice_GetIndices, PyObject *r, Py_ssize_t length,Py_ssize_t *start, Py_ssize_t *stop, Py_ssize_t *step);
    DECLARE_DLSYM(PyObject *, PyModule_GetNameObject, PyObject *);
    DECLARE_DLSYM(PyObject *, PyRun_FileEx, FILE *fp, const char *p, int s, PyObject *g, PyObject *l, int c);
    DECLARE_DLSYM_TYPE(PyClassMethodDescr_Type);
    DECLARE_DLSYM_TYPE(PyFunction_Type);
    DECLARE_DLSYM(PyObject*, PyUnicode_AsLatin1String,PyObject *unicode           /* Unicode object */);
    DECLARE_DLSYM(void, PyInterpreterState_Delete, PyInterpreterState *);
    DECLARE_DLSYM(void, PyEval_RestoreThread, PyThreadState *);
    DECLARE_DLSYM(PyObject *, PyODict_New, void);
    DECLARE_DLSYM(int, PyUnicode_CompareWithASCIIString,PyObject *left,const char *right           /* ASCII-encoded string */);
    DECLARE_DLSYM(void, Py_SetPythonHome, const wchar_t *);
    DECLARE_DLSYM(const char *, PyEval_GetFuncName, PyObject *);
    DECLARE_DLSYM(Py_complex, PyComplex_AsCComplex, PyObject *op);
    DECLARE_DLSYM(wchar_t *, Py_GetProgramName, void);
    DECLARE_DLSYM(int, PyContextVar_Get,PyObject *var, PyObject *default_value, PyObject **value);
    DECLARE_DLSYM(PyObject *, PyNumber_Multiply, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(void *, PyObject_Calloc, size_t nelem, size_t elsize);
    DECLARE_DLSYM(void *, PyMem_Calloc, size_t nelem, size_t elsize);
    DECLARE_DLSYM(PyObject *, PyCFunction_GetSelf, PyObject *);
    DECLARE_DLSYM(PyObject*, PyUnicode_Splitlines,PyObject *s,                /* String to split */int keepends                /* If true, line end markers are included */);
    DECLARE_DLSYM(int, PySequence_DelItem, PyObject *o, Py_ssize_t i);
    DECLARE_DLSYM_TYPE(PyGen_Type);
    DECLARE_DLSYM(void, PyConfig_Clear, PyConfig *);
    DECLARE_DLSYM(void *, PyCapsule_GetContext, PyObject *capsule);
    DECLARE_DLSYM(PyObject *, PyCodec_StrictErrors, PyObject *exc);
    DECLARE_DLSYM_TYPE(PyLong_Type);
    DECLARE_DLSYM(PyObject *, PySys_GetXOptions, void);
    DECLARE_DLSYM(Py_UCS4, PyUnicode_ReadChar,PyObject *unicode,Py_ssize_t index);
    DECLARE_DLSYM(int, PyErr_WarnFormat,PyObject *category,Py_ssize_t stack_level,const char *format,         /* ASCII-encoded string  */...);
    DECLARE_DLSYM(int, Py_FrozenMain, int argc, char **argv);
    DECLARE_DLSYM(PyObject *, PyDict_Items, PyObject *mp);
    DECLARE_DLSYM(PyObject *, PyUnicodeTranslateError_Create,PyObject *object,Py_ssize_t start,Py_ssize_t end,const char *reason          /* UTF-8 encoded string */);
    DECLARE_DLSYM(int, PyList_SetSlice, PyObject *, Py_ssize_t, Py_ssize_t, PyObject *);
    DECLARE_DLSYM(PyInterpreterState *, PyInterpreterState_New, void);
    DECLARE_DLSYM(int, PyCodec_Register,PyObject *search_function);
    DECLARE_DLSYM(int, PyObject_AsWriteBuffer, PyObject *obj,void **buffer,Py_ssize_t *buffer_len);
    DECLARE_DLSYM(PyObject *, PyWeakref_NewRef, PyObject *ob,PyObject *callback);
    DECLARE_DLSYM(PyObject*, PyUnicode_EncodeFSDefault,PyObject *unicode);
    DECLARE_DLSYM(PyObject *, PyContext_CopyCurrent, void);
    DECLARE_DLSYM(const char *, PyExceptionClass_Name, PyObject *);
    DECLARE_DLSYM(PyObject *, PySys_GetObject, const char *);
    DECLARE_DLSYM(int, PyUnicodeEncodeError_SetReason,PyObject *exc,const char *reason          /* UTF-8 encoded string */);
    DECLARE_DLSYM(int, PyUnicode_Resize,PyObject **unicode,         /* Pointer to the Unicode object */Py_ssize_t length           /* New length */);
    DECLARE_DLSYM(Py_ssize_t, PySequence_Index, PyObject *o, PyObject *value);
    DECLARE_DLSYM(unsigned long, PyType_GetFlags, PyTypeObject*);
    DECLARE_DLSYM(PyObject *, PyDict_Keys, PyObject *mp);
    DECLARE_DLSYM_TYPE(PyCell_Type);
    DECLARE_DLSYM(PyObject*, PyUnicode_DecodeLocaleAndSize,const char *str,Py_ssize_t len,const char *errors);
    DECLARE_DLSYM(int, PyErr_WarnExplicit,PyObject *category,const char *message,        /* UTF-8 encoded string */const char *filename,       /* decoded from the filesystem encoding */int lineno,const char *module,         /* UTF-8 encoded string */PyObject *registry);
    DECLARE_DLSYM(long, PyImport_GetMagicNumber, void);
    DECLARE_DLSYM(int, PyCapsule_IsValid, PyObject *capsule, const char *name);
    DECLARE_DLSYM(PyObject *, PyFile_FromFd, int, const char *, const char *, int,const char *, const char *,const char *, int);
    DECLARE_DLSYM_TYPE(PyInstanceMethod_Type);
    DECLARE_DLSYM_TYPE(PyZip_Type);
    DECLARE_DLSYM(void, Py_Finalize, void);
    DECLARE_DLSYM(double, PyFloat_GetMax, void);
    DECLARE_DLSYM(PyGILState_STATE, PyGILState_Ensure, void);
    DECLARE_DLSYM(PyObject *, PySequence_List, PyObject *o);
    DECLARE_DLSYM(PyObject *, PyCodec_IncrementalDecoder,const char *encoding,const char *errors);
    DECLARE_DLSYM(void, PyErr_Clear, void);
    DECLARE_DLSYM(Py_ssize_t, PyObject_Length, PyObject *o);
    DECLARE_DLSYM(PyObject *, PyCapsule_New,void *pointer,const char *name,PyCapsule_Destructor destructor);
    DECLARE_DLSYM(PyObject *, PyLong_FromSsize_t, Py_ssize_t);
    DECLARE_DLSYM_TYPE(PyByteArray_Type);
    DECLARE_DLSYM(void, PyThreadState_Delete, PyThreadState *);
    DECLARE_DLSYM(PyObject *, PyObject_GenericGetDict, PyObject *, void *);
    DECLARE_DLSYM(PyObject *, PyContext_New, void);
    DECLARE_DLSYM(PyStatus, PyConfig_Read, PyConfig *config);
    DECLARE_DLSYM(void, PyThreadState_DeleteCurrent, void);
    DECLARE_DLSYM(PyObject*, PyUnicode_DecodeASCII,const char *string,         /* ASCII encoded string */Py_ssize_t length,          /* size of string */const char *errors          /* error handling */);
    DECLARE_DLSYM(PyObject *, PyUnicodeTranslateError_GetReason, PyObject *);
    DECLARE_DLSYM_TYPE(PyTraceBack_Type);
    DECLARE_DLSYM(PyObject *, PyNumber_InPlaceMultiply, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(int, PyArg_ValidateKeywordArguments, PyObject *);
    DECLARE_DLSYM(void*, PyModule_GetState, PyObject*);
    DECLARE_DLSYM_TYPE(PyWrapperDescr_Type);
    DECLARE_DLSYM(PyObject *, PyLong_FromVoidPtr, void *);
    DECLARE_DLSYM(int, PyModule_SetDocString, PyObject *, const char *);
    DECLARE_DLSYM(PyObject *, PyErr_NewExceptionWithDoc,const char *name, const char *doc, PyObject *base, PyObject *dict);
    DECLARE_DLSYM(void, PyErr_SyntaxLocationEx,const char *filename,       /* decoded from the filesystem encoding */int lineno,int col_offset);
    DECLARE_DLSYM(void *, PyObject_Malloc, size_t size);
    DECLARE_DLSYM(PyObject *, PyNumber_InPlaceRemainder, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(void *, PyThread_tss_get, Py_tss_t *key);
    DECLARE_DLSYM(void, PyOS_AfterFork_Child, void);
    DECLARE_DLSYM_TYPE(PyDictIterItem_Type);
    DECLARE_DLSYM(PyObject *, PySet_Pop, PyObject *set);
    DECLARE_DLSYM(int, PyObject_DelItem, PyObject *o, PyObject *key);
    DECLARE_DLSYM(PyObject *, PySeqIter_New, PyObject *);
    DECLARE_DLSYM_TYPE(PyBufferedReader_Type);
    DECLARE_DLSYM(int, PySequence_SetSlice, PyObject *o, Py_ssize_t i1, Py_ssize_t i2,PyObject *v);
    DECLARE_DLSYM(char *, PyOS_double_to_string, double val,char format_code,int precision,int flags,int *type);
    DECLARE_DLSYM(int, PyObject_IsTrue, PyObject *);
    DECLARE_DLSYM(PyObject *, PyImport_GetModule, PyObject *name);
    DECLARE_DLSYM(int, PyStatus_IsError, PyStatus err);
    DECLARE_DLSYM(PyObject *, PyFloat_GetInfo, void);
    DECLARE_DLSYM(int, PyRun_SimpleString, const char *s);
    DECLARE_DLSYM(int, PyCapsule_SetPointer, PyObject *capsule, void *pointer);
    DECLARE_DLSYM(void, PyObject_GetArenaAllocator, PyObjectArenaAllocator *allocator);
    DECLARE_DLSYM(PyObject *, PyUnicode_FromFormat,const char *format,   /* ASCII-encoded string  */...);
    DECLARE_DLSYM(unsigned long long, PyLong_AsUnsignedLongLong, PyObject *);
    DECLARE_DLSYM(PyObject *, PyObject_Str, PyObject *);
    DECLARE_DLSYM(PyObject *, PyObject_CallMethodObjArgs,PyObject *obj,PyObject *name,...);
    DECLARE_DLSYM(PyObject *, PyException_GetCause, PyObject *);
    DECLARE_DLSYM(PyObject*, PyUnicode_AsUTF32String,PyObject *unicode           /* Unicode object */);
    DECLARE_DLSYM(void, Py_IncRef, PyObject *);
    DECLARE_DLSYM(PyObject*, PyUnicode_DecodeUTF16Stateful,const char *string,         /* UTF-16 encoded string */Py_ssize_t length,          /* size of string */const char *errors,         /* error handling */int *byteorder,             /* pointer to byteorder to use0=native;-1=LE,1=BE; updated onexit */Py_ssize_t *consumed        /* bytes consumed */);
    DECLARE_DLSYM(int, PySequence_SetItem, PyObject *o, Py_ssize_t i, PyObject *v);
    DECLARE_DLSYM(PyObject *, PyTuple_Pack, Py_ssize_t, ...);
    DECLARE_DLSYM(Py_ssize_t, PyUnicode_CopyCharacters,PyObject *to,Py_ssize_t to_start,PyObject *from,Py_ssize_t from_start,Py_ssize_t how_many);
    DECLARE_DLSYM_TYPE(PyStaticMethod_Type);
    DECLARE_DLSYM(Py_UCS4*, PyUnicode_AsUCS4,PyObject *unicode,Py_UCS4* buffer,Py_ssize_t buflen,int copy_null);
    DECLARE_DLSYM(PyObject *, PyCell_New, PyObject *);
    DECLARE_DLSYM_TYPE(PyEllipsis_Type);
    DECLARE_DLSYM(Py_ssize_t, PyDict_Size, PyObject *mp);
    DECLARE_DLSYM(void, Py_SetRecursionLimit, int);
    DECLARE_DLSYM(PyObject *, PyNumber_Add, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(double, PyComplex_ImagAsDouble, PyObject *op);
    DECLARE_DLSYM(wchar_t *, Py_GetExecPrefix, void);
    DECLARE_DLSYM(int, PyUnicodeDecodeError_GetStart, PyObject *, Py_ssize_t *);
    DECLARE_DLSYM(PyObject *, PyMemoryView_FromBuffer, Py_buffer *info);
    DECLARE_DLSYM(PyObject *, PyComplex_FromDoubles, double real, double imag);
    DECLARE_DLSYM(int, PyTraceMalloc_Untrack,unsigned int domain,uintptr_t ptr);
    DECLARE_DLSYM(PyObject *, PyDescr_NewMethod, PyTypeObject *, PyMethodDef *);
    DECLARE_DLSYM(PyObject*, PyUnicode_AsASCIIString,PyObject *unicode           /* Unicode object */);
    DECLARE_DLSYM(int, PyThreadState_SetAsyncExc, unsigned long, PyObject *);
    DECLARE_DLSYM(PyObject*, PyUnicode_Concat,PyObject *left,             /* Left string */PyObject *right             /* Right string */);
    DECLARE_DLSYM(PyObject *, Py_CompileStringExFlags,const char *str,const char *filename,       /* decoded from the filesystem encoding */int start,PyCompilerFlags *flags,int optimize);
    DECLARE_DLSYM(PyObject *, PyGen_New, PyFrameObject *);
    DECLARE_DLSYM(PyObject *, PyRun_FileExFlags,FILE *fp,const char *filename,       /* decoded from the filesystem encoding */int start,PyObject *globals,PyObject *locals,int closeit,PyCompilerFlags *flags);
    DECLARE_DLSYM(PyObject *, PyCodec_BackslashReplaceErrors, PyObject *exc);
    DECLARE_DLSYM(int, PyArg_ParseTuple, PyObject *, const char *, ...);
    DECLARE_DLSYM(PyObject*, PyUnicode_DecodeUTF7Stateful,const char *string,         /* UTF-7 encoded string */Py_ssize_t length,          /* size of string */const char *errors,         /* error handling */Py_ssize_t *consumed        /* bytes consumed */);
    DECLARE_DLSYM(PyObject *, PyObject_Call, PyObject *callable,PyObject *args, PyObject *kwargs);
    DECLARE_DLSYM_TYPE(PyMap_Type);
    DECLARE_DLSYM(int, PyRun_AnyFile, FILE *fp, const char *name);
    DECLARE_DLSYM(PyObject *, PyObject_CallFunction, PyObject *callable,const char *format, ...);
    DECLARE_DLSYM(void, Py_FatalError, const char *message);
    DECLARE_DLSYM(void *, PyMem_Realloc, void *ptr, size_t new_size);
    DECLARE_DLSYM(PyObject *, PyFunction_GetKwDefaults, PyObject *);
    DECLARE_DLSYM(PyObject*, PyUnicode_New,Py_ssize_t size,            /* Number of code points in the new string */Py_UCS4 maxchar             /* maximum code point value in the string */);
    DECLARE_DLSYM_TYPE(PyListIter_Type);
    DECLARE_DLSYM(PyObject *, PyUnicode_Format,PyObject *format,           /* Format string */PyObject *args              /* Argument tuple or dictionary */);
    DECLARE_DLSYM(Py_ssize_t, PySet_Size, PyObject *anyset);
    DECLARE_DLSYM(int, PyObject_SetAttr, PyObject *, PyObject *, PyObject *);
    DECLARE_DLSYM(int, PyUnicodeEncodeError_SetStart, PyObject *, Py_ssize_t);
    DECLARE_DLSYM(PyObject *, PyCodec_NameReplaceErrors, PyObject *exc);
    DECLARE_DLSYM(Py_tss_t *, PyThread_tss_alloc, void);
    DECLARE_DLSYM(Py_hash_t, PyObject_HashNotImplemented, PyObject *);
    DECLARE_DLSYM(int, PyObject_IsSubclass, PyObject *object, PyObject *typeorclass);
    DECLARE_DLSYM(PyObject *, PyErr_SetFromErrno, PyObject *);
    DECLARE_DLSYM(PyObject *, PyFunction_GetCode, PyObject *);
    DECLARE_DLSYM(PyThreadState *, Py_NewInterpreter, void);
    DECLARE_DLSYM(PyObject *, PyModule_New,const char *name            /* UTF-8 encoded string */);
    DECLARE_DLSYM(void, PyBuffer_FillContiguousStrides, int ndims,Py_ssize_t *shape,Py_ssize_t *strides,int itemsize,char fort);
    DECLARE_DLSYM(PyObject *, PyObject_SelfIter, PyObject *);
    DECLARE_DLSYM(PyObject *, PyMemoryView_GetContiguous, PyObject *base,int buffertype,char order);
    DECLARE_DLSYM(const char *, PyModule_GetName, PyObject *);
    DECLARE_DLSYM(PyObject *, PyPickleBuffer_FromObject, PyObject *);
    DECLARE_DLSYM(wchar_t *, Py_GetPythonHome, void);
    DECLARE_DLSYM(int, PyCapsule_SetName, PyObject *capsule, const char *name);
    DECLARE_DLSYM_TYPE(PyReversed_Type);
    DECLARE_DLSYM(PyCFunction, PyCFunction_GetFunction, PyObject *);
    DECLARE_DLSYM(int, PyCFunction_GetFlags, PyObject *);
    DECLARE_DLSYM_TYPE(PyPickleBuffer_Type);
    DECLARE_DLSYM(void, PyEval_SetProfile, Py_tracefunc, PyObject *);
    DECLARE_DLSYM(int, PyObject_HasAttr, PyObject *, PyObject *);
    DECLARE_DLSYM(int, PySys_AddAuditHook, Py_AuditHookFunction, void*);
    DECLARE_DLSYM(PyObject *, PyObject_Type, PyObject *o);
    DECLARE_DLSYM(PyObject *, PyImport_ExecCodeModuleWithPathnames,const char *name,           /* UTF-8 encoded string */PyObject *co,const char *pathname,       /* decoded from the filesystem encoding */const char *cpathname       /* decoded from the filesystem encoding */);
    DECLARE_DLSYM(PyObject *, PySequence_GetItem, PyObject *o, Py_ssize_t i);
    DECLARE_DLSYM(PyObject *, PyMember_GetOne, const char *, struct PyMemberDef *);
    DECLARE_DLSYM(void, PySys_FormatStderr, const char *format, ...);
    DECLARE_DLSYM(void, PyStructSequence_InitType, PyTypeObject *type,PyStructSequence_Desc *desc);
    DECLARE_DLSYM(int, PyUnicode_FSDecoder, PyObject*, void*);
    DECLARE_DLSYM(int, PyType_Ready, PyTypeObject *);
    DECLARE_DLSYM(PyObject *, PyNumber_InPlacePower, PyObject *o1, PyObject *o2,PyObject *o3);
    DECLARE_DLSYM(unsigned int, PyType_ClearCache, void);
    DECLARE_DLSYM(const char *, Py_GetPlatform, void);
    DECLARE_DLSYM(int, PyRun_InteractiveLoop, FILE *f, const char *p);
    DECLARE_DLSYM(PyObject *, PyDict_Values, PyObject *mp);
    DECLARE_DLSYM(PyObject *, PyNumber_Long, PyObject *o);
    DECLARE_DLSYM(void, PyErr_SetString,PyObject *exception,const char *string   /* decoded from utf-8 */);
    DECLARE_DLSYM(PyObject *, PyNumber_InPlaceTrueDivide, PyObject *o1,PyObject *o2);
    DECLARE_DLSYM(PyObject *, PyInterpreterState_GetDict, PyInterpreterState *);
    DECLARE_DLSYM(void, PyDict_Clear, PyObject *mp);
    DECLARE_DLSYM(int, PyState_RemoveModule, struct PyModuleDef*);
    DECLARE_DLSYM(void, PyException_SetContext, PyObject *, PyObject *);
    DECLARE_DLSYM(PyObject *, PyNumber_InPlaceLshift, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(PyObject *, PyBool_FromLong, long);
    DECLARE_DLSYM(PyObject *, PyMemoryView_FromObject, PyObject *base);
    DECLARE_DLSYM(PyObject*, PyStructSequence_GetItem, PyObject*, Py_ssize_t);
    DECLARE_DLSYM(PyObject*, PyUnicode_Partition,PyObject *s,                /* String to partition */PyObject *sep               /* String separator */);
    DECLARE_DLSYM(PyObject*, PyCode_Optimize, PyObject *code, PyObject* consts,PyObject *names, PyObject *lnotab);
    DECLARE_DLSYM(PyTryBlock *, PyFrame_BlockPop, PyFrameObject *);
    DECLARE_DLSYM(void, PyThread_tss_delete, Py_tss_t *key);
    DECLARE_DLSYM(PyObject *, PyMapping_Values, PyObject *o);
    DECLARE_DLSYM(int, PyBuffer_ToContiguous, void *buf, Py_buffer *view,Py_ssize_t len, char order);
    DECLARE_DLSYM(PyInterpreterState *, PyInterpreterState_Next, PyInterpreterState *);
    DECLARE_DLSYM(PyObject *, PyErr_Occurred, void);
    DECLARE_DLSYM(int, Py_AddPendingCall, int (*func)(void *), void *arg);
    DECLARE_DLSYM(PyObject *, PyFrozenSet_New, PyObject *);
    DECLARE_DLSYM(int, PyModule_AddObject, PyObject *mod, const char *, PyObject *value);
    DECLARE_DLSYM(int, PyThread_tss_is_created, Py_tss_t *key);
    DECLARE_DLSYM(wchar_t *, Py_DecodeLocale,const char *arg,size_t *size);
    DECLARE_DLSYM(int, PyErr_GivenExceptionMatches, PyObject *, PyObject *);
    DECLARE_DLSYM(unsigned long, PyThread_start_new_thread, void (*)(void *), void *);
    DECLARE_DLSYM(int, PyRun_InteractiveLoopFlags,FILE *fp,const char *filename,       /* decoded from the filesystem encoding */PyCompilerFlags *flags);
    DECLARE_DLSYM(void, PyInterpreterState_Clear, PyInterpreterState *);
    DECLARE_DLSYM_TYPE(PyBytesIO_Type);
    DECLARE_DLSYM(PyObject*, PyUnicode_BuildEncodingMap,PyObject* string            /* 256 character map */);
    DECLARE_DLSYM(wchar_t *, Py_GetPrefix, void);
    DECLARE_DLSYM_TYPE(PyList_Type);
    DECLARE_DLSYM(int, PyObject_CallFinalizerFromDealloc, PyObject *);
    DECLARE_DLSYM(PyObject *, PyMarshal_ReadLastObjectFromFile, FILE *);
    DECLARE_DLSYM(int, PyErr_ResourceWarning,PyObject *source,Py_ssize_t stack_level,const char *format,         /* ASCII-encoded string  */...);
    DECLARE_DLSYM(PyObject *, PyCodec_Decoder,const char *encoding);
    DECLARE_DLSYM(void, PySys_SetPath, const wchar_t *);
    DECLARE_DLSYM(PyObject *, PyNumber_Rshift, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(PyObject*, PyUnicode_AsCharmapString,PyObject *unicode,          /* Unicode object */PyObject *mapping           /* encoding mapping */);
    DECLARE_DLSYM(PyOS_sighandler_t, PyOS_getsig, int);
    DECLARE_DLSYM(PyObject *, PyByteArray_FromStringAndSize, const char *, Py_ssize_t);
    DECLARE_DLSYM(PyObject *, PyNumber_InPlaceXor, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(void *, PyBuffer_GetPointer, Py_buffer *view, Py_ssize_t *indices);
    DECLARE_DLSYM(PyObject *, PyByteArray_FromObject, PyObject *);
    DECLARE_DLSYM(PyObject *, PyLong_FromDouble, double);
    DECLARE_DLSYM(void, Py_InitializeEx, int);
    DECLARE_DLSYM(PyObject *, PyNumber_Positive, PyObject *o);
    DECLARE_DLSYM(Py_ssize_t, PyBytes_Size, PyObject *);
    DECLARE_DLSYM_TYPE(PyDictProxy_Type);
    DECLARE_DLSYM_TYPE(PyBufferedRandom_Type);
    DECLARE_DLSYM(char *, Py_UniversalNewlineFgets, char *, int, FILE*, PyObject *);
    DECLARE_DLSYM(void, PyErr_Print, void);
    DECLARE_DLSYM(PyObject *, PySequence_InPlaceConcat, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(int, PyStatus_IsExit, PyStatus err);
    DECLARE_DLSYM(PyObject *, PyNumber_InPlaceRshift, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(Py_ssize_t, PySequence_Size, PyObject *o);
    DECLARE_DLSYM(PyObject *, PyObject_GetItem, PyObject *o, PyObject *key);
    DECLARE_DLSYM_TYPE(PyDictValues_Type);
    DECLARE_DLSYM_TYPE(PyBytesIter_Type);
    DECLARE_DLSYM(PyObject *, PyObject_GetIter, PyObject *);
    DECLARE_DLSYM_TYPE(PyTextIOBase_Type);
    DECLARE_DLSYM(PyObject *, PyWeakref_GetObject, PyObject *ref);
    DECLARE_DLSYM(Py_UCS4*, PyUnicode_AsUCS4Copy, PyObject *unicode);
    DECLARE_DLSYM(unsigned long, PyThread_get_thread_native_id, void);
    DECLARE_DLSYM(PyObject *, PyMemoryView_FromMemory, char *mem, Py_ssize_t size,int flags);
    DECLARE_DLSYM(long, PyMarshal_ReadLongFromFile, FILE *);
    DECLARE_DLSYM(void, PyOS_BeforeFork, void);
    DECLARE_DLSYM(PyVarObject *, PyObject_InitVar, PyVarObject *,PyTypeObject *, Py_ssize_t);
    DECLARE_DLSYM(int, PyList_Append, PyObject *, PyObject *);
    DECLARE_DLSYM(PyThreadState *, PyThreadState_Swap, PyThreadState *);
    DECLARE_DLSYM(int, PyObject_GetBuffer, PyObject *obj, Py_buffer *view,int flags);
    DECLARE_DLSYM(Py_ssize_t, PyNumber_AsSsize_t, PyObject *o, PyObject *exc);
    DECLARE_DLSYM(unsigned long, PyThread_get_thread_ident, void);
    DECLARE_DLSYM(int, PyMember_SetOne, char *, struct PyMemberDef *, PyObject *);
    DECLARE_DLSYM(PyObject *, PyErr_FormatV,PyObject *exception,const char *format,va_list vargs);
    DECLARE_DLSYM(void, Py_ExitStatusException, PyStatus err);
    DECLARE_DLSYM(void, PyErr_SyntaxLocationObject,PyObject *filename,int lineno,int col_offset);
    DECLARE_DLSYM(PyObject *, PyEval_GetGlobals, void);
    DECLARE_DLSYM(int, PyMapping_Check, PyObject *o);
    DECLARE_DLSYM(int, PyObject_DelItemString, PyObject *o, const char *key);
    DECLARE_DLSYM(void, PyUnicode_Append,PyObject **pleft,           /* Pointer to left string */PyObject *right             /* Right string */);
    DECLARE_DLSYM(int, Py_ReprEnter, PyObject *);
    DECLARE_DLSYM(int, PyMapping_HasKeyString, PyObject *o, const char *key);
    DECLARE_DLSYM(void, Py_GetArgcArgv, int *argc, wchar_t ***argv);
    DECLARE_DLSYM(PyObject *, PyObject_CallFunctionObjArgs, PyObject *callable,...);
    DECLARE_DLSYM(PyObject*, PyUnicode_DecodeCharmap,const char *string,         /* Encoded string */Py_ssize_t length,          /* size of string */PyObject *mapping,          /* decoding mapping */const char *errors          /* error handling */);
    DECLARE_DLSYM(PyObject *, PyImport_ImportModuleNoBlock,const char *name            /* UTF-8 encoded string */);
    DECLARE_DLSYM_TYPE(PyGetSetDescr_Type);
    DECLARE_DLSYM(PyObject *, PyModule_NewObject,PyObject *name);
    DECLARE_DLSYM(double, PyFloat_GetMin, void);
    DECLARE_DLSYM(wchar_t*, PyUnicode_AsWideCharString,PyObject *unicode,          /* Unicode object */Py_ssize_t *size            /* number of characters of the result */);
    DECLARE_DLSYM(int, PySequence_Check, PyObject *o);
    DECLARE_DLSYM(void, PyBytes_Concat, PyObject **, PyObject *);
    DECLARE_DLSYM(PyObject *, PyDescr_NewClassMethod, PyTypeObject *, PyMethodDef *);
    DECLARE_DLSYM_TYPE(PyFileIO_Type);
    DECLARE_DLSYM(PyObject *, PyUnicodeDecodeError_GetObject, PyObject *);
    DECLARE_DLSYM(void, PyEval_AcquireThread, PyThreadState *tstate);
    DECLARE_DLSYM(PyStatus, PyStatus_NoMemory, void);
    DECLARE_DLSYM(PyObject *, PyNumber_Power, PyObject *o1, PyObject *o2,PyObject *o3);
    DECLARE_DLSYM(PyObject *, PyErr_SetFromErrnoWithFilename,PyObject *exc,const char *filename   /* decoded from the filesystem encoding */);
    DECLARE_DLSYM_TYPE(PyTuple_Type);
    DECLARE_DLSYM(void *, PyCapsule_GetPointer, PyObject *capsule, const char *name);
    DECLARE_DLSYM(int, PyUnicodeEncodeError_GetEnd, PyObject *, Py_ssize_t *);
    DECLARE_DLSYM(PyObject *, PyErr_ProgramText,const char *filename,       /* decoded from the filesystem encoding */int lineno);
    DECLARE_DLSYM(PyObject *, PyDict_GetItemString, PyObject *dp, const char *key);
    DECLARE_DLSYM(void, PyFrame_LocalsToFast, PyFrameObject *, int);
    DECLARE_DLSYM(PyObject *, PyNumber_TrueDivide, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(int, PyFunction_SetKwDefaults, PyObject *, PyObject *);
    DECLARE_DLSYM(void, PyObject_GC_UnTrack, void *);
    DECLARE_DLSYM(PyObject*, PyUnicode_EncodeUTF16,PyObject* unicode,          /* Unicode object */const char *errors,         /* error handling */int byteorder               /* byteorder to use 0=BOM+native;-1=LE,1=BE */);
    DECLARE_DLSYM(void, PyFrame_FastToLocals, PyFrameObject *);
    DECLARE_DLSYM(PyObject *, PyNumber_Absolute, PyObject *o);
    DECLARE_DLSYM(int64_t, PyInterpreterState_GetID, PyInterpreterState *);
    DECLARE_DLSYM(int, PyFunction_SetClosure, PyObject *, PyObject *);
    DECLARE_DLSYM(int, PySys_SetObject, const char *, PyObject *);
    DECLARE_DLSYM(PyOS_sighandler_t, PyOS_setsig, int, PyOS_sighandler_t);
    DECLARE_DLSYM(int, PyFrame_GetLineNumber, PyFrameObject *);
    DECLARE_DLSYM(PyObject *, PyGen_NewWithQualName, PyFrameObject *,PyObject *name, PyObject *qualname);
    DECLARE_DLSYM(PyObject*, PyUnicode_DecodeRawUnicodeEscape,const char *string,         /* Raw-Unicode-Escape encoded string */Py_ssize_t length,          /* size of string */const char *errors          /* error handling */);
    DECLARE_DLSYM(PyObject *, PyNumber_Divmod, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(PyObject*, PyUnicode_FromStringAndSize,const char *u,             /* UTF-8 encoded string */Py_ssize_t size            /* size of buffer */);
    DECLARE_DLSYM(int, PyObject_GenericSetDict, PyObject *, PyObject *, void *);
    DECLARE_DLSYM(void, PyEval_SetTrace, Py_tracefunc, PyObject *);
    DECLARE_DLSYM(int, PyCallable_Check, PyObject *);
    DECLARE_DLSYM(PyObject *, PyNumber_InPlaceSubtract, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(unsigned long, PyLong_AsUnsignedLongMask, PyObject *);
    DECLARE_DLSYM(PyInterpreterState *, PyInterpreterState_Head, void);
    DECLARE_DLSYM_TYPE(PyEnum_Type);
    DECLARE_DLSYM(PyObject*, PyUnicode_DecodeUTF7,const char *string,         /* UTF-7 encoded string */Py_ssize_t length,          /* size of string */const char *errors          /* error handling */);
    DECLARE_DLSYM(PyObject*, PyUnicode_DecodeUTF8,const char *string,         /* UTF-8 encoded string */Py_ssize_t length,          /* size of string */const char *errors          /* error handling */);
    DECLARE_DLSYM(int, PyObject_SetAttrString, PyObject *, const char *, PyObject *);
    DECLARE_DLSYM(int, Py_SetStandardStreamEncoding, const char *encoding,const char *errors);
    DECLARE_DLSYM(int, PyContextVar_Reset, PyObject *var, PyObject *token);
    DECLARE_DLSYM(int, PyImport_AppendInittab,const char *name,           /* ASCII encoded string */PyObject* (*initfunc)(void));
    DECLARE_DLSYM(void, PyThread_tss_free, Py_tss_t *key);
    DECLARE_DLSYM(int, PySet_Clear, PyObject *set);
    DECLARE_DLSYM(int, PyThread_acquire_lock, PyThread_type_lock, int);
    DECLARE_DLSYM(int, PyUnicode_Compare,PyObject *left,             /* Left string */PyObject *right             /* Right string */);
    DECLARE_DLSYM(PyObject *, PyFunction_GetAnnotations, PyObject *);
    DECLARE_DLSYM(const Py_UNICODE *, PyUnicode_AsUnicode,PyObject *unicode           /* Unicode object */);
    DECLARE_DLSYM(PyCodeObject *, PyCode_New,int, int, int, int, int, PyObject *, PyObject *,PyObject *, PyObject *, PyObject *, PyObject *,PyObject *, PyObject *, int, PyObject *);
    DECLARE_DLSYM_TYPE(PyCallIter_Type);
    DECLARE_DLSYM(PyObject *, PyErr_SetFromErrnoWithFilenameObject,PyObject *, PyObject *);
    DECLARE_DLSYM(int, PySet_Contains, PyObject *anyset, PyObject *key);
    DECLARE_DLSYM(int, PySlice_GetIndicesEx, PyObject *r, Py_ssize_t length,Py_ssize_t *start, Py_ssize_t *stop,Py_ssize_t *step,Py_ssize_t *slicelength);
    DECLARE_DLSYM(const char *, Py_GetCompiler, void);
    DECLARE_DLSYM(PyObject *, Py_CompileString, const char *, const char *, int);
    DECLARE_DLSYM(PyObject*, PyUnicode_FromString,const char *u              /* UTF-8 encoded string */);
    DECLARE_DLSYM(PyObject *, PyObject_Repr, PyObject *);
    DECLARE_DLSYM(PyObject*, PyState_FindModule, struct PyModuleDef*);
    DECLARE_DLSYM(PyObject *, PyMethod_Function, PyObject *);
    DECLARE_DLSYM(void, PyErr_SetObject, PyObject *, PyObject *);
    DECLARE_DLSYM(int, PyEval_MergeCompilerFlags, PyCompilerFlags *cf);
    DECLARE_DLSYM(PyObject *, PyObject_CallObject, PyObject *callable,PyObject *args);
    DECLARE_DLSYM_TYPE(PyStdPrinter_Type);
    DECLARE_DLSYM_TYPE(PyDictIterValue_Type);
    DECLARE_DLSYM_TYPE(PyIOBase_Type);
    DECLARE_DLSYM(PyObject*, PyUnicode_FromKindAndData,int kind,const void *buffer,Py_ssize_t size);
    DECLARE_DLSYM(PyObject *, PyCodec_StreamReader,const char *encoding,PyObject *stream,const char *errors);
    DECLARE_DLSYM(PyObject*, PyUnicode_DecodeFSDefaultAndSize,const char *s,               /* encoded string */Py_ssize_t size              /* size */);
    DECLARE_DLSYM(int, PyDict_Contains, PyObject *mp, PyObject *key);
    DECLARE_DLSYM(PyObject*, PyUnicode_Substring,PyObject *str,Py_ssize_t start,Py_ssize_t end);
    DECLARE_DLSYM(PyObject *, PyNumber_Or, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(PyObject*, PyUnicode_RSplit,PyObject *s,                /* String to split */PyObject *sep,              /* String separator */Py_ssize_t maxsplit         /* Maxsplit count */);
    DECLARE_DLSYM(int, PyRun_InteractiveOneObject,FILE *fp,PyObject *filename,PyCompilerFlags *flags);
    DECLARE_DLSYM_TYPE(PyMemberDescr_Type);
    DECLARE_DLSYM_TYPE(PyDictRevIterValue_Type);
    DECLARE_DLSYM(void, Py_DecRef, PyObject *);
    DECLARE_DLSYM(PyObject*, PyUnicode_DecodeUTF32Stateful,const char *string,         /* UTF-32 encoded string */Py_ssize_t length,          /* size of string */const char *errors,         /* error handling */int *byteorder,             /* pointer to byteorder to use0=native;-1=LE,1=BE; updated onexit */Py_ssize_t *consumed        /* bytes consumed */);
    DECLARE_DLSYM(int, PyRun_SimpleFileExFlags,FILE *fp,const char *filename,       /* decoded from the filesystem encoding */int closeit,PyCompilerFlags *flags);
    DECLARE_DLSYM(PyObject *, PyMethod_New, PyObject *, PyObject *);
    DECLARE_DLSYM(PyObject*, PyType_FromSpec, PyType_Spec*);
    DECLARE_DLSYM(PyObject*, PyUnicode_EncodeUTF32,PyObject *object,           /* Unicode object */const char *errors,         /* error handling */int byteorder               /* byteorder to use 0=BOM+native;-1=LE,1=BE */);
    DECLARE_DLSYM(void, PyConfig_InitIsolatedConfig, PyConfig *config);
    DECLARE_DLSYM(void, Py_SetProgramName, const wchar_t *);
    DECLARE_DLSYM(int, PyUnicodeTranslateError_GetEnd, PyObject *, Py_ssize_t *);
    DECLARE_DLSYM(PyObject *, PyBytes_FromString, const char *);
    DECLARE_DLSYM(int, PyUnicodeTranslateError_SetStart, PyObject *, Py_ssize_t);
    DECLARE_DLSYM(int, Py_FinalizeEx, void);
    DECLARE_DLSYM(PyObject *, PyImport_GetModuleDict, void);
    DECLARE_DLSYM(int, PySys_HasWarnOptions, void);
    DECLARE_DLSYM(void, PyUnicode_InternInPlace, PyObject **);
    DECLARE_DLSYM(int, PyModule_AddIntConstant, PyObject *, const char *, long);
    DECLARE_DLSYM(Py_ssize_t, PyObject_LengthHint, PyObject *o, Py_ssize_t);
    DECLARE_DLSYM_TYPE(PyCapsule_Type);
    DECLARE_DLSYM(int, PyContext_Exit, PyObject *);
    DECLARE_DLSYM(PyStatus, PyConfig_SetString,PyConfig *config,wchar_t **config_str,const wchar_t *str);
    DECLARE_DLSYM(PyStatus, PyConfig_SetBytesArgv,PyConfig *config,Py_ssize_t argc,char * const *argv);
    DECLARE_DLSYM(PyObject *, PyEval_EvalFrameEx, PyFrameObject *f, int exc);
    DECLARE_DLSYM(int, PyUnicodeEncodeError_SetEnd, PyObject *, Py_ssize_t);
    DECLARE_DLSYM(int, PyIter_Check, PyObject *);
    DECLARE_DLSYM(int, PyState_AddModule, PyObject*, struct PyModuleDef*);
    DECLARE_DLSYM(void, PySys_AddWarnOptionUnicode, PyObject *);
    DECLARE_DLSYM(PyObject *, PyContext_Copy, PyObject *);
    DECLARE_DLSYM_TYPE(PyByteArrayIter_Type);
    DECLARE_DLSYM(PyObject*, PyUnicode_DecodeFSDefault,const char *s               /* encoded string */);
    DECLARE_DLSYM(PyObject *, PyStaticMethod_New, PyObject *);
    DECLARE_DLSYM(int, PyCell_Set, PyObject *, PyObject *);
    DECLARE_DLSYM(PyObject *, PyContextVar_New,const char *name, PyObject *default_value);
    DECLARE_DLSYM(PyObject *, PyNumber_MatrixMultiply, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(PyObject *, PyCoro_New, PyFrameObject *,PyObject *name, PyObject *qualname);
    DECLARE_DLSYM(int, PyRun_SimpleStringFlags, const char *, PyCompilerFlags *);
    DECLARE_DLSYM(Py_ssize_t, PyUnicode_GetLength,PyObject *unicode);
    DECLARE_DLSYM(int, PyModule_AddStringConstant, PyObject *, const char *, const char *);
    DECLARE_DLSYM(PyObject *, PyAsyncGen_New, PyFrameObject *,PyObject *name, PyObject *qualname);
    DECLARE_DLSYM(PyStatus, PyWideStringList_Insert, PyWideStringList *list,Py_ssize_t index,const wchar_t *item);
    DECLARE_DLSYM_TYPE(PyCode_Type);
    DECLARE_DLSYM_TYPE(PyProperty_Type);
    DECLARE_DLSYM(PyObject *, PyException_GetContext, PyObject *);
    DECLARE_DLSYM(PyObject *, PyEval_GetBuiltins, void);
    DECLARE_DLSYM(int, PyMapping_HasKey, PyObject *o, PyObject *key);
    DECLARE_DLSYM(PyObject *, PyImport_ImportModuleLevel,const char *name,           /* UTF-8 encoded string */PyObject *globals,PyObject *locals,PyObject *fromlist,int level);
    DECLARE_DLSYM(Py_ssize_t, PyGC_Collect, void);
    DECLARE_DLSYM_TYPE(PyRange_Type);
    DECLARE_DLSYM(PyTypeObject*, PyStructSequence_NewType, PyStructSequence_Desc *desc);
    DECLARE_DLSYM(void, PyThread_exit_thread, void);
    DECLARE_DLSYM(Py_ssize_t, PyUnicode_Count,PyObject *str,              /* String */PyObject *substr,           /* Substring to count */Py_ssize_t start,           /* Start index */Py_ssize_t end              /* Stop index */);
    DECLARE_DLSYM(void, PyThread_init_thread, void);
    DECLARE_DLSYM(PyObject *, PyErr_SetImportError, PyObject *, PyObject *,PyObject *);
    DECLARE_DLSYM(PyObject*, PyUnicode_FromEncodedObject,PyObject *obj,              /* Object */const char *encoding,       /* encoding */const char *errors          /* error handling */);
    DECLARE_DLSYM(PyFrameObject *, PyEval_GetFrame, void);
    DECLARE_DLSYM(PyObject *, PyLong_FromUnsignedLong, unsigned long);
    DECLARE_DLSYM(void *, PyMem_RawMalloc, size_t size);
    DECLARE_DLSYM_TYPE(PyBool_Type);
    DECLARE_DLSYM(void, PyFrame_BlockSetup, PyFrameObject *, int, int, int);
    DECLARE_DLSYM_TYPE(PyDictItems_Type);
    DECLARE_DLSYM(PyObject *, PyEval_EvalCodeEx, PyObject *co,PyObject *globals,PyObject *locals,PyObject *const *args, int argc,PyObject *const *kwds, int kwdc,PyObject *const *defs, int defc,PyObject *kwdefs, PyObject *closure);
    DECLARE_DLSYM(wchar_t *, Py_GetProgramFullPath, void);
    DECLARE_DLSYM(int, PyArg_Parse, PyObject *, const char *, ...);
    DECLARE_DLSYM(PyObject *, PyInstanceMethod_Function, PyObject *);
    DECLARE_DLSYM(void, PyObject_Free, void *ptr);
    DECLARE_DLSYM(PyThreadState *, PyGILState_GetThisThreadState, void);
    DECLARE_DLSYM_TYPE(PyUnicodeIter_Type);
    DECLARE_DLSYM(PyObject *, PyUnicodeDecodeError_Create,const char *encoding,       /* UTF-8 encoded string */const char *object,Py_ssize_t length,Py_ssize_t start,Py_ssize_t end,const char *reason          /* UTF-8 encoded string */);
    DECLARE_DLSYM(Py_ssize_t, PyLong_AsSsize_t, PyObject *);
    DECLARE_DLSYM_TYPE(PyComplex_Type);
    DECLARE_DLSYM(PyObject *, PyNumber_And, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(void, PyErr_SyntaxLocation,const char *filename,       /* decoded from the filesystem encoding */int lineno);
    DECLARE_DLSYM(PyObject*, PyType_FromSpecWithBases, PyType_Spec*, PyObject*);
    DECLARE_DLSYM(void, PyObject_GC_Del, void *);
    DECLARE_DLSYM(int, PyCode_Addr2Line, PyCodeObject *, int);
    DECLARE_DLSYM(long long, PyLong_AsLongLongAndOverflow, PyObject *, int *);
    DECLARE_DLSYM(int, PyRun_AnyFileExFlags,FILE *fp,const char *filename,       /* decoded from the filesystem encoding */int closeit,PyCompilerFlags *flags);
    DECLARE_DLSYM_TYPE(PyTextIOWrapper_Type);
    DECLARE_DLSYM(unsigned long, PyOS_strtoul, const char *, char **, int);
    DECLARE_DLSYM(PyObject *, PyObject_ASCII, PyObject *);
    DECLARE_DLSYM(PyObject *, PyImport_ReloadModule, PyObject *m);
    DECLARE_DLSYM(PyObject *, PyMapping_Items, PyObject *o);
    DECLARE_DLSYM(PyObject *, PySequence_Fast, PyObject *o, const char* m);
    DECLARE_DLSYM(PyObject *, PyObject_RichCompare, PyObject *, PyObject *, int);
    DECLARE_DLSYM(void, PyObject_GC_Track, void *);
    DECLARE_DLSYM(PyObject*, PyUnicode_AsEncodedString,PyObject *unicode,          /* Unicode object */const char *encoding,       /* encoding */const char *errors          /* error handling */);
    DECLARE_DLSYM(PyObject *, PyDict_Copy, PyObject *mp);
    DECLARE_DLSYM(PyObject *, PyUnicodeEncodeError_GetObject, PyObject *);
    DECLARE_DLSYM(int, Py_Main, int argc, wchar_t **argv);
    DECLARE_DLSYM(PyObject *, PyModule_GetFilenameObject, PyObject *);
    DECLARE_DLSYM(int, PyFunction_SetDefaults, PyObject *, PyObject *);
    DECLARE_DLSYM(void *, PyLong_AsVoidPtr, PyObject *);
    DECLARE_DLSYM(int, PyUnicodeTranslateError_SetEnd, PyObject *, Py_ssize_t);
    DECLARE_DLSYM(char*, Py_EncodeLocale,const wchar_t *text,size_t *error_pos);
    DECLARE_DLSYM(int, PyDict_SetItemString, PyObject *dp, const char *key, PyObject *item);
    DECLARE_DLSYM(PyObject *, PyCFunction_NewEx, PyMethodDef *, PyObject *,PyObject *);
    DECLARE_DLSYM(PyObject *, PyCodec_ReplaceErrors, PyObject *exc);
    DECLARE_DLSYM(PyObject *, PyErr_NewException,const char *name, PyObject *base, PyObject *dict);
    DECLARE_DLSYM(PyObject *, PyDict_GetItemWithError, PyObject *mp, PyObject *key);
    DECLARE_DLSYM_TYPE(PyODict_Type);
    DECLARE_DLSYM(void, PyStructSequence_SetItem, PyObject*, Py_ssize_t, PyObject*);
    DECLARE_DLSYM(PyObject *, PyUnicode_Replace,PyObject *str,              /* String */PyObject *substr,           /* Substring to find */PyObject *replstr,          /* Substring to replace */Py_ssize_t maxcount         /* Max. number of replacements to apply;-1 = all */);
    DECLARE_DLSYM(long long, PyLong_AsLongLong, PyObject *);
    DECLARE_DLSYM(void, PyErr_NormalizeException, PyObject**, PyObject**, PyObject**);
    DECLARE_DLSYM(PyObject *, PyList_GetSlice, PyObject *, Py_ssize_t, Py_ssize_t);
    DECLARE_DLSYM(unsigned long, PyLong_AsUnsignedLong, PyObject *);
    DECLARE_DLSYM(PyThreadState *, PyThreadState_Get, void);
    DECLARE_DLSYM(PyObject*, PyUnicode_AsUTF8String,PyObject *unicode           /* Unicode object */);
    DECLARE_DLSYM_TYPE(PyFilter_Type);
    DECLARE_DLSYM(void, PyErr_WriteUnraisable, PyObject *);
    DECLARE_DLSYM_TYPE(PyODictItems_Type);
    DECLARE_DLSYM_TYPE(PyBaseObject_Type);
    DECLARE_DLSYM(PyObject *, PySlice_New, PyObject* start, PyObject* stop,PyObject* step);
    DECLARE_DLSYM(PyObject *, PyFunction_GetDefaults, PyObject *);
    DECLARE_DLSYM(int, PyFrame_FastToLocalsWithError, PyFrameObject *f);
    DECLARE_DLSYM(PyObject *, PyMethod_Self, PyObject *);
    DECLARE_DLSYM(void*, PyType_GetSlot, PyTypeObject*, int);
    DECLARE_DLSYM(Py_ssize_t, PyUnicode_AsWideChar,PyObject *unicode,          /* Unicode object */wchar_t *w,                 /* wchar_t buffer */Py_ssize_t size             /* size of buffer */);
    DECLARE_DLSYM(int, PyObject_AsCharBuffer, PyObject *obj,const char **buffer,Py_ssize_t *buffer_len);
    DECLARE_DLSYM(void, PyBuffer_Release, Py_buffer *view);
    DECLARE_DLSYM(PyObject *, PyObject_GetAttrString, PyObject *, const char *);
    DECLARE_DLSYM(PyObject *, PyDescr_NewMember, PyTypeObject *,struct PyMemberDef *);
    DECLARE_DLSYM(int, PyList_Reverse, PyObject *);
    DECLARE_DLSYM(PyObject *, PyEval_EvalFrame, PyFrameObject *);
    DECLARE_DLSYM_TYPE(PyContext_Type);
    DECLARE_DLSYM(int, PyCodec_KnownEncoding,const char *encoding);
    DECLARE_DLSYM(PyObject *, PyDict_SetDefault,PyObject *mp, PyObject *key, PyObject *defaultobj);
    DECLARE_DLSYM(PyObject*, PyUnicode_FromObject,PyObject *obj      /* Object */);
    DECLARE_DLSYM(PyObject *, PyMarshal_ReadObjectFromFile, FILE *);
    DECLARE_DLSYM(double, PyOS_string_to_double, const char *str,char **endptr,PyObject *overflow_exception);
    DECLARE_DLSYM(void, PyMarshal_WriteLongToFile, long, FILE *, int);
    DECLARE_DLSYM(PyObject*, PyUnicode_Decode,const char *s,              /* encoded string */Py_ssize_t size,            /* size of buffer */const char *encoding,       /* encoding */const char *errors          /* error handling */);
    DECLARE_DLSYM(int, PyObject_SetItem, PyObject *o, PyObject *key, PyObject *v);
    DECLARE_DLSYM(void, PySys_SetArgvEx, int, wchar_t **, int);
    DECLARE_DLSYM(int, PyType_IsSubtype, PyTypeObject *, PyTypeObject *);
    DECLARE_DLSYM(int, PyRun_SimpleFile, FILE *f, const char *p);
    DECLARE_DLSYM(const char *, Py_GetVersion, void);
    DECLARE_DLSYM(PyObject *, PyNumber_Float, PyObject *o);
    DECLARE_DLSYM(PyObject *, PyCallIter_New, PyObject *, PyObject *);
    DECLARE_DLSYM(int, Py_MakePendingCalls, void);
    DECLARE_DLSYM(void, PyEval_ReleaseThread, PyThreadState *tstate);
    DECLARE_DLSYM(PyObject *, PyFloat_FromDouble, double);
    DECLARE_DLSYM(PyObject *, PyFunction_GetClosure, PyObject *);
    DECLARE_DLSYM(int, PyCompile_OpcodeStackEffectWithJump, int opcode, int oparg, int jump);
    DECLARE_DLSYM(PyObject *, PyErr_SetImportErrorSubclass, PyObject *, PyObject *,PyObject *, PyObject *);
    DECLARE_DLSYM(PyObject *, PyBytes_FromObject, PyObject *);
    DECLARE_DLSYM(int, Py_RunMain, void);
    DECLARE_DLSYM(PyStatus, Py_PreInitialize,const PyPreConfig *src_config);
    DECLARE_DLSYM(PyObject *, PyFunction_New, PyObject *, PyObject *);
    DECLARE_DLSYM(int, PyBuffer_FillInfo, Py_buffer *view, PyObject *o, void *buf,Py_ssize_t len, int readonly,int flags);
    DECLARE_DLSYM(PyObject *, PyNumber_Index, PyObject *o);
    DECLARE_DLSYM(PyObject *, PyList_AsTuple, PyObject *);
    DECLARE_DLSYM_TYPE(PyDictRevIterKey_Type);
    DECLARE_DLSYM(PyObject *, PyCodec_Encoder,const char *encoding);
    DECLARE_DLSYM(PyThreadState *, PyEval_SaveThread, void);
    DECLARE_DLSYM(PyObject*, PyUnicode_Join,PyObject *separator,        /* Separator string */PyObject *seq               /* Sequence object */);
    DECLARE_DLSYM(wchar_t *, Py_GetPath, void);
    DECLARE_DLSYM(Py_ssize_t, PySequence_Count, PyObject *o, PyObject *value);
    DECLARE_DLSYM(int, PyOS_InterruptOccurred, void);
    DECLARE_DLSYM(int, PyException_SetTraceback, PyObject *, PyObject *);
    DECLARE_DLSYM(PyObject *, PySequence_GetSlice, PyObject *o, Py_ssize_t i1, Py_ssize_t i2);
    DECLARE_DLSYM_TYPE(PyDictKeys_Type);
    DECLARE_DLSYM(Py_ssize_t, PyUnicode_FindChar,PyObject *str,Py_UCS4 ch,Py_ssize_t start,Py_ssize_t end,int direction);
    DECLARE_DLSYM(PyObject*, PyUnicode_AsUnicodeEscapeString,PyObject *unicode           /* Unicode object */);
    DECLARE_DLSYM(PyObject*, PyUnicode_DecodeUTF16,const char *string,         /* UTF-16 encoded string */Py_ssize_t length,          /* size of string */const char *errors,         /* error handling */int *byteorder              /* pointer to byteorder to use0=native;-1=LE,1=BE; updated onexit */);
    DECLARE_DLSYM(PyObject *, PyLong_FromString, const char *, char **, int);
    DECLARE_DLSYM(void, PyType_Modified, PyTypeObject *);
    DECLARE_DLSYM(PyObject*, PyUnicode_DecodeUnicodeEscape,const char *string,         /* Unicode-Escape encoded string */Py_ssize_t length,          /* size of string */const char *errors          /* error handling */);
    DECLARE_DLSYM(PyObject *, PySequence_InPlaceRepeat, PyObject *o, Py_ssize_t count);
    DECLARE_DLSYM(PyObject *, PySequence_Concat, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(PyObject *, PyNumber_InPlaceOr, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM_TYPE(PyDict_Type);
    DECLARE_DLSYM(size_t, PyLong_AsSize_t, PyObject *);
    DECLARE_DLSYM(int, Py_FdIsInteractive, FILE *, const char *);
    DECLARE_DLSYM(PyObject *, PyUnicodeDecodeError_GetReason, PyObject *);
    DECLARE_DLSYM(PyObject *, PyNumber_Remainder, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM_TYPE(PySTEntry_Type);
    DECLARE_DLSYM(int, PyOS_mystricmp, const char *, const char *);
    DECLARE_DLSYM(PyObject *, PyMapping_GetItemString, PyObject *o,const char *key);
    DECLARE_DLSYM(int, PyDict_Next,PyObject *mp, Py_ssize_t *pos, PyObject **key, PyObject **value);
    DECLARE_DLSYM(void, PyObject_SetArenaAllocator, PyObjectArenaAllocator *allocator);
    DECLARE_DLSYM(PyObject *, PyNumber_InPlaceAdd, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(void, Py_Initialize, void);
    DECLARE_DLSYM(int, PyBytes_AsStringAndSize,PyObject *obj,      /* bytes object */char **s,           /* pointer to buffer variable */Py_ssize_t *len     /* pointer to length variable or NULL */);
    DECLARE_DLSYM(int, PyToken_OneChar, int);
    DECLARE_DLSYM(PyObject *, PyCodec_Decode,PyObject *object,const char *encoding,const char *errors);
    DECLARE_DLSYM(PyObject*, PyUnicode_DecodeLocale,const char *str,const char *errors);
    DECLARE_DLSYM(PyObject *, PyNumber_InPlaceMatrixMultiply, PyObject *o1, PyObject *o2);
    DECLARE_DLSYM(int, PyODict_DelItem, PyObject *od, PyObject *key);
    DECLARE_DLSYM(int, PyObject_CopyData, PyObject *dest, PyObject *src);
    DECLARE_DLSYM_TYPE(PySet_Type);
    DECLARE_DLSYM(double, PyFloat_AsDouble, PyObject *);
    DECLARE_DLSYM(PyObject *, PyUnicode_Translate,PyObject *str,              /* String */PyObject *table,            /* Translate table */const char *errors          /* error handling */);
    DECLARE_DLSYM(PyObject *, PyLong_FromLongLong, long long);
    DECLARE_DLSYM(void, PyErr_SetInterrupt, void);
    DECLARE_DLSYM_TYPE(PyRangeIter_Type);
    DECLARE_DLSYM(void, PyMem_SetupDebugHooks, void);
    DECLARE_DLSYM(int, PyErr_WarnEx,PyObject *category,const char *message,        /* UTF-8 encoded string */Py_ssize_t stack_level);
    DECLARE_DLSYM(double, PyComplex_RealAsDouble, PyObject *op);
    DECLARE_DLSYM(PyObject*, PyUnicode_AsUTF16String,PyObject *unicode           /* Unicode object */);
    DECLARE_DLSYM_TYPE(PyMethod_Type);
    DECLARE_DLSYM_TYPE(PyContextVar_Type);
    DECLARE_DLSYM(PyObject *, PyDescr_NewGetSet, PyTypeObject *,struct PyGetSetDef *);
    DECLARE_DLSYM(unsigned long long, PyLong_AsUnsignedLongLongMask, PyObject *);
    DECLARE_DLSYM(int, PySet_Discard, PyObject *set, PyObject *key);
    DECLARE_DLSYM(PyObject *, PyFile_OpenCode, const char *utf8path);
    DECLARE_DLSYM(PyLockStatus, PyThread_acquire_lock_timed, PyThread_type_lock,PY_TIMEOUT_T microseconds,int intr_flag);
    DECLARE_DLSYM(void, PyErr_Display, PyObject *, PyObject *, PyObject *);
    DECLARE_DLSYM(PyObject*, PyUnicode_DecodeUTF32,const char *string,         /* UTF-32 encoded string */Py_ssize_t length,          /* size of string */const char *errors,         /* error handling */int *byteorder              /* pointer to byteorder to use0=native;-1=LE,1=BE; updated onexit */);
    DECLARE_DLSYM(PyObject*, PyUnicode_FromOrdinal, int ordinal);
    DECLARE_DLSYM(Py_ssize_t, PyByteArray_Size, PyObject *);
    DECLARE_DLSYM(PyObject *, PyCodec_XMLCharRefReplaceErrors, PyObject *exc);
    DECLARE_DLSYM(PyObject *, PyMapping_Keys, PyObject *o);
    DECLARE_DLSYM(int, PyUnicodeEncodeError_GetStart, PyObject *, Py_ssize_t *);
    DECLARE_DLSYM(PyObject *, PyImport_ExecCodeModule,const char *name,           /* UTF-8 encoded string */PyObject *co);
    DECLARE_DLSYM(PyObject *, PyCodec_Encode,PyObject *object,const char *encoding,const char *errors);
    DECLARE_DLSYM_TYPE(PyType_Type);
    DECLARE_DLSYM(PyObject *, PyErr_ProgramTextObject,PyObject *filename,int lineno);
    DECLARE_DLSYM(void, PyErr_SetNone, PyObject *);
    DECLARE_DLSYM(void, PyException_SetCause, PyObject *, PyObject *);
    DECLARE_DLSYM(PyObject *, PyRun_String, const char *str, int s, PyObject *g, PyObject *l);
    DECLARE_DLSYM(PyObject *, PyCFunction_New, PyMethodDef *, PyObject *);
    DECLARE_DLSYM(PyObject *, PyClassMethod_New, PyObject *);
    DECLARE_DLSYM_TYPE(PyODictValues_Type);
    DECLARE_DLSYM(int, PyModule_ExecDef, PyObject *module, PyModuleDef *def);
    DECLARE_DLSYM(int, PyTraceMalloc_Track,unsigned int domain,uintptr_t ptr,size_t size);
    DECLARE_DLSYM(int, PyByteArray_Resize, PyObject *, Py_ssize_t);
    DECLARE_DLSYM(PyObject *, PyList_GetItem, PyObject *, Py_ssize_t);
    DECLARE_DLSYM(PyObject *, Py_VaBuildValue, const char *, va_list);
    DECLARE_DLSYM(int, PyObject_Print, PyObject *, FILE *, int);
    DECLARE_DLSYM(char *, PyByteArray_AsString, PyObject *);
    DECLARE_DLSYM(long, PyLong_AsLong, PyObject *);
    DECLARE_DLSYM(int, PyFile_SetOpenCodeHook, Py_OpenCodeHookFunction hook, void *userData);
    DECLARE_DLSYM(PyObject *, PyObject_GenericGetAttr, PyObject *, PyObject *);
    DECLARE_DLSYM(Py_hash_t, PyObject_Hash, PyObject *);
    DECLARE_DLSYM(int, Py_GetRecursionLimit, void);
    DECLARE_DLSYM(void, PyPreConfig_InitPythonConfig, PyPreConfig *config);
    DECLARE_DLSYM(PyInterpreterState *, PyInterpreterState_Main, void);
    DECLARE_DLSYM(PyObject *, PyImport_ImportModule,const char *name            /* UTF-8 encoded string */);
    DECLARE_DLSYM(int, Py_BytesMain, int argc, char **argv);
    DECLARE_DLSYM(PyObject *, PyDescr_NewWrapper, PyTypeObject *,struct wrapperbase *, void *);
    DECLARE_DLSYM(PyObject *, PyImport_ExecCodeModuleObject,PyObject *name,PyObject *co,PyObject *pathname,PyObject *cpathname);
    DECLARE_DLSYM(PyObject *, PyErr_Format,PyObject *exception,const char *format,   /* ASCII-encoded string  */...);
    DECLARE_DLSYM(PyObject *, PySet_New, PyObject *);
    DECLARE_DLSYM(int, PyObject_Not, PyObject *);
    DECLARE_DLSYM(int, PyDict_DelItem, PyObject *mp, PyObject *key);
    DECLARE_DLSYM_TYPE(PyRawIOBase_Type);
    DECLARE_DLSYM(PyStatus, PyConfig_SetWideStringList, PyConfig *config,PyWideStringList *list,Py_ssize_t length, wchar_t **items);
    DECLARE_DLSYM(int, PyUnicodeDecodeError_SetStart, PyObject *, Py_ssize_t);
    DECLARE_DLSYM(PyObject *, PyFunction_GetModule, PyObject *);
    DECLARE_DLSYM(const char *, PyUnicode_AsUTF8, PyObject *unicode);
    DECLARE_DLSYM(PyObject*, PyUnicode_RPartition,PyObject *s,                /* String to partition */PyObject *sep               /* String separator */);
    DECLARE_DLSYM(PyObject *, PyNumber_Invert, PyObject *o);
    DECLARE_DLSYM(void, PyObject_ClearWeakRefs, PyObject *);
    DECLARE_DLSYM_TYPE(PyBufferedWriter_Type);
    DECLARE_DLSYM(PyObject *, PyFile_NewStdPrinter, int);
    DECLARE_DLSYM(PyObject *, PyVectorcall_Call, PyObject *callable, PyObject *tuple, PyObject *dict);
    DECLARE_DLSYM_TYPE(PyContextTokenMissing_Type);
    DECLARE_DLSYM(PyObject *, PyContextVar_Set, PyObject *var, PyObject *value);
    DECLARE_DLSYM_TYPE(PyBytes_Type);
    DECLARE_DLSYM(PyObject *, PyUnicode_FromFormatV,const char *format,   /* ASCII-encoded string  */va_list vargs);
    DECLARE_DLSYM(void, PyErr_BadInternalCall, void);
    DECLARE_DLSYM(int, PyUnicodeTranslateError_SetReason,PyObject *exc,const char *reason          /* UTF-8 encoded string */);
    DECLARE_DLSYM(int, PyDict_MergeFromSeq2, PyObject *d,PyObject *seq2,int override);
    DECLARE_DLSYM(PyFrameObject *, PyFrame_New, PyThreadState *, PyCodeObject *,PyObject *, PyObject *);
    DECLARE_DLSYM(PyObject *, PyFunction_NewWithQualName, PyObject *, PyObject *, PyObject *);
    DECLARE_DLSYM(void *, PyMem_RawCalloc, size_t nelem, size_t elsize);
    DECLARE_DLSYM(PyObject *, PyCodec_IncrementalEncoder,const char *encoding,const char *errors);
    DECLARE_DLSYM(PyObject *, PyStructSequence_New, PyTypeObject* type);
    DECLARE_DLSYM(int, PyODict_SetItem, PyObject *od, PyObject *key, PyObject *item);
    DECLARE_DLSYM(PyObject*, PyUnicode_TransformDecimalToASCII,Py_UNICODE *s,              /* Unicode buffer */Py_ssize_t length           /* Number of Py_UNICODE chars to transform */);
    DECLARE_DLSYM(const char *, PyImport_GetMagicTag, void);
    DECLARE_DLSYM(PyObject *, PyEval_GetLocals, void);
    DECLARE_DLSYM(int, PyImport_ExtendInittab, struct _inittab *newtab);
    DECLARE_DLSYM(PyObject *, PyRun_File, FILE *fp, const char *p, int s, PyObject *g, PyObject *l);
    DECLARE_DLSYM(PyObject *, PyRun_FileFlags, FILE *fp, const char *p, int s, PyObject *g, PyObject *l, PyCompilerFlags *flags);
    DECLARE_DLSYM(void, PyMem_Free, void *ptr);
    DECLARE_DLSYM(PyObject *, PyErr_NoMemory, void);
    DECLARE_DLSYM(int, PyCompile_OpcodeStackEffect, int opcode, int oparg);
    DECLARE_DLSYM(int, PyFunction_SetAnnotations, PyObject *, PyObject *);
    DECLARE_DLSYM_TYPE(PyBufferedRWPair_Type);
    DECLARE_DLSYM(PyObject *, PyThreadState_GetDict, void);
    DECLARE_DLSYM(PyObject *, PyWeakref_NewProxy, PyObject *ob,PyObject *callback);
    DECLARE_DLSYM(Py_ssize_t, PySlice_AdjustIndices, Py_ssize_t length,Py_ssize_t *start, Py_ssize_t *stop,Py_ssize_t step);
    DECLARE_DLSYM(PyObject *, PyModuleDef_Init, struct PyModuleDef*);
    DECLARE_DLSYM(PyObject *, PyModule_Create2, struct PyModuleDef*,int apiver);
    DECLARE_DLSYM(void, PyMem_RawFree, void *ptr);
    DECLARE_DLSYM(long, PyLong_AsLongAndOverflow, PyObject *, int *);
    DECLARE_DLSYM(PyThreadState *, PyThreadState_Next, PyThreadState *);
    DECLARE_DLSYM(Py_ssize_t, PyMapping_Size, PyObject *o);
    DECLARE_DLSYM_TYPE(PyTupleIter_Type);
    DECLARE_DLSYM(PyObject *, PyTuple_GetSlice, PyObject *, Py_ssize_t, Py_ssize_t);
    DECLARE_DLSYM_TYPE(PyDictRevIterItem_Type);
    DECLARE_DLSYM(PyObject *, PyCodec_IgnoreErrors, PyObject *exc);
    DECLARE_DLSYM(PyObject *, PyFile_GetLine, PyObject *, int);
    DECLARE_DLSYM(void, PyMem_GetAllocator, PyMemAllocatorDomain domain,PyMemAllocatorEx *allocator);
    DECLARE_DLSYM(void, PyErr_SetExcInfo, PyObject *, PyObject *, PyObject *);
    DECLARE_DLSYM_TYPE(PyFrame_Type);
    DECLARE_DLSYM(int, PyTraceBack_Here, PyFrameObject *);
    DECLARE_DLSYM(PyObject *, PyImport_AddModuleObject,PyObject *name);
    DECLARE_DLSYM(void, PySys_AddWarnOption, const wchar_t *);
    DECLARE_DLSYM_TYPE(PySlice_Type);
    DECLARE_DLSYM(PyObject *, PyCell_Get, PyObject *);
    DECLARE_DLSYM(int, PyGILState_Check, void);
    DECLARE_DLSYM(int, PyArg_UnpackTuple, PyObject *, const char *, Py_ssize_t, Py_ssize_t, ...);
    DECLARE_DLSYM(PyObject *, PyUnicodeEncodeError_GetReason, PyObject *);
    DECLARE_DLSYM(int, PyRun_InteractiveOne, FILE *f, const char *p);
    DECLARE_DLSYM(int, PyMapping_SetItemString, PyObject *o, const char *key,PyObject *value);
    DECLARE_DLSYM(PyObject *, Py_CompileStringObject,const char *str,PyObject *filename, int start,PyCompilerFlags *flags,int optimize);
    DECLARE_DLSYM(PyObject *, PyLong_FromUnsignedLongLong, unsigned long long);
    DECLARE_DLSYM(int, PyThread_tss_create, Py_tss_t *key);
    DECLARE_DLSYM(int, PyObject_AsFileDescriptor, PyObject *);
    DECLARE_DLSYM(int, PyImport_ImportFrozenModule,const char *name            /* UTF-8 encoded string */);
    DECLARE_DLSYM(PyObject *, PyException_GetTraceback, PyObject *);
    DECLARE_DLSYM(Py_ssize_t, PyObject_Size, PyObject *o);
    DECLARE_DLSYM(void, PyErr_Fetch, PyObject **, PyObject **, PyObject **);
    DECLARE_DLSYM_TYPE(PySeqIter_Type);
    DECLARE_DLSYM(PyObject *, PyModule_GetDict, PyObject *);
    DECLARE_DLSYM(int, PyArg_ParseTupleAndKeywords, PyObject *, PyObject *,const char *, char **, ...);
    DECLARE_DLSYM(int, PyDict_Merge, PyObject *mp,PyObject *other,int override);
    DECLARE_DLSYM(void *, PyMem_Malloc, size_t size);
    DECLARE_DLSYM(void, PyBytes_ConcatAndDel, PyObject **, PyObject *);
    DECLARE_DLSYM(PyObject *, PyByteArray_Concat, PyObject *, PyObject *);
    DECLARE_DLSYM(int, PyUnicode_WriteChar,PyObject *unicode,Py_ssize_t index,Py_UCS4 character);
    DECLARE_DLSYM(int, PyThread_tss_set, Py_tss_t *key, void *value);
    DECLARE_DLSYM_TYPE(PyIncrementalNewlineDecoder_Type);
    DECLARE_DLSYM(struct PyModuleDef*, PyModule_GetDef, PyObject*);
    DECLARE_DLSYM(int, PyTuple_SetItem, PyObject *, Py_ssize_t, PyObject *);
    DECLARE_DLSYM(int, PyRun_AnyFileEx, FILE *fp, const char *name, int closeit);
    DECLARE_DLSYM(PyStatus, Py_PreInitializeFromBytesArgs,const PyPreConfig *src_config,Py_ssize_t argc,char **argv);
    DECLARE_DLSYM(int, PyDict_SetItem, PyObject *mp, PyObject *key, PyObject *item);
    DECLARE_DLSYM(PyObject *, PyOS_FSPath, PyObject *path);
    DECLARE_DLSYM(const char *, Py_GetBuildInfo, void);
    DECLARE_DLSYM(PyObject *, PyIter_Next, PyObject *);
    DECLARE_DLSYM(int, PyObject_IsInstance, PyObject *object, PyObject *typeorclass);
    DECLARE_DLSYM(void *, PyMem_RawRealloc, void *ptr, size_t new_size);
    DECLARE_DLSYM(int, Py_IsInitialized, void);
    DECLARE_DLSYM(const char*, PyUnicode_GetDefaultEncoding, void);
    DECLARE_DLSYM_TYPE(PyAsyncGen_Type);
    DECLARE_DLSYM(void, Py_EndInterpreter, PyThreadState *);
    DECLARE_DLSYM(int, PyErr_CheckSignals, void);
    DECLARE_DLSYM(int, PyObject_HasAttrString, PyObject *, const char *);
    DECLARE_DLSYM(PyObject *, PyImport_GetImporter, PyObject *path);
    DECLARE_DLSYM(int, PyRun_SimpleFileEx, FILE *f, const char *p, int c);
    DECLARE_DLSYM(void, PyGILState_Release, PyGILState_STATE);

    static inline int _Py_IS_TYPE(const PyObject *ob, const PyTypeObject *type) {
        return ob->ob_type == type;
    }

    static inline int _PyObject_TypeCheck(PyObject *ob, PyTypeObject *type) {
        return Py_IS_TYPE(ob, type) || PyType_IsSubtype(Py_TYPE(ob), type);
    }

    static inline int PyType_HasFeature(PyTypeObject *type, unsigned long feature) {
        return ((type->tp_flags & feature) != 0);
    }

    static inline PyObject* _Py_NewRef(PyObject *obj) {
        Py_IncRef(obj);
        return obj;
    }

    void AddNativeModule(PyModuleDef& def);

}