# SKILLS — E-Paper Success 2026

> Documentación maestra condensada del proyecto.  
> Versión del proyecto: **1.5.0**  
> Última actualización: 2026-07-28

---

## 1. Resumen del Proyecto

**E-Paper Success 2026** es una aplicación de control de pantallas electrónicas (E-Paper/EPD) para Raspberry Pi, desarrollada en **C++20**.

| Aspecto | Valor |
|:--------|:------|
| Versión | 1.5.0 |
| Autor | Lio |
| Licencia | GNU |
| Hardware objetivo | Raspberry Pi 2W, Pi 4, Pi Zero 2W |
| Compilador | g++ |
| Estándar | C++20 |
| Dependencias | librería bcm2835, libqrencode |

### Características principales
- Multi-pantalla: resoluciones desde 1.54" hasta 4.37" (BW y BWR)
- Actualización Global (Global Update) y Fast Update
- QR Code Generator integrado con escalado configurable
- Sistema de fuentes bitmap (8 fuentes)
- GPIO + SPI nativo via bcm2835
- Scripts de depuración GDB preconfigurados

### Pantallas soportadas

| Diagonal | Resolución (VxH) | Código | Modo |
|----------|------------------|--------|------|
| 1.54" | 152 x 152 | 0x1509 | BW/BWR |
| 2.13" | 212 x 104 | 0x2100 | BW/BWR |
| 2.66" | 296 x 152 | 0x2600 | BW/BWR |
| 2.71" | 264 x 176 | 0x2700 | BW/BWR |
| 2.87" | 296 x 128 | 0x2800 | BW/BWR |
| 3.70" | 416 x 240 | 0x3700 | BW/BWR |
| 4.17" | 300 x 400 | 0x4100 | BW/BWR |
| 4.37" | 480 x 176 | 0x430C | BW/BWR |

---

## 2. Índice de Proyectos en el Repositorio

### 2.1 `EPD_Driver_GU_small/` — Driver de Referencia
- **Estado**: Funcional, probado en Raspberry Pi.
- **Driver**: `EPD_Driver` (clase única, sin namespace).
- **SPI**: `bcm2835_spi_transfer()` con CS manual toggle entre bytes.
- **GPIO**: `bcm2835_gpio_fsel/write/lev` directo.
- **Pines**: BUSY=25, DC=24, RESET=23, CS=27, flashCS=22.
- **SPI Mode**: MODE0 (CPOL=0, CPHA=0), Clock Divider 256 (~976 KHz).

### 2.2 `epaper_success_v1.0.0/` — Primera Versión Limpiada
- **Estado**: Corregida pero no probada en Pi.
- **Driver**: `EPAPER::EPD_Driver` (namespace, hereda de `Gpio_t`).
- **Cambios**: CS toggle, timeouts, constructor simplificado, registros corregidos.

