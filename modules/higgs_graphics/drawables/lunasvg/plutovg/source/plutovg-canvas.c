#include "plutovg-private.h"
#include "plutovg-utils.h"

int higgs_plutovg_version(void)
{
    return HIGGS_PLUTOVG_VERSION;
}

const char* higgs_plutovg_version_string(void)
{
    return HIGGS_PLUTOVG_VERSION_STRING;
}

#define HIGGS_PLUTOVG_DEFAULT_STROKE_STYLE ((higgs_plutovg_stroke_style_t){1.f, HIGGS_PLUTOVG_LINE_CAP_BUTT, HIGGS_PLUTOVG_LINE_JOIN_MITER, 10.f})

static higgs_plutovg_state_t* higgs_plutovg_state_create(void)
{
    higgs_plutovg_state_t* state = malloc(sizeof(higgs_plutovg_state_t));
    state->paint = NULL;
    state->font_face = NULL;
    state->color = HIGGS_PLUTOVG_BLACK_COLOR;
    state->matrix = HIGGS_PLUTOVG_IDENTITY_MATRIX;
    state->stroke.style = HIGGS_PLUTOVG_DEFAULT_STROKE_STYLE;
    state->stroke.dash.offset = 0.f;
    higgs_plutovg_array_init(state->stroke.dash.array);
    higgs_plutovg_span_buffer_init(&state->clip_spans);
    state->winding = HIGGS_PLUTOVG_FILL_RULE_NON_ZERO;
    state->op = HIGGS_PLUTOVG_OPERATOR_SRC_OVER;
    state->font_size = 12.f;
    state->opacity = 1.f;
    state->clipping = false;
    state->next = NULL;
    return state;
}

static void higgs_plutovg_state_reset(higgs_plutovg_state_t* state)
{
    higgs_plutovg_paint_destroy(state->paint);
    higgs_plutovg_font_face_destroy(state->font_face);
    state->paint = NULL;
    state->font_face = NULL;
    state->color = HIGGS_PLUTOVG_BLACK_COLOR;
    state->matrix = HIGGS_PLUTOVG_IDENTITY_MATRIX;
    state->stroke.style = HIGGS_PLUTOVG_DEFAULT_STROKE_STYLE;
    state->stroke.dash.offset = 0.f;
    higgs_plutovg_array_clear(state->stroke.dash.array);
    higgs_plutovg_span_buffer_reset(&state->clip_spans);
    state->winding = HIGGS_PLUTOVG_FILL_RULE_NON_ZERO;
    state->op = HIGGS_PLUTOVG_OPERATOR_SRC_OVER;
    state->font_size = 12.f;
    state->opacity = 1.f;
    state->clipping = false;
}

static void higgs_plutovg_state_copy(higgs_plutovg_state_t* state, const higgs_plutovg_state_t* source)
{
    state->paint = higgs_plutovg_paint_reference(source->paint);
    state->font_face = higgs_plutovg_font_face_reference(source->font_face);
    state->color = source->color;
    state->matrix = source->matrix;
    state->stroke.style = source->stroke.style;
    state->stroke.dash.offset = source->stroke.dash.offset;
    higgs_plutovg_array_clear(state->stroke.dash.array);
    higgs_plutovg_array_append(state->stroke.dash.array, source->stroke.dash.array);
    higgs_plutovg_span_buffer_copy(&state->clip_spans, &source->clip_spans);
    state->winding = source->winding;
    state->op = source->op;
    state->font_size = source->font_size;
    state->opacity = source->opacity;
    state->clipping = source->clipping;
}

static void higgs_plutovg_state_destroy(higgs_plutovg_state_t* state)
{
    higgs_plutovg_paint_destroy(state->paint);
    higgs_plutovg_font_face_destroy(state->font_face);
    higgs_plutovg_array_destroy(state->stroke.dash.array);
    higgs_plutovg_span_buffer_destroy(&state->clip_spans);
    free(state);
}

higgs_plutovg_canvas_t* higgs_plutovg_canvas_create(higgs_plutovg_surface_t* surface)
{
    higgs_plutovg_canvas_t* canvas = malloc(sizeof(higgs_plutovg_canvas_t));
    higgs_plutovg_init_reference(canvas);
    canvas->surface = higgs_plutovg_surface_reference(surface);
    canvas->path = higgs_plutovg_path_create();
    canvas->state = higgs_plutovg_state_create();
    canvas->freed_state = NULL;
    canvas->face_cache = NULL;
    canvas->clip_rect = HIGGS_PLUTOVG_MAKE_RECT(0, 0, surface->width, surface->height);
    higgs_plutovg_span_buffer_init(&canvas->clip_spans);
    higgs_plutovg_span_buffer_init(&canvas->fill_spans);
    return canvas;
}

