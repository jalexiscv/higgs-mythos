# OpenGL

El módulo `higgs_opengl` permite renderizar con OpenGL dentro de cualquier componente. Puedes
usarlo de dos formas: para **acelerar el dibujo 2D** de la interfaz normal, o para escribir tu
**propio código OpenGL** (3D, *shaders*, visualizaciones).

| Plataforma | API |
|---|---|
| macOS | OpenGL de escritorio (framework `OpenGL`) |
| Windows | OpenGL de escritorio |
| Linux / BSD | OpenGL de escritorio vía **EGL** (desde 9.0.0), u **OpenGL ES** con `HIGGS_OPENGL_ES=1` / `NEEDS_OPENGL_ES TRUE` |
| iOS, Android | OpenGL ES (siempre) |

Dependencias en Linux: `libegl-dev`, y `libglu1-mesa-dev` + `mesa-common-dev` (OpenGL de
escritorio) o `libgles-dev` (OpenGL ES). Véase [Dependencias en Linux](../compilacion/linux.md).

## Acelerar el dibujo de un componente

Adjuntar un `OpenGLContext` a un componente hace que él y sus hijos se dibujen con OpenGL, sin
cambiar tu código de `paint()`:

```cpp
class MiInterfaz final : public higgs::Component
{
public:
    MiInterfaz()  { contextoGL.attachTo (*this); }
    ~MiInterfaz() override { contextoGL.detach(); }

private:
    higgs::OpenGLContext contextoGL;
};
```

## Renderizado propio

Implementa `OpenGLRenderer`. Sus tres métodos se llaman **en el hilo de renderizado de OpenGL**,
no en el hilo de mensajes:

```cpp
class Escena final : public higgs::Component,
                     private higgs::OpenGLRenderer
{
public:
    Escena()
    {
        contexto.setRenderer (this);
        contexto.setContinuousRepainting (true);
        contexto.attachTo (*this);
    }

    ~Escena() override { contexto.detach(); }

private:
    void newOpenGLContextCreated() override
    {
        sombreador = std::make_unique<higgs::OpenGLShaderProgram> (contexto);
        // sombreador->addVertexShader (...); sombreador->addFragmentShader (...); sombreador->link();
    }

    void renderOpenGL() override
    {
        higgs::OpenGLHelpers::clear (higgs::Colours::black);
        // ... dibujo con gl* ...
    }

    void openGLContextClosing() override
    {
        sombreador.reset();   // libera los recursos de GPU aquí
    }

    higgs::OpenGLContext contexto;
    std::unique_ptr<higgs::OpenGLShaderProgram> sombreador;
};
```

Para aplicaciones completas, `OpenGLAppComponent` ya combina un componente, un contexto y un
renderizador, con métodos `initialise()`, `render()` y `shutdown()` (véase `OpenGLAppDemo`).

## Clases auxiliares

| Clase | Uso |
|---|---|
| `OpenGLShaderProgram` | Compilar y enlazar *shaders*, gestionar *uniforms* y atributos |
| `OpenGLTexture` | Cargar una `Image` como textura |
| `OpenGLFrameBuffer` | Renderizado fuera de pantalla |
| `OpenGLImageType` | Imágenes almacenadas en la GPU que se pueden dibujar con `Graphics` |
| `OpenGLGraphicsContextCustomShader` | Rellenar formas 2D con un *shader* propio |
| `OpenGLPixelFormat` | Bits de color, profundidad, *stencil* y multimuestreo |
| `OpenGLHelpers` | Utilidades: limpiar, comprobar extensiones, errores… |
| `Vector3D`, `Matrix3D`, `Quaternion`, `Draggable3DOrientation` | Matemáticas 3D y rotación con el ratón |

`OpenGLContext::setOpenGLVersionRequired()` permite pedir un perfil núcleo moderno (por ejemplo
OpenGL 3.2) y `setComponentPaintingEnabled (false)` desactiva el dibujo 2D de los componentes si
solo te interesa tu renderizado.

## Demos relacionadas

`OpenGLAppDemo`, `OpenGLDemo` (3D con *shaders* editables), `OpenGLDemo2D`.
