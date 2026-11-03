# higgs_opengl

Renderizado OpenGL dentro de componentes: `OpenGLContext`, `OpenGLRenderer`, *shaders*, texturas, *framebuffers*, imágenes en GPU y matemáticas 3D. En Linux usa EGL y admite OpenGL ES.

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_opengl` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos OpenGL classes |
| Descripción declarada | Classes for rendering OpenGL in a Higgs Mythos window. |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_gui_extra`](higgs_gui_extra.md) |
| Dependencias indirectas | [`higgs_core`](higgs_core.md), [`higgs_events`](higgs_events.md), [`higgs_data_structures`](higgs_data_structures.md), [`higgs_graphics`](higgs_graphics.md), [`higgs_gui_basics`](higgs_gui_basics.md) |
| Frameworks de macOS | `OpenGL` |
| Frameworks de iOS | `OpenGLES` |
| Paquetes pkg-config (Linux) | `egl`, `gl` |
| Código fuente | [`modules/higgs_opengl`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_opengl) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_opengl)
```

```cpp
#include <higgs_opengl/higgs_opengl.h>
```

Guías relacionadas: [OpenGL](../guias/opengl.md).

## Opciones de configuración

Se cambian con `target_compile_definitions(MiTarget PUBLIC OPCION=valor)` o en la sección *Modules* de Mythos Studio.

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_OPENGL_ES` | `0` | Compila `higgs_opengl` con OpenGL ES en lugar de OpenGL de escritorio. Solo se puede elegir en Linux y BSD, para sistemas cuyos controladores gráficos solo proporcionan OpenGL ES, como muchas placas Arm embebidas. Siempre está habilitado en iOS y Android, y no puede habilitarse en macOS ni en Windows. Compilar con OpenGL ES requiere enlazar con `libGLESv2` en lugar de `libGL`. Con CMake, habilítelo con la opción de destino `NEEDS_OPENGL_ES` en lugar de definir esta macro, ya que esa opción también enlaza las bibliotecas correctas. En Mythos Studio, habilite el indicador del módulo, o agregue `HIGGS_OPENGL_ES=1` a las definiciones de preprocesador de un exportador de Linux cuando el proyecto también tenga como destino otras plataformas. |

## Código de terceros incluido

| Componente | Licencia | Ubicación |
|---|---|---|
| [OpenGL registry headers](https://registry.khronos.org/OpenGL) | `BSD-3-Clause AND MIT` | `modules/higgs_opengl/opengl/higgs_gl.h` |

## Catálogo de clases

15 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_opengl.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `geometry`

| Clase | Descripción | Cabecera |
|---|---|---|
| `Vector3D` | Un vector de tres coordenadas. | [`higgs_Vector3D.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_opengl/geometry/higgs_Vector3D.h) |
| `Matrix3D` | Una matriz de transformación 3D de 4x4. | [`higgs_Matrix3D.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_opengl/geometry/higgs_Matrix3D.h) |
| `Quaternion` | Contiene un cuaternión (un vector 3D y un valor escalar). | [`higgs_Quaternion.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_opengl/geometry/higgs_Quaternion.h) |
| `Draggable3DOrientation` | Almacena una orientación 3D, que puede rotarse arrastrando con el mouse. | [`higgs_Draggable3DOrientation.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_opengl/geometry/higgs_Draggable3DOrientation.h) |

### `opengl`

| Clase | Descripción | Cabecera |
|---|---|---|
| `OpenGLHelpers` | Un conjunto de diversas funciones auxiliares de OpenGL. | [`higgs_OpenGLHelpers.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_opengl/opengl/higgs_OpenGLHelpers.h) |
| `OpenGLPixelFormat` | Representa las diversas propiedades de un formato de píxel de OpenGL. | [`higgs_OpenGLPixelFormat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_opengl/opengl/higgs_OpenGLPixelFormat.h) |
| `OpenGLRenderer` | Una clase base que deben implementar las clases que deseen renderizar con openGL en un hilo en segundo plano. | [`higgs_OpenGLRenderer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_opengl/opengl/higgs_OpenGLRenderer.h) |
| `OpenGLVersion` | Contiene información de versión sobre una API de OpenGL concreta. | [`higgs_OpenGLContext.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_opengl/opengl/higgs_OpenGLContext.h) |
| `OpenGLContext` | Crea un contexto OpenGL que se puede asociar a un componente. | [`higgs_OpenGLContext.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_opengl/opengl/higgs_OpenGLContext.h) |
| `OpenGLFrameBuffer` | Crea un búfer de fotogramas de OpenGL. | [`higgs_OpenGLFrameBuffer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_opengl/opengl/higgs_OpenGLFrameBuffer.h) |
| `OpenGLGraphicsContextCustomShader` | Se usa para crear shaders personalizados para usar con un contexto de renderizado 2D de OpenGL. | [`higgs_OpenGLGraphicsContext.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_opengl/opengl/higgs_OpenGLGraphicsContext.h) |
| `OpenGLImageType` | Un tipo de ImagePixelData que almacena sus datos de imagen en un búfer de fotogramas de OpenGL, lo que permite que un objeto Image de HIGGS envuelva un búfer de fotogramas. | [`higgs_OpenGLImage.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_opengl/opengl/higgs_OpenGLImage.h) |
| `OpenGLShaderProgram` | Administra un programa de shaders de OpenGL. | [`higgs_OpenGLShaderProgram.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_opengl/opengl/higgs_OpenGLShaderProgram.h) |
| `OpenGLTexture` | Crea una textura de openGL a partir de una Image. | [`higgs_OpenGLTexture.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_opengl/opengl/higgs_OpenGLTexture.h) |

### `utils`

| Clase | Descripción | Cabecera |
|---|---|---|
| `OpenGLAppComponent` | Una clase base para escribir aplicaciones gráficas simples de una sola página. | [`higgs_OpenGLAppComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_opengl/utils/higgs_OpenGLAppComponent.h) |
