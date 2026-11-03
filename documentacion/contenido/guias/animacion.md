# Animación

El módulo `higgs_animation` proporciona un sistema declarativo para animar valores en la interfaz:
constructores inmutables que crean objetos `Animator`, funciones de suavizado (*easings*) y
actualizadores sincronizados con el refresco de pantalla.

## Piezas principales

| Clase | Función |
|---|---|
| `ValueAnimatorBuilder` | Describe una animación: duración, suavizado, *callbacks* de inicio, cambio de valor y fin |
| `Animator` | La animación construida; se inicia con `start()` y se termina con `complete()` |
| `AnimatorSetBuilder` | Combina animaciones en secuencia (`followedBy`) o en paralelo (`togetherWith`) |
| `AnimatorUpdater` | Actualiza varios `Animator` a la vez |
| `VBlankAnimatorUpdater` | Actualizador sincronizado con el refresco (*vblank*) de la pantalla de un componente |
| `Easings` | Funciones de suavizado: `createEase`, `createEaseIn`, `createEaseOut`, `createEaseInOut`, `createEaseInOutCubic`, `createEaseOutBack`, `createSpring`, `createCubicBezier`… |
| `StaticAnimationLimits` / `makeAnimationLimits` | Interpolar entre dos valores (posiciones, colores…) |

## Ejemplo: aparecer con un muelle

```cpp
class Tarjeta final : public higgs::Component
{
public:
    Tarjeta()
    {
        actualizador.addAnimator (aparecer);
    }

    void mostrar() { aparecer.start(); }

    void paint (higgs::Graphics& g) override
    {
        g.setColour (higgs::Colours::orange.withAlpha (opacidad));
        g.fillRoundedRectangle (getLocalBounds().toFloat().reduced (10.0f * (1.0f - opacidad)), 8.0f);
    }

private:
    float opacidad = 0.0f;

    higgs::Animator aparecer = higgs::ValueAnimatorBuilder{}
                                   .withEasing (higgs::Easings::createSpring())
                                   .withDurationMs (600)
                                   .withValueChangedCallback ([this] (float progreso)
                                   {
                                       opacidad = higgs::jlimit (0.0f, 1.0f, progreso);
                                       repaint();
                                   })
                                   .build();

    higgs::VBlankAnimatorUpdater actualizador { this };
};
```

El valor que recibe el *callback* va de 0,0 a 1,0 a lo largo de la duración (una función de
suavizado puede producir valores fuera de ese rango, como el «rebote» de un muelle). Un
`ValueAnimatorBuilder` es inmutable: cada `with…` devuelve una copia, y cada `build()` crea un
animador independiente. Las copias de un `Animator` comparten la misma animación.

## Secuencias

```cpp
auto secuencia = higgs::AnimatorSetBuilder (entrarTitulo)
                     .followedBy (200)          // pausa de 200 ms
                     .followedBy (entrarBotones)
                     .build();
```

## Otras utilidades de animación

En `higgs_gui_basics` también encontrarás `ComponentAnimator` (mover y desvanecer componentes),
`AnimatedPosition` (desplazamiento con inercia) y `VBlankAttachment`; en `higgs_gui_extra`,
`AnimatedAppComponent` para aplicaciones que se redibujan a una frecuencia fija.

## Demos relacionadas

`AnimatorsDemo`, `AnimationEasingDemo`, `AnimationAppDemo`, `BouncingBallWavetableDemo`.
