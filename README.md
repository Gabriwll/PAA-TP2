# PAA-TP2
# Requisitos para rodar em Windows
    - Necessita de ter Wingw64, para ser compativel com SDL3
    Para baixar pelo MSYS
        1 - pacman -Syu                          # Atualização do sistema
        2 - pacman -S mingw-w64-ucrt-x86_64-gcc  # Instalação GCC 64-bit
        3 - Configuração do PATH
            Adicionar C:\msys64\ucrt64\bin ao PATH do sistema ( se tiver versão anterior do mingw32, tem q colocar a variavel de ambiente do x64 em cima)
        4 -  gcc --version
            # gcc (GCC) 15.2.0
            # Target: x86_64-w64-mingw32

# Comando de Compilação Final
make ou mingw32-make (dependendo de como configurou)

ou por 

gcc -Wall -O2 -std=c99 -Iinclude -I"../SDL3-devel-3.2.26-mingw/SDL3-3.2.26/x86_64-w64-mingw32/include" -L"../SDL3-devel-3.2.26-mingw/SDL3-3.2.26/x86_64-w64-mingw32/lib" src/main.c src/problem.c src/dp_solver.c src/path.c src/utils.c src/gui.c src/generator.c -lmingw32 -lSDL3 -o trabalho.exe

# Fazer as execuções
Basta alterar o nome do arquivo de entrada
.\trabalho.exe exemplos\[nome do arquivo].txt -v

Plotar gráfico
python benchmark.py


.\trabalho.exe --generate
Basta alterar o nome do arquivo de saida
.\trabalho.exe [nome do arquivo].txt -v