higgs_plutovg_canvas_t* higgs_plutovg_canvas_reference(higgs_plutovg_canvas_t* canvas)
{
    higgs_plutovg_increment_reference(canvas);
    return canvas;
}

void higgs_plutovg_canvas_destroy(higgs_plutovg_canvas_t* canvas)
{
    if(higgs_plutovg_destroy_reference(canvas)) {
        while(canvas->state) {
            higgs_plutovg_state_t* state = canvas->state;
            canvas->state = state->next;
            higgs_plutovg_state_destroy(state);
        }

        while(canvas->freed_state) {
            higgs_plutovg_state_t* state = canvas->freed_state;
            canvas->freed_state = state->next;
            higgs_plutovg_state_destroy(state);
        }

        higgs_plutovg_font_face_cache_destroy(canvas->face_cache);
        higgs_plutovg_span_buffer_destroy(&canvas->fill_spans);
        higgs_plutovg_span_buffer_destroy(&canvas->clip_spans);
        higgs_plutovg_surface_destroy(canvas->surface);
        higgs_plutovg_path_destroy(canvas->path);
        free(canvas);
    }
}

int higgs_plutovg_canvas_get_reference_count(const higgs_plutovg_canvas_t* canvas)
{
    return higgs_plutovg_get_reference_count(canvas);
}

higgs_plutovg_surface_t* higgs_plutovg_canvas_get_surface(const higgs_plutovg_canvas_t* canvas)
{
    return canvas->surface;
}

void higgs_plutovg_canvas_save(higgs_plutovg_canvas_t* canvas)
{
    higgs_plutovg_state_t* new_state = canvas->freed_state;
    if(new_state == NULL)
        new_state = higgs_plutovg_state_create();
    else
        canvas->freed_state = new_state->next;
    higgs_plutovg_state_copy(new_state, canvas->state);
    new_state->next = canvas->state;
    canvas->state = new_state;
}

void higgs_plutovg_canvas_restore(higgs_plutovg_canvas_t* canvas)
{
    if(canvas->state->next == NULL)
        return;
    higgs_plutovg_state_t* old_state = canvas->state;
    canvas->state = old_state->next;
    higgs_plutovg_state_reset(old_state);
    old_state->next = canvas->freed_state;
    canvas->freed_state = old_state;
}

void higgs_plutovg_canvas_set_rgb(higgs_plutovg_canvas_t* canvas, float r, float g, float b)
{
    higgs_plutovg_canvas_set_rgba(canvas, r, g, b, 1.f);
}

void higgs_plutovg_canvas_set_rgba(higgs_plutovg_canvas_t* canvas, float r, float g, float b, float a)
{
    higgs_plutovg_color_init_rgba(&canvas->state->color, r, g, b, a);
    higgs_plutovg_canvas_set_paint(canvas, NULL);
}

void higgs_plutovg_canvas_set_color(higgs_plutovg_canvas_t* canvas, const higgs_plutovg_color_t* color)
{
    higgs_plutovg_canvas_set_rgba(canvas, color->r, color->g, color->b, color->a);
}

void higgs_plutovg_canvas_set_linear_gradient(higgs_plutovg_canvas_t* canvas, float x1, float y1, float x2, float y2, higgs_plutovg_spread_method_t spread, const higgs_plutovg_gradient_stop_t* stops, int nstops, const higgs_plutovg_matrix_t* matrix)
{
    higgs_plutovg_paint_t* paint = higgs_plutovg_paint_create_linear_gradient(x1, y1, x2, y2, spread, stops, nstops, matrix);
    higgs_plutovg_canvas_set_paint(canvas, paint);
    higgs_plutovg_paint_destroy(paint);
}

void higgs_plutovg_canvas_set_radial_gradient(higgs_plutovg_canvas_t* canvas, float cx, float cy, float cr, float fx, float fy, float fr, higgs_plutovg_spread_method_t spread, const higgs_plutovg_gradient_stop_t* stops, int nstops, const higgs_plutovg_matrix_t* matrix)
{
    higgs_plutovg_paint_t* paint = higgs_plutovg_paint_create_radial_gradient(cx, cy, cr, fx, fy, fr, spread, stops, nstops, matrix);
    higgs_plutovg_canvas_set_paint(canvas, paint);
    higgs_plutovg_paint_destroy(paint);
}

