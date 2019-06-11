#include <bits/stdc++.h>

using namespace std;


enum{
    INT_LITERAL, CHAR_LITERAL, FLOAT_LITERAL, IDENTIFICADOR, OPERADOR_LOG_ARIT, OPERADOR_RELACIONAL, OPERADOR_ATRIBUICAO, SIMBOLO_ESPECIAL, PALAVRA_RESERV
};

vector<string> palReservadas= {"if" ,"else" ,"while","return","float", "char", "void", "prnt", "int","and","or","not","proc","var"};
FILE *fp;
char c_GLOBAL;
int linha = 1;
int coluna = 0;
int coluna_atual;

class tokem{
    
    public:
    int linha;
    int coluna;
    int tipo;
    string tok;
    
    tokem(int l, int c, int tip, string toke){
        linha = l;
        coluna = c;
        tipo = tip;
        tok = toke;
    }
    bool operator<(const tokem& rhs) const{
        if(this->linha <= rhs.linha){
            return true;
        }
        return false;
    }
   
};
vector<tokem> v_tokens;
vector<tokem> errosLex;
int indice_v;
tokem erroSint = tokem(0,0,0,"");

void lexer();
bool lerSimbolosEOperadores(char c);
char lerIdentificador(char c);
char lerNumeros(char c, char sinal);
char lerCoisaEstranha(string s);
char lerCaracterLiteral();
void lerComentario();
void lerComentarioEmBloco();
void lerEspaco(char c);
pair<string,char> lerNumeroComVirgula(string s);
bool eEspaco(char c);
bool eLetraMin(char c);
bool eLetra(char c);
bool eNumero(char c);
bool eSimboloOuOperador(char c);

bool parser();
bool lerExpBas();
bool lerExp();
bool lerListaExp();
bool lerChamadaFunc();
bool lerRetorno();
bool lerChamadaFuncCmd();
bool lerEscrita();
bool lerDecisao();
bool lerIteracao();
bool lerAtribuicao();
bool lerComando();
bool lerListaCom();
bool lerBloco();
bool lerParamFormais();
bool lerNomeArgs();
bool lerDeclFunc();
bool lerTipo();
bool lerListaIdents();
bool lerDeclVar();
bool lerDeclGlobal();
bool lerProgram();
void imprimirLex();

priority_queue<tokem> tabela;

void tabelaDeSimbolos();


int main(int argc, char const *argv[]){
	char *nomeArq;

    //cout << "Digite o nome do arquivo"<< endl;
    //cin >> nomeArq;
    //fp = fopen(nomeArq, "r");
    
    fp = fopen("a.txt", "r");
    
    ////Testar leitura arquivo
    // char c = getc(fp); coluna++;
    // while(c!= EOF){
    //     cout<<c;
    //     c = getc(fp); coluna++;
    // }

    lexer();

    if(errosLex.size()>0){
        for (tokem t:errosLex)
        {
            cout << "Erro Lexico: "<< endl;
            cout << "Identificador desconhecido ou fora da linguagem \""  << t.tok << "\" na linha "<< t.linha << " e coluna " << t.coluna << endl;
        }
        
        
    }
    else{
        if(!parser()){
            cout << "Erro Sintatico: "<< endl;
            cout << "E' esperado um(a) \""  << erroSint.tok << "\" na linha "<< erroSint.linha << " e coluna " << erroSint.coluna << endl;
        }
        else{
            
            //tabelaDeSimbolos();
            imprimirLex();
        }
    }

    //Testar se lexer funcionou imprimindo no arquivo
    // FILE *fp2;
    // fopen("saida.txt","w");
    
    

    fclose(fp);
    // fclose(fp2);
	return 0;
}

void imprimirLex(){
    for(tokem t:v_tokens){
            //fprintf(fp2,"\"%s\" na linha %d e coluna %d", t.tok , t.linha, t.coluna);
            cout << "\""<< t.tok << "\" na linha "<< t.linha << " e coluna " << t.coluna << endl;
        }
}




using namespace std;
void lexer(){
	c_GLOBAL = getc(fp); coluna++;
    while(c_GLOBAL!=EOF){
        //ignora espaços e quebras de linha
        if(!eEspaco(c_GLOBAL)){
            //Le o caracter seguinte caso seja um simbolo ou um operador
            if(!lerSimbolosEOperadores(c_GLOBAL)){
                //se c for uma aspa simples (coloquei c==39 pois nao estava identificando '\'')
                if(c_GLOBAL == '\'' || c_GLOBAL == 39){
                    //verifica se e' um caracter literal (entre aspas simples)
                    c_GLOBAL = lerCaracterLiteral();
                }
                //verifica se e um numero
                else if(eNumero(c_GLOBAL)){
                    c_GLOBAL = lerNumeros(c_GLOBAL,false);
                }
                //verifica se e uma letra minuscula para ver se e palavra reservada ou identificador
                else if(eLetraMin(c_GLOBAL) ){
                    c_GLOBAL = lerIdentificador(c_GLOBAL);
                }
                
                //nao e' nada aceito pela linguagem
                else{
                    string s;
                    s.push_back(c_GLOBAL);
                    c_GLOBAL = lerCoisaEstranha(s);
                }
            }else{
                c_GLOBAL = getc(fp); coluna++;
            }
        }else{
            lerEspaco(c_GLOBAL);
            c_GLOBAL = getc(fp); coluna++;
        }
    }
}

