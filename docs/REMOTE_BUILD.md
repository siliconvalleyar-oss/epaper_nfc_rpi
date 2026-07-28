# Compilación Remota en Raspberry Pi

USER="pi"
HOSTNAME="raspi.local"
REPO_DIR="/home/pi/src/epaper_nfc_rpi"
APP_DIR="${REPO_DIR}/src_epaper_nfc"

## Prerrequisitos

### En la Raspberry Pi

1. Instalar dependencias:
```bash
sudo apt-get update
sudo apt-get install -y build-essential libbcm2835-dev
```

2. Habilitar SSH:
```bash
sudo raspi-config
# Interface Options -> SSH -> Enable
```

3. Configurar usuario y contraseña (ejemplo):
```bash
# Usuario de ejemplo: pi
# Contraseña de ejemplo: raspberry
sudo passwd pi
```

### En tu máquina local

1. Instalar SSH client (si no lo tienes):
```bash
# Linux/Mac
sudo apt-get install openssh-client

# Windows (PowerShell)
Add-WindowsCapability -Online -Name OpenSSH.Client~~~~0.0.1.0
```

2. Configurar acceso sin contraseña (opcional pero recomendado):
```bash
# Generar clave SSH
ssh-keygen -t rsa -b 4096

# Copiar clave a la Raspberry
ssh-copy-id $USER@$HOSTNAME
```

## Compilación y Ejecución Remoto

### Comando básico

```bash
ssh $USER@$HOSTNAME "cd ${APP_DIR} && git pull origin main && make clean && make -j4 && sudo make run"
```

### Desglose del comando

| Parte | Descripción |
|-------|-------------|
| `ssh $USER@$HOSTNAME` | Conectar a la Raspberry Pi |
| `cd ${APP_DIR}` | Ir al directorio de la app NFC |
| `git pull origin main` | Actualizar código desde la rama main |
| `make clean` | Limpiar archivos compilados anteriores |
| `make -j4` | Compilar usando 4 cores del procesador |
| `sudo make run` | Ejecutar `bin/nfc_app` (necesita root para GPIO/SPI/UART) |

### Variaciones útiles

```bash
# Solo compilar (sin ejecutar)
ssh $USER@$HOSTNAME "cd ${APP_DIR} && git pull origin main && make clean && make -j4"

# Ejecutar sin recompilar
ssh $USER@$HOSTNAME "cd ${APP_DIR} && sudo ./bin/nfc_app"

# Compilar con modo debug
ssh $USER@$HOSTNAME "cd ${APP_DIR} && make clean && make -j4 CXXFLAGS='-std=c++20 -Ilibs -Isrc -Wall -pedantic -g -DDBG_EPAPER'"

# Ver logs de compilación
ssh $USER@$HOSTNAME "cd ${APP_DIR} && make clean && make -j4 V=1"
```

## Compilación con contraseña (sshpass)

> Nota: esta opción almacena credenciales en texto plano.
> Solo usar en redes confiables.

```bash
PASSWORD="raspberry"

# Compilar y ejecutar src_epaper_nfc/
sshpass -p "$PASSWORD" ssh $USER@$HOSTNAME "cd ${APP_DIR} && git pull origin main && make clean && make -j4 && sudo make run"
```

## Estructura del Proyecto

```
epaper_nfc_rpi/
├── docs/
├── scripts_tools/
├── src_epaper_nfc/
│   ├── Makefile
│   ├── src/
│   │   └── main_nfc.cpp
│   └── libs/
│       ├── epaper/
│       ├── fonts/
│       └── nfc/
├── VERSION
└── README.md
```

## Solución de Problemas

### Error: "Permission denied"
```bash
# Asegurate de usar sudo para ejecutar
sudo ./bin/nfc_app
```

### Error: "bcm2835: Unable to open /dev/mem"
```bash
# Necesitas permisos de root
sudo ./bin/nfc_app
```

### Error: "No abre /dev/ttyS0"
```bash
# Habilitar UART en la Pi:
sudo raspi-config
# Interface Options -> Serial -> Login shell NO -> Serial port YES

# Verificar dispositivo:
ls -l /dev/ttyS0 /dev/ttyAMA0 /dev/serial0
```

### Error: "git pull" falla
```bash
# Verificar conexión a internet
ping google.com

# Verificar que el repositorio existe
ls -la ${REPO_DIR}
```

## Notas Importantes

- **Siempre ejecutar con `sudo`** para acceder a GPIO, SPI y UART
- **Verificar conexiones físicas** antes de ejecutar:
  - E-Paper SPI: MOSI, SCK, BUSY, D/C, RESET, CS
  - PN532 UART: TXD -> RXD, RXD -> TXD, GND -> GND, 3.3V -> VCC
- **El binario es `bin/nfc_app`**
- **Usar `bcm2835_spi_begin()`** después de `bcm2835_init()` para inicializar SPI