void higgs_plutovg_canvas_set_texture(higgs_plutovg_canvas_t* canvas, higgs_plutovg_surface_t* surface, higgs_plutovg_texture_type_t type, float opacity, const higgs_plutovg_matrix_t* matrix)
{
    higgs_plutovg_paint_t* paint = higgs_plutovg_paint_create_texture(surface, type, opacity, matrix);
    higgs_plutovg_canvas_set_paint(canvas, paint);
    higgs_plutovg_paint_destroy(paint);
}

void higgs_plutovg_canvas_set_paint(higgs_plutovg_canvas_t* canvas, higgs_plutovg_paint_t* paint)
{
    paint = higgs_plutovg_paint_reference(paint);
    higgs_plutovg_paint_destroy(canvas->state->paint);
    canvas->state->paint = paint;
}

higgs_plutovg_paint_t* higgs_plutovg_canvas_get_paint(const higgs_plutovg_canvas_t* canvas, higgs_plutovg_color_t* color)
{
    if(color)
        *color = canvas->state->color;
    return canvas->state->paint;
}

void higgs_plutovg_canvas_set_font_face_cache(higgs_plutovg_canvas_t* canvas, higgs_plutovg_font_face_cache_t* cache)
{
    cache = higgs_plutovg_font_face_cache_reference(cache);
    higgs_plutovg_font_face_cache_destroy(canvas->face_cache);
    canvas->face_cache = cache;
}

higgs_plutovg_font_face_cache_t* higgs_plutovg_canvas_get_font_face_cache(const higgs_plutovg_canvas_t* canvas)
{
    return canvas->face_cache;
}

void higgs_plutovg_canvas_add_font_face(higgs_plutovg_canvas_t* canvas, const char* family, bool bold, bool italic, higgs_plutovg_font_face_t* face)
{
    if(canvas->face_cache == NULL)
        canvas->face_cache = higgs_plutovg_font_face_cache_create();
    higgs_plutovg_font_face_cache_add(canvas->face_cache, family, bold, italic, face);
}

bool higgs_plutovg_canvas_add_font_file(higgs_plutovg_canvas_t* canvas, const char* family, bool bold, bool italic, const char* filename, int ttcindex)
{
    if(canvas->face_cache == NULL)
        canvas->face_cache = higgs_plutovg_font_face_cache_create();
    return higgs_plutovg_font_face_cache_add_file(canvas->face_cache, family, bold, italic, filename, ttcindex);
}

bool higgs_plutovg_canvas_select_font_face(higgs_plutovg_canvas_t* canvas, const char* family, bool bold, bool italic)
{
    if(canvas->face_cache == NULL)
        return false;
    higgs_plutovg_font_face_t* face = higgs_plutovg_font_face_cache_get(canvas->face_cache, family, bold, italic);
    if(face == NULL)
        return false;
    higgs_plutovg_canvas_set_font_face(canvas, face);
    return true;
}

void higgs_plutovg_canvas_set_font(higgs_plutovg_canvas_t* canvas, higgs_plutovg_font_face_t* face, float size)
{
    higgs_plutovg_canvas_set_font_face(canvas, face);
    higgs_plutovg_canvas_set_font_size(canvas, size);
}

void higgs_plutovg_canvas_set_font_face(higgs_plutovg_canvas_t* canvas, higgs_plutovg_font_face_t* face)
{
    face = higgs_plutovg_font_face_reference(face);
    higgs_plutovg_font_face_destroy(canvas->state->font_face);
    canvas->state->font_face = face;
}

higgs_plutovg_font_face_t* higgs_plutovg_canvas_get_font_face(const higgs_plutovg_canvas_t* canvas)
{
    return canvas->state->font_face;
}

void higgs_plutovg_canvas_set_font_size(higgs_plutovg_canvas_t* canvas, float size)
{
    canvas->state->font_size = size;
}

float higgs_plutovg_canvas_get_font_size(const higgs_plutovg_canvas_t* canvas)
{
    return canvas->state->font_size;
}

void higgs_plutovg_canvas_set_fill_rule(higgs_plutovg_canvas_t* canvas, higgs_plutovg_fill_rule_t winding)
{
    canvas->state->winding = winding;
}

