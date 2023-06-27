/*
scoped_manifest.h

As scoped<> variables provide an alternative to explicit argument passing, it is easier to write
badly designed interfaces where scoped<> variables function as hidden knobs.

To solve this issue the following methodology is suggested:
(1) A module owners should explicitly advertise the "scoped interface" to be used with specific functions, classes, or class methods.
(2) This is done by defining a struct containing the scoped definitions, and attaching it to a function, class, or method using
    the macros below.
(3) A module users should only use scoped definitions that are associated with the functions/classes/methods they are using.

Example:

// Header, declaring a manifest for the "get_number" function
struct get_number_manifest {
    using Thresh = scoped::scoped<int, struct ThresholdTag>;
};

int get_number(int x);

SCOPED_DECLARE_FUNCTION_MANIFEST(get_number, get_number_manifest);

// Body, using the manifest
int get_number(int x) {
    if (auto thresh = SCOPED_GET_FUNCTION_MANIFEST(get_number)::Thresh::top()) {
        ...
    }
}

// User of get_number, also fetching the associated manifest
int foo() {
    SCOPED_GET_FUNCTION_MANIFEST(get_number)::Thresh scoped_threshold(4);
    ...
    std::cout << get_number(10);
    ...
}

Author: Eran Talmor 2023, the.eran.talmor@gmail.com
*/

template <typename T> using scoped_manifest_of = decltype(get_scoped_manifest_traits((T*)nullptr));
template <typename F, F f> struct scoped_function_type {};

#define SCOPED_FUNCTION_TYPE(f) scoped_function_type<decltype(&f), &f>

#define SCOPED_DECLARE_CLASS_MANIFEST(Class, Manifest) Manifest get_scoped_manifest_traits(Class *);
#define SCOPED_DECLARE_FUNCTION_MANIFEST(Func, Manifest) Manifest get_scoped_manifest_traits(SCOPED_FUNCTION_TYPE(Func)*);

#define SCOPED_GET_CLASS_MANIFEST(Class) scoped_manifest_of<Class>
#define SCOPED_GET_OBJECT_MANIFEST(Obj) scoped_manifest_of<decltype(Obj)>
#define SCOPED_GET_FUNCTION_MANIFEST(Func) scoped_manifest_of<SCOPED_FUNCTION_TYPE(Func)>