//Uso variavel global em casos que se deve ler mais de um caracter. 
//Nesses casos, caso ele seja um simbolo ou operador e nao e' necesario ler o proximo caracter fora desta funcao ela elimina os espacos e se chama recursivamente retornando o valor que achar nesta recursao
bool lerSimbolosEOperadores(char c){
    string s;
    switch (c)
        {
            case '+':
                    //cout << "Operador logico-aritmetico [+] ["<<linha<<", "<<coluna << "]" << endl; 
                    s.push_back(c);
                    v_tokens.push_back(tokem(linha,coluna,OPERADOR_LOG_ARIT,s));
                    return true;
            case '*': 
                    coluna_atual = coluna;
                    c_GLOBAL = getc(fp); coluna++;
                    if(c_GLOBAL != '*'){
                        //cout << "Operador logico-aritmetico [*] ["<<linha<<", "<<coluna_atual << "]" << endl; 
                    s.push_back(c);
                    v_tokens.push_back(tokem(linha,coluna_atual,OPERADOR_LOG_ARIT,s));
                        while (eEspaco(c_GLOBAL))
                        {
                            lerEspaco(c_GLOBAL);
                            c_GLOBAL = getc(fp); coluna++;
                        }
                        return lerSimbolosEOperadores(c_GLOBAL);
                    }else{
                        lerComentario();
                        return true;
                    }
                    
            
            case '/':
                    //cout << "Operador logico-aritmetico [/] ["<<linha<<", "<<coluna << "]" << endl;
                    s.push_back(c);
                    v_tokens.push_back(tokem(linha,coluna,OPERADOR_LOG_ARIT,s));
                    return true;
            case '-': 
                    coluna_atual = coluna;
                    c_GLOBAL = getc(fp); coluna++;
                    if(!eNumero(c_GLOBAL)){
                        //cout << "Operador logico-aritmetico [-] ["<<linha<<", "<<coluna_atual << "]" << endl; 
                        v_tokens.push_back(tokem(linha,coluna_atual,OPERADOR_LOG_ARIT,"-"));
                    }else{
                        c_GLOBAL = lerNumeros(c,true);
                    }
                    while (eEspaco(c_GLOBAL))
                    {
                        lerEspaco(c_GLOBAL);
                        c_GLOBAL = getc(fp); coluna++;
                    }
                    return lerSimbolosEOperadores(c_GLOBAL);
            case '%': 
                    //cout << "Operador logico-aritmetico [%] ["<<linha<<", "<<coluna << "]" << endl; 
                    v_tokens.push_back(tokem(linha,coluna,OPERADOR_LOG_ARIT,"%"));
                    return true;


            case '=': 
                    //cout << "Operador relacional [=] ["<<linha<<", "<<coluna << "]" << endl;
                    v_tokens.push_back(tokem(linha,coluna,OPERADOR_RELACIONAL,"=")); 
                    return true;
            case '<':
                    coluna_atual = coluna;
                    c_GLOBAL = getc(fp); coluna++;
                    if(c_GLOBAL=='='||c_GLOBAL=='>'){
                        //cout << "Operador relacional [<"<<c_GLOBAL<<"] ["<<linha<<", "<<coluna_atual << "]" << endl;
                        s.push_back('<');
                        s.push_back(c_GLOBAL);
                        v_tokens.push_back(tokem(linha,coluna_atual,OPERADOR_RELACIONAL,s)); 
                        return true;
                    }else{
                        //cout << "Operador relacional [<] ["<<linha<<", "<<coluna_atual << "]" << endl; 
                        s.push_back(c);
                        v_tokens.push_back(tokem(linha,coluna_atual,OPERADOR_RELACIONAL,s)); 
                        while (eEspaco(c_GLOBAL))
                        {
                            lerEspaco(c_GLOBAL);
                            c_GLOBAL = getc(fp); coluna++;
                        }
                        return lerSimbolosEOperadores(c_GLOBAL);
                    }
            case '>':  
                    coluna_atual = coluna;
                    c_GLOBAL = getc(fp); coluna++;
                    if(c_GLOBAL=='='){
                        //cout << "Operador relacional [>=] ["<<linha<<", "<<coluna_atual << "]" << endl;
                        v_tokens.push_back(tokem(linha,coluna_atual,OPERADOR_RELACIONAL,">=")); 
                        return true;
                    }else if(c_GLOBAL=='>'){
                        lerComentarioEmBloco(); return true;
                    } 
                    else{
                        //cout << "Operador relacional [>] ["<<linha<<", "<<coluna_atual << "]" << endl; 
                        v_tokens.push_back(tokem(linha,coluna_atual,OPERADOR_RELACIONAL,">")); 
                        while (eEspaco(c_GLOBAL))
                        {
                            lerEspaco(c_GLOBAL);
                            c_GLOBAL = getc(fp); coluna++;
                        }
                        return lerSimbolosEOperadores(c_GLOBAL);
                    }

            case '(': //cout << "Simbolo especial [(] ["<<linha<<", "<<coluna << "]" << endl;
                    v_tokens.push_back(tokem(linha,coluna,SIMBOLO_ESPECIAL,"("));  
                    return true;
            case ')': //cout << "Simbolo especial [)] ["<<linha<<", "<<coluna << "]" << endl;
                    v_tokens.push_back(tokem(linha,coluna,SIMBOLO_ESPECIAL,")")); 
                    return true;
            case '{': //cout << "Simbolo especial [{] ["<<linha<<", "<<coluna << "]" << endl; 
                    v_tokens.push_back(tokem(linha,coluna,SIMBOLO_ESPECIAL,"{")); 
                    return true;
            case '}': //cout << "Simbolo especial [}] ["<<linha<<", "<<coluna << "]" << endl; 
                    v_tokens.push_back(tokem(linha,coluna,SIMBOLO_ESPECIAL,"}")); 
                    return true;
            case ',':
                    coluna_atual = coluna;
                    c_GLOBAL = getc(fp); coluna++;
                    if(eNumero(c_GLOBAL)){
                        c_GLOBAL = lerNumeros(c_GLOBAL,',');
                    }
                    else{
                        //cout << "Simbolo especial [,] ["<<linha<<", "<<coluna_atual << "]" << endl; 
                        v_tokens.push_back(tokem(linha,coluna_atual,SIMBOLO_ESPECIAL,",")); 
                    }
                    //Nao sera coisa estranha, pois virgula sempre pode estar colado com qualque coisa
                    // else{
                    //     string s = ","+c_GLOBAL;
                    //     lerCoisaEstranha(s);
                    // }
                    //e' necessario atualizar o c_Global para um caracter que nao seja espaco, e retornar a funcao recursivamente
                    while (eEspaco(c_GLOBAL))
                    {
                        lerEspaco(c_GLOBAL);
                        c_GLOBAL = getc(fp); coluna++;
                    }
                    return lerSimbolosEOperadores(c_GLOBAL);
            case ';': //cout << "Simbolo especial [;] ["<<linha<<", "<<coluna << "]" << endl; 
                        v_tokens.push_back(tokem(linha,coluna,SIMBOLO_ESPECIAL,";")); 
                        return true;

            case ':': 
                    coluna_atual = coluna;
                    c_GLOBAL = getc(fp); coluna++;
                    if(c_GLOBAL=='='){
                        //cout << "Operador de atribuicao [:=] ["<<linha<<", "<<coluna_atual << "]" << endl;
                        v_tokens.push_back(tokem(linha,coluna_atual,OPERADOR_ATRIBUICAO,":=")); 
                    }else{
                        if(eEspaco(c_GLOBAL)){
                            //cout << "Caracter invalido [:] ["<<linha<<", "<<coluna_atual << "]" << endl;
                            errosLex.push_back(tokem(linha,coluna_atual,-1,":")); 
                            while (eEspaco(c_GLOBAL))
                            {
                                lerEspaco(c_GLOBAL);
                                c_GLOBAL = getc(fp); coluna++;
                            }
                            return lerSimbolosEOperadores(c_GLOBAL);
                        }
                        else{
                            string s;
                            s.push_back(':');
                            s.push_back(c_GLOBAL);
                            c_GLOBAL = lerCoisaEstranha(s);
                        }
                    } return true;
            default: return false;
        }
}

