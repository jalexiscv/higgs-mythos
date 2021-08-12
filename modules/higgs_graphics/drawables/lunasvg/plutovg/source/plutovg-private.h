#ifndef HIGGS_PLUTOVG_PRIVATE_H
#define HIGGS_PLUTOVG_PRIVATE_H

#include "../include/plutovg.h"

#if defined(_WIN32)

#include <windows.h>

typedef LONG higgs_plutovg_ref_count_t;

#define higgs_plutovg_init_reference(ob) ((ob)->ref_count = 1)
#define higgs_plutovg_increment_reference(ob) (void)(ob && InterlockedIncrement(&(ob)->ref_count))
#define higgs_plutovg_destroy_reference(ob) (ob && InterlockedDecrement(&(ob)->ref_count) == 0)
#define higgs_plutovg_get_reference_count(ob) ((ob) ? InterlockedCompareExchange((LONG*)&(ob)->ref_count, 0, 0) : 0)

#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_ATOMICS__)

#include <stdatomic.h>

typedef atomic_int higgs_plutovg_ref_count_t;

#define higgs_plutovg_init_reference(ob) atomic_init(&(ob)->ref_count, 1)
#define higgs_plutovg_increment_reference(ob) (void)(ob && atomic_fetch_add(&(ob)->ref_count, 1))
#define higgs_plutovg_destroy_reference(ob) (ob && atomic_fetch_sub(&(ob)->ref_count, 1) == 1)
#define higgs_plutovg_get_reference_count(ob) ((ob) ? atomic_load(&(ob)->ref_count) : 0)

#else

typedef int higgs_plutovg_ref_count_t;

#define higgs_plutovg_init_reference(ob) ((ob)->ref_count = 1)
#define higgs_plutovg_increment_reference(ob) (void)(ob && ++(ob)->ref_count)
#define higgs_plutovg_destroy_reference(ob) (ob && --(ob)->ref_count == 0)
#define higgs_plutovg_get_reference_count(ob) ((ob) ? (ob)->ref_count : 0)

#endif

struct higgs_plutovg_surface {
    higgs_plutovg_ref_count_t ref_count;
    int width;
    int height;
    int stride;
    unsigned char* data;
};

struct higgs_plutovg_path {
    higgs_plutovg_ref_count_t ref_count;
    int num_points;
    int num_contours;
    int num_curves;
    higgs_plutovg_point_t start_point;
    struct {
        higgs_plutovg_path_element_t* data;
        int size;
        int capacity;
    } elements;
};

typedef enum {
    HIGGS_PLUTOVG_PAINT_TYPE_COLOR,
    HIGGS_PLUTOVG_PAINT_TYPE_GRADIENT,
    HIGGS_PLUTOVG_PAINT_TYPE_TEXTURE
} higgs_plutovg_paint_type_t;

struct higgs_plutovg_paint {
    higgs_plutovg_ref_count_t ref_count;
    higgs_plutovg_paint_type_t type;
};

typedef struct {
    higgs_plutovg_paint_t base;
    higgs_plutovg_color_t color;
} higgs_plutovg_solid_paint_t;

typedef enum {
    HIGGS_PLUTOVG_GRADIENT_TYPE_LINEAR,
    HIGGS_PLUTOVG_GRADIENT_TYPE_RADIAL
} higgs_plutovg_gradient_type_t;

typedef struct {
    higgs_plutovg_paint_t base;
    higgs_plutovg_gradient_type_t type;
    higgs_plutovg_spread_method_t spread;
    higgs_plutovg_matrix_t matrix;
    higgs_plutovg_gradient_stop_t* stops;
    int nstops;
    float values[6];
} higgs_plutovg_gradient_paint_t;

typedef struct {
    higgs_plutovg_paint_t base;
    higgs_plutovg_texture_type_t type;
    float opacity;
    higgs_plutovg_matrix_t matrix;
    higgs_plutovg_surface_t* surface;
} higgs_plutovg_texture_paint_t;

typedef struct {
    int x;
    int len;
    int y;
    unsigned char coverage;
} higgs_plutovg_span_t;

typedef struct {
    struct {
        higgs_plutovg_span_t* data;
        int size;
        int capacity;
    } spans;

    int x;
    int y;
    int w;
    int h;
} higgs_plutovg_span_buffer_t;

typedef struct {
    float offset;
    struct {
        float* data;
        int size;
        int capacity;
    } array;
} higgs_plutovg_stroke_dash_t;

typedef struct {
    float width;
    higgs_plutovg_line_cap_t cap;
    higgs_plutovg_line_join_t join;
    float miter_limit;
} higgs_plutovg_stroke_style_t;

typedef struct {
    higgs_plutovg_stroke_style_t style;
    higgs_plutovg_stroke_dash_t dash;
} higgs_plutovg_stroke_data_t;

typedef struct higgs_plutovg_state {
    higgs_plutovg_paint_t* paint;
    higgs_plutovg_font_face_t* font_face;
    higgs_plutovg_color_t color;
    higgs_plutovg_matrix_t matrix;
    higgs_plutovg_stroke_data_t stroke;
    higgs_plutovg_span_buffer_t clip_spans;
    higgs_plutovg_fill_rule_t winding;
    higgs_plutovg_operator_t op;
    float font_size;
    float opacity;
    bool clipping;
    struct higgs_plutovg_state* next;
} higgs_plutovg_state_t;

struct higgs_plutovg_canvas {
    higgs_plutovg_ref_count_t ref_count;
    higgs_plutovg_surface_t* surface;
    higgs_plutovg_path_t* path;
    higgs_plutovg_state_t* state;
    higgs_plutovg_state_t* freed_state;
    higgs_plutovg_font_face_cache_t* face_cache;
    higgs_plutovg_rect_t clip_rect;
    higgs_plutovg_span_buffer_t clip_spans;
    higgs_plutovg_span_buffer_t fill_spans;
};

void higgs_plutovg_span_buffer_init(higgs_plutovg_span_buffer_t* span_buffer);
void higgs_plutovg_span_buffer_init_rect(higgs_plutovg_span_buffer_t* span_buffer, int x, int y, int width, int height);
void higgs_plutovg_span_buffer_reset(higgs_plutovg_span_buffer_t* span_buffer);
void higgs_plutovg_span_buffer_destroy(higgs_plutovg_span_buffer_t* span_buffer);
void higgs_plutovg_span_buffer_copy(higgs_plutovg_span_buffer_t* span_buffer, const higgs_plutovg_span_buffer_t* source);
bool higgs_plutovg_span_buffer_contains(const higgs_plutovg_span_buffer_t* span_buffer, float x, float y);
void higgs_plutovg_span_buffer_extents(higgs_plutovg_span_buffer_t* span_buffer, higgs_plutovg_rect_t* extents);
void higgs_plutovg_span_buffer_intersect(higgs_plutovg_span_buffer_t* span_buffer, const higgs_plutovg_span_buffer_t* a, const higgs_plutovg_span_buffer_t* b);

void higgs_plutovg_rasterize(higgs_plutovg_span_buffer_t* span_buffer, const higgs_plutovg_path_t* path, const higgs_plutovg_matrix_t* matrix, const higgs_plutovg_rect_t* clip_rect, const higgs_plutovg_stroke_data_t* stroke_data, higgs_plutovg_fill_rule_t winding);
void higgs_plutovg_blend(higgs_plutovg_canvas_t* canvas, const higgs_plutovg_span_buffer_t* span_buffer);
void higgs_plutovg_memfill32(unsigned int* dest, int length, unsigned int value);

#endif // HIGGS_PLUTOVG_PRIVATE_H