### 2.3 `src_epaper_nfc/` — Versión Actual NFC (Activa)
- **Estado**: Versión actual del proyecto en disco.
- **Branch**: `Bitcoin` integrado aquí.
- **Binario**: `bin/bitcoin_app` (ticker BTC/USD en display 2.66").
- **Makefile**: Compila y ejecuta con `make run`.

### 2.4 `PDLS_EXT3_Basic_Global-main/` — Librería de Referencia (Rei Vilo)
- Librería Arduino de Pervasive Displays para EXT3.
- Referencia externa para comandos COG, LUTs y secuencias.

### 2.5 `scripts_tools/` — Scripts de Utilidad
| Script | Propósito |
|:-------|:----------|
| `setGpioPinsOutput.sh` | Configura pines GPIO como salida |
| `uniquePinGpio.sh` | Verifica que no haya pines duplicados |
| `spiSettings.sh` | Muestra/configura parámetros SPI |
| `verifGpios.sh` | Verifica estado de los GPIOs |
| `runGdb.sh` | Ejecuta el binario con GDB |
| `install_libs.sh` | Instala dependencias (bcm2835, qrencode) |
| `menuConfigGpiosSuccess.sh` | Menú interactivo para configurar GPIOs |
| `check_epaper_gpio.sh` | Diagnóstico de GPIOs del display |

---

## 3. Hardware y Conexiones

### 3.1 Modelos soportados

| Modelo | Arquitectura | GPIO BCM utilizados |
|--------|-------------|---------------------|
| Raspberry Pi 2W | 64-bit (ARMv7/aarch64) | 25, 24, 23, 8, 22 |
| Raspberry Pi 4B | 64-bit | 27, 18, 17, 8, 22 |
| Raspberry Pi Zero 2W | 64-bit | 25, 24, 23, 8, 22 |

### 3.2 Tabla de Pinout completa (configuración recomendada)

| Señal | GPIO BCM | Pin Físico | Color Cable | Dirección |
|:------|:--------:|:----------:|:-----------:|:---------:|
| BUSY | 25 | 22 | Rojo | INPUT |
| D/C | 24 | 18 | Naranja | OUTPUT |
| RESET | 23 | 16 | Amarillo | OUTPUT |
| CS | 27 | 13 | Gris | OUTPUT |
| flashCS | 22 | 15 | Violeta | OUTPUT (no usado) |
| MOSI | 10 | 19 | Azul | SPI HW |
| SCLK | 11 | 23 | Marrón | SPI HW |
| MISO | 9 | 21 | Verde | SPI HW (no usado) |

### 3.3 Configuración alternativa (Others)

| Señal Panel | GPIO BCM | Pin Header RPi |
|:-----------:|:--------:|:--------------:|
| Panel_CS | 2 | 13 (SPI CE1) |
| MOSI | 12 | 19 |
| SCK | 14 | 23 |
| MISO | 13 | 21 |
| Flash_CS | 3 | 15 |
| RESET | 6 | 22 |
| D/C | 10 | 24 |
| BUSY | 11 | 26 |

### 3.4 Pinout del conector FPC del display

| Pin # | Símbolo | Descripción |
|:-----:|:-------:|-------------|
| 1 | VCC | Alimentación 3.3V |
| 2 | SCK | SPI Clock |
| 3 | BUSY | Salida de estado ocupado |
| 4 | D/C | Control Data/Command |
| 5 | RST | Entrada de reset |
| 6 | MISO | SPI MISO |
| 7 | MOSI | SPI MOSI |
| 8 | FCSM | Chip Select maestro Flash |
| 9 | ECSM | Chip Select maestro EPD |
| 10 | GND | Tierra |
| 11 | GND | Tierra |
| 12-20 | (reservados) | No conectar a GPIO |

---

## 4. Sistema de Compilación

### 4.1 Makefile

**Targets principales**:
```bash
make          # Compila todo → bin/epaper_app
make run      # Compila y ejecuta con sudo
make clean    # Limpia obj/ y bin/
make tx       # Compila como transmisor (bin/epaper_app_tx)
make rx       # Compila como receptor (bin/epaper_app_rx)
```

**Flags de compilación**:
```makefile
CC = g++
CXXFLAGS = -std=c++20 -Ilibs -Isrc -Wall -pedantic -g
LIBRARIES = -pthread -lqrencode -lbcm2835
```

### 4.2 Estructura de directorios (todos los proyectos)

```
proyecto/
├── src/
│   └── main.cpp                 # Punto de entrada
├── libs/
│   ├── epaper/
│   │   ├── epaper.h             # Driver EPD (header)
│   │   ├── epaper.cpp           # Driver EPD (implementación)
│   │   ├── epaper_display.h     # Capa de dibujo (opcional)
│   │   ├── epaper_display.cpp
│   │   └── boards.h             # Configuraciones de pines
│   ├── gpio/
│   │   └── gpio.h/.cpp          # GPIO wrapper
│   ├── tyme/
│   │   └── tyme.h/.cpp          # Delays
│   ├── qr/
│   │   └── qr_gen.h/.cpp        # Generación QR
│   ├── graphics/
│   │   └── userImageData.h      # Buffers de imagen
│   ├── fonts/                    # Solo en Master/ y Bitcoin
│   └── app/
│       └── config.h             # Detección CPU 32/64 bits
├── bash/                        # Scripts de utilidad
├── rules/                       # Reglas GDB
├── Makefile                     # Build system
├── obj/                         # Objetos compilados
└── bin/                         # Binarios
```

### 4.3 Ejemplo Makefile Bitcoin (RASPI_EPD_SRC_Bitcoin)

```makefile
CC = g++
CXXFLAGS = -std=c++20 -Ilibs -Isrc -Wall -pedantic -g
LIBRARIES = -pthread -lbcm2835

make            # → bin/bitcoin_app
make run        # → Compila + ejecuta con sudo + TZ=America/Argentina/Buenos_Aires
make clean      # → Borra obj/ y bin/
```

---

## 5. Driver EPD — Funcionamiento Interno

### 5.1 Arquitectura de capas

```
┌──────────────────────────────────┐
│      Aplicación (main.cpp)       │
│   Crea EPD_Driver, llama COG_initial,
│   globalUpdate, COG_powerOff     │
├──────────────────────────────────┤
│         EPD_Driver                │
│   - sendIndexData()              │
│   - COG_initial()                │
│   - globalUpdate()               │
│   - DCDC_powerOn()               │
│   - displayRefresh()             │
├──────────────────────────────────┤
│         Spi_t                     │
│   - bcm2835_spi_transfer()       │
│   - CS toggle manual por GPIO    │
├────────────┬─────────────────────┤
│  Gpio_t    │  TYME::delay        │
│  (bcm2835) │  (usleep/sleep)     │
├────────────┴─────────────────────┤
│         bcm2835                   │
│   Acceso a registros del SoC    │
└──────────────────────────────────┘
```

### 5.2 Configuración SPI (MODO CORRECTO)

| Parámetro | Valor | Razón |
|:----------|:------|:------|
| Modo SPI | MODE0 (CPOL=0, CPHA=0) | El COG samplea datos en flanco de subida |
| Bit Order | MSB First | Estándar SPI |
| Clock Divider | 256 (~976 KHz) | Valor de la referencia |
| CS | Manual por GPIO | `bcm2835_spi_chipSelect(BCM2835_SPI_CS_NONE)` |
| CS Toggle | Sí, entre cada byte | CRÍTICO: el COG sincroniza con flanco CS |

### 5.3 Secuencia Completa de Operación

#### Inicialización (COG_initial)
```
Timing diagram:
┌─────────────────────────────────────────────────────┐
│ Delay 5ms                                           │
├─────────────────────────────────────────────────────┤
│ RESET pulse: HIGH(5ms) → LOW(10ms) → HIGH(5ms)     │
├─────────────────────────────────────────────────────┤
│ Soft Reset: sendIndexData(0x00, &reg[1], 1)        │
│ Wait BUSY=HIGH                                      │
├─────────────────────────────────────────────────────┤
│ sendIndexData(0xE5, &reg[2], 1)   // Input Temp    │
│ sendIndexData(0xE0, &reg[3], 1)   // Active Temp   │
│ sendIndexData(0x00, &reg[4], 2)   // PSR config    │
└─────────────────────────────────────────────────────┘
```

**Registros de configuración** (array de 6 bytes):
```
register_data_sm = { 0x00, 0x0e, 0x19, 0x02, 0xcf, 0x8d };
                      [0]    [1]    [2]    [3]    [4]    [5]

Comando 0xE5 (Input Temperature):  reg[2] = 0x19 = 25°C
Comando 0xE0 (Active Temperature): reg[3] = 0x02
Comando 0x00 (PSR):               reg[4..5] = 0xcf, 0x8d
```

⚠️ **Importante**: Los índices `[2]`, `[3]`, `[4..5]` son específicos.

#### Actualización Global (globalUpdate)
```
sendIndexData(0x10, data_black, image_data_size)   // Frame negro
sendIndexData(0x13, data_red,   image_data_size)   // Frame rojo
      ↓
DCDC_powerOn()   → sendIndexData(0x04, dummy, 0) → Wait BUSY=HIGH
      ↓
displayRefresh() → sendIndexData(0x12, dummy, 0) → Wait BUSY=HIGH
```

**Tamaño de frame**: `image_data_size = screenSizeV * (screenSizeH / 8)`

| Pantalla | Resolución | Frame size (bytes) |
|:---------|:-----------|:------------------:|
| 1.54" | 152x152 | 2,888 |
| 2.13" | 212x104 | 2,756 |
| 2.66" | 296x152 | 5,624 |
| 2.71" | 264x176 | 5,808 |
| 2.87" | 296x128 | 4,736 |
| 3.70" | 416x240 | 12,480 |
| 4.17" | 300x400 | 15,000 |
| 4.37" | 480x176 | 10,560 |

#### Apagado (COG_powerOff)
```
sendIndexData(0x02, dummy, 0)   // Power OFF command
Wait BUSY=HIGH
DC = LOW
CS = LOW
Delay 150ms
RESET = LOW
```

### 5.4 Timeouts

| Operación | Timeout | Razón |
|:----------|:-------:|:------|
| `softReset()` | 5 seg | El reset debe ser rápido |
| `DCDC_powerOn()` | 5-10 seg | Encender DC/DC toma ~1-2 seg |
| `displayRefresh()` | 20-60 seg | El refresh completo puede tomar hasta 20 seg |
| `COG_powerOff()` | 5 seg | Apagado rápido |

### 5.5 Orden de inicialización de bcm2835

```cpp
// 1. PRIMERO: inicializar bcm2835
if (!bcm2835_init()) return 1;

// 2. El constructor de EPD_Driver llama a Spi_t → bcm2835_spi_begin()
//    que configura los pines SPI (GPIO 9-11) como ALT0

// 3. Usar el display...

// 4. AL FINAL: liberar recursos EN ORDEN
epaper.reset();          // destructor → ~Spi_t → bcm2835_spi_end()
bcm2835_close();         // cierra /dev/mem
```

---

## 6. Protocolo SPI

### 6.1 Configuración

| Parámetro | Valor |
|:----------|:------|
| Modo SPI | **MODE0** (CPOL=0, CPHA=0) |
| Bit Order | MSB primero |
| Clock | Divider 256 (~976 KHz) |
| CS | Manual por GPIO (`BCM2835_SPI_CS_NONE`) |

### 6.2 Formato de comando (sendIndexData)

```cpp
// ✅ CORRECTO (como en EPD_Driver_GU_small):
void sendIndexData(uint8_t index, const uint8_t *data, uint32_t len) {
    // Comando
    digitalWrite(DC, LOW);
    digitalWrite(CS, LOW);
    bcm2835_spi_transfer(index);
    digitalWrite(CS, HIGH);     // ← CS sube entre comando y datos

    // Cada byte de dato
    for (uint32_t i = 0; i < len; i++) {
        digitalWrite(DC, HIGH);
        digitalWrite(CS, LOW);
        bcm2835_spi_transfer(data[i]);
        digitalWrite(CS, HIGH); // ← CS sube entre cada byte
    }
}
```

⚠️ **CRÍTICO**: CS debe togglear entre cada byte. Sin el toggle, el COG pierde la sincronización.

### 6.3 Tabla de comandos

| CMD | Nombre | Descripción | Parámetros |
|:---:|:-------|:------------|:-----------|
| 0x00 | Soft Reset | Reinicia COG | 1 byte: PSR |
| 0x02 | Power OFF | Apaga DC/DC | 0 bytes |
| 0x04 | Power ON | Enciende DC/DC | 0 bytes |
| 0x10 | First Frame | Carga frame negro | N bytes (5,624 para 2.66") |
| 0x12 | Display Refresh | Refresca pantalla | 0 bytes |
| 0x13 | Second Frame | Carga frame rojo | N bytes (5,624 para 2.66") |
| 0xE0 | Active Temp | Temperatura activa | 1 byte |
| 0xE5 | Input Temp | Temperatura entrada | 1 byte |
| 0x50 | CDI | Charge/Discharge Interval | 1 byte (para Fast Update) |

---

## 7. Sistema de GPIO

### 7.1 Capas de abstracción

1. **GPIO::Gpio_t**: Clase genérica con gestión de colecciones (sysfs + bcm2835).
2. **EPAPER::Gpio_t**: Clase base del driver EPD, wrapper directo de bcm2835.

### 7.2 Estados de pines

| Estado | panelDC | panelCS | panelReset | panelBusy |
|:-------|:-------:|:--------|:-----------|:---------:|
| Reposo | HIGH | HIGH | HIGH | LOW |
| Comando SPI | LOW | LOW | HIGH | X |
| Dato SPI | HIGH | LOW | HIGH | X |
| Reset activo | X | HIGH | LOW | X |
| COG ocupado | X | X | X | LOW |
| COG listo | X | X | X | HIGH |

### 7.3 Configuración en COG_initial

```cpp
pinMode(panelBusy, INPUT);
pinMode(panelDC, OUTPUT);   digitalWrite(panelDC, HIGH);
pinMode(panelReset, OUTPUT); digitalWrite(panelReset, HIGH);
pinMode(panelCS, OUTPUT);   digitalWrite(panelCS, HIGH);
```

---

## 8. Depuración

### 8.1 Metodología

```
1. Pines GPIO cableados incorrectamente  ← 80% de los problemas
2. Configuración SPI incorrecta           ← 15% de los problemas
3. Protocolo COG incorrecto               ← 5% de los problemas
```

### 8.2 Macros de debug

| Macro | Módulo | Función |
|:------|:-------|:--------|
| `DBG_EPAPER` | epaper.cpp | Trazas del driver EPD |
| `DBG_SPI` | spi.cpp | Trazas de transferencias SPI |
| `DBG_GPIO` | gpio.cpp | Trazas de operaciones GPIO |

```bash
g++ -std=c++20 ... -DDBG_EPAPER -DDBG_SPI -DDBG_GPIO
```

### 8.3 Scripts de diagnóstico

```bash
sudo ./bash/verifGpios.sh settings     # Verifica GPIOs del display
sudo ./bash/spiSettings.sh              # Muestra configuración SPI
sudo ./bash/menuConfigGpiosSuccess.sh list    # Lista estado GPIOs
sudo ./bash/uniquePinGpio.sh 22         # Prueba pin individual

# GDB con reglas preconfiguradas
sudo gdb -x rules/rulesEpaper.gdb ./bin/epaper_app
```

### 8.4 Errores comunes

| Síntoma | Causa probable | Solución |
|:--------|:---------------|:---------|
| Display no responde | Permisos o bcm2835 no instalado | `sudo apt-get install libraspberrypi-dev` |
| Imagen corrupta | CS no togglea entre bytes | Revisar sendIndexData() |
| Mitad imagen corrupta | SPI Mode incorrecto | Verificar MODE0 |
| No hay comunicación SPI | bcm2835_spi_begin() no llamado | Agregar en constructor |
| Programa cuelga en BUSY | COG no responde o sin timeout | Agregar timeout de 5-20s |
| Corrupción de datos | Velocidad SPI excesiva | Reducir a 800 kHz |

---

## 9. Reparaciones y Bugs Corregidos

### Bug #1: SPI Mode Incorrecto (MODE1 vs MODE0)
| Archivo | epaper.cpp → Spi_t |
| Síntoma | Display no respondía |
| Causa | MODE1 en vez de MODE0 |
| Solución | Cambiar a MODE0 |

### Bug #2: CS sin Toggle entre Bytes
| Archivo | sendIndexData() |
| Síntoma | Display no mostraba nada o datos corruptos |
| Causa | CS se mantenía LOW durante toda la transacción |
| Solución | Toggle CS = HIGH entre cada byte |

### Bug #3: Índices Incorrectos de `register_data`
| Archivo | COG_initial() |
| Síntoma | Comportamiento errático en inicialización |
| Causa | Usar [2] para 0xE0 y [3] para 0x00 (incorrecto) |
| Solución | Usar [3] para 0xE0 y [4..5] para 0x00 |

**Correcto**:
```cpp
sendIndexData(0xE5, &register_data[2], 1);  // Input Temp: 0x19
sendIndexData(0xE0, &register_data[3], 1);  // Active Temp: 0x02
sendIndexData(0x00, &register_data[4], 2);  // PSR: 0xCF, 0x8D
```

### Bug #4: Pines GPIO Incorrectos en `boards.h`
| Archivo | boards.h |
| Síntoma | Display no respondía |
| Causa | boardRaspberryPiZero2W tenía BUSY=7, DC=8, RESET=25 |
| Solución | Corregir a BUSY=25, DC=24, RESET=23, CS=27 |

### Bug #5: `bcm2835_spi_begin()` Faltante
| Archivo | Spi_t constructor |
| Síntoma | SPI no transfería datos |
| Causa | No se llamaba bcm2835_spi_begin() |
| Solución | Agregar en constructor |

### Bug #6: CS Configurado como SPI Hardware
| Archivo | Spi_t constructor |
| Síntoma | CS no controllable |
| Causa | No llamar `bcm2835_spi_chipSelect(BCM2835_SPI_CS_NONE)` |
| Solución | Deshabilitar CS hardware |

### Bug #7: `DCDC_powerOn()` y `displayRefresh()` con Datos Extra
| Archivo | epaper.cpp |
| Síntoma | Comandos enviados con byte extra |
| Causa | `sendIndexData(0x04, &register_data[0], 1)` |
| Solución | `sendIndexData(0x04, &dummy, 0)` — solo comando |

### Bug #8: `COG_powerOff()` Escribiendo en BUSY
| Archivo | epaper.cpp |
| Síntoma | Conflicto potencial |
| Causa | `digitalWrite(panelBusy, LOW)` — BUSY es INPUT |
| Solución | Eliminar la línea |

### Bug #9: `~Spi_t()` llamando a `exit(EXIT_SUCCESS)`
| Archivo | spi.cpp (legacy, no compilado) |
| Síntoma | Muerte súbita del proceso |
| Causa | `exit(EXIT_SUCCESS)` en destructor |
| Solución | Código no usado, excluir del build |

### Bug #10: `bcm2835_spi_end()` llamado dos veces
| Archivo | main.cpp |
| Síntoma | Doble liberación de recursos |
| Causa | ~Spi_t() y main() llamaban a spi_end |
| Solución | Usar bloque {} para controlar ciclo de vida |

### Matriz de correcciones por proyecto

| Bug | origin/ | v1.0.0 | v1.0.1 | Master/ |
|:---:|:-------:|:------:|:------:|:-------:|
| #1 SPI Mode | ❌ | ✅ | ✅ | ✅ |
| #2 CS Toggle | ❌ → ✅ | ✅ | ✅ | ✅ |
| #3 Register indices | ❌ | ✅ | ✅ | ⚠️ ([2],[3]) |
| #4 Pines | ❌ → ✅ | ✅ | ✅ | ✅ |
| #5 spi_begin | ❌ → ✅ | ✅ | ✅ | ✅ |
| #6 CS_NONE | ❌ → ✅ | ✅ | ✅ | ✅ |
| #7 Datos extra | ❌ | ✅ | ✅ | ✅ |
| #8 BUSY write | ❌ | ✅ | ✅ | ✅ |
| #9 exit() | ❌ | ❌ | ❌ | ❌ |
| #10 Doble spi_end | ❌ → ✅ | ✅ | ✅ | ✅ |

---

## 10. Aplicaciones Especializadas

### 10.1 Bitcoin Ticker (`src_epaper_nfc/`)

Aplicación que muestra el precio BTC/USD en tiempo real en un display 2.66" (296x152).

**Características**:
- Precio desde CoinGecko API (cada 60s)
- Zona horaria Argentina (ART)
- 6 fuentes bitmap integradas
- Fast Update con `softReset()` obligatorio
- Inicial con `globalUpdate`, luego `fastUpdate` para evitar flash

**Build y ejecución**:
```bash
cd src_epaper_nfc
make
sudo TZ=America/Argentina/Buenos_Aires ./bin/bitcoin_app
```

**fetchBtcPrice()**:
```cpp
// curl -s --connect-timeout 5 --max-time 10
//   'https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=usd'
// Parse JSON: {"bitcoin":{"usd":64375.0}}
// Timeout 5s conexión, 10s total
```

**update()**:
```cpp
bool update() {
    if (!hasContentChanged()) return false;  // memcmp buffers
    m_driver->fastUpdate(m_prevBuffer, m_buffer);
    memcpy(m_prevBuffer, m_buffer, m_bufferSize);
    return true;
}
```

**Fuentes disponibles**:

| FontType | Ancho | Alto | Caracteres | Bytes/char |
|:---------|:-----:|:----:|:----------:|:----------:|
| FONT_8x8 | 8 | 8 | 0-127 | 8 |
| FONT_5x8 | 5 | 8 | 0-127 | 5 |
| FONT_7x8_THICK | 7 | 8 | 32-127 | 7 |
| FONT_4x8_SEG | 4 | 8 | 32-122 | 4 |
| FONT_8x8_WIDE | 8 | 8 | 32-90 | 8 |
| FONT_3x8_TINY | 3 | 8 | 32-126 | 3 |
| FONT_7x8_HOMESPUN | 7 | 8 | 32-126 | 7 |
| FONT_16x32_BIGNUM | 16 | 32 | 48-58 ('0'-'9'+':') | 64 |
| FONT_16x16_MEDNUM | 16 | 16 | 48-58 | 32 |

### 10.2 QR Codes

**Librería**: libqrencode

**Clase**: `QR::Qr_gen_t`

**Formato WiFi**:
```
WIFI:T:<auth_type>;S:<SSID>;P:<password>;H:<hidden>;;
```

**Factor de escala recomendado**: 5
- QR 21px: 105x105 (cómodo en 152x296)
- QR 29px: 145x145 (óptimo para 2.66")
- QR 37px: 185x185 (requiere pantalla >= 2.87")

**Uso**:
```cpp
QR::Qr_gen_t qrGen;
qrGen.drawQRCode("WIFI:T:WPA;S:MiRed;P:Clave;;", 5);
epaper->globalUpdate(qrGen.imageBuffer, BW_0x00Buffer);
```

---
### 10.3 NFC Reader (`src_epaper_nfc/` — `nfc_app`)

Aplicación que lee tags/tarjetas NFC con un lector PN532 conectado por UART y muestra el UID en el E-Paper 2.66" (296x152).

**Estado**: Código generado, listo para compilar en `src_epaper_nfc/`.

**Hardware**:
- Raspberry Pi (Pi 2W, 3B/4B, Zero 2W, 5)
- Módulo lector NFC PN532 (UART)
- Pantalla E-Paper Pervasive Display 2.66" (296x152)

**Dependencias**:
- bcm2835 (GPIO + E-Paper)
- Sin dependencias adicionales para NFC (UART nativo)

**Build y ejecución**:
```bash
cd src_epaper_nfc
make
sudo make run
```

Equivale a ejecutar:
```bash
sudo ./bin/nfc_app
```

**Comportamiento**:
- Inicializa el E-Paper con full refresh global.
- Abre el UART del PN532 y lo inicializa (SAMConfiguration en modo normal).
- Hace polling continuo de tags/tarjetas NFC.
- Muestra en el display:
  - Título `NFC READER`
  - Separador
  - `Acerca una tarjeta NFC o tag al lector PN532` mientras espera
  - `Tag detectado:` + UID en formato `XX:XX:XX:...` cuando lee
  - Tipo de tag (`Type A` u otro) y cantidad de bytes del UID
  - `(retire la tarjeta)` tras lecturas estables

**Estabilidad de lectura**: requiere 3 polls consecutivos con el mismo UID para mostrar, evitando parpadeos por ruido en el aire.

---

### 10.3 NFC Reader (`src_epaper_nfc/` — `nfc_app`)

Aplicación que lee tags/tarjetas NFC con un lector PN532 conectado por UART y muestra el UID en el E-Paper 2.66" (296x152).

**Estado**: Código generado, listo para compilar en `src_epaper_nfc/`.

**Hardware**:
- Raspberry Pi (Pi 2W, 3B/4B, Zero 2W, 5)
- Módulo lector NFC PN532 (UART)
- Pantalla E-Paper Pervasive Display 2.66" (296x152)

**Dependencias**:
- bcm2835 (GPIO + E-Paper)
- Sin dependencias adicionales para NFC (UART nativo)

**Estructura de módulo**:
- `src_epaper_nfc/libs/nfc/nfc.h` — interfaz del lector
- `src_epaper_nfc/libs/nfc/nfc.cpp` — implementación UART PN532
- `src_epaper_nfc/src/main_nfc.cpp` — integración NFC + E-Paper
- Binario: `bin/nfc_app`

**Makefile**:
- `make` → `bin/nfc_app`
- `make run` → compila y ejecuta `sudo ./bin/nfc_app`

**Cableado PN532 (UART)**:

| PN532 | Raspberry Pi | Nota |
|-------|--------------|------|
| VCC | 3.3V | No conectar a 5V |
| GND | GND | |
| TXD | GPIO15 (UART TX) / Pin 10 | |
| RXD | GPIO14 (UART RX) / Pin 8 | |
| | | Irq/Reset no usados obligatorios |

**UART a usar**: `/dev/ttyS0` (serial0) en la mayoría de los modelos.

**Secuencia de inicialización**:
1. `NfcReader::open()` — abre UART a 115200 baud, 8N1, raw.
2. `NfcReader::init()` — envía `SAMConfiguration` (modo normal).
3. Loop: `poll()` envía `InListPassiveTarget` y espera respuesta.

**Polling**:
- `InListPassiveTarget` (106A, 1 max target)
- Si respuesta: extrae longitud UID, tipo y bytes.
- Requiere 3 lecturas estables antes de mostrar en pantalla.

**Formato del UID en display**:

```
XX:XX:XX:XX:XX:XX
```

**Comportamiento en pantalla**:
- Título `NFC READER`
- Texto `Acerca una tarjeta NFC o tag al lector PN532` mientras espera
- Al leer: `Tag detectado:`, UID, bytes y tipo
- `(retire la tarjeta)` cuando se mantiene estable

**Compilación y ejecución**:
```bash
cd src_epaper_nfc
make clean
make
sudo make run
```

**Solución de problemas**:
- `ERROR opening NFC device` → revisar cableado TXD/RXD (cruzados), puerto UART habilitado en `/boot/config.txt` con `enable_uart=1`.
- `NFC init failed` → alimentación 3.3V estable, GND común, baud rate correcto.
- No lee tags → acercar más la tarjeta, revisar que sea 13.56 MHz (Mifare Classic/Ultralight, etc.).

---

## 11. Fast Update

### 11.1 Modos de actualización

| Modo | Descripción | Tiempo | Calidad |
|:-----|:------------|:------:|:-------:|
| Normal | Waveform completa (inverse → shaking → imaging) | 5-20s | Mejor |
| Fast | Waveform corta, solo píxeles cambiados | ~1s | Buena |

### 11.2 Diferencias clave

| Componente | Normal | Fast |
|:-----------|:------:|:----:|
| Temperatura init | `0xE5, temp` | `0xE5, temp \| 0x40` |
| PSR init | `0x00, 0xCF, 0x8D` | `0x00, 0xDF, 0x8F` |
| CDI | No se envía | `0x50, 0x07` |
| Frame 0x10 | NEW image | **OLD image** (en pantalla) |
| Frame 0x13 | 0x00 (dummy) | **NEW image** (a mostrar) |
| Refresh | 0x12 | 0x12 (igual) |

### 11.3 Fast Update para 2.66" (E266)

**Valores**:
- PSR normal: `0xCF, 0x8D`
- PSR fast: `0xCF | 0x10 = 0xDF`, `0x8D | 0x02 = 0x8F`
- Temperatura: `0x19 | 0x40 = 0x59` (a 25°C)

**Flujo completo**:
```
1. Reset + Power on COG (igual que normal)
2. Soft-reset: SPI(0x00, 0x0E)
3. Input Temperature: SPI(0xE5, temp | 0x40)
4. Active Temperature: SPI(0xE0, 0x02)
5. Panel Settings: SPI(0x00, 0xDF, 0x8F)
6. CDI: SPI(0x50, 0x07)
7. SPI(0x10, oldImage, 5624)  ← IMAGEN VIEJA
8. SPI(0x13, newImage, 5624)  ← IMAGEN NUEVA
9. Power on: SPI(0x04)
10. wait BUSY = HIGH
11. Display Refresh: SPI(0x12)
12. wait BUSY = HIGH
```

⚠️ **CRÍTICO**: En Fast Update los frames están invertidos respecto a Normal.

---

## 12. Deploy Remoto

### 12.1 vía SSH

```bash
ssh $USER@$HOSTNAME "cd /home/pi/src/epaper_rpi && git pull && make clean && make -j4 && sudo make run"
```

### 12.2 vía rsync

```bash
rsync -avz --exclude='bin/' --exclude='obj/' \
  /ruta/local/epaper_success_v1.0.1/ \
  pi@raspi.local:~/src/epaper_rpi/epaper_success_v1.0.1/

ssh pi@raspi.local "cd ~/src/epaper_rpi/epaper_success_v1.0.1 && \
  make clean && make -j4 && sudo ./bin/epaper_app"
```

### 12.3 Compilación remota con contraseña

```bash
PASSWORD="tu_password"
FOLDER="epaper_success_v1.0.1"
sshpass -p $PASSWORD ssh $USER@$HOSTNAME "cd /home/pi/src/epaper_rpi && \
  git pull && make -C ${FOLDER} clean && make -C ${FOLDER} && make -C ${FOLDER} run"
```

---

## 13. Referencias Rápidas

### 13.1 Comandos de compilación

```bash
make            # Compila bin/nfc_app
make run        # Compila y ejecuta con sudo ./bin/nfc_app
make clean      # Limpia obj/ y bin/
```

### 13.2 Dependencias del sistema

```bash
sudo apt-get install libraspberrypi-dev -y   # para bcm2835
sudo apt-get install libqrencode-dev -y      # para QR codes
sudo apt-get install gpiod -y                # para gpioinfo (debug)
```

### 13.3 Permisos

```bash
sudo usermod -aG gpio $USER      # Acceso a /sys/class/gpio
sudo usermod -aG dialout $USER   # Acceso a /dev/spidev0.0 y /dev/ttyS*
```

Al usar PN532 por UART, verificar:

```bash
# Habilitar UART hardware en la Pi:
sudo raspi-config
# Interface Options -> Serial -> Login shell NO -> Serial port YES

# O editar /boot/config.txt y /boot/cmdline.txt:
enable_uart=1
dtoverlay=disable-bt  # si se usa mini-uart, deshabilitar Bluetooth para liberar /dev/ttyS0

# Reiniciar y verificar dispositivo:
ls -l /dev/ttyS0 /dev/ttyAMA0 /dev/serial0
```

> Importante: asegurar GND común entre PN532 y Raspberry Pi.

> Importante: PN532 opera a 3.3V. No conectar a 5V.

> Nota: si usas módulo NFC basado en PN532 con level shifter o adaptador USB-Serial, ajustar la ruta del dispositivo en `main_nfc.cpp`.

---

Ahora actualizo la sección correspondiente en `src_epaper_nfc/Makefile`.

El flujo actual es:
- `make` → compila `bin/nfc_app`
- `sudo make run` → ejecuta la app

Nota importante sobre `main_nfc.cpp`: el programa retorna código 1 si el UART no se puede abrir como /dev/ttyS0, y reintenta automáticamente `/dev/ttyAMA0` y `/dev/serial0`.

Para ejecutar:

```bash
cd src_epaper_nfc
make clean && make
sudo make run
```

Equivale a:

```bash
sudo ./bin/nfc_app
```

---

## 14. Ejemplo mínimo RC (Run-Component)

### Lectura NFC → E-Paper (src_epaper_nfc)

```cpp
NFC::NfcReader nfc("/dev/ttyS0");
if (!nfc.open()) { /* ... */ }
if (!nfc.init()) { /* ... */ }

NFC::NfcTag tag;
while (running) {
    if (nfc.poll(tag)) {
        std::string uid = nfc.uidToString(tag);
        display->clearScreen(true);
        display->drawCenteredString(20, "NFC READER", /* ... */ true);
        display->drawCenteredString(40, "Tag detectado:", /* ... */ true);
        display->drawCenteredString(55, uid, /* ... */ true);
        display->update();
    }
    usleep(200000);
}
```

---

## 15. Referencias rápidas útiles

```bash
# Habilitar UART (I cart)
sudo raspi-config

# Ver puertos seriales
ls -l /dev/ttyS0 /dev/ttyAMA0 /dev/serial0

# Verificar logs UART
sudo dmesg | grep tty

# Escanear UART
sudo cat /dev/ttyS0
```

```bash
# Compilación rápida de app NFC
cd src_epaper_nfc
make
sudo ./bin/nfc_app
```

---

## Notas

- Las macros `DBG_EPAPER`, `DBG_SPI`, `DBG_GPIO` siguen aplicando para depurar el subsistema E-Paper.
- Para NFC no hay trazas condicionales implementadas por ahora; se ejecuta sin logging interno.
- El módulo `NFC::NfcReader` es autocontenido y puede reutilizarse en otros proyectos Raspberry + UART.
- Si el PN532 tiene interrupciones por IRQ, sería necesario usar `select()` con timeout condicional por `poll()`. La versión actual usa polling bloqueante con timeout.

Fin de `docs/SKILLS.md`.

Al usar PN532 por UART, verificar:

```bash
# Habilitar UART hardware en la Pi:
sudo raspi-config
# Interface Options -> Serial -> Login shell NO -> Serial port YES

# O editar /boot/config.txt y /boot/cmdline.txt:
enable_uart=1
dtoverlay=disable-bt  # si se usa mini-uart, deshabilitar Bluetooth para liberar /dev/ttyS0

# Reiniciar y verificar dispositivo:
ls -l /dev/ttyS0 /dev/ttyAMA0 /dev/serial0
```

### 13.4 Versionado

- `VERSION` en la raíz del proyecto: **1.5.0**
- Debe coincidir con el tag de git: `git tag v1.5.0`

```bash
cat VERSION         # → 1.5.0
git describe --tags # → v1.5.0
```

### 13.5 Archivos de documentación

| Documento | Contenido |
|:----------|:----------|
| `README.md` | Descripción general del proyecto |
| `instalacion.md` | Guía de instalación y dependencias |
| `hardware.md` | Conexiones físicas, pinout y tablas de wiring |
| `arquitectura.md` | Diseño arquitectural, capas y flujo de datos |
| `api.md` | Referencia completa de API y clases |
| `spi-protocolo.md` | Especificación del protocolo SPI |
| `gpio.md` | Sistema de gestión de GPIO |
| `build.md` | Sistema de compilación Makefile |
| `debugging.md` | Depuración con GDB y scripts auxiliares |
| `pinout-display.md` | Pinout del conector FPC del display |
| `qr-codes.md` | Generación de códigos QR |
| `datasheets.md` | Referencia a datasheets y documentos técnicos |
| `aprendizaje-pdf-vs-implementacion.md` | Lecciones aprendidas |
| `CHANGELOG.md` | Historial de cambios |
| `TODO.md` | Tareas pendientes |
| `REMOTE_BUILD.md` | Compilación remota |
| `EPAPER_FAST_UPDATE.md` | Documentación técnica de Fast Update |
| `SKILL_PROYECTOS.md` | Índice de proyectos del repositorio |
| `SKILL_BUILD_DEPLOY.md` | Build system y deploy remoto |
| `SKILL_REPARACIONES.md` | Bugs encontrados y corregidos |
| `SKILL_DRIVER_EPD.md` | Funcionamiento interno del driver |
| `SKILL_DEBUG_SPI_GPIO.md` | Depuración de SPI y GPIO |
| `SKILL_BTC.md` | Bitcoin ticker en E-Paper |
| `SKILL_NFC_READER.md` | NFC Reader PN532 → E-Paper |
| `SKILL_NFC_READER.md` | NFC Reader PN532 → E-Paper |
| `SKILL_NFC_READER.md` | NFC Reader PN532 → E-Paper |

---

## 14. Ejemplo de Uso Completo

```cpp
#include <iostream>
#include <memory>
#include <epaper/epaper.h>
#include <epaper/boards.h>
#include <graphics/userImageData.h>
#include <tyme/tyme.h>

int main() {
    // 1. Inicializar hardware
    if (!bcm2835_init()) {
        std::cerr << "Error bcm2835_init()" << std::endl;
        return 1;
    }

    // 2. Crear driver
    auto epaper = std::make_unique<EPAPER::EPD_Driver>(
        EPAPER::eScreen_EPD_266,
        EPAPER::boardRaspberryPiZero2W
    );

    // 3. Inicializar COG
    epaper->COG_initial();
    epaper->printGpios();

    // 4. Actualizaciones
    epaper->globalUpdate(BW_monoBuffer, BW_0x00Buffer);
    TYME::delay(900);
    epaper->globalUpdate(BWR_blackBuffer, BWR_redBuffer);

    // 5. Apagar
    epaper->COG_powerOff();
    bcm2835_close();

    return 0;
}
```

```bash
# Compilar y ejecutar
make
sudo ./bin/epaper_app
```
