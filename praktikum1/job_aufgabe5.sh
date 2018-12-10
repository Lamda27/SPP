#!/bin/bash
#
# Begriffe die in < > Zeichen geklammert sind, enthalten eine Beschreibung,
# die sie durch einen entsprechenden Wert oder Namen ersetzten müssen.
# Insbesondere sollten sie ihrem Job
#   - einen Namen geben (nach dem -J Parameter)
#   - eine Datei in die die Ausgabe von stderr erfolgen soll (nach dem -e Parameter)
#   - eine Datei in die die Ausgabe von stdout erfolgen soll (nach dem -o Parameter)
#   - Die Anzahl der Threads die sie benötigen (nach dem -c Parameter)
#
#SBATCH -J aufgabe5_mitRedu_job
#SBATCH --mail-type=ALL
# Bitte achten Sie auf vollständige Pfad-Angaben:
#SBATCH -e /home/kurse/kurs00025/aq45zyme/spp_praktikum1/aufgabe5_mitRedu_job.err.%j
#SBATCH -o /home/kurse/kurs00025/aq45zyme/spp_praktikum1/aufgabe5_mitRedu_job.out.%j
#SBATCH --exclusive
#SBATCH --account=kurs00025
#SBATCH --partition=kurs00025
#SBATCH --reservation=kurs00025
#
#SBATCH -n 1                  # 1 Prozess
#SBATCH -c 16                 # 16 Kerne pro Prozess
#SBATCH --mem-per-cpu=1600    # Hauptspeicher in MByte pro Rechenkern
#SBATCH -t 00:05:00           # in Stunden:Minuten:Sekunden (Maximum ist 5 min)

# -------------------------------
# Anschließend schreiben Sie Ihre eigenen Befehle, wie z.B.
# Laden sie benötigte Module
module load gcc
module load openmpi/gcc

# Wechseln sie in das Verzeichnis in dem ihr Executable liegt
cd ~/spp_praktikum1

# Ihr Programmaufruf
make heated-plate-parallel
./heated-plate-parallel 1
./heated-plate-parallel 2
./heated-plate-parallel 4
./heated-plate-parallel 8
./heated-plate-parallel 16
./heated-plate-parallel 32
