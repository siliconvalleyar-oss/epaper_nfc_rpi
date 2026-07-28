# SKILL: NFC Reader PN532 → E-Paper

Aplicación lista para compilar en `src_epaper_nfc/` que muestra en el display E-Paper Pervasive Display el UID del tag/tarjeta detectado por un lector PN532 conectado por UART a la Raspberry Pi.

---

## 1. Visión General

La aplicación integra dos subsistemas:

- **E-Paper** (pantalla 2.66" / 296x152): driver COG por SPI, actualización global inicial + refrescos differentiales (fast update) luego.
- **NFC** (PN532 por UART 115200 8N1): polling continuo de tags/tarjetas 106A, extracción de UID y unificado en display.

---

## 2. Hardware

### 2.1 Raspberry Pi

Soportada:
- Raspberry Pi 2W, 3B, 4B, 5
- Raspberry Pi Zero 2W

### 2.2 E-Paper

- Diagonal: 2.66"
- Resolución: 296 x 152 px
- Conexión SPI0 (GPIO10 MOSI, GPIO11 SCLK)
- Pines control: GPIO25 BUSY, GPIO24 D/C, GPIO23 RESET, GPIO27 CS

Cableado documentado en `docs/hardware.md`.

### 2.3 PN532

| PN532 Pin | Raspberry Pi | Nota |
|-----------|--------------|------|
| VCC | 3.3V | **No 5V** |
| GND | GND | obligatorio común |
| TXD | GPIO15 (UART TX) Pin 10 | Conectar a RXD del PN532 |
| RXD | GPIO14 (UART RX) Pin 8 | Conectar a TXD del PN532 |
| IRQ | No conectado | (opcional, no usado) |
| RST | No conectado | (opcional, no usado) |

> Nota: PN532 opera a 3.3V. Las líneas UART son directas (no usar level shifter).

---

## 3. Software

### 3.1 Módulo NFC

Archivos:
- `src_epaper_nfc/libs/nfc/nfc.h`
- `src_epaper_nfc/libs/nfc/nfc.cpp`

Clase principal: `NFC::NfcReader`.

Responsabilidades:
- Abrir UART a 115200 8N1.
- Inicializar módulo con SAMConfiguration en modo normal.
- Detectar tags pasivos 106A con `InListPassiveTarget`.
- Devolver UID y tipo.

### 3.2 Integración con E-Paper

Archivo:
- `src_epaper_nfc/src/main_nfc.cpp`

Flujo:
1. `bcm2835_init()`
2. Crear `EpaperDisplay` 2.66" + `boardRaspberryPi`
3. `display->init()`: COG_initial + globalUpdate inicial
4. Crear `NfcReader`
5. Loop principal:
   - Limpiar pantalla
   - Dibujar título `NFC READER`
   - `nfc.poll(tag)` con timeout ~500 ms
   - Si tag: esperar 3 lecturas estables, dibujar UID en pantalla
   - Mostrar: `Tag detectado:`, UID en `XX:XX:...`, tipo y bytes
   - `display->update()` (fastUpdate diferencial)
   - Esperar 200 ms

### 3.3 Estabilidad de lectura

Para evitar parpadeos por ruido electromagnético o múltiples lecturas ruidosas, se requiere **3 lecturas estables** (mismo UID) antes de renderizar el tag en pantalla.

### 3.4 Señales SIGINT / SIGTERM

La aplicación captura `SIGINT` y `SIGTERM`. Al recibirlas, sale del loop, libera buffers y cierra bcm2835 limpio.

---

## 4. Build System

Makefile actualizado (`src_epaper_nfc/Makefile`):
- Detecta automáticamente `libs/*/*.cpp` (incluye nuevo módulo `nfc.cpp`).
- Target por defecto: `bin/nfc_app`.
- `make run` → `sudo ./bin/nfc_app`.

---

## 5. Ejecución

```bash
cd src_epaper_nfc
make clean
make
sudo make run
```

O directo:
```bash
sudo ./bin/nfc_app
```

El binario requiere `sudo` porque:
- bcm2835 accede a `/dev/mem`
- El UART requiere permisos de acceso a `/dev/ttyS0`

---

## 6. Comportamiento esperado en pantalla

### Esperando tag:

```
        NFC READER
  ------------------------
   Acerca una tarjeta NFC
     o tag al lector PN532
   
         Esperando...
```

### Tag detectado (ejemplo UID de 4 bytes tipo A):

```
        NFC READER
  ------------------------
         Tag detectado:
         04:A3:2B:C1
       UID: 4 bytes
         Type A
      (retire la tarjeta)
```

---

## 7. Dependencias NFC (sin dependencias externas)

No requiere `libnfc`. La capa `NFC::NfcReader` implementa el protocolo PN532 sobre UART directamente.

Dispositivos soportados probables por protocolo:
- Mifare Classic, Ultralight, NTAG (ISO14443A)
- ISO14443A genérico (ATQA/SAK decodificables en capas superiores)

> Nota: El driver actual solo lee el UID. No realiza autenticación Mifare Classic ni lectura de bloques NDEF. Es el punto de partida para extensiones.

---

## 8. Próximas extensiones

- Leer contenido NDEF del tag (texto, URI, vCard).
- Autenticación Mifare Classic (clave A/B).
- Escritura de tags (NDEF Data Write).
- Múltiples lecturas consecutivas en bucle (cola).
- Interacción con API backend (POST UID leído).
- Formateo multilínea / paginado del UID largo.

---

## 9. Resumen de uso recomendado

```bash
# 1. Conectar hardware E-Paper (SPI) y PN532 (UART)
# 2. Verificar UART en la Pi:
ls -l /dev/ttyS0 /dev/serial0

# 3. Compilar:
cd src_epaper_nfc
make clean && make

# 4. Ejecutar y probar:
sudo ./bin/nfc_app

# 5. Acercar tarjeta/tag al PN532 → UID aparece en E-Paper.
```