higgs_plutovg_fill_rule_t higgs_plutovg_canvas_get_fill_rule(const higgs_plutovg_canvas_t* canvas)
{
    return canvas->state->winding;
}

void higgs_plutovg_canvas_set_operator(higgs_plutovg_canvas_t* canvas, higgs_plutovg_operator_t op)
{
    canvas->state->op = op;
}

higgs_plutovg_operator_t higgs_plutovg_canvas_get_operator(const higgs_plutovg_canvas_t* canvas)
{
    return canvas->state->op;
}

void higgs_plutovg_canvas_set_opacity(higgs_plutovg_canvas_t* canvas, float opacity)
{
    canvas->state->opacity = higgs_plutovg_clamp(opacity, 0.f, 1.f);
}

float higgs_plutovg_canvas_get_opacity(const higgs_plutovg_canvas_t* canvas)
{
    return canvas->state->opacity;
}

void higgs_plutovg_canvas_set_line_width(higgs_plutovg_canvas_t* canvas, float line_width)
{
    canvas->state->stroke.style.width = line_width;
}

float higgs_plutovg_canvas_get_line_width(const higgs_plutovg_canvas_t* canvas)
{
    return canvas->state->stroke.style.width;
}

void higgs_plutovg_canvas_set_line_cap(higgs_plutovg_canvas_t* canvas, higgs_plutovg_line_cap_t line_cap)
{
    canvas->state->stroke.style.cap = line_cap;
}

higgs_plutovg_line_cap_t higgs_plutovg_canvas_get_line_cap(const higgs_plutovg_canvas_t* canvas)
{
    return canvas->state->stroke.style.cap;
}

void higgs_plutovg_canvas_set_line_join(higgs_plutovg_canvas_t* canvas, higgs_plutovg_line_join_t line_join)
{
    canvas->state->stroke.style.join = line_join;
}

higgs_plutovg_line_join_t higgs_plutovg_canvas_get_line_join(const higgs_plutovg_canvas_t* canvas)
{
    return canvas->state->stroke.style.join;
}

void higgs_plutovg_canvas_set_miter_limit(higgs_plutovg_canvas_t* canvas, float miter_limit)
{
    canvas->state->stroke.style.miter_limit = miter_limit;
}

float higgs_plutovg_canvas_get_miter_limit(const higgs_plutovg_canvas_t* canvas)
{
    return canvas->state->stroke.style.miter_limit;
}

void higgs_plutovg_canvas_set_dash(higgs_plutovg_canvas_t* canvas, float offset, const float* dashes, int ndashes)
{
    higgs_plutovg_canvas_set_dash_offset(canvas, offset);
    higgs_plutovg_canvas_set_dash_array(canvas, dashes, ndashes);
}

void higgs_plutovg_canvas_set_dash_offset(higgs_plutovg_canvas_t* canvas, float offset)
{
    canvas->state->stroke.dash.offset = offset;
}

float higgs_plutovg_canvas_get_dash_offset(const higgs_plutovg_canvas_t* canvas)
{
    return canvas->state->stroke.dash.offset;
}

void higgs_plutovg_canvas_set_dash_array(higgs_plutovg_canvas_t* canvas, const float* dashes, int ndashes)
{
    higgs_plutovg_array_clear(canvas->state->stroke.dash.array);
    higgs_plutovg_array_append_data(canvas->state->stroke.dash.array, dashes, ndashes);
}

int higgs_plutovg_canvas_get_dash_array(const higgs_plutovg_canvas_t* canvas, const float** dashes)
{
    if(dashes)
        *dashes = canvas->state->stroke.dash.array.data;
    return canvas->state->stroke.dash.array.size;
}

void higgs_plutovg_canvas_translate(higgs_plutovg_canvas_t* canvas, float tx, float ty)
{
    higgs_plutovg_matrix_translate(&canvas->state->matrix, tx, ty);
}

void higgs_plutovg_canvas_scale(higgs_plutovg_canvas_t* canvas, float sx, float sy)
{
    higgs_plutovg_matrix_scale(&canvas->state->matrix, sx, sy);
}

void higgs_plutovg_canvas_shear(higgs_plutovg_canvas_t* canvas, float shx, float shy)
{
    higgs_plutovg_matrix_shear(&canvas->state->matrix, shx, shy);
}

void higgs_plutovg_canvas_rotate(higgs_plutovg_canvas_t* canvas, float angle)
{
    higgs_plutovg_matrix_rotate(&canvas->state->matrix, angle);
}