bool eEspaco(char c){
    return (c==9 || c==10 || c==13 || c==32);
}
bool eLetraMin(char c){
    return (c>96&&c<122);
}
bool eLetra(char c){
    return ((c>64&&c<92) || (c>96&&c<122));
}
bool eNumero(char c){
    return (c>47&&c<58);
}
bool eSimboloOuOperador(char c){
    return ( (c>=58 && c<=62) || (c==123) || (c==125) || (c==37) || (c==47) || (c>=40 && c <= 45) );
}


//[a-z]([a-zA-Z_]|[0-9])*
//Já recebe um minúsculo
char lerIdentificador(char c){
    string s;
    s.push_back(c);
    coluna_atual = coluna;
    c = getc(fp); coluna++;
    //Enquanto for apenas letra
    while(eLetra(c)){
        s.push_back(c);
        c = getc(fp); coluna++;
    }
    //se encontrou algum numero só pode ser variável ou função (chamada de identificador)
    if(eNumero(c)){
        s.push_back(c);
        c = getc(fp); coluna++;
        //enquanto letra ou numero
        while(eLetra(c)||eNumero(c)){
            s.push_back(c);
            c = getc(fp); coluna++;
        }
        //Se for fim, espaço ou sinal
        if(eEspaco(c) || eSimboloOuOperador(c) ){
            // cout << "Identificador [" << s <<  "] ["<<linha<<", "<<coluna_atual << "]" <<endl;
            v_tokens.push_back(tokem(linha,coluna_atual,IDENTIFICADOR,s)); 
            return c;
        }
    }
    //Se não encontrou número no meio:
    else{
        //Se for fim, espaço ou sinal
        if(eEspaco(c) || eSimboloOuOperador(c) ){
            //Verifica se é palavra chave
            for(string st: palReservadas){
                if(!s.compare(st)){
                    // cout << "Palavra-chave reservada [" << s << "] ["<<linha<<", "<<coluna_atual << "]"  << endl;
                    v_tokens.push_back(tokem(linha,coluna_atual,PALAVRA_RESERV,s)); 
                    return c;
                }
            }
            // cout << "Identificador [" << s << "] ["<<linha<<", "<<coluna_atual << "]"  << endl;
            v_tokens.push_back(tokem(linha,coluna_atual,IDENTIFICADOR,s)); 
            return c;
        }
    }
    //Se encontrou algum caracter inválido:
    return lerCoisaEstranha(s);
}

