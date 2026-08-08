# Tarea #6 - ESP32: Gestión de Energía

## Descripción del Proyecto
Este repositorio contiene el código correspondiente a la Tarea #6 de Sistemas Embebidos, cuyo objetivo es aplicar modos de ahorro de energía del ESP32[cite: 1]. 

Para este ejercicio se ha implementado el modo **Light Sleep**. En este estado, la CPU del microcontrolador entra en pausa para reducir el consumo, pero mantiene operativos la memoria RTC, los periféricos RTC y el coprocesador ULP[cite: 1]. Esto permite que el sistema alterne eficientemente entre periodos de ejecución activa y reposo[cite: 1].

## Disposición del Código
El código fuente principal de este proyecto se encuentra en la siguiente ruta: **[DEBER6/src/main.cpp](https://github.com/tsedestroier/TAREA-6-STEFANO/blob/main/DEBER6/src/main.cpp)**

El sistema está desarrollado de manera modular y se divide en las siguientes funciones principales dentro de dicho archivo:

*   **`configurarPines()`:** Establece los pines GPIO 25, 26 y 27 como salidas para controlar un LED RGB, y configura el GPIO 4 como entrada con resistencia *pull-up* interna para el botón de despertar.
*   **`colorLED()`:** Función auxiliar para controlar de forma simplificada el encendido y apagado de los canales de color del LED RGB.
*   **`tareaActiva()`:** Simula una carga de trabajo en el procesador. Utiliza el puerto serial para indicar el número de despertar actual, parpadea el LED en color verde para representar el procesamiento de datos y finaliza preparando el sistema para dormir.
*   **`setup()`:** Se ejecuta una sola vez al inicio. Inicializa la comunicación serial a 115200 baudios y llama a la configuración inicial de los pines.
*   **`loop()`:** Contiene el ciclo de vida principal del modo de energía:
    1.  Llama a `tareaActiva()`.
    2.  Enciende brevemente el LED en rojo como indicador visual de transición y luego lo apaga por completo para ahorrar energía.
    3.  Realiza la configuración explícita de los eventos de despertar requeridos: por temporizador (`esp_sleep_enable_timer_wakeup`) configurado a 10 segundos, y por señal en un pin externo (`esp_sleep_enable_ext0_wakeup`) en el GPIO 4[cite: 1].
    4.  Ejecuta la instrucción `esp_light_sleep_start()` para entrar en reposo. Al detectar un evento de despertar, la CPU reanuda su ejecución exactamente desde la línea posterior a esta instrucción.

## Instrucciones de Compilación y Ejecución
Este proyecto ha sido estructurado para ser desarrollado y compilado utilizando el entorno de desarrollo PlatformIO integrado en Visual Studio Code[cite: 1].

### Requisitos Previos
*   Tener instalado [Visual Studio Code](https://code.visualstudio.com/).
*   Contar con la extensión de **PlatformIO** instalada en VSC.
*   (Opcional) Extensión de **Wokwi** si se desea realizar la simulación virtual.

### Pasos para la Ejecución
1.  **Clonar el repositorio:** Descarga este repositorio en tu máquina local.
2.  **Abrir el proyecto:** Inicia Visual Studio Code y abre la carpeta raíz del proyecto. PlatformIO reconocerá automáticamente la estructura del entorno.
3.  **Compilar el código:** En la barra inferior de tareas de PlatformIO, haz clic en el botón **Build** (icono de visto bueno `✓`). Verifica que la terminal indique "SUCCESS".
4.  **Subir al hardware real:** Conecta tu placa ESP32 por USB y haz clic en el botón **Upload** (icono de flecha `→`). 
5.  **Simulación en Wokwi:** Si utilizas el simulador en lugar de hardware real, abre el archivo `diagram.json` o presiona `F1` y selecciona *Wokwi: Start Simulator*.
6.  **Monitor Serial:** Abre el **Serial Monitor** (icono de enchufe en PlatformIO) y configúralo a 115200 baudios para observar los registros de encendido, procesamiento de datos y eventos de despertar del sistema.
