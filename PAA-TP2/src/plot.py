import matplotlib.pyplot as plt
import argparse
import sys

def main():
    parser = argparse.ArgumentParser(description='Plotador de dados')
    
    parser.add_argument('--title', '-t', default='Meu Gráfico', 
                       help='Título do gráfico')
    parser.add_argument('--xlabel', '-x', default='Eixo X', 
                       help='Rótulo do eixo X')
    parser.add_argument('--ylabel', '-y', default='Eixo Y', 
                       help='Rótulo do eixo Y')
    
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
            y.append(float(values[1]))  # Agora suporta float
    
    # Verifica se são estatísticas do solver (4 pontos específicos)
    is_stats_plot = len(x) == 4 and x == [1, 2, 3, 4]
    
    # Configurações do gráfico
    plt.figure(figsize=(12, 6))
    
    if is_stats_plot:
        # Gráfico de barras para estatísticas
        labels = ['Nós Expandidos', 'Estados Alcançáveis', 'Fronteira Máxima', 'Tempo (ms)']
        colors = ['blue', 'green', 'orange', 'red']
        
        bars = plt.bar(labels, y, color=colors, alpha=0.7)
        plt.title(f'Estatísticas do Solver DP\n{args.title}')
        plt.ylabel(args.ylabel)
        
        # Adicionar valores nas barras
        for bar, value in zip(bars, y):
            plt.text(bar.get_x() + bar.get_width()/2, bar.get_height() + max(y)*0.01,
                    f'{value:,.0f}', ha='center', va='bottom', fontweight='bold')
        
        # Formatar eixo Y para números grandes
        plt.ticklabel_format(style='plain', axis='y')
        
    else:
        # Gráfico de linha normal
        plt.plot(x, y, 'o-', linewidth=2, markersize=8, label='Dados')
        plt.title(args.title)
        plt.xlabel(args.xlabel)
        plt.ylabel(args.ylabel)
        plt.legend()
        plt.grid(True, alpha=0.3)
    
    plt.xticks(rotation=45 if is_stats_plot else 0)
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    main()