void higgs_plutovg_canvas_transform(higgs_plutovg_canvas_t* canvas, const higgs_plutovg_matrix_t* matrix)
{
    higgs_plutovg_matrix_multiply(&canvas->state->matrix, matrix, &canvas->state->matrix);
}

void higgs_plutovg_canvas_reset_matrix(higgs_plutovg_canvas_t* canvas)
{
    higgs_plutovg_matrix_init_identity(&canvas->state->matrix);
}

void higgs_plutovg_canvas_set_matrix(higgs_plutovg_canvas_t* canvas, const higgs_plutovg_matrix_t* matrix)
{
    canvas->state->matrix = matrix ? *matrix : HIGGS_PLUTOVG_IDENTITY_MATRIX;
}

void higgs_plutovg_canvas_get_matrix(const higgs_plutovg_canvas_t* canvas, higgs_plutovg_matrix_t* matrix)
{
    *matrix = canvas->state->matrix;
}

void higgs_plutovg_canvas_map(const higgs_plutovg_canvas_t* canvas, float x, float y, float* xx, float* yy)
{
    higgs_plutovg_matrix_map(&canvas->state->matrix, x, y, xx, yy);
}

void higgs_plutovg_canvas_map_point(const higgs_plutovg_canvas_t* canvas, const higgs_plutovg_point_t* src, higgs_plutovg_point_t* dst)
{
    higgs_plutovg_matrix_map_point(&canvas->state->matrix, src, dst);
}

void higgs_plutovg_canvas_map_rect(const higgs_plutovg_canvas_t* canvas, const higgs_plutovg_rect_t* src, higgs_plutovg_rect_t* dst)
{
    higgs_plutovg_matrix_map_rect(&canvas->state->matrix, src, dst);
}

void higgs_plutovg_canvas_move_to(higgs_plutovg_canvas_t* canvas, float x, float y)
{
    higgs_plutovg_path_move_to(canvas->path, x, y);
}

void higgs_plutovg_canvas_line_to(higgs_plutovg_canvas_t* canvas, float x, float y)
{
    higgs_plutovg_path_line_to(canvas->path, x, y);
}

void higgs_plutovg_canvas_quad_to(higgs_plutovg_canvas_t* canvas, float x1, float y1, float x2, float y2)
{
    higgs_plutovg_path_quad_to(canvas->path, x1, y1, x2, y2);
}

void higgs_plutovg_canvas_cubic_to(higgs_plutovg_canvas_t* canvas, float x1, float y1, float x2, float y2, float x3, float y3)
{
    higgs_plutovg_path_cubic_to(canvas->path, x1, y1, x2, y2, x3, y3);
}

void higgs_plutovg_canvas_arc_to(higgs_plutovg_canvas_t* canvas, float rx, float ry, float angle, bool large_arc_flag, bool sweep_flag, float x, float y)
{
    higgs_plutovg_path_arc_to(canvas->path, rx, ry, angle, large_arc_flag, sweep_flag, x, y);
}

void higgs_plutovg_canvas_rect(higgs_plutovg_canvas_t* canvas, float x, float y, float w, float h)
{
    higgs_plutovg_path_add_rect(canvas->path, x, y, w, h);
}

void higgs_plutovg_canvas_round_rect(higgs_plutovg_canvas_t* canvas, float x, float y, float w, float h, float rx, float ry)
{
    higgs_plutovg_path_add_round_rect(canvas->path, x, y, w, h, rx, ry);
}

void higgs_plutovg_canvas_ellipse(higgs_plutovg_canvas_t* canvas, float cx, float cy, float rx, float ry)
{
    higgs_plutovg_path_add_ellipse(canvas->path, cx, cy, rx, ry);
}

void higgs_plutovg_canvas_circle(higgs_plutovg_canvas_t* canvas, float cx, float cy, float r)
{
    higgs_plutovg_path_add_circle(canvas->path, cx, cy, r);
}

void higgs_plutovg_canvas_arc(higgs_plutovg_canvas_t* canvas, float cx, float cy, float r, float a0, float a1, bool ccw)
{
    higgs_plutovg_path_add_arc(canvas->path, cx, cy, r, a0, a1, ccw);
}

void higgs_plutovg_canvas_add_path(higgs_plutovg_canvas_t* canvas, const higgs_plutovg_path_t* path)
{
    higgs_plutovg_path_add_path(canvas->path, path, NULL);
}