//Le numeros inteiros, reais e com expoente. Caso nao seja nenhum desses, e' um tokem invalido
//recebe um char que e' o primeiro numero a ser lido, e insere o sinal recebido na string de impressao
//sinal pode ser '+', '-', ou ','. 
char lerNumeros(char c,char sinal){
    //int n = 0;
    coluna_atual = coluna;
    string s;
    s.push_back(sinal);
    //enquanto numero
    while(eNumero(c)){
        //n = n*10 + c-48;
        s.push_back(c);
        c = getc(fp); coluna++;
    }
    //Se for virgula pode ser um valor real 
    //Se sinal for virgula ele nao verifica se e' real, pois ele já e'
    if(c==','&& sinal!=','){
        //string s = to_string(n);
        pair<string,char> p = lerNumeroComVirgula(s);
        c = p.second;
        s = p.first;
        //Se for fim, espaço ou outro sinal
        if(eEspaco(c) || eSimboloOuOperador(c)){
            //cout << "Valor real literal [" << s << "] ["<<linha<<", "<<coluna_atual << "]"  << endl;
            v_tokens.push_back(tokem(linha,coluna_atual,FLOAT_LITERAL,s)); 
            return c;
        }
        //Numero com expoente
        else if(c=='E'){
            s.push_back(c);
            c = getc(fp); coluna++;
            while(eNumero(c)){
                s.push_back(c);
                c = getc(fp); coluna++;
            }
            if(c==','){
                p = lerNumeroComVirgula(s);
                c = p.second;
                s = p.first;
                 //Se for fim, espaço ou outro sinal
                if(eEspaco(c) || eSimboloOuOperador(c)){
                    //cout << "Valor real literal com expoente [" << s << "] ["<<linha<<", "<<coluna_atual << "]"  << endl;
                    v_tokens.push_back(tokem(linha,coluna_atual,FLOAT_LITERAL,s)); 
                    return c;
                }
                else return lerCoisaEstranha(s);
            }
            else if(eEspaco(c) || eSimboloOuOperador(c) ){
                //cout << "Valor real literal com expoente [" << s << "] ["<<linha<<", "<<coluna_atual << "]"  << endl;
                v_tokens.push_back(tokem(linha,coluna_atual,FLOAT_LITERAL,s)); 
                return c;
            }
        }
        return lerCoisaEstranha(s);
    }

    //Numero com expoente
    else if(c=='E'){
        s.push_back(c);
        while(eNumero(c)){
            s.push_back(c);
            c = getc(fp); coluna++;
        }
        if(c==','){
            pair<string,char> p = lerNumeroComVirgula(s);
            c = p.second;
            s = p.first;
                //Se for fim, espaço ou outro sinal
            if(eEspaco(c) || eSimboloOuOperador(c)){
                //cout << "Valor real literal com expoente [" << s << "] ["<<linha<<", "<<coluna_atual << "]"  << endl;
                v_tokens.push_back(tokem(linha,coluna_atual,FLOAT_LITERAL,s)); 
                return c;
            }
        }
        else if(eEspaco(c) || eSimboloOuOperador(c) ){
            //cout << "Valor real literal com expoente [" << s << "] ["<<linha<<", "<<coluna_atual << "]"  << endl;
            v_tokens.push_back(tokem(linha,coluna_atual,FLOAT_LITERAL,s)); 
            return c;
        }
        return lerCoisaEstranha(s);
    }

    //Se for fim, espaço ou outro sinal
    else if(eEspaco(c) || eSimboloOuOperador(c) ){
        if(sinal!=','){
            //cout << "Valor inteiro literal [" << s << "] ["<<linha<<", "<<coluna_atual << "]"  << endl;
            v_tokens.push_back(tokem(linha,coluna_atual,INT_LITERAL,s)); 
        }
        else{
            //cout << "Valor real literal [" << s << "] ["<<linha<<", "<<coluna_atual << "]"  << endl;
            v_tokens.push_back(tokem(linha,coluna_atual,FLOAT_LITERAL,s)); 
        }
        return c;
    }
    return lerCoisaEstranha(s);
}

char lerCoisaEstranha(string s){
    if(s[0] == EOF)
        return EOF;
    coluna_atual = coluna - s.length();
    char c = getc(fp); coluna++;
    //Se nao for fim, espaço ou sinal
    while(!eEspaco(c) && !eSimboloOuOperador(c) && c!= EOF){
        s.push_back(c);
        c = getc(fp); coluna++;
    }
    // cout << "Identificador desconhecido ou fora da linguagem [" << s << "] ["<<linha<<", "<<coluna_atual << "]"  << endl;
    if(s.length()>0)
        errosLex.push_back(tokem(linha,coluna_atual,-1,s)); 
    return c;
}


void lerComentarioEmBloco(){
    coluna_atual = coluna;
    char c =  getc(fp); coluna++;
    while(c!=EOF){
        if(c=='<'){
            c =  getc(fp); coluna++;
            if(c == '<'){
                return;
            }
        }
        
        //ler espaço aumenta o numero de linhas e colunas
        lerEspaco(c);
        c =  getc(fp); coluna++;
    }
}

void lerComentario(){
    char c =  getc(fp); coluna++;
    while(c!= '\n'){
        c =  getc(fp); coluna++;
    }
    linha++;
}

void lerEspaco(char c){
    if( c==10 ){
        linha++;
        coluna = 0;
    }
}

//Ja e' passado para a funcao um numero na string. Ela coloca a virgula e os proximos digitos, se forem numeros, na string.  
pair<string,char> lerNumeroComVirgula(string s){
    char c;
    s.push_back(',');
    c = getc(fp); coluna++;
    while(eNumero(c)){
        s.push_back(c);
        c = getc(fp); coluna++;
    }
    // return make_pair(s,c);
    return {s,c};
}

//So vem para esta funcao caso ache uma aspa simples ['].
char lerCaracterLiteral(){
    string s;
    s.push_back('\'');
    coluna_atual = coluna;
    char c = getc(fp); coluna++;
    //char c_imp;
    // verificar se c e' aceito 
    if(eNumero(c) || eLetra(c) || c == ':' || c == ',' || c == '(' || c == ')' || c == ' '){
        //c_imp = c;
        s.push_back(c);
        c = getc(fp); coluna++;
        if (c == '\''|| c == 39){
            // cout << "Valor caracter literal [\'" << c_imp << "\'] ["<<linha<<", "<<coluna_atual << "]"  << endl;
            s.push_back(c);
            v_tokens.push_back(tokem(linha,coluna_atual,CHAR_LITERAL,s)); 
            return getc(fp); coluna++;
        }
    }
    else if( c == '\\'){
        s.push_back(c);
        c = getc(fp); coluna++;
        if (c == 'n' || c == 't'){
            //c_imp = c;
            s.push_back(c);
            c = getc(fp); coluna++;
            if (c == '\''|| c == 39){
                // cout << "Valor caracter literal [\'\\" << c_imp << "\'] ["<<linha<<", "<<coluna_atual << "]"  << endl;
                s.push_back(c);
                v_tokens.push_back(tokem(linha,coluna_atual,CHAR_LITERAL,s)); 
                return getc(fp); coluna++;
            }
        }
    }
    return c;   
}





