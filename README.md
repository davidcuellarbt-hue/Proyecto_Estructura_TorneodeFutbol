<div align="center">

# ⚽ Sistema de Gestión de Torneo de Fútbol

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Qt](https://img.shields.io/badge/Qt-6.x-41CD52?style=for-the-badge&logo=qt&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.16+-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-En%20Desarrollo-orange?style=for-the-badge)

**Aplicación de escritorio para administrar un torneo completo de fútbol estilo Liga MX.**  
Implementada íntegramente en memoria con estructuras de datos dinámicas: BST, listas enlazadas y arreglos estáticos.

[📺 Video Demo](#-video-de-demostración) · [🚀 Instalación](#-instalación) · [📖 Uso](#-uso-rápido) · [🏗️ Arquitectura](#%EF%B8%8F-arquitectura-de-datos)

</div>

---

## 📋 Tabla de Contenidos

- [Descripción General](#-descripción-general)
- [Video de Demostración](#-video-de-demostración)
- [Características](#-características)
- [Arquitectura de Datos](#%EF%B8%8F-arquitectura-de-datos)
- [Estructura del Proyecto](#-estructura-del-proyecto)
- [Requisitos del Sistema](#-requisitos-del-sistema)
- [Instalación](#-instalación)
- [Uso Rápido](#-uso-rápido)
- [Fases del Torneo](#-fases-del-torneo)
- [Análisis de Complejidad](#-análisis-de-complejidad)
- [Validaciones del Sistema](#-validaciones-del-sistema)

---

## 📌 Descripción General

Este proyecto simula la organización de un torneo de fútbol profesional de **20 equipos** con dos fases bien definidas:

| Fase | Formato | Partidos |
|------|---------|----------|
| **Fase Regular** | Round-robin (todos contra todos, ida) · 19 jornadas · 10 partidos/jornada | 190 partidos |
| **Liguilla (Eliminatorias)** | Top 8 clasificados · Cuartos → Semifinales → Final · Sin empates | 7 partidos |

> **Nota de diseño:** el sistema opera **100% en memoria RAM** (sin base de datos ni archivos externos). Al cerrar la aplicación los datos no persisten — esto es intencional para enfocarse en la manipulación pura de estructuras de datos.

---

## 📺 Video de Demostración

<div align="center">

> 🎬 **Haz clic en la imagen para ver el video de demostración completo**

[![Demo del Sistema de Torneo de Fútbol](https://img.youtube.com/vi/YOUTUBE_VIDEO_ID/maxresdefault.jpg)](https://youtu.be/ZAZLbWbajPU)

*Demostración completa: registro de equipos, jornadas, tabla de posiciones y Liguilla.*

</div>

### ¿Qué muestra el video?

- ✅ Carga automática de 20 equipos y 220 jugadores
- ✅ Registro de resultados y actualización en tiempo real de la tabla
- ✅ Búsqueda de jugadores por nombre y por dorsal (búsqueda binaria)
- ✅ Generación y navegación del bracket de Liguilla
- ✅ Registro del campeón y tabla de goleadores final

---

## ✨ Características

### Gestión de Equipos y Jugadores
- Alta de equipos (máx. 20) con validación de duplicados en el BST
- Alta de jugadores con nombre, dorsal único por equipo y posición
- Carga masiva automática: **20 equipos + 220 jugadores + 190 partidos** en un clic

### Calendario Inteligente
- Generación automática del calendario mediante **algoritmo round-robin circular**
- 19 jornadas, cada par de equipos se enfrenta exactamente una vez
- Navegación por jornada con visualización de los 10 partidos

### Registro de Resultados
- Ingreso de goles local/visitante con **bloqueo post-registro** (inmutable una vez guardado)
- Asignación de goles a jugadores individuales mediante selector desplegable (QComboBox)
- Validación de no re-asignación de goles en el mismo partido

### Tabla de Posiciones en Tiempo Real
- Actualización automática tras cada resultado
- Criterios de desempate: **Puntos → Diferencia de Goles → Goles a Favor**
- Renderizado en `QTableWidget` con corte visual al Top 8

### Estadísticas de Goleadores
- Tabla ordenada descendente por goles totales
- Filtro por equipo, búsqueda por nombre (lineal) y búsqueda por dorsal (**binaria**)

### Eliminatorias (Liguilla)
- Bracket visual completo: Cuartos → Semifinales → Final
- Emparejamiento automático: 1° vs 8°, 2° vs 7°, 3° vs 6°, 4° vs 5°
- Avance automático de ganadores a la siguiente fase
- Registro de campeón y **Bota de Oro** al finalizar

---

## 🏗️ Arquitectura de Datos

El sistema usa **cuatro estructuras de datos** implementadas desde cero con memoria dinámica (`new` / `delete`), sin contenedores de la STL como `std::map` o `std::list`.

```
┌─────────────────────────────────────────────────────────┐
│                  ÁRBOL BST — Equipos                    │
│                                                         │
│              [América]                                  │
│             /         \                                 │
│        [Atlas]      [Chivas]                            │
│           |             |                               │
│        Lista          Lista        ← Lista Enlazada     │
│       Jugadores      Jugadores       de Jugadores       │
│                                                         │
└─────────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────────┐
│            VECTOR — Calendario (Fase Regular)            │
│                                                          │
│  Jornada[0]   Jornada[1]  ...  Jornada[18]              │
│  Partido[0..9] Partido[0..9]   Partido[0..9]            │
└──────────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────────┐
│          ESTRUCTURA — Bracket Eliminatorias              │
│                                                          │
│  Cuartos[0..3] → Semis[0..1] → Final[0]                 │
│  (4 partidos)   (2 partidos)  (1 partido)                │
└──────────────────────────────────────────────────────────┘
```

| Estructura | Uso | Inserción | Búsqueda |
|------------|-----|-----------|----------|
| **BST (Árbol Binario de Búsqueda)** | Almacén de equipos | O(log n) prom. | O(log n) prom. |
| **Lista Enlazada Simple** | Plantilla de jugadores por equipo | O(1) al inicio | O(n) |
| **Arreglo estático `Jornada[19]`** | Calendario round-robin | N/A (tamaño fijo) | O(1) por índice |
| **Struct `Eliminatorias`** | Bracket cuartos/semis/final | N/A (tamaño fijo) | O(1) por índice |

---

## 📁 Estructura del Proyecto

```
Proyecto_Estructura_TorneodeFutbol/
│
├── estructuras.h          # Definición de nodos BST, Jornada, Partido,
│                          # Eliminatorias y estadísticas de equipo
├── estructuras.cpp        # Lógica del torneo: inserción en BST,
│                          # round-robin, registro de resultados,
│                          # ordenamiento tabla, bracket liguilla
│
├── player.h               # Struct Player (nombre, dorsal, posición, goles)
├── playerlist.h           # Declaración de la lista enlazada de jugadores
├── playerlist.cpp         # Implementación: insertar, buscar, recorrer,
│                          # búsqueda binaria por dorsal
│
├── mainwindow.h           # Declaración de la ventana principal Qt
├── mainwindow.cpp         # Interfaz gráfica: slots, señales, QTableWidget,
│                          # QComboBox, navegación entre pestañas
├── mainwindow.ui          # Archivo de diseño Qt Designer
│
├── main.cpp               # Punto de entrada QApplication
├── CMakeLists.txt         # Configuración de build con CMake
└── README.md
```

---

## 💻 Requisitos del Sistema

| Componente | Versión Mínima |
|------------|---------------|
| Compilador | GCC 9+ / MSVC 2019+ / Clang 10+ con **C++17** |
| Qt Framework | **Qt 6.x** (módulo `Widgets` requerido) |
| CMake | 3.16+ *(o qmake incluido en Qt Creator)* |
| RAM | 256 MB *(sin almacenamiento en disco)* |
| SO | Windows 10/11 · macOS 11+ · Ubuntu 20.04+ |

---

## 🚀 Instalación

### Opción A — Qt Creator (recomendado)

```bash
# 1. Clonar el repositorio
git clone https://github.com/davidcuellarbt-hue/Proyecto_Estructura_TorneodeFutbol.git
cd Proyecto_Estructura_TorneodeFutbol

# 2. Abrir Qt Creator → Archivo → Abrir Proyecto
#    Seleccionar CMakeLists.txt

# 3. Elegir kit Desktop Qt 6.x y compilar (Ctrl+B)

# 4. Ejecutar (Ctrl+R)
```

### Opción B — Línea de comandos con CMake

```bash
git clone https://github.com/davidcuellarbt-hue/Proyecto_Estructura_TorneodeFutbol.git
cd Proyecto_Estructura_TorneodeFutbol

mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release

# Ejecutar el binario generado
./TorneoDeFutbol        # Linux / macOS
TorneoDeFutbol.exe      # Windows
```

> **Importante:** asegúrate de que las bibliotecas Qt estén en tu `PATH` o usa `windeployqt` / `macdeployqt` para empaquetar las dependencias.

---

## 📖 Uso Rápido

### Flujo completo del torneo

```
1. Registrar equipos  →  2. Agregar jugadores  →  3. Simular jornadas
         ↓                                               ↓
6. Registrar campeón  ←  5. Liguilla  ←  4. Tabla de posiciones
```

### Inicio rápido con datos de prueba

Si solo quieres explorar el sistema sin capturar datos manualmente:

1. Abre la aplicación
2. Haz clic en **"Cargar Datos de Prueba"**
3. El sistema insertará automáticamente **20 equipos · 220 jugadores · 190 resultados**
4. Navega a la pestaña **Tabla** para ver la clasificación generada
5. Accede a **Eliminatorias** para ver el bracket con los 8 clasificados

---

## 🏆 Fases del Torneo

### Fase Regular — Round-Robin (19 Jornadas)

El calendario se genera con un **algoritmo de rotación circular**: el primer equipo se fija y los 19 restantes rotan en sentido horario jornada a jornada. Esto garantiza que cada par de equipos se enfrente exactamente una vez.

```
Jornada 1:  E1 vs E20 | E2 vs E19 | E3 vs E18 | ... | E10 vs E11
Jornada 2:  E1 vs E19 | E20 vs E18 | E2 vs E17 | ... | E9  vs E11
...
Jornada 19: último emparejamiento de cada equipo pendiente
```

**Actualización de estadísticas tras cada resultado:**

| Resultado | Equipo ganador | Equipo perdedor | Empate (ambos) |
|-----------|---------------|----------------|----------------|
| Puntos    | +3            | +0             | +1             |
| GF / GC   | Se suman goles propios y ajenos en cada equipo | | |
| DG        | GF − GC recalculado automáticamente | | |

### Liguilla — Bracket Eliminatorio

```
CUARTOS DE FINAL          SEMIFINALES             FINAL
  1° vs 8°  ──┐
               ├── Ganador A ──┐
  4° vs 5°  ──┘                 │
                                 ├── CAMPEÓN 🏆
  2° vs 7°  ──┐                 │
               ├── Ganador B ──┘
  3° vs 6°  ──┘
```

> ⚠️ Los **empates no están permitidos** en eliminatorias. El sistema rechaza resultados igualados y solicita corrección.

---

## 📊 Análisis de Complejidad

| Algoritmo | Tiempo (promedio) | Tiempo (peor caso) | Espacio |
|-----------|:-----------------:|:------------------:|:-------:|
| Inserción en BST | O(log n) | O(n) árbol degenerado | O(1) |
| Búsqueda en BST | O(log n) | O(n) | O(1) |
| Inserción en lista enlazada | **O(1)** | O(1) | O(1) |
| Búsqueda lineal jugadores | O(n) | O(n) | O(1) |
| Generación round-robin | O(n²) | O(n²) | O(n) |
| Ordenamiento tabla (Bubble Sort) | O(n²) | O(n²) | **O(1)** |
| Búsqueda binaria por dorsal | **O(log n)** | O(log n) | O(1) |
| Avance bracket eliminatorias | **O(1)** | O(1) | O(1) |
| Destructor BST (post-order) | O(n) | O(n) | O(h) |

> `n` = número de elementos en la estructura · `h` = altura del árbol BST

---

## ✅ Validaciones del Sistema

| Situación | Comportamiento |
|-----------|---------------|
| Equipo duplicado | Rechazado por el BST; mensaje de error en UI |
| Partido ya registrado | Campo bloqueado (flag `jugado = true`); no editable |
| Empate en eliminatoria | Sistema rechaza el resultado y solicita corrección |
| Dorsal duplicado en equipo | Rechazado al insertar; cada dorsal es único por club |
| Gol re-asignado en mismo partido | Validación en QComboBox; segunda asignación bloqueada |
| Más de 20 equipos | Botón de alta deshabilitado al alcanzar el límite |

---

## 🧩 Stack Tecnológico

| Capa | Tecnología |
|------|-----------|
| Lenguaje | C++17 |
| Framework GUI | Qt 6 (Widgets) |
| IDE | Qt Creator |
| Build System | CMake 3.16+ / qmake |
| Gestión de memoria | Punteros crudos, `new` / `delete` (sin smart pointers) |
| Librerías estándar usadas | `<string>`, `<vector>` *(solo para datos auxiliares)* |

---

<div align="center">

Hecho con ☕ y muchos punteros · Materia: Estructuras de Datos y Algoritmos

</div>
