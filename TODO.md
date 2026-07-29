# TODO - Proyecto epaper_nfc_rpi

## Mifare Classic 1K - Capacidades y Features a implementar

El lector ya detecta Mifare Classic 1K (`ATQA: 0x04:0x00`, `SAK: 0x08`) y muestra UID, ATQA, SAK, tipo y QR en el e-Paper.

### Capacidades técnicas del tag (Mifare Classic 1K)
- **ECC (Conditional Event Control)**: permite bloques de control y acceso condicional.
- **16 sectores × 4 bloques/bloque = 64 bytes/bloque = 4096 bytes totales.
- **Sector trailer (último bloque de cada sector)**: claves A/B, bits de acceso, datos de configuración.
- **Claves de autenticación**: 6 bytes por clave (A y B). Clave por defecto: `FFFFFFFFFFFF`.
- **Acceso condicional**: ACCBITS (Access Condition Bits) definen qué operaciones permiten lectura/escritura por sector.

### Features implementables próximas

#### Escaneo y visualización básica
- [ ] Mostrar **capacidad total** del sector y bloque actual en el display.
- [ ] Leer **datos raw de un bloque** sector/bloque específico y mostrar hexdump en pantalla.
- [ ] Mostrar **clave A** (formato oculto/oscurcido) y **bits de acceso** del sector trailer en el display.
- [ ] Resumen de ** ACCBITS** por sector: READ/WRITE/INCREMENT/DECREMENT permitido.
- [ ] Modo debug: mostrar tabla de sectores/bloques en e-Paper con navegación por botón/tecla.

#### Autenticación y manipulación
- [ ] Autenticar con clave conocida (default `FFFFFFFFFFFF`) y leer bloque sector 0.
- [ ] Autenticar y mostrar **UID real del sector 0** (BCC, NVB, datos de fabricante).
- [ ] Implementar **clave personalizada** para un sector (placeholder en config).
- [ ] Mostrar **access conditions** después de autenticación exitosa.

#### Interacción avanzada
- [ ] Modo "emulación": usar datos del tag para generar salida JSON/HTTP POST a backend (similar a `pn532_lib/payment.cpp`) al mantener tag X segundos.
- [ ] **Historial de accesos** por UID: guardar en SD/USB/archivo local y mostrar en e-Paper cuántas veces se leyó cada UID.
- [ ] Modo "copiar/pegar UID": leer datos de un tag y mostrar como string/base64 en QR (para transferir datos de acceso).
- [ ] Implementar **anti-collisión mejorada**: mostrar número de tarjetas detectadas simultáneamente.

#### UI/UX mejorada
- [ ] Navegación en e-Paper: mostrar resumen de sector y permitir avanzar/reversar con botones físicos (GPIO).
- [ ] Indicador visual en display de **sectores públicos vs. privados**.
- [ ] Pantalla de diagnóstico: mostrar ATQA/SAK/sector trailer en tiempo real sin necesidad de avanzar manualmente.

---

## Prioridades sugeridas

1. **Leer y mostrar dump de sector 0** (accsesible sin autenticación en la mayoría de tags) → info básica visible sin clave.
2. **Autenticar con `FFFFFFFFFFFF` y leer datos reales** → lectura general.
3. **Mostrar bits de acceso** → información de seguridad inmediata sin autenticación.
4. **Historial de UIDs** → feature diferenciada y útil para auditoría.

---

## Notas técnicas

- **Dependencia principal**: `libnfc` ya maneja autenticación Mifare Classic a través de `nfc_initiator_mifare_authenticate()`.
- **Si se necesita lectura de bloques protegidos**: implementar en `libs/nfc/NfcReader.cpp` método `readMifareBlock(sector, block, clave)`.
- **ACC BITS parsing**: definir helper que devuelva `{readable, writable, incrementable}` por sector trailer.
- **Display limitations**: el e-Paper de 2.66" (296×152) es B/W/R; priorizar información legible en monocromo para optimizar refresco.
