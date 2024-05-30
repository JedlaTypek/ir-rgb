# Ovládání RGB led IR ovladačem
## Zapojení součástek
### RGB led
- červená led na pin D9
- zelená led na pin D5
- modrá led na pin D6
- mínusový pin zapojit na GND pin
### IR přijímač
- datový pin zapojit na pin D2
- mínusový pin zapojit na GND pin
## Použití
### Funkční tlačítka
V případě jiného ovladače si musíte přepsat kódy z ovladače v main.cpp souboru
- Potvrzení červené barvy - **CH+**
- Povrzení zelené barvy - **pause/play**
- Potvrzení modré barvy - **next**
- smazání zadaného čísla - **CH**
- Zapnutí/vypnutí všech led - **CH-**
### Postup
Pro kontrolu si můžete zapnout seriový monitor.
1. Zadejte číslo od 0 do 255
2. Poté potvrďte číslo na barvy které chcete
3. Číslo bude v paměti, dokud jej nesmažete tlačítkem na na smazání čísla
### Speacialní funkce
Pokud žádná led nesvítí, můžete všechny led zapnout naplno tlačítkem na zapnutí/vypnutí. Pokud jakákoliv led svítí, vypne to všechny ledky.
