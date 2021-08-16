#include "graphics.h"
#include "../include/lunasvg.h"

#include <cfloat>
#include <cmath>
namespace {

namespace lunasvg {

const Color Color::Black(0xFF000000);
const Color Color::White(0xFFFFFFFF);
const Color Color::Transparent(0x00000000);

const Rect Rect::Empty(0, 0, 0, 0);
const Rect Rect::Invalid(0, 0, -1, -1);
const Rect Rect::Infinite(-FLT_MAX / 2.f, -FLT_MAX / 2.f, FLT_MAX, FLT_MAX);

Rect::Rect(const Box& box)
    : x(box.x), y(box.y), w(box.w), h(box.h)
{
}

const Transform Transform::Identity(1, 0, 0, 1, 0, 0);

Transform::Transform()
{
    higgs_plutovg_matrix_init_identity(&m_matrix);
}

Transform::Transform(float a, float b, float c, float d, float e, float f)
{
    higgs_plutovg_matrix_init(&m_matrix, a, b, c, d, e, f);
}

Transform::Transform(const Matrix& matrix)
    : Transform(matrix.a, matrix.b, matrix.c, matrix.d, matrix.e, matrix.f)
{
}

Transform Transform::operator*(const Transform& transform) const
{
    higgs_plutovg_matrix_t result;
    higgs_plutovg_matrix_multiply(&result, &transform.m_matrix, &m_matrix);
    return result;
}

Transform& Transform::operator*=(const Transform& transform)
{
    return (*this = *this * transform);
}

Transform& Transform::multiply(const Transform& transform)
{
    return (*this *= transform);
}

Transform& Transform::translate(float tx, float ty)
{
    return multiply(translated(tx, ty));
}

Transform& Transform::scale(float sx, float sy)
{
    return multiply(scaled(sx, sy));
}

Transform& Transform::rotate(float angle, float cx, float cy)
{
    return multiply(rotated(angle, cx, cy));
}

Transform& Transform::shear(float shx, float shy)
{
    return multiply(sheared(shx, shy));
}

Transform& Transform::postMultiply(const Transform& transform)
{
    return (*this = transform * *this);
}

Transform& Transform::postTranslate(float tx, float ty)
{
    return postMultiply(translated(tx, ty));
}

Transform& Transform::postScale(float sx, float sy)
{
    return postMultiply(scaled(sx, sy));
}

Transform& Transform::postRotate(float angle, float cx, float cy)
{
    return postMultiply(rotated(angle, cx, cy));
}

Transform& Transform::postShear(float shx, float shy)
{
    return postMultiply(sheared(shx, shy));
}

Transform Transform::inverse() const
{
    higgs_plutovg_matrix_t inverse;
    higgs_plutovg_matrix_invert(&m_matrix, &inverse);
    return inverse;
}

Transform& Transform::invert()
{
    higgs_plutovg_matrix_invert(&m_matrix, &m_matrix);
    return *this;
}

void Transform::reset()
{
    higgs_plutovg_matrix_init_identity(&m_matrix);
}

Point Transform::mapPoint(float x, float y) const
{
    higgs_plutovg_matrix_map(&m_matrix, x, y, &x, &y);
    return Point(x, y);
}

Point Transform::mapPoint(const Point& point) const
{
    return mapPoint(point.x, point.y);
}

Rect Transform::mapRect(const Rect& rect) const
{
    if(!rect.isValid()) {
        return Rect::Invalid;
    }

    higgs_plutovg_rect_t result = {rect.x, rect.y, rect.w, rect.h};
    higgs_plutovg_matrix_map_rect(&m_matrix, &result, &result);
    return result;
}

float Transform::xScale() const
{
    return std::sqrt(m_matrix.a * m_matrix.a + m_matrix.b * m_matrix.b);
}

float Transform::yScale() const
{
    return std::sqrt(m_matrix.c * m_matrix.c + m_matrix.d * m_matrix.d);
}

bool Transform::parse(const char* data, size_t length)
{
    return higgs_plutovg_matrix_parse(&m_matrix, data, length);
}

Transform Transform::rotated(float angle, float cx, float cy)
{
    higgs_plutovg_matrix_t matrix;
    if(cx == 0.f && cy == 0.f) {
        higgs_plutovg_matrix_init_rotate(&matrix, HIGGS_PLUTOVG_DEG2RAD(angle));
    } else {
        higgs_plutovg_matrix_init_translate(&matrix, cx, cy);
        higgs_plutovg_matrix_rotate(&matrix, HIGGS_PLUTOVG_DEG2RAD(angle));
        higgs_plutovg_matrix_translate(&matrix, -cx, -cy);
    }

    return matrix;
}

Transform Transform::scaled(float sx, float sy)
{
    higgs_plutovg_matrix_t matrix;
    higgs_plutovg_matrix_init_scale(&matrix, sx, sy);
    return matrix;
}

Transform Transform::sheared(float shx, float shy)
{
    higgs_plutovg_matrix_t matrix;
    higgs_plutovg_matrix_init_shear(&matrix, HIGGS_PLUTOVG_DEG2RAD(shx), HIGGS_PLUTOVG_DEG2RAD(shy));
    return matrix;
}

Transform Transform::translated(float tx, float ty)
{
    higgs_plutovg_matrix_t matrix;
    higgs_plutovg_matrix_init_translate(&matrix, tx, ty);
    return matrix;
}

Path::Path(const Path& path)
    : m_data(higgs_plutovg_path_reference(path.data()))
{
}

Path::Path(Path&& path)
    : m_data(path.release())
{
}

Path::~Path()
{
    higgs_plutovg_path_destroy(m_data);
}

Path& Path::operator=(const Path& path)
{
    Path(path).swap(*this);
    return *this;
}

Path& Path::operator=(Path&& path)
{
    Path(std::move(path)).swap(*this);
    return *this;
}

void Path::moveTo(float x, float y)
{
    higgs_plutovg_path_move_to(ensure(), x, y);
}

void Path::lineTo(float x, float y)
{
    higgs_plutovg_path_line_to(ensure(), x, y);
}

void Path::quadTo(float x1, float y1, float x2, float y2)
{
    higgs_plutovg_path_quad_to(ensure(), x1, y1, x2, y2);
}

void Path::cubicTo(float x1, float y1, float x2, float y2, float x3, float y3)
{
    higgs_plutovg_path_cubic_to(ensure(), x1, y1, x2, y2, x3, y3);
}

void Path::arcTo(float rx, float ry, float xAxisRotation, bool largeArcFlag, bool sweepFlag, float x, float y)
{
    higgs_plutovg_path_arc_to(ensure(), rx, ry, HIGGS_PLUTOVG_DEG2RAD(xAxisRotation), largeArcFlag, sweepFlag, x, y);
}

void Path::close()
{
    higgs_plutovg_path_close(ensure());
}

void Path::addEllipse(float cx, float cy, float rx, float ry)
{
    higgs_plutovg_path_add_ellipse(ensure(), cx, cy, rx, ry);
}

void Path::addRoundRect(float x, float y, float w, float h, float rx, float ry)
{
    higgs_plutovg_path_add_round_rect(ensure(), x, y, w, h, rx, ry);
}

void Path::addRect(float x, float y, float w, float h)
{
    higgs_plutovg_path_add_rect(ensure(), x, y, w, h);
}

void Path::addEllipse(const Point& center, const Size& radii)
{
    addEllipse(center.x, center.y, radii.w, radii.h);
}

void Path::addRoundRect(const Rect& rect, const Size& radii)
{
    addRoundRect(rect.x, rect.y, rect.w, rect.h, radii.w, radii.h);
}

void Path::addRect(const Rect& rect)
{
    addRect(rect.x, rect.y, rect.w, rect.h);
}

void Path::reset()
{
    if(m_data == nullptr)
        return;
    if(isUnique()) {
        higgs_plutovg_path_reset(m_data);
    } else {
        higgs_plutovg_path_destroy(m_data);
        m_data = nullptr;
    }
}

Rect Path::boundingRect() const
{
    if(m_data == nullptr)
        return Rect::Empty;
    higgs_plutovg_rect_t extents;
    higgs_plutovg_path_extents(m_data, &extents, false);
    return extents;
}

bool Path::isEmpty() const
{
    if(m_data)
        return higgs_plutovg_path_get_elements(m_data, nullptr) == 0;
    return true;
}

bool Path::isUnique() const
{
    return higgs_plutovg_path_get_reference_count(m_data) == 1;
}

bool Path::parse(const char* data, size_t length)
{
    higgs_plutovg_path_reset(ensure());
    return higgs_plutovg_path_parse(m_data, data, length);
}

higgs_plutovg_path_t* Path::ensure()
{
    if(isNull()) {
        m_data = higgs_plutovg_path_create();
    } else if(!isUnique()) {
        higgs_plutovg_path_destroy(m_data);
        m_data = higgs_plutovg_path_clone(m_data);
    }

    return m_data;
}

PathIterator::PathIterator(const Path& path)
    : m_size(higgs_plutovg_path_get_elements(path.data(), &m_elements))
    , m_index(0)
{
}

PathCommand PathIterator::currentSegment(std::array<Point, 3>& points) const
{
    auto command = m_elements[m_index].header.command;
    switch(command) {
    case HIGGS_PLUTOVG_PATH_COMMAND_MOVE_TO:
        points[0] = m_elements[m_index + 1].point;
        break;
    case HIGGS_PLUTOVG_PATH_COMMAND_LINE_TO:
        points[0] = m_elements[m_index + 1].point;
        break;
    case HIGGS_PLUTOVG_PATH_COMMAND_CUBIC_TO:
        points[0] = m_elements[m_index + 1].point;
        points[1] = m_elements[m_index + 2].point;
        points[2] = m_elements[m_index + 3].point;
        break;
    case HIGGS_PLUTOVG_PATH_COMMAND_CLOSE:
        points[0] = m_elements[m_index + 1].point;
        break;
    }

    return PathCommand(command);
}

void PathIterator::next()
{
    m_index += m_elements[m_index].header.length;
}

FontFace::FontFace(higgs_plutovg_font_face_t* face)
    : m_face(higgs_plutovg_font_face_reference(face))
{
}

FontFace::FontFace(const void* data, size_t length, higgs_plutovg_destroy_func_t destroy_func, void* closure)
    : m_face(higgs_plutovg_font_face_load_from_data(data, length, 0, destroy_func, closure))
{
}

FontFace::FontFace(const char* filename)
    : m_face(higgs_plutovg_font_face_load_from_file(filename, 0))
{
}

FontFace::FontFace(const FontFace& face)
    : m_face(higgs_plutovg_font_face_reference(face.get()))
{
}

FontFace::FontFace(FontFace&& face)
    : m_face(face.release())
{
}

FontFace::~FontFace()
{
    higgs_plutovg_font_face_destroy(m_face);
}

FontFace& FontFace::operator=(const FontFace& face)
{
    FontFace(face).swap(*this);
    return *this;
}

FontFace& FontFace::operator=(FontFace&& face)
{
    FontFace(std::move(face)).swap(*this);
    return *this;
}

void FontFace::swap(FontFace& face)
{
    std::swap(m_face, face.m_face);
}

higgs_plutovg_font_face_t* FontFace::release()
{
    return std::exchange(m_face, nullptr);
}

bool FontFaceCache::addFontFace(const std::string& family, bool bold, bool italic, const FontFace& face)
{
    if(!face.isNull())
        higgs_plutovg_font_face_cache_add(m_cache, family.data(), bold, italic, face.get());
    return !face.isNull();
}

FontFace FontFaceCache::getFontFace(const std::string& family, bool bold, bool italic) const
{
    if(auto face = higgs_plutovg_font_face_cache_get(m_cache, family.data(), bold, italic)) {
        return FontFace(face);
    }

    static const struct {
        const char* generic;
        const char* fallback;
    } generic_fallbacks[] = {
#if defined(__linux__)
        {"sans-serif", "DejaVu Sans"},
        {"serif", "DejaVu Serif"},
        {"monospace", "DejaVu Sans Mono"},
#else
        {"sans-serif", "Arial"},
        {"serif", "Times New Roman"},
        {"monospace", "Courier New"},
#endif
        {"cursive", "Comic Sans MS"},
        {"fantasy", "Impact"}
    };

    for(auto value : generic_fallbacks) {
        if(value.generic == family || family.empty()) {
            return FontFace(higgs_plutovg_font_face_cache_get(m_cache, value.fallback, bold, italic));
        }
    }

    return FontFace();
}

FontFaceCache::FontFaceCache()
    : m_cache(higgs_plutovg_font_face_cache_create())
{
#ifndef LUNASVG_DISABLE_LOAD_SYSTEM_FONTS
    higgs_plutovg_font_face_cache_load_sys(m_cache);
#endif
}

FontFaceCache* fontFaceCache()
{
    static FontFaceCache cache;
    return &cache;
}

std::shared_ptr<Canvas> Canvas::create(const Bitmap& bitmap)
{
    return createFromBitmap(bitmap);
}

std::shared_ptr<Canvas> Canvas::create(float x, float y, float width, float height)
{
    constexpr int kMaxSize = 1 << 15;
    if(width <= 0 || height <= 0 || width >= kMaxSize || height >= kMaxSize)
        return createFromBounds(0, 0, 1, 1);
    auto l = static_cast<int>(std::floor(x));
    auto t = static_cast<int>(std::floor(y));
    auto r = static_cast<int>(std::ceil(x + width));
    auto b = static_cast<int>(std::ceil(y + height));
    return createFromBounds(l, t, r - l, b - t);
}

std::shared_ptr<Canvas> Canvas::create(const Rect& extents)
{
    return create(extents.x, extents.y, extents.w, extents.h);
}

std::shared_ptr<Canvas> CanvasImpl::createCanvasImpl(const Bitmap& bitmap)
{
    return std::shared_ptr<Canvas>(new CanvasImpl(bitmap));
}

void CanvasImpl::setColor(const Color& color)
{
    setColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
}

void CanvasImpl::setColor(float r, float g, float b, float a)
{
    higgs_plutovg_canvas_set_rgba(m_canvas, r, g, b, a);
}

void CanvasImpl::setLinearGradient(float x1, float y1, float x2, float y2, SpreadMethod spread, const GradientStops& stops, const Transform& transform)
{
    higgs_plutovg_canvas_set_linear_gradient(m_canvas, x1, y1, x2, y2, static_cast<higgs_plutovg_spread_method_t>(spread), stops.data(), stops.size(), &transform.matrix());
}

void CanvasImpl::setRadialGradient(float cx, float cy, float r, float fx, float fy, SpreadMethod spread, const GradientStops& stops, const Transform& transform)
{
    higgs_plutovg_canvas_set_radial_gradient(m_canvas, cx, cy, r, fx, fy, 0.f, static_cast<higgs_plutovg_spread_method_t>(spread), stops.data(), stops.size(), &transform.matrix());
}

void CanvasImpl::setTexture(const Canvas& source, TextureType type, float opacity, const Transform& transform)
{
    higgs_plutovg_canvas_set_texture(m_canvas, source.surface(), static_cast<higgs_plutovg_texture_type_t>(type), opacity, &transform.matrix());
}

void CanvasImpl::fillPath(const Path& path, FillRule fillRule, const Transform& transform)
{
    higgs_plutovg_canvas_set_matrix(m_canvas, &m_translation);
    higgs_plutovg_canvas_transform(m_canvas, &transform.matrix());
    higgs_plutovg_canvas_set_fill_rule(m_canvas, static_cast<higgs_plutovg_fill_rule_t>(fillRule));
    higgs_plutovg_canvas_set_operator(m_canvas, HIGGS_PLUTOVG_OPERATOR_SRC_OVER);
    higgs_plutovg_canvas_fill_path(m_canvas, path.data());
}

void CanvasImpl::strokePath(const Path& path, const StrokeData& strokeData, const Transform& transform)
{
    higgs_plutovg_canvas_set_matrix(m_canvas, &m_translation);
    higgs_plutovg_canvas_transform(m_canvas, &transform.matrix());
    higgs_plutovg_canvas_set_line_width(m_canvas, strokeData.lineWidth());
    higgs_plutovg_canvas_set_miter_limit(m_canvas, strokeData.miterLimit());
    higgs_plutovg_canvas_set_line_cap(m_canvas, static_cast<higgs_plutovg_line_cap_t>(strokeData.lineCap()));
    higgs_plutovg_canvas_set_line_join(m_canvas, static_cast<higgs_plutovg_line_join_t>(strokeData.lineJoin()));
    higgs_plutovg_canvas_set_dash_offset(m_canvas, strokeData.dashOffset());
    higgs_plutovg_canvas_set_dash_array(m_canvas, strokeData.dashArray().data(), strokeData.dashArray().size());
    higgs_plutovg_canvas_set_operator(m_canvas, HIGGS_PLUTOVG_OPERATOR_SRC_OVER);
    higgs_plutovg_canvas_stroke_path(m_canvas, path.data());
}

void CanvasImpl::fillText(const std::u32string_view& text, const Font& font, const Point& origin, const Transform& transform)
{
    higgs_plutovg_canvas_set_matrix(m_canvas, &m_translation);
    higgs_plutovg_canvas_transform(m_canvas, &transform.matrix());
    higgs_plutovg_canvas_set_fill_rule(m_canvas, HIGGS_PLUTOVG_FILL_RULE_NON_ZERO);
    higgs_plutovg_canvas_set_operator(m_canvas, HIGGS_PLUTOVG_OPERATOR_SRC_OVER);
//    higgs_plutovg_canvas_set_font(m_canvas, font.face().get(), font.size());
    higgs_plutovg_canvas_fill_text(m_canvas, text.data(), text.length(), HIGGS_PLUTOVG_TEXT_ENCODING_UTF32, origin.x, origin.y);
}

void CanvasImpl::strokeText(const std::u32string_view& text, float strokeWidth, const Font& font, const Point& origin, const Transform& transform)
{
    higgs_plutovg_canvas_set_matrix(m_canvas, &m_translation);
    higgs_plutovg_canvas_transform(m_canvas, &transform.matrix());
    higgs_plutovg_canvas_set_line_width(m_canvas, strokeWidth);
    higgs_plutovg_canvas_set_miter_limit(m_canvas, 4.f);
    higgs_plutovg_canvas_set_line_cap(m_canvas, HIGGS_PLUTOVG_LINE_CAP_BUTT);
    higgs_plutovg_canvas_set_line_join(m_canvas, HIGGS_PLUTOVG_LINE_JOIN_MITER);
    higgs_plutovg_canvas_set_dash_offset(m_canvas, 0.f);
    higgs_plutovg_canvas_set_dash_array(m_canvas, nullptr, 0);
    higgs_plutovg_canvas_set_operator(m_canvas, HIGGS_PLUTOVG_OPERATOR_SRC_OVER);
//    higgs_plutovg_canvas_set_font(m_canvas, font.face().get(), font.size());
    higgs_plutovg_canvas_stroke_text(m_canvas, text.data(), text.length(), HIGGS_PLUTOVG_TEXT_ENCODING_UTF32, origin.x, origin.y);
}

void CanvasImpl::clipPath(const Path& path, FillRule clipRule, const Transform& transform)
{
    higgs_plutovg_canvas_set_matrix(m_canvas, &m_translation);
    higgs_plutovg_canvas_transform(m_canvas, &transform.matrix());
    higgs_plutovg_canvas_set_fill_rule(m_canvas, static_cast<higgs_plutovg_fill_rule_t>(clipRule));
    higgs_plutovg_canvas_clip_path(m_canvas, path.data());
}

void CanvasImpl::clipRect(const Rect& rect, FillRule clipRule, const Transform& transform)
{
    higgs_plutovg_canvas_set_matrix(m_canvas, &m_translation);
    higgs_plutovg_canvas_transform(m_canvas, &transform.matrix());
    higgs_plutovg_canvas_set_fill_rule(m_canvas, static_cast<higgs_plutovg_fill_rule_t>(clipRule));
    higgs_plutovg_canvas_clip_rect(m_canvas, rect.x, rect.y, rect.w, rect.h);
}

void CanvasImpl::drawImage(const Bitmap& image, const Rect& dstRect, const Rect& srcRect, const Transform& transform)
{
    auto xScale = dstRect.w / srcRect.w;
    auto yScale = dstRect.h / srcRect.h;
    higgs_plutovg_matrix_t matrix = { xScale, 0, 0, yScale, -srcRect.x * xScale, -srcRect.y * yScale };
    higgs_plutovg_canvas_set_matrix(m_canvas, &m_translation);
    higgs_plutovg_canvas_transform(m_canvas, &transform.matrix());
    higgs_plutovg_canvas_translate(m_canvas, dstRect.x, dstRect.y);
    higgs_plutovg_canvas_set_fill_rule(m_canvas, HIGGS_PLUTOVG_FILL_RULE_NON_ZERO);
    higgs_plutovg_canvas_set_operator(m_canvas, HIGGS_PLUTOVG_OPERATOR_SRC_OVER);
    higgs_plutovg_canvas_set_texture(m_canvas, image.surface(), HIGGS_PLUTOVG_TEXTURE_TYPE_PLAIN, 1.f, &matrix);
    higgs_plutovg_canvas_fill_rect(m_canvas, 0, 0, dstRect.w, dstRect.h);
}

void CanvasImpl::blendCanvas(const Canvas& canvas, BlendMode blendMode, float opacity)
{
    higgs_plutovg_matrix_t matrix = { 1, 0, 0, 1, static_cast<float>(canvas.x()), static_cast<float>(canvas.y()) };
    higgs_plutovg_canvas_set_matrix(m_canvas, &m_translation);
    higgs_plutovg_canvas_set_operator(m_canvas, static_cast<higgs_plutovg_operator_t>(blendMode));
    higgs_plutovg_canvas_set_texture(m_canvas, canvas.surface(), HIGGS_PLUTOVG_TEXTURE_TYPE_PLAIN, opacity, &matrix);
    higgs_plutovg_canvas_paint(m_canvas);
}

void CanvasImpl::save()
{
    higgs_plutovg_canvas_save(m_canvas);
}

void CanvasImpl::restore()
{
    higgs_plutovg_canvas_restore(m_canvas);
}

int CanvasImpl::width() const
{
    return higgs_plutovg_surface_get_width(m_surface);
}

int CanvasImpl::height() const
{
    return higgs_plutovg_surface_get_height(m_surface);
}

void CanvasImpl::convertToLuminanceMask()
{
    auto width = higgs_plutovg_surface_get_width(m_surface);
    auto height = higgs_plutovg_surface_get_height(m_surface);
    auto stride = higgs_plutovg_surface_get_stride(m_surface);
    auto data = higgs_plutovg_surface_get_data(m_surface);
    for(int y = 0; y < height; y++) {
        auto pixels = reinterpret_cast<uint32_t*>(data + stride * y);
        for(int x = 0; x < width; x++) {
            auto pixel = pixels[x];
            auto a = (pixel >> 24) & 0xFF;
            auto r = (pixel >> 16) & 0xFF;
            auto g = (pixel >> 8) & 0xFF;
            auto b = (pixel >> 0) & 0xFF;
            if(a) {
                r = (r * 255) / a;
                g = (g * 255) / a;
                b = (b * 255) / a;
            }

            auto l = (r * 0.2125 + g * 0.7154 + b * 0.0721);
            pixels[x] = static_cast<uint32_t>(l * (a / 255.0)) << 24;
        }
    }
}

CanvasImpl::~CanvasImpl()
{
    higgs_plutovg_canvas_destroy(m_canvas);
    higgs_plutovg_surface_destroy(m_surface);
}

CanvasImpl::CanvasImpl(const Bitmap& bitmap)
    : m_surface(higgs_plutovg_surface_reference(bitmap.surface()))
    , m_canvas(higgs_plutovg_canvas_create(m_surface))
    , m_translation({1, 0, 0, 1, 0, 0})
    , m_x(0), m_y(0)
{
}

CanvasImpl::CanvasImpl(int x, int y, int width, int height)
    : m_surface(higgs_plutovg_surface_create(width, height))
    , m_canvas(higgs_plutovg_canvas_create(m_surface))
    , m_translation({1, 0, 0, 1, -static_cast<float>(x), -static_cast<float>(y)})
    , m_x(x), m_y(y)
{
}

std::shared_ptr<Canvas> CanvasImpl::createFromBounds(int x, int y, int width, int height)
{
    return std::shared_ptr<Canvas>(new CanvasImpl(x, y, width, height));
}

std::shared_ptr<Canvas> CanvasImpl::createFromBitmap(const Bitmap& bitmap)
{
    return std::shared_ptr<Canvas>(new CanvasImpl(bitmap));
}

} // namespace lunasvg
}
