@ECHO OFF
"C:\Program Files\Atmel\AVR Tools\AvrAssembler2\avrasm2.exe" -S "D:\projets\insadestijl\robot\bootloader\labels.tmp" -fI -W+ie -C V2E -o "D:\projets\insadestijl\robot\bootloader\bootloader.hex" -d "D:\projets\insadestijl\robot\bootloader\bootloader.obj" -e "D:\projets\insadestijl\robot\bootloader\bootloader.eep" -m "D:\projets\insadestijl\robot\bootloader\bootloader.map" "D:\projets\insadestijl\robot\bootloader\bootloader.asm"
