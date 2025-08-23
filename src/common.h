#ifndef __COMMON_H__
#define __COMMON_H__

// define offsetof macro
#define offsetof(type, member) __builtin_offsetof(type, member)

/* container_of() - Calculate address of object that contains address ptr
 * @ptr: pointer to member variable
 * @type: type of the structure containing ptr
 * @member: name of the member variable in struct @type
 *
 * Return: @type pointer of object containing ptr
 */
#define container_of(ptr, type, member)                                        \
    __extension__({                                                            \
        const __typeof__(((type *)0)->member) *(__pmember) = (ptr);            \
        (type *)((char *)__pmember - offsetof(type, member));                  \
    })

#endif