void higgs_plutovg_canvas_new_path(higgs_plutovg_canvas_t* canvas)
{
    higgs_plutovg_path_reset(canvas->path);
}

void higgs_plutovg_canvas_close_path(higgs_plutovg_canvas_t* canvas)
{
    higgs_plutovg_path_close(canvas->path);
}

void higgs_plutovg_canvas_get_current_point(const higgs_plutovg_canvas_t* canvas, float* x, float* y)
{
    higgs_plutovg_path_get_current_point(canvas->path, x, y);
}

higgs_plutovg_path_t* higgs_plutovg_canvas_get_path(const higgs_plutovg_canvas_t* canvas)
{
    return canvas->path;
}

bool higgs_plutovg_canvas_fill_contains(higgs_plutovg_canvas_t* canvas, float x, float y)
{
    higgs_plutovg_rasterize(&canvas->fill_spans, canvas->path, &canvas->state->matrix, NULL, NULL, canvas->state->winding);
    return higgs_plutovg_span_buffer_contains(&canvas->fill_spans, x, y);
}

bool higgs_plutovg_canvas_stroke_contains(higgs_plutovg_canvas_t* canvas, float x, float y)
{
    higgs_plutovg_rasterize(&canvas->fill_spans, canvas->path, &canvas->state->matrix, NULL, NULL, canvas->state->winding);
    return higgs_plutovg_span_buffer_contains(&canvas->fill_spans, x, y);
}

bool higgs_plutovg_canvas_clip_contains(higgs_plutovg_canvas_t* canvas, float x, float y)
{
    if(canvas->state->clipping) {
        return higgs_plutovg_span_buffer_contains(&canvas->state->clip_spans, x, y);
    }

    float l = canvas->clip_rect.x;
    float t = canvas->clip_rect.y;
    float r = canvas->clip_rect.x + canvas->clip_rect.w;
    float b = canvas->clip_rect.y + canvas->clip_rect.h;

    return x >= l && x <= r && y >= t && y <= b;
}

void higgs_plutovg_canvas_fill_extents(higgs_plutovg_canvas_t *canvas, higgs_plutovg_rect_t* extents)
{
    higgs_plutovg_rasterize(&canvas->fill_spans, canvas->path, &canvas->state->matrix, NULL, NULL, canvas->state->winding);
    higgs_plutovg_span_buffer_extents(&canvas->fill_spans, extents);
}

void higgs_plutovg_canvas_stroke_extents(higgs_plutovg_canvas_t *canvas, higgs_plutovg_rect_t* extents)
{
    higgs_plutovg_rasterize(&canvas->fill_spans, canvas->path, &canvas->state->matrix, NULL, &canvas->state->stroke, HIGGS_PLUTOVG_FILL_RULE_NON_ZERO);
    higgs_plutovg_span_buffer_extents(&canvas->fill_spans, extents);
}

void higgs_plutovg_canvas_clip_extents(higgs_plutovg_canvas_t* canvas, higgs_plutovg_rect_t* extents)
{
    if(canvas->state->clipping) {
        higgs_plutovg_span_buffer_extents(&canvas->state->clip_spans, extents);
    } else {
        extents->x = canvas->clip_rect.x;
        extents->y = canvas->clip_rect.y;
        extents->w = canvas->clip_rect.w;
        extents->h = canvas->clip_rect.h;
    }
}

void higgs_plutovg_canvas_fill(higgs_plutovg_canvas_t* canvas)
{
    higgs_plutovg_canvas_fill_preserve(canvas);
    higgs_plutovg_canvas_new_path(canvas);
}

void higgs_plutovg_canvas_stroke(higgs_plutovg_canvas_t* canvas)
{
    higgs_plutovg_canvas_stroke_preserve(canvas);
    higgs_plutovg_canvas_new_path(canvas);
}

void higgs_plutovg_canvas_clip(higgs_plutovg_canvas_t* canvas)
{
    higgs_plutovg_canvas_clip_preserve(canvas);
    higgs_plutovg_canvas_new_path(canvas);
}

void higgs_plutovg_canvas_paint(higgs_plutovg_canvas_t* canvas)
{
    if(canvas->state->clipping) {
        higgs_plutovg_blend(canvas, &canvas->state->clip_spans);
    } else {
        higgs_plutovg_span_buffer_init_rect(&canvas->clip_spans, 0, 0, canvas->surface->width, canvas->surface->height);
        higgs_plutovg_blend(canvas, &canvas->clip_spans);
    }
}

