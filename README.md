#  ProjetoCompiladores2019  
Projeto de compilador escrito em c++ para a disciplina de Compiladores do Curso de Ciência da Computação do IFB.  
Contém a análise léxica, análise sintática e a criação da tabela de símbolos. 

#Imprimir Tabela de Símbolos ou Tokens da Análise Léxica  
Caso queira imprimir somente a Tabela de Símbolos, retire o comentário (//) da linha 123 (tabelaDeSimbolos();) 
e comente a função da linha 124.  
Caso queira apenas imprimir os Tokens da Análise Léxica, comente a linha 123 e descomente a 124.  
  
  
##Como compilar:  
Use no terminal:  
    g++ -std=c++11 main.cpp -o nomeDoExecutavel  
  
##Como executar:  
Use no terminal:  
    ./nomeDoExecutavel  
  
##Ler de arquivo
Por padrão ele lê o arquivo a.txt. Caso queira mudar o nome do arquivo de leitura mude o nome entre aspas da linha 96 do arquivo main.cpp para o nome do arquivo desejado.  
  
##Salvar em arquivo  
Execute o programa no terminal do seguinte modo:  
    ./nomeDoExecutavel > nomeArquivoSaida.txt 
  
