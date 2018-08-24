#ifndef RED_EYE_DEBUG_H
#define RED_EYE_DEBUG_H

#define debug_print(...) \
          fprintf(stderr, "%s:%d:%s(): ", __FILE__, __LINE__, __func__); \
          fprintf(stderr, __VA_ARGS__);

#endif