void higgs_plutovg_canvas_fill_preserve(higgs_plutovg_canvas_t* canvas)
{
    higgs_plutovg_rasterize(&canvas->fill_spans, canvas->path, &canvas->state->matrix, &canvas->clip_rect, NULL, canvas->state->winding);
    if(canvas->state->clipping) {
        higgs_plutovg_span_buffer_intersect(&canvas->clip_spans, &canvas->fill_spans, &canvas->state->clip_spans);
        higgs_plutovg_blend(canvas, &canvas->clip_spans);
    } else {
        higgs_plutovg_blend(canvas, &canvas->fill_spans);
    }
}

void higgs_plutovg_canvas_stroke_preserve(higgs_plutovg_canvas_t* canvas)
{
    higgs_plutovg_rasterize(&canvas->fill_spans, canvas->path, &canvas->state->matrix, &canvas->clip_rect, &canvas->state->stroke, HIGGS_PLUTOVG_FILL_RULE_NON_ZERO);
    if(canvas->state->clipping) {
        higgs_plutovg_span_buffer_intersect(&canvas->clip_spans, &canvas->fill_spans, &canvas->state->clip_spans);
        higgs_plutovg_blend(canvas, &canvas->clip_spans);
    } else {
        higgs_plutovg_blend(canvas, &canvas->fill_spans);
    }
}

void higgs_plutovg_canvas_clip_preserve(higgs_plutovg_canvas_t* canvas)
{
    if(canvas->state->clipping) {
        higgs_plutovg_rasterize(&canvas->fill_spans, canvas->path, &canvas->state->matrix, &canvas->clip_rect, NULL, canvas->state->winding);
        higgs_plutovg_span_buffer_intersect(&canvas->clip_spans, &canvas->fill_spans, &canvas->state->clip_spans);
        higgs_plutovg_span_buffer_copy(&canvas->state->clip_spans, &canvas->clip_spans);
    } else {
        higgs_plutovg_rasterize(&canvas->state->clip_spans, canvas->path, &canvas->state->matrix, &canvas->clip_rect, NULL, canvas->state->winding);
        canvas->state->clipping = true;
    }
}

void higgs_plutovg_canvas_fill_rect(higgs_plutovg_canvas_t* canvas, float x, float y, float w, float h)
{
    higgs_plutovg_canvas_new_path(canvas);
    higgs_plutovg_canvas_rect(canvas, x, y, w, h);
    higgs_plutovg_canvas_fill(canvas);
}

void higgs_plutovg_canvas_fill_path(higgs_plutovg_canvas_t* canvas, const higgs_plutovg_path_t* path)
{
    higgs_plutovg_canvas_new_path(canvas);
    higgs_plutovg_canvas_add_path(canvas, path);
    higgs_plutovg_canvas_fill(canvas);
}

void higgs_plutovg_canvas_stroke_rect(higgs_plutovg_canvas_t* canvas, float x, float y, float w, float h)
{
    higgs_plutovg_canvas_new_path(canvas);
    higgs_plutovg_canvas_rect(canvas, x, y, w, h);
    higgs_plutovg_canvas_stroke(canvas);
}

void higgs_plutovg_canvas_stroke_path(higgs_plutovg_canvas_t* canvas, const higgs_plutovg_path_t* path)
{
    higgs_plutovg_canvas_new_path(canvas);
    higgs_plutovg_canvas_add_path(canvas, path);
    higgs_plutovg_canvas_stroke(canvas);
}

void higgs_plutovg_canvas_clip_rect(higgs_plutovg_canvas_t* canvas, float x, float y, float w, float h)
{
    higgs_plutovg_canvas_new_path(canvas);
    higgs_plutovg_canvas_rect(canvas, x, y, w, h);
    higgs_plutovg_canvas_clip(canvas);
}

void higgs_plutovg_canvas_clip_path(higgs_plutovg_canvas_t* canvas, const higgs_plutovg_path_t* path)
{
    higgs_plutovg_canvas_new_path(canvas);
    higgs_plutovg_canvas_add_path(canvas, path);
    higgs_plutovg_canvas_clip(canvas);
}

float higgs_plutovg_canvas_add_glyph(higgs_plutovg_canvas_t* canvas, higgs_plutovg_codepoint_t codepoint, float x, float y)
{
    higgs_plutovg_state_t* state = canvas->state;
    if(state->font_face && state->font_size > 0.f)
        return higgs_plutovg_font_face_get_glyph_path(state->font_face, state->font_size, x, y, codepoint, canvas->path);
    return 0.f;
}