/**Lembrar de colocar, caso precise:
 * else depois de if;
 * indice_v++; antes de return true ou depois de ler/consumir; (obs.: ele consome caso seja chamada algumas das seguintes funcoes, portanto nao incremente se for o caso)
 * erro no fim;
 * t.tok.compare(" ") == 0; ao comparar tok com string
 */


//if(t.tok.compare("")==0)

bool parser(){
    // for(tokem t : v_tokens){
    //     if(t.tok.compare("")==0){

    //     }
    // }
    indice_v = 0;
    
    return lerProgram();

}

bool lerExpBas(){
    int token_ini = indice_v;
    if(indice_v < v_tokens.size()){
        tokem t = v_tokens[indice_v];
        if(t.tok.compare("(") == 0){
            indice_v++;
            t = v_tokens[indice_v];
            if (lerExp()){
                t = v_tokens[indice_v];
                if(t.tok.compare(")") == 0){
                    indice_v++;
                    // tabela.push(tokem(token_ini,indice_v,-1,"Expressao Basica\t"));
                    return true;
                }
                else{
                    erroSint = tokem(t.linha,t.coluna,-1, ")");
                    return false;
                }
            }
            else{
                return false;
            } 
        }
        else if(t.tok.compare("not") == 0){
            indice_v++;
            return lerExpBas();   
        }
        else if(t.tok.compare("-") == 0){
            indice_v++;
            return lerExpBas();  
        }else if(t.tipo == IDENTIFICADOR){
            lerChamadaFunc();
            // tabela.push(tokem(token_ini,indice_v,-1,"Expressao Basica\t"));
            return true;
        }
        else if(t.tipo == INT_LITERAL || t.tipo == CHAR_LITERAL || t.tipo == FLOAT_LITERAL ){
            indice_v++;
            // tabela.push(tokem(token_ini,indice_v,-1,"Expressao Basica\t"));
            return true;
        }
        else{
            // erroSint = tokem(t.linha,t.coluna,-1, "expressao basica");
            return false;
        }
    }
    // erroSint = tokem(linha,coluna,-1, "expressao basica");
    return false;
}

bool lerExp(){
    int token_ini = indice_v;
    if(indice_v < v_tokens.size()){
        tokem t = v_tokens[indice_v];
        if(lerExpBas()){
            t = v_tokens[indice_v];
            //tem que ser um operador relacional ou aritmetico exceto not
            if( (t.tipo == OPERADOR_RELACIONAL) || (t.tipo == OPERADOR_LOG_ARIT) ){
                indice_v++;
                t = v_tokens[indice_v];
                if(t.tok.compare("not")!=0){
                    return lerExp();
                }
                else{
                    erroSint = tokem(t.linha,t.coluna,-1,"operador relacional ou aritmetico diferente de \"not\"");
                    return false;
                }
            }
            else{
                tabela.push(tokem(token_ini,indice_v,-1,"Expressao\t\t"));
                return true;
            }
        }
        else{
            return false;
        }
    }
    erroSint = tokem(linha,coluna,-1, "expressao");
    return false;
}

bool lerListaExp(){
    int token_ini = indice_v;
    if(indice_v < v_tokens.size()){
        if(lerExp()){
            tokem t = v_tokens[indice_v];
            while(t.tok.compare(",") == 0){
                indice_v++;
                if(!lerExp()){
                    return false;
                }
                t = v_tokens[indice_v];
            }
        }
        else{
            //se nao for expressao significa que e' ler lista exp e' vazio, portanto retorna verdadeiro
            tabela.push(tokem(token_ini,indice_v,-1,"Lista de Expressoes\t"));
            return true;
        } 
    }
    //se acabar os tokens ou nao for a opcao significa que e' vazio, portanto retorna verdadeiro
    tabela.push(tokem(token_ini,indice_v,-1,"Lista de Expressoes\t"));
    return true;
}

bool lerChamadaFunc(){
    int token_ini = indice_v;
    if(indice_v < v_tokens.size()){
        tokem t = v_tokens[indice_v];
        if(t.tipo == IDENTIFICADOR){
            do{
                indice_v++;
                t = v_tokens[indice_v];
                if(t.tok.compare("(") == 0){
                    indice_v++;
                    t = v_tokens[indice_v];
                    if (lerListaExp()){
                        t = v_tokens[indice_v];
                        if(t.tok.compare(")") == 0){
                            indice_v++;
                            t = v_tokens[indice_v];
                        }
                        else{
                            erroSint = tokem(t.linha,t.coluna,-1, ")");
                            return false;
                        }
                    }
                    else{
                        return false;
                    }
                }
                else{
                    erroSint = tokem(t.linha,t.coluna,-1, "(");
                    return false;
                }
            }while(t.tipo == IDENTIFICADOR);
            tabela.push(tokem(token_ini,indice_v,-1,"Chamada de Funcao\t"));
            return true;
        }
        else{
            erroSint = tokem(t.linha,t.coluna,-1, "identificador de funcao");
            return false;
        }
    }
    erroSint = tokem(linha,coluna,-1, "identificador de funcao");
    return false;
}

bool lerRetorno(){
    int token_ini = indice_v;
    if(indice_v < v_tokens.size()){
        tokem t = v_tokens[indice_v];
        if(t.tok.compare("return") == 0){
            indice_v++;
            if(lerExp()){
                t = v_tokens[indice_v];
                if(t.tok.compare(";") == 0){
                    indice_v++;
                    tabela.push(tokem(token_ini,indice_v,-1,"Retorno\t\t"));
                    return true;
                }
                else{
                    erroSint = tokem(t.linha,t.coluna,-1, ";");
                    return false;
                }
            }
            else{
                return false;
            }
        }
        else{
            erroSint = tokem(t.linha,t.coluna,-1, "return");
            return false;
        }
    }
    erroSint = tokem(linha,coluna,-1, "return");
    return false;

}

