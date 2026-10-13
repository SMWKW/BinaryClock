# Binary Clock

**Zeit, in Bits gedacht.**
Dieses Projekt ist die Firmware-Basis für eine Binäruhr auf dem **Raspberry Pi Pico 2 W**.

> **Status: frühe Entwicklung**  
> Die aktuelle Firmware testet GP2 und GP3 per PWM: Die Ausgänge wechseln einmal pro Sekunde in entgegengesetzte Zustände, der aktive Ausgang startet mit `pwm_level = 10` (etwa 10 % Tastverhältnis). Die WLAN-Initialisierung ist vorbereitet, wird im Programmeinstieg aber derzeit nicht aufgerufen. Uhranzeige und Zeitsynchronisierung sind noch nicht implementiert.

## Binär statt Ziffern

Jede Zahl lässt sich als Kombination von Bits darstellen. Zum Beispiel:

```text
Dezimal   13
Binär     1101
          ||||
Wert      8 + 4 + 0 + 1
```

Das ist die Idee hinter dem Projekt: Zeit auf einen Blick lesen, ohne dass sie wie eine gewöhnliche Digitaluhr aussieht.

## Hardware und Software

- Raspberry Pi Pico 2 W
- Raspberry Pi Pico SDK **2.3.1**
- CMake und Ninja
- USB-Verbindung zum Bauen, Flashen und Ausgeben von Statusmeldungen
- Für einen sichtbaren GPIO-Test: zwei externe LEDs mit passenden Vorwiderständen

## Loslegen

### Voraussetzungen und WLAN-Konfiguration

Installiere das [Pico SDK](https://github.com/raspberrypi/pico-sdk) und eine passende ARM-Embedded-Toolchain. Mit der Raspberry Pi Pico-Erweiterung für VS Code kann die vorhandene Projektkonfiguration direkt verwendet werden.

Für den Build wird `WiFi/config_WiFi.h` benötigt. Erstelle sie aus der Vorlage:

```powershell
Copy-Item WiFi/config_WiFi.example.h WiFi/config_WiFi.h
```

Ersetze in `WiFi/config_WiFi.h` `your_SSID` und `your_PASSWORD` durch deine WLAN-Zugangsdaten. Die Werte müssen in Anführungszeichen stehen. Die lokale Datei wird von Git ignoriert; echte Zugangsdaten gehören weder in die eingecheckte Beispieldatei noch in Commits oder Screenshots. WLAN ist im Quellcode vorbereitet, aber aktuell deaktiviert.

### Bauen

Das UF2-Image wird unter `build/pico2w_binary_clock.uf2` erzeugt.

Führe hierzu die Befehle im Projektverzeichnis aus.
Beim ersten Build oder nach Änderungen an der CMake-Konfiguration wird zunächst konfiguriert:

```sh
cmake -S . -B build
cmake --build build
```

**Mit VS Code:** (empfohlen)
In VS Code den Task **Compile Project** verwenden, sobald das Projekt konfiguriert ist. 


### Aufspielen

Das UF2-Image liegt nach dem Build unter `build/pico2w_binary_clock.uf2`.

**Manuell:** 
Trenne den Pico vom USB, halte **BOOTSEL** gedrückt und verbinde ihn wieder.
Sobald der Pico als Laufwerk erscheint, kannst du **BOOTSEL** loslassen.
Kopiere die UF2-Datei auf das Laufwerk. 
Nach dem Kopieren startet der Pico automatisch neu.

**Mit VS Code:** (empfohlen)
Schließe den Pico per USB an und starte den Task **Run Project**. Die Task versetzt den Pico automatisch in den **BOOTSEL-Modus**, lädt die Firmware und startet sie. Ein manuelles Drücken von **BOOTSEL** ist dafür normalerweise nicht nötig; falls der Pico nicht erkannt wird, kannst du ihn manuell in den BOOTSEL-Modus versetzen und den Task erneut starten.

### Aktuelles Verhalten

Nach dem Start gibt die Firmware über USB-Seriell `Main loop started.` und anschließend einmal pro Sekunde `Main loop running ...` aus. GP2 und GP3 wechseln jede Sekunde in entgegengesetzte Zustände; der jeweils aktive Ausgang wird mit PWM und zunächst 10 % Tastverhältnis angesteuert. Den Compare-Wert gibst du direkt über `pwm_level` in `pico2w_binary_clock.c` vor; bei Wrap 99 entsprechen Werte von 0 bis 100 ungefähr 0 bis 100 % Tastverhältnis. Da der Zähler von 0 bis einschließlich Wrap zählt, ergeben 100 Zählstufen eine Auflösung von ungefähr 1 Prozentpunkt pro Level; `pwm_level = 100` schaltet den Ausgang dauerhaft ein. Die PWM-Frequenz berechnet sich als `f_sys / (clkdiv * (wrap + 1))`. Bei einem Systemtakt von 150 MHz, `clkdiv = 40` und `wrap = 99` ergibt das `150 MHz / (40 * 100) = 37,5 kHz`. Der Wrap-Wert ist eingeschlossen, daher zählen die Werte 0 bis 99 als 100 Schritte. Die tatsächliche Frequenz hängt vom konfigurierten Systemtakt ab. Für einen sichtbaren Test schließe je eine externe LED mit passendem Vorwiderstand zwischen GP2 beziehungsweise GP3 und GND an. GP2 liegt am physischen Pin 4, GP3 am physischen Pin 5. Die eingebaute LED des Pico 2 W wird nicht über diese Pins angesteuert.

## Projektstruktur

| Datei | Zweck |
| --- | --- |
| `pico2w_binary_clock.c` | Firmware-Einstiegspunkt und GPIO-Test |
| `CMakeLists.txt` | Build-Konfiguration für Pico 2 W und Pico SDK |
| `WiFi/config_WiFi.example.h` | Vorlage für die lokale WLAN-Konfiguration |
| `WiFi/WLAN_init.c` | Vorbereitete WLAN-Initialisierung; derzeit nicht vom Programmeinstieg aufgerufen |
| `WiFi/WLAN_init.h` | Deklaration der WLAN-Initialisierungsfunktion |
| `GPIO/config_GPIO.h` | GPIO-Zuordnung für `Sec_Bit0` (GP2), `Sec_Bit1` (GP3) und PWM-Wrap-Wert |
| `GPIO/GPIO_init.c` | Schaltet GP2/GP3 auf PWM und konfiguriert den gemeinsamen PWM-Slice |
| `GPIO/GPIO_init.h` | Deklaration der GPIO-Initialisierungsfunktion |
| `lwipopts.h` | Konfiguration des lwIP-Netzwerkstacks |

## Geplante nächste Schritte

- WLAN-Zeitsynchronisierung und Initialisierung eines DS3231-RTC-Moduls umsetzen
- Uhrzeit aus der RTC lesen und binär darstellen
- LEDs für die Zeitanzeige ansteuern
- Entscheiden, ob die Anzeige jede Sekunde per Scheduler aktualisiert und die RTC regelmäßig abgeglichen werden soll