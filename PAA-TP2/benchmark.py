import subprocess
import re
import matplotlib.pyplot as plt
import os

inputs = [
    "exemplos/mapa_5x5.txt",
    "exemplos/mapa_10x10.txt",
    "exemplos/mapa_15x15.txt",
    "exemplos/mapa_20x20.txt",
]

sizes = []
times = []

for infile in inputs:
    if not os.path.exists(infile):
        print(f"[!] Arquivo não encontrado: {infile}")
        continue

    print(f"Rodando {infile}...")
    # envia 'n' automaticamente para a pergunta da interface gráfica
    result = subprocess.run(
        ["./trabalho.exe", infile, "-v"],
        input="n\n",
        capture_output=True,
        text=True,
        encoding="utf-8",
        errors="ignore"
    )

    # procura o tempo mesmo com acentos estranhos
    m = re.search(r"Tempo\s+aproximado[:=]\s*([0-9.]+)", result.stdout)
    if not m:
        print(f"⚠️  Tempo não encontrado no output de {infile}")
        continue

    t = float(m.group(1))
    with open(infile, "r") as f:
        h, w = map(int, f.readline().split()[:2])
    sizes.append(h * w)
    times.append(t)
    print(f"→ {h}x{w} => {t:.3f} ms")

if not times:
    print("\n❌ Nenhum dado coletado. Verifique se os arquivos estão em 'exemplos/'.")
    exit()

plt.figure(figsize=(7,4))
plt.plot(sizes, times, marker="o", label="Tempo medido")
plt.title("Desempenho do algoritmo (Programação Dinâmica)")
plt.xlabel("Tamanho do mapa (h × w)")
plt.ylabel("Tempo (ms)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()