bool lerChamadaFuncCmd(){
    int token_ini = indice_v;
    if(indice_v < v_tokens.size()){
        if(lerChamadaFunc()){
            tokem t = v_tokens[indice_v];
            if(t.tok.compare(";") == 0){
                indice_v++;
                tabela.push(tokem(token_ini,indice_v,-1,"Chamada de Funcao\t"));
                return true;
            }
            else{
                erroSint = tokem(t.linha,t.coluna,-1, ";");
                return false;
            }
        }
        else{
            return false;
        }
    }    
    erroSint = tokem(linha,coluna,-1, "funcao");
    return false;
}

bool lerEscrita(){
    int token_ini = indice_v;
    if(indice_v < v_tokens.size()){
        tokem t = v_tokens[indice_v];
        if(t.tok.compare("prnt")==0){
            indice_v++;
            t = v_tokens[indice_v];
            if(t.tok.compare("(") == 0){
                indice_v++;
                t = v_tokens[indice_v];
                if (lerListaExp()){
                    t = v_tokens[indice_v];
                    if(t.tok.compare(")") == 0){
                        indice_v++;
                        t = v_tokens[indice_v];
                        if(t.tok.compare(";") == 0){
                            indice_v++;
                            tabela.push(tokem(token_ini,indice_v,-1,"Escrita\t\t"));
                            return true;
                        }
                        else{
                           erroSint = tokem(t.linha,t.coluna,-1, ";");
                            return false; 
                        }
                    }
                    else{
                        erroSint = tokem(t.linha,t.coluna,-1, ")");
                        return false;
                    }
                }
                else{
                    return false;
                } 
            }
            else{
                erroSint = tokem(t.linha,t.coluna,-1, "(");
                return false;
            }
        }
        else{
            erroSint = tokem(t.linha,t.coluna,-1, "prnt");
            return false;
        }
    }
    erroSint = tokem(linha,coluna,-1, "prnt");
    return false;
}
 
bool lerDecisao(){
    int token_ini = indice_v;
    if(indice_v < v_tokens.size()){
        tokem t = v_tokens[indice_v];
        if(t.tok.compare("if")==0){
            indice_v++;
            t = v_tokens[indice_v];
            if(t.tok.compare("(") == 0){
                indice_v++;
                t = v_tokens[indice_v];
                if (lerExp()){
                    t = v_tokens[indice_v];
                    if(t.tok.compare(")") == 0){
                        indice_v++;
                        if(lerComando()){
                            t = v_tokens[indice_v];
                            if(t.tok.compare("else") == 0){
                                indice_v++;
                                if(lerComando()){
                                    return true;
                                    tabela.push(tokem(token_ini,indice_v,-1,"Decisao\t\t\t"));
                                }
                                else {
                                    return false;
                                }
                            }
                            else{
                                //Ele aceita se acabar aqui, portanto retorna true
                                return true;
                                tabela.push(tokem(token_ini,indice_v,-1,"Decisao\t\t\t"));
                            }
                        }
                        else{
                            return false;
                        }
                    }
                    else{
                        erroSint = tokem(t.linha,t.coluna,-1, ")");
                        return false;
                    }
                } 
                else{
                    return false;
                }
            }
            else{
                erroSint = tokem(t.linha,t.coluna,-1, "(");
                return false;
            }
        }
        else{
            erroSint = tokem(t.linha,t.coluna,-1, "if");
            return false;
        }
    }
    erroSint = tokem(linha,coluna,-1, "if");
    return false;
}

bool lerIteracao(){ 
    int token_ini = indice_v;
    if(indice_v < v_tokens.size()){
        tokem t = v_tokens[indice_v];
        if(t.tok.compare("while")==0){
            indice_v++;
            t = v_tokens[indice_v];
            if(t.tok.compare( "(") == 0){
                indice_v++;
                t = v_tokens[indice_v];
                if (lerExp()){
                    t = v_tokens[indice_v];
                    if(t.tok.compare(")") == 0){
                        indice_v++;
                        if(lerComando()){
                            return true;
                            tabela.push(tokem(token_ini,indice_v,-1,"Iteracao\t\t\t"));
                        }
                        else{
                            return false;
                        }
                    }
                    else{
                        erroSint = tokem(t.linha,t.coluna,-1, ")");
                        return false;
                    }
                } 
                else{
                    return false;
                }
            }
            else{
                erroSint = tokem(t.linha,t.coluna,-1, "(");
                return false;
            }
        }
        else{
            erroSint = tokem(t.linha,t.coluna,-1, "while");
            return false;
        }
    }
    erroSint = tokem(linha,coluna,-1, "while");
    return false; 
}

bool lerAtribuicao(){ 
    int token_ini = indice_v;
    if(indice_v < v_tokens.size()){
        tokem t = v_tokens[indice_v];
        if(t.tipo == IDENTIFICADOR){
            indice_v++;
            t = v_tokens[indice_v];
            if(t.tok.compare(":=")==0){
                indice_v++;
                if(lerExp()){
                    t = v_tokens[indice_v];
                    if(t.tok.compare(";")==0){
                        indice_v++;
                        return true;
                        tabela.push(tokem(token_ini,indice_v,-1,"Atribuicao\t\t"));
                    }
                    else{
                        erroSint = tokem(t.linha,t.coluna,-1, ";");
                        return false;
                    }
                }
                else{
                    return false;
                }
            }else{
                erroSint = tokem(t.linha,t.coluna,-1, ":=");
                return false;
            }
        }
        else{
            erroSint = tokem(t.linha,t.coluna,-1, "identificador de variavel");
            return false;
        }
    }
    erroSint = tokem(linha,coluna,-1, "identificador de variavel");
    return false; 
}

