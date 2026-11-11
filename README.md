# Puerto de Contenedores

**Operación Contrarreloj** · Trabajo Práctico de Algoritmos y Estructuras de Datos (UNLaM)

Simulación por consola, desarrollada en C, donde un usuario asume el rol de Operador del Puerto. Debe gestionar el arribo de buques, la descarga de contenedores en zonas de almacenamiento y la entrega a camiones, intentando completar la mayor cantidad de operaciones antes de que termine la jornada.

## Integrantes

[![GitHub](https://img.shields.io/badge/GitHub-Lautaro%20Quintana-333333?style=for-the-badge&logo=github&logoColor=white)](https://github.com/lautarodquintana-jpg) [![GitHub](https://img.shields.io/badge/GitHub-Julian%20Luna-333333?style=for-the-badge&logo=github&logoColor=white)](https://github.com/JuliannLuna) [![GitHub](https://img.shields.io/badge/GitHub-Nehuen%20Lencina-333333?style=for-the-badge&logo=github&logoColor=white)](https://github.com/nehuenlencina) [![GitHub](https://img.shields.io/badge/GitHub-Lucas%20Costilla-333333?style=for-the-badge&logo=github&logoColor=white)](https://github.com/Lucasleonel4)

## Comandos

| Comando | Descripción |
|---------|-------------|
| `DES <muelle> <zona>` | Descarga el próximo contenedor del buque y lo almacena en una zona |
| `REU <zona_origen> <zona_destino>` | Reubica el contenedor del tope de una zona a otra |
| `ENT` | Entrega el contenedor al camión que está al frente de la cola |
| `VER` | Muestra el estado actual (no consume tiempo) |
| `ESP` | Espera un minuto |

## Puntuación

- +10 puntos por cada contenedor entregado.
- +5 puntos por cada buque que completa su descarga.
- −2 puntos por cada camión que queda esperando al finalizar.