float higgs_plutovg_canvas_add_text(higgs_plutovg_canvas_t* canvas, const void* text, int length, higgs_plutovg_text_encoding_t encoding, float x, float y)
{
    higgs_plutovg_state_t* state = canvas->state;
    if(state->font_face == NULL || state->font_size <= 0.f)
        return 0.f;
    higgs_plutovg_text_iterator_t it;
    higgs_plutovg_text_iterator_init(&it, text, length, encoding);
    float advance_width = 0.f;
    while(higgs_plutovg_text_iterator_has_next(&it)) {
        higgs_plutovg_codepoint_t codepoint = higgs_plutovg_text_iterator_next(&it);
        advance_width += higgs_plutovg_font_face_get_glyph_path(state->font_face, state->font_size, x + advance_width, y, codepoint, canvas->path);
    }

    return advance_width;
}

float higgs_plutovg_canvas_fill_text(higgs_plutovg_canvas_t* canvas, const void* text, int length, higgs_plutovg_text_encoding_t encoding, float x, float y)
{
    higgs_plutovg_canvas_new_path(canvas);
    float advance_width = higgs_plutovg_canvas_add_text(canvas, text, length, encoding, x, y);
    higgs_plutovg_canvas_fill(canvas);
    return advance_width;
}

float higgs_plutovg_canvas_stroke_text(higgs_plutovg_canvas_t* canvas, const void* text, int length, higgs_plutovg_text_encoding_t encoding, float x, float y)
{
    higgs_plutovg_canvas_new_path(canvas);
    float advance_width = higgs_plutovg_canvas_add_text(canvas, text, length, encoding, x, y);
    higgs_plutovg_canvas_stroke(canvas);
    return advance_width;
}

float higgs_plutovg_canvas_clip_text(higgs_plutovg_canvas_t* canvas, const void* text, int length, higgs_plutovg_text_encoding_t encoding, float x, float y)
{
    higgs_plutovg_canvas_new_path(canvas);
    float advance_width = higgs_plutovg_canvas_add_text(canvas, text, length, encoding, x, y);
    higgs_plutovg_canvas_clip(canvas);
    return advance_width;
}

void higgs_plutovg_canvas_font_metrics(const higgs_plutovg_canvas_t* canvas, float* ascent, float* descent, float* line_gap, higgs_plutovg_rect_t* extents)
{
    higgs_plutovg_state_t* state = canvas->state;
    if(state->font_face && state->font_size > 0.f) {
        higgs_plutovg_font_face_get_metrics(state->font_face, state->font_size, ascent, descent, line_gap, extents);
        return;
    }

    if(ascent) *ascent = 0.f;
    if(descent) *descent = 0.f;
    if(line_gap) *line_gap = 0.f;
    if(extents) {
        extents->x = 0.f;
        extents->y = 0.f;
        extents->w = 0.f;
        extents->h = 0.f;
    }
}

void higgs_plutovg_canvas_glyph_metrics(higgs_plutovg_canvas_t* canvas, higgs_plutovg_codepoint_t codepoint, float* advance_width, float* left_side_bearing, higgs_plutovg_rect_t* extents)
{
    higgs_plutovg_state_t* state = canvas->state;
    if(state->font_face && state->font_size > 0.f) {
        higgs_plutovg_font_face_get_glyph_metrics(state->font_face, state->font_size, codepoint, advance_width, left_side_bearing, extents);
        return;
    }

    if(advance_width) *advance_width = 0.f;
    if(left_side_bearing) *left_side_bearing = 0.f;
    if(extents) {
        extents->x = 0.f;
        extents->y = 0.f;
        extents->w = 0.f;
        extents->h = 0.f;
    }
}

float higgs_plutovg_canvas_text_extents(higgs_plutovg_canvas_t* canvas, const void* text, int length, higgs_plutovg_text_encoding_t encoding, higgs_plutovg_rect_t* extents)
{
    higgs_plutovg_state_t* state = canvas->state;
    if(state->font_face && state->font_size > 0.f) {
        return higgs_plutovg_font_face_text_extents(state->font_face, state->font_size, text, length, encoding, extents);
    }

    if(extents) {
        extents->x = 0.f;
        extents->y = 0.f;
        extents->w = 0.f;
        extents->h = 0.f;
    }

    return 0.f;
}