bool lerComando(){  
    int token_ini = indice_v;
    if(indice_v < v_tokens.size()){
        tokem t = v_tokens[indice_v];
        if(t.tok.compare("var") == 0){
            return lerDeclVar();
        }else if(t.tipo == IDENTIFICADOR){
            t = v_tokens[indice_v+1];
            if(t.tok.compare("(") == 0){
                return lerChamadaFuncCmd();  
            }
            else if(t.tok.compare(":=") == 0){
                return lerAtribuicao();   
            }
            else{
                erroSint = tokem(t.linha,t.coluna,-1, "(\" ou \":=");
                return false;
            }
        }
        else if(t.tok.compare("while") == 0){
            return lerIteracao();  
        }
        else if(t.tok.compare("if") == 0){
            return lerDecisao();  
        }
        else if(t.tok.compare("prnt") == 0){
            return lerEscrita();  
        }
        else if(t.tok.compare("return") == 0){
            return lerRetorno();  
        }
        else if(t.tok.compare("{") == 0){
            return lerBloco();  
        }
        else{
            erroSint = tokem(t.linha,t.coluna,-1, "comando");
            return false;
        }
    }
    erroSint = tokem(linha,coluna,-1, "comando");
    return false;
}

bool lerListaCom(){  
    int token_ini = indice_v;
    if(indice_v < v_tokens.size()){
        tokem t = v_tokens[indice_v];
        //Apenas blocos utilizam listaCom, e ele acaba com '}', portanto e' lo'gico dizer que enquanto nao encontrar um '}', o tokem lido deve fazer parte de um comando
        while(t.tok.compare("}") != 0){
            if(!lerComando()){
                return false;
            }
            t = v_tokens[indice_v];
        }
        return true;
        tabela.push(tokem(token_ini,indice_v,-1,"Lista Comandos\t"));
    }
    erroSint = tokem(linha,coluna,-1, "comando");
    return false;
}

bool lerBloco(){  
    int token_ini = indice_v;
    if(indice_v < v_tokens.size()){
        tokem t = v_tokens[indice_v];
        if(t.tok.compare("{") == 0){
            indice_v++;
            if(lerListaCom()){
                t = v_tokens[indice_v];
                if(t.tok.compare("}") == 0){
                    indice_v++;
                    return true;
                    tabela.push(tokem(token_ini,indice_v,-1,"Bloco\t\t\t"));
                }
                else{
                    erroSint = tokem(t.linha,t.coluna,-1, "}");
                    return false;
                }
            }
            else{
                return false;
            }
        }
        else{
            erroSint = tokem(t.linha,t.coluna,-1, "{");
            return false;
        }
    }
    erroSint = tokem(linha,coluna,-1, "{");
    return false;
}

bool lerParamFormais(){  
    int token_ini = indice_v;
    if(indice_v < v_tokens.size()){
        tokem t = v_tokens[indice_v];

        //Primeira iteracao                
        if(t.tipo== IDENTIFICADOR){
            indice_v++;
            t = v_tokens[indice_v];
            if(t.tok.compare("-") == 0){
                indice_v++;
                if(lerTipo()){
                }
                else{
                    return false;
                }
            }else{
                erroSint = tokem(t.linha,t.coluna,-1, "-");
                return false;
            }
        }
        else{
            //pode ser vazio
            return true;
            tabela.push(tokem(token_ini,indice_v,-1,"Parametros Formais\t"));
        } 


        //se tiver mais de um, sera separado por ','
        while (t.tok.compare(",")==0){
            indice_v++;
            t = v_tokens[indice_v];
            if(t.tipo== IDENTIFICADOR){
                indice_v++;
                t = v_tokens[indice_v];
                if(t.tok.compare("-") == 0){
                    indice_v++;
                   if(lerTipo()){
                    }
                    else{
                        return false;
                    }
                }else{
                    erroSint = tokem(t.linha,t.coluna,-1, "-");
                    return false;
                }
            }
            else{
                erroSint = tokem(t.linha,t.coluna,-1, ")\" ou identificador de variavel");
                return false;
            } 
        }
        tabela.push(tokem(token_ini,indice_v,-1,"Parametros Formais\t"));
        return true;
    }
    //pode ser vazio
    return true;
}
bool lerNomeArgs(){  
    int token_ini = indice_v;
    if(indice_v < v_tokens.size()){
        tokem t = v_tokens[indice_v];
        do{
            if(t.tipo== IDENTIFICADOR){
                indice_v++;
                t = v_tokens[indice_v];
                if(t.tok.compare( "(") == 0){
                    indice_v++;
                    t = v_tokens[indice_v];
                    if (lerParamFormais()){
                        t = v_tokens[indice_v];
                        if(t.tok.compare(")") == 0){
                            indice_v++;
                            t = v_tokens[indice_v];
                        }
                        else{
                            erroSint = tokem(t.linha,t.coluna,-1, ")");
                            return false;
                        }
                    } 
                    else{
                        return false;
                    }
                }
                else{
                    erroSint = tokem(t.linha,t.coluna,-1, "(");
                    return false;
                }
            }
            else{
                erroSint = tokem(t.linha,t.coluna,-1, "identificador de funcao");
                return false;
            }
        }while(t.tipo== IDENTIFICADOR);
        tabela.push(tokem(token_ini,indice_v,-1,"Nome Args\t\t"));
        return true;
        
    }
    erroSint = tokem(linha,coluna,-1, "identificador de funcao");
    return false;
}

