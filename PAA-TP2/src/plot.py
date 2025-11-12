import matplotlib.pyplot as plt
import argparse
import sys

def main():
    parser = argparse.ArgumentParser(description='Plotador de dados')
    
    # Define os argumentos que o script aceita
    parser.add_argument('--title', '-t', default='Meu Gráfico', 
                       help='Título do gráfico')
    parser.add_argument('--xlabel', '-x', default='Eixo X', 
                       help='Rótulo do eixo X')
    parser.add_argument('--ylabel', '-y', default='Eixo Y', 
                       help='Rótulo do eixo Y')
    
    # Parse dos argumentos da linha de comando
    args = parser.parse_args()
    
    # Lê os dados do stdin
    x = []
    y = []
    
    for line in sys.stdin:
        if line.strip() == 'e':
            break
        values = line.split()
        if len(values) >= 2:
            x.append(int(values[0]))
            y.append(int(values[1]))
    
    # Cria o gráfico com os parâmetros recebidos
    plt.plot(x, y, 'o-', label='Dados')
    plt.title(args.title)
    plt.xlabel(args.xlabel)
    plt.ylabel(args.ylabel)
    plt.legend()
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    main()