# TODO — E-Paper NFC Reader 2.66"

## Actual

- [x] Módulo NFC PN532 por UART (`libs/nfc/`)
- [x] Integración E-Paper 2.66" Pervasive Display (`libs/epaper/`)
- [x] App principal `nfc_app` (`src/main_nfc.cpp`)
- [x] Makefile actualizado + `.gitignore`
- [x] Repo git público + rama `main` + remote `siliconvalleyar-oss/epaper_nfc_rpi`
- [x] Tag `v1.5.0` pusheado, `VERSION=1.5.0`
- [x] Documentación `docs/SKILL_NFC_READER.md` y `docs/REMOTE_BUILD.md` saneados
- [x] Código muerto eliminado (Bitcoin legacy, fonts muertas, GPIO legacy, Tyme, config.h)

## Corto plazo

- [ ] Mostrar en display **Nombre + RUT + Carrera** del alumno (reemplazar `uidToHexString` por lookup)
- [ ] Persistir lectura en archivo local CSV/JSON (`logs/` con timestamp + UID)
- [ ] Agregar QR de comprobante con datos formateados (libqrencode)
- [ ] Manejo de señal `SIGINT/SIGTERM` con apagado limpio
- [ ] Lista blanca de UIDs permitidos o estado de acceso (`ACEPTADO`/`RECHAZADO`)
- [ ] Timeout configurable para auto-cierre de lectura en display

## Mediano plazo

- [ ] Generar y cachear **registro/charla** en backend HTTP POST
- [ ] Web Record Viewer local (servidor HTTP en RPi para ver últimas lecturas)
- [ ] Sincronización WiFi periódica (cola offline)
- [ ] Modo demo sin hardware NFC (simula UID aleatorios para pruebas)

## Largo plazo

- [ ] Multi-lector / multi-display
- [ ] Firmware de lector actualizable OTA
- [ ] Dashboard analytics desde historial

## Bugs conocidos / Deuda técnica

- [ ] `uidToString()` declarada en `libs/nfc/nfc.h` pero no implementada en `.cpp`
- [ ] Falta manejo de excepciones en E-Paper (si bcm2835 falla)
- [ ] `SIGPIPE` no capturado en modo HTTP
- [ ] No hay re-exposición tras `COG_powerOff()` (requiere reinicio app)

## Versionado

Sigue reglas de `docs/LEARNINGS.md`:
- `VERSION` debe coincidir con último tag publicado
- Mensajes de commit en conventional commits (`feat:`, `fix:`, `docs:`...)