bool lerDeclFunc(){  
    int token_ini = indice_v;
    if(indice_v < v_tokens.size()){
        tokem t = v_tokens[indice_v];
        if(t.tok.compare("proc")==0){
            indice_v++;
            if(lerNomeArgs()){
                t = v_tokens[indice_v];
                if(t.tok.compare("{") == 0){
                    return lerBloco();
                }
                else if (t.tok.compare("-") == 0){
                    indice_v++;
                    if(lerTipo()){
                        if(lerBloco()){
                            tabela.push(tokem(token_ini,indice_v,-1,"Declaracao de Funcao\t"));
                            return true;
                        }
                        else{
                            return false;
                        }
                    }
                    else{
                        return false;
                    }
                } 
                
                else{
                    erroSint = tokem(t.linha,t.coluna,-1, "-\" ou \"{");
                    return false;
                }
            }
            else{
                return false;
            }
        }
        else{
            erroSint = tokem(t.linha,t.coluna,-1, "proc");
            return false;
        }
    }
    erroSint = tokem(linha,coluna,-1, "proc");
    return false;
}

bool lerTipo(){  
    int token_ini = indice_v;
    if(indice_v < v_tokens.size()){
        tokem t = v_tokens[indice_v];
        if((t.tok.compare("int") == 0) || (t.tok.compare("char") == 0) || (t.tok.compare("float") == 0)){
            indice_v++;
            tabela.push(tokem(token_ini,indice_v,-1,"Tipo\t\t\t"));
            return true;
        }
        else{
            erroSint = tokem(t.linha,t.coluna,-1, "tipo");
            return false;
        }
    }
    erroSint = tokem(linha,coluna,-1, "tipo");
    return false;
}

bool lerListaIdents(){  
    int token_ini = indice_v;
    if(indice_v < v_tokens.size()){
        tokem t = v_tokens[indice_v];

        //Primeira iteracao                
        if(t.tipo== IDENTIFICADOR){
            indice_v++;
            t = v_tokens[indice_v];
        }
        else{
            erroSint = tokem(t.linha,t.coluna,-1, "identificador de variavel");
            return false;
        } 

        //se tiver mais de um, sera separado por ','
        while (t.tok.compare(",")==0){
            indice_v++;
            t = v_tokens[indice_v];
            if(t.tipo== IDENTIFICADOR){
                indice_v++;
                t = v_tokens[indice_v];
            }
            else{
                erroSint = tokem(t.linha,t.coluna,-1, "identificador de variavel");
                return false;
            } 
        }
        return true;
        tabela.push(tokem(token_ini,indice_v,-1,"Lista Identificadores"));
    }
    erroSint = tokem(linha,coluna,-1, "identificador de variavel");
    return false;
}

bool lerDeclVar(){  
    int token_ini = indice_v;
    if(indice_v < v_tokens.size()){
        tokem t = v_tokens[indice_v];
        if(t.tok.compare("var")==0){
            indice_v++;
            t = v_tokens[indice_v];
            if(lerListaIdents()){
                t = v_tokens[indice_v];
                if(t.tok.compare("-")==0){
                    indice_v++;
                    if(lerTipo()){
                        t = v_tokens[indice_v];
                        if(t.tok.compare(";")==0){
                            indice_v++;
                            tabela.push(tokem(token_ini,indice_v,-1,"Declaracao de Variavel"));
                            return true;
                        }
                        else{
                            erroSint = tokem(t.linha,t.coluna,-1, ";");
                            return false;
                        }
                    }else
                    {
                        return false;
                    }
                    
                }
                else{
                    erroSint = tokem(t.linha,t.coluna,-1, "-");
                    return false;
                }
            }else{
                return false;
            }
        }
        else{
            erroSint = tokem(t.linha,t.coluna,-1, "var");
            return false;
        }
    }
    erroSint = tokem(linha,coluna,-1, "var");
    return false; 
}

bool lerDeclGlobal(){ 
    if(indice_v < v_tokens.size()){
        tokem t = v_tokens[indice_v];
        if(t.tok.compare("var")==0){
            return lerDeclVar();
        }
        else if(t.tok.compare("proc")==0){
            return lerDeclFunc();
        }
        else{
            erroSint = tokem(linha,coluna,-1, "var\" ou \"proc");
            return false; 
        }
    }
    erroSint = tokem(linha,coluna,-1, "var\" ou \"proc");
    return false; 
}

bool lerProgram(){  
    while(indice_v < v_tokens.size()){
        if (!lerDeclGlobal()){
            return false;
        }
    }
    return true;
}


void tabelaDeSimbolos(){
    cout << "TABELA DE SIMBOLOS: " << endl;
    cout << "| TIPO DO SIMBOLO\t\t || TOKENS " << endl;

    while(!tabela.empty()){
        tokem t = tabela.top();
        tabela.pop();
        
        //t.tok = Tipo do simbolo
        cout<< "| " << t.tok <<"\t || \" ";
        
        //t.linha = tokem inicial
        //t.coluna = tokem final

        int i = t.linha;
        bool maiorQue30 = false;
        //so imprime 30 tokens
        if((t.coluna-i)>30){
            maiorQue30 = true;
        }
        if(!maiorQue30){
            while(i<t.coluna){
                cout << v_tokens[i].tok << " ";
                i++;
            }
            cout<< "\"" << endl;
        }
        else{
            int j = 0;
            //imprime os 20 tokens iniciais
            while(j<20){
                cout << v_tokens[i].tok << " ";
                i++;
                j++;
            }
            cout<< "  [...]  ";

            //imprime os 10 tokens finais
            j = t.coluna - 10;
            while(j<t.coluna){
                cout << v_tokens[i].tok << " ";
                i++;
                j++;
            }
            cout<< "\"" << endl;
        }
    }
    cout << endl << endl;
}