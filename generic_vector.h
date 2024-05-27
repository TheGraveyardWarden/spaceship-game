#ifndef _GENERIC_VECTOR_H
#define _GENERIC_VECTOR_H

typedef char i8_t;
typedef unsigned char u8_t;

typedef char bool;

#define c(ptr, type, member) (type*)((char*)ptr - ((size_t)(&((type*)0)->member)))

#define struct_vec_item(item)   \
struct item##_vec_item          \
{                               \
    struct item item;           \
    bool empty;                 \
}

#define vec(item, name, max_size)       \
struct item##_vec_item name[max_size]

#define vec_of_ptr(item, name, max_size)        \
struct item##_vec_item* name[max_size]

#define vec_for_each(item, name, tmp, i, max_size)              \
for (i = 0; i < max_size && ({ tmp = name[i].empty ? 0 : &name[i].item; 1; }); i++)

#define DEFINE_VEC_METHODS(item)                                                                                                                                                              \
void item##_vec_init(struct item##_vec_item* items, u8_t size)  \
{\
    u8_t i; \
    \
    for (i = 0; i < size; i++)  \
        items[i].empty = 1; \
};\
\
bool item##_vec_is_empty(struct item##_vec_item* items, u8_t size) \
{\
    u8_t i;\
    \
    for (i = 0; i < size; i++)\
        if (!(items[i].empty)) return 0;\
    \
    return 1;\
};\
\
bool item##_vec_is_full(struct item##_vec_item* items, u8_t size) \
{\
    u8_t i;\
    \
    for (i = 0; i < size; i++)\
        if (items[i].empty) return 0;\
    \
    return 1;\
};\
\
bool item##_vec_add(struct item##_vec_item* items, struct item* new_item, u8_t size) \
{\
    u8_t i;\
    struct item##_vec_item* tmp;\
    \
    tmp = 0;\
    \
    for (i = 0; i < size; i++)      \
    {\
        if (items[i].empty) {\
            tmp = &items[i];\
            break;\
        }\
    }\
    if (!tmp) return 1;\
    tmp->item = *new_item;\
    tmp->empty = 0;\
    return 0;\
};\
\
struct item* item##_vec_get_item(struct item##_vec_item* items, u8_t size)  \
{\
    struct item##_vec_item* tmp;\
    u8_t i;\
    \
    tmp = 0;\
    for (i = 0; i < size; i++)\
    {\
        if (items[i].empty)\
        {\
            tmp = &items[i];\
            break;\
        }\
    }\
    \
    if (tmp)\
    {\
        tmp->empty = 0;\
        return &tmp->item;\
    }\
    return 0;\
};\
\
void item##_vec_remove_idx(struct item##_vec_item* items, u8_t idx)     \
{\
    items[idx].empty = 1;\
};\
\
void item##_vec_remove(struct item##_vec_item* items, struct item* r)   \
{\
    struct item##_vec_item* tmp = c(r, struct item##_vec_item, item);\
    tmp->empty = 1;\
}

#endif
