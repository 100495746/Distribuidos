/* Jordan Ikukele Bomolo Mia Raúl Armas Seriñá 504 
100495854@alumnos.uc3m.es 100495746@alumnos.uc3m.es*/

%{                          // SECCION 1 Declaraciones de C-Yacc

#include <stdio.h>
#include <ctype.h>            // declaraciones para tolower
#include <string.h>           // declaraciones para cadenas
#include <stdlib.h>           // declaraciones para exit ()

#define FF fflush(stdout);    // para forzar la impresion inmediata

int yylex () ;
int yyerror () ;
char *mi_malloc (int) ;
char *gen_code (char *) ;
char *int_to_string (int) ;
char *char_to_string (char) ;
char *gen_variable_name(char*, char*);
void add_local_var(char *funcion, char *variable);
int is_local_var(char *funcion, char *variable);

char temp [2048] ;
char current_function[2048] = "";

// Estructuras para manejar las variables dentro de funciones

typedef struct {
    char *f_name;
    char *v_name;
} l_var;
l_var variables_locales[100];

int var_counter = 0;


// Abstract Syntax Tree (AST) Node Structure

typedef struct ASTnode t_node ;

struct ASTnode {
    char *op ;
    int type ;		// leaf, unary or binary nodes
    t_node *left ;
    t_node *right ;
} ;


// Definitions for explicit attributes

typedef struct s_attr {
    int value ;    // - Numeric value of a NUMBER 
    char *code ;   // - to pass IDENTIFIER names, and other translations 
    t_node *node ; // - for possible future use of AST
} t_attr ;

#define YYSTYPE t_attr

%}

// Definitions for explicit attributes

%token NUMBER        
%token IDENTIF       // Identificador=variable
%token INTEGER       // identifica el tipo entero
%token STRING
%token MAIN          // identifica el comienzo del proc. main
%token WHILE         
%token PUTS
%token PRINTF
%token AND
%token OR
%token NEQ
%token EQEQ
%token LEQ
%token MEQ
%token IF
%token FOR
%token ELSE
%token RETURN


%right '='                    // es la ultima operacion que se debe realizar
%left OR
%left AND
%left EQEQ NEQ                     // == y != 
%left '<' '>' LEQ MEQ              // < > <= >= 
%left '+' '-'                 // menor orden de precedencia
%left '*' '/' '%'                // orden de precedencia intermedio
%left UNARY_SIGN '!'              // mayor orden de precedencia

%%                            // Seccion 3 Gramatica - Semantico

axioma:     programa             {  }
            ;

// manejamos la estructura del programa con esta regla 1º la declaracion de variables globales luego las funciones y ultimo el main
programa:  
        var_globales funciones mainer {  printf("%s\n%s\n%s", $1.code, $2.code, $3.code);}

;



// Las siguientes reglas se encargaran de las declaracion de VARIABLES GLOBALES  mediante recursion:



var_globales:  declaracion ';' var_globales   { sprintf(temp, "%s\n%s", $1.code, $3.code);
                                              $$.code = gen_code(temp);}
              |                              {$$.code= "";}


;

// gracias a lista de declaraciones podemos declarar varias variables en una sola linea, mas tarde se extiende a vectores tambien
declaracion:    INTEGER lista_declaraciones          {$$.code = $2.code; }  


;
lista_declaraciones:  
                        // variables
                        IDENTIF asignacion_valor {
                                                if(strcmp(current_function, "") != 0){ add_local_var(current_function, $1.code);}
                                                char *nombre_final = gen_variable_name(current_function, $1.code);
                                                sprintf(temp, "(setq %s %s)", nombre_final, $2.code);
                                                $$.code = gen_code(temp);
                                            }
                    
                      | IDENTIF asignacion_valor ',' lista_declaraciones {
                                                if(strcmp(current_function, "") != 0){ add_local_var(current_function, $1.code);}
                                                char *nombre_final = gen_variable_name(current_function, $1.code);
                                                sprintf(temp, "(setq %s %s)\n%s", nombre_final, $2.code, $4.code);
                                                $$.code = gen_code(temp);
                                            }
                        // vectores 
                      | IDENTIF '[' NUMBER ']' {
                                                    if(strcmp(current_function, "") != 0){ add_local_var(current_function, $1.code); }
                                                    char *nombre_final = gen_variable_name(current_function, $1.code);
                                                    sprintf(temp, "(setq %s (make-array %d))", nombre_final, $3.value);
                                                    $$.code = gen_code(temp);
                                                }
                      | IDENTIF '[' NUMBER ']' ',' lista_declaraciones {
                                                    if(strcmp(current_function, "") != 0){ add_local_var(current_function, $1.code); }
                                                    char *nombre_final = gen_variable_name(current_function, $1.code);
                                                    sprintf(temp, "(setq %s (make-array %d))\n%s", nombre_final, $3.value, $5.code);
                                                    $$.code = gen_code(temp);
                                                }
;


asignacion_valor:   '=' expresion               {$$.code = $2.code;}   // int a = 3+2;
                    |                            { $$.code = "0"; } //int a;

;







// Las siguientes reglas se encargaran de las declaracion y el cuerpo de las FUNCIONES:


funciones:   funcion funciones               { sprintf(temp, "%s\n%s", $1.code, $2.code);
                                              $$.code = gen_code(temp);}

              |                               { $$.code= ""; }

;

// la funcion contiene un bloque inline que maneja la actualizacion de la current_function antes de procesar el bloque de esta forma se nombraran las variables con el nombre de la funcion correspondiente


funcion:  IDENTIF '(' parametros ')' '{'{ strcpy(current_function, $1.code); } bloque '}'  { 
                                                            sprintf(temp, "(defun %s (%s) \n %s\n)",$1.code, $3.code, $7.code);
                                                            $$.code = gen_code(temp);
                                                            strcpy(current_function,"");  //reset 
}
;


parametros:      INTEGER IDENTIF ',' parametros {
                                                     sprintf(temp, "%s %s", $2.code, $4.code);
                                                     $$.code = gen_code(temp);  }
                | INTEGER IDENTIF             {
                                                     sprintf(temp, "%s", $2.code);
                                                     $$.code = gen_code(temp);  }
                |                              {$$.code= ""; }


;

// maneja todo lo que vamos a poder escribir tanto en fnciones como en el main

bloque:     sentencias_bloque ';' bloque { sprintf(temp, "%s\n%s", $1.code, $3.code);
                                           $$.code = gen_code(temp);}
            | whifoo bloque              { sprintf(temp, "%s\n%s", $1.code, $2.code);
                                           $$.code = gen_code(temp);}
            |                            {$$.code="";}
;
 // cada una de las setencias ira en su no terminal correspondiente, nombres bastante intuitivos y descriptivos de que hace cada uno 

sentencias_bloque:   acciones_variables            {$$.code = $1.code; }
                    | printf                       {$$.code = $1.code; }
                    | putin                       {$$.code = $1.code; }
                    | llamada_funcion             {$$.code = $1.code; }
                    | retornador                  {$$.code = $1.code; }

;

// Indica las 2 posibles acciones que se pueden hacer sobre una variale, mas tarde extendido a vectores

acciones_variables:   asignacion         { $$.code = $1.code; }
                     | declaracion        { $$.code = $1.code; }


;

asignacion:   
             // variables
             IDENTIF '=' expresion {
                                        char *nombre_final = gen_variable_name(current_function, $1.code);
                                        sprintf(temp, "(setf %s %s)", nombre_final, $3.code);
                                        $$.code = gen_code(temp); }
              // vectores
             | IDENTIF '[' expresion ']' '=' expresion {
                                        char *nombre_final = gen_variable_name(current_function, $1.code);
                                        sprintf(temp, "(setf (aref %s %s) %s)", nombre_final, $3.code, $6.code);
                                        $$.code = gen_code(temp);
                                    }
;
 

printf:  // cuando solo haya cadena de formato no imprimir nada
        PRINTF '(' STRING ')' { $$.code = gen_code(""); 

        }

       | PRINTF '(' STRING ',' lista_printf ')' { sprintf(temp, "%s", $5.code);
                                                   $$.code = gen_code(temp);
         }

;


lista_printf:    STRING r_printf      {sprintf(temp, "(princ \"%s\") %s", $1.code, $2.code);
                                                $$.code = gen_code(temp);}

                | expresion r_printf  {sprintf(temp, "(princ %s) %s", $1.code, $2.code);
                                                $$.code = gen_code(temp); }


;


r_printf: ',' lista_printf          { $$.code = $2.code; }

               |                    { $$.code=""; }


;


putin:  PUTS '(' STRING ')'                { sprintf (temp, "(print \"%s\")", $3.code) ;
                                           $$.code = gen_code (temp) ;}



;

llamada_funcion:  IDENTIF '(' argumentos ')' {
                                            sprintf(temp, "(%s %s)", $1.code, $3.code);
                                            $$.code = gen_code(temp);}


;

retornador:  RETURN expresion	{
	char *nombre_inter = gen_code(current_function);
	sprintf(temp,
		"(return-from %s (progn %s))",
		nombre_inter, $2.code
	);
	$$.code = gen_code(temp);
}
| { $$.code = ""; }
;



// whifoo -> while+if+for, es un equivalente a bloque, pero para setencias que no precisen tener un ';' al final 

whifoo: while_dec               {$$.code = $1.code; }
        |if_dec                 {$$.code = $1.code; }
        |for_dec                {$$.code = $1.code; }

;


while_dec:      WHILE '(' expresion ')''{'bloque'}'     { sprintf(temp, "(loop while %s do %s)\n",$3.code, $6.code);
                                                          $$.code = gen_code(temp);}

;

if_dec:  
                IF '(' expresion ')' '{' bloque '}' r_if 
                                {
                     char *bloque_then = strstr($6.code, "\n") ? gen_code("(progn\n") : "";
                     char *bloque_else = strstr($8.code, "\n") ? gen_code("(progn\n") : "";
                     sprintf(temp, "(if %s %s%s%s %s%s%s)\n", $3.code, bloque_then, $6.code, bloque_then[0] ? ")" : "",bloque_else, $8.code, bloque_else[0] ? ")" : "");
                     $$.code = gen_code(temp);
                        }
;

r_if:           ELSE '{'bloque'}'                       {$$.code=$3.code;}
                |                                       {$$.code="";}

;
for_dec:          FOR '(' asignacion ';' expresion ';' expresion ')' '{'bloque'}'  { sprintf(temp, "%s\n(loop while %s do %s %s)",$3.code ,$5.code, $10.code, $7.code);
                                                                                    $$.code = gen_code(temp);}

;

 
// Las siguientes reglas se encargaran de la declaracion y el cuerpo del unico MAIN, al ser practicamente una funcion su estructura es similar

mainer:     MAIN '(' ')' '{' set_main bloque '}'    {
                                            sprintf(temp, "(defun main () \n %s\n)\n", $6.code);
                                            $$.code = gen_code(temp);
                                            strcpy(current_function,"");  //reset 
}
;

// este no terminal sirve para asignar el nombre de la current_function antes de procesar el bloque es el equivalante al codigo inline de las funciones nombradas arriba
set_main:  {  strcpy(current_function, "main"); }


;


          
expresion:      termino                  { $$ = $1 ; }
            |   expresion '+' expresion  { sprintf (temp, "(+ %s %s)", $1.code, $3.code) ;
                                           $$.code = gen_code (temp) ; }
            |   expresion '-' expresion  { sprintf (temp, "(- %s %s)", $1.code, $3.code) ;
                                           $$.code = gen_code (temp) ; }
            |   expresion '*' expresion  { sprintf (temp, "(* %s %s)", $1.code, $3.code) ;
                                           $$.code = gen_code (temp) ; }
            |   expresion '/' expresion  { sprintf (temp, "(/ %s %s)", $1.code, $3.code) ;
                                           $$.code = gen_code (temp) ; }
            |   expresion AND expresion  { sprintf (temp, "(and %s %s)", $1.code, $3.code) ;
                                           $$.code = gen_code (temp) ; }
            |   expresion OR expresion   { sprintf (temp, "(or %s %s)", $1.code, $3.code) ;
                                           $$.code = gen_code (temp) ; }
            |  '!' expresion             { sprintf (temp, "(not %s)", $2.code) ;
                                           $$.code = gen_code (temp) ; }
            |   expresion NEQ expresion  { sprintf (temp, "(/= %s %s)", $1.code, $3.code) ;
                                           $$.code = gen_code (temp) ; }
            |   expresion EQEQ expresion  { sprintf (temp, "(= %s %s)", $1.code, $3.code) ;
                                           $$.code = gen_code (temp) ; }
            |   expresion '<' expresion  { sprintf (temp, "(< %s %s)", $1.code, $3.code) ;
                                           $$.code = gen_code (temp) ; }
            |   expresion LEQ expresion  { sprintf (temp, "(<= %s %s)", $1.code, $3.code) ;
                                           $$.code = gen_code (temp) ; }
            |   expresion '>' expresion  { sprintf (temp, "(> %s %s)", $1.code, $3.code) ;
                                           $$.code = gen_code (temp) ; }
            |   expresion MEQ expresion  { sprintf (temp, "(>= %s %s)", $1.code, $3.code) ;
                                           $$.code = gen_code (temp) ; }
            |   expresion '%' expresion  { sprintf (temp, "(mod %s %s)", $1.code, $3.code) ;
                                           $$.code = gen_code (temp) ; }
            | asignacion                 {  $$.code = $1.code;}


            
            ;


termino:        operando                           { $$ = $1 ; }                          
            |   '+' operando %prec UNARY_SIGN      { $$ = $1 ; }
            |   '-' operando %prec UNARY_SIGN      { sprintf (temp, "(- %s)", $2.code) ;
                                                     $$.code = gen_code (temp) ; }    
            ;

operando:       IDENTIF {
                            char *nombre_final = gen_variable_name(current_function, $1.code);
                            sprintf(temp, "%s", nombre_final);
                            $$.code = gen_code(temp); }

            |   NUMBER                   { sprintf (temp, "%d", $1.value) ;
                                           $$.code = gen_code (temp) ; }
            |   '(' expresion ')'        { $$ = $2 ; }

            |  llamada_funcion           {$$ = $1;}

            | IDENTIF '[' expresion ']' {
                        char *nombre_final = gen_variable_name(current_function, $1.code);
                        sprintf(temp, "(aref %s %s)", nombre_final, $3.code);
                        $$.code = gen_code(temp);
                    }


;


argumentos: expresion                    { $$.code = $1.code; }
         | expresion ',' argumentos {
             sprintf(temp, "%s %s", $1.code, $3.code);
             $$.code = gen_code(temp);
         }
         |                             { $$.code = ""; } 

;

%%                            // SECCION 4    Codigo en C

int n_line = 1 ;

int yyerror (mensaje)
char *mensaje ;
{
    fprintf (stderr, "%s en la linea %d\n", mensaje, n_line) ;
    printf ( "\n") ;	// bye
}

char *int_to_string (int n)
{
    sprintf (temp, "%d", n) ;
    return gen_code (temp) ;
}

char *char_to_string (char c)
{
    sprintf (temp, "%c", c) ;
    return gen_code (temp) ;
}

char *my_malloc (int nbytes)       // reserva n bytes de memoria dinamica
{
    char *p ;
    static long int nb = 0;        // sirven para contabilizar la memoria
    static int nv = 0 ;            // solicitada en total

    p = malloc (nbytes) ;
    if (p == NULL) {
        fprintf (stderr, "No queda memoria para %d bytes mas\n", nbytes) ;
        fprintf (stderr, "Reservados %ld bytes en %d llamadas\n", nb, nv) ;
        exit (0) ;
    }
    nb += (long) nbytes ;
    nv++ ;

    return p ;
}


/***************************************************************************/
/********************** Seccion de Palabras Reservadas *********************/
/***************************************************************************/

typedef struct s_keyword { // para las palabras reservadas de C
    char *name ;
    int token ;
} t_keyword ;

t_keyword keywords [] = { // define las palabras reservadas y los
    "main",        MAIN,           // y los token asociados
    "int",         INTEGER,
    "while",      WHILE,
    "puts",      PUTS,
    "printf",   PRINTF,
    "&&",       AND,
    "||",      OR,
    "!=",      NEQ,
    "==",      EQEQ,
    "<=",      LEQ,
    ">=",      MEQ,
    "if",       IF,
    "for",      FOR,
    "else",     ELSE,
    "return",   RETURN,

   NULL,          0               // para marcar el fin de la tabla
} ;

t_keyword *search_keyword (char *symbol_name)
{                                  // Busca n_s en la tabla de pal. res.
                                   // y devuelve puntero a registro (simbolo)
    int i ;
    t_keyword *sim ;

    i = 0 ;
    sim = keywords ;
    while (sim [i].name != NULL) {
	    if (strcmp (sim [i].name, symbol_name) == 0) {
		                             // strcmp(a, b) devuelve == 0 si a==b
            return &(sim [i]) ;
        }
        i++ ;
    }

    return NULL ;
}

 
/***************************************************************************/
/******************* Seccion del Analizador Lexicografico ******************/
/***************************************************************************/

char *gen_code (char *name)     // copia el argumento a un
{                                      // string en memoria dinamica
    char *p ;
    int l ;
	
    l = strlen (name)+1 ;
    p = (char *) my_malloc (l) ;
    strcpy (p, name) ;
	
    return p ;
}

// Necesitamos una fucnion parecida para el nombre de las variables 
// aprovechamos y metemos tambien el bloque if en esta funcion para que compruebe si lleva prefijo o no 

char *gen_variable_name(char *prefix, char *name) {
    char buffer[2048];
    if (strcmp(prefix, "") != 0 && is_local_var(prefix, name)) {
        sprintf(buffer, "%s_%s", prefix, name);
    } else {
        sprintf(buffer, "%s", name);
    }
    return gen_code(buffer); // usa my_malloc internamente
}

void add_local_var(char *funcion, char *variable) {
    variables_locales[var_counter].f_name = gen_code(funcion);
    variables_locales[var_counter].v_name = gen_code(variable);
    var_counter++;
}

int is_local_var(char *funcion, char *variable) {
    for(int i = 0; i < var_counter; i++) {
        if(strcmp(variables_locales[i].f_name, funcion) == 0 && 
           strcmp(variables_locales[i].v_name, variable) == 0) {
            return 1;
        }
    }
    return 0;
}





int yylex ()
{
// NO MODIFICAR ESTA FUNCION SIN PERMISO
    int i ;
    unsigned char c ;
    unsigned char cc ;
    char ops_expandibles [] = "!<=|>%&/+-*" ;
    char temp_str [256] ;
    t_keyword *symbol ;

    do {
        c = getchar () ;

        if (c == '#') {	// Ignora las lineas que empiezan por #  (#define, #include)
            do {		//	OJO que puede funcionar mal si una linea contiene #
                c = getchar () ;
            } while (c != '\n') ;
        }

        if (c == '/') {	// Si la linea contiene un / puede ser inicio de comentario
            cc = getchar () ;
            if (cc != '/') {   // Si el siguiente char es /  es un comentario, pero...
                ungetc (cc, stdin) ;
            } else {
                c = getchar () ;	// ...
                if (c == '@') {	// Si es la secuencia //@  ==> transcribimos la linea
                    do {		// Se trata de codigo inline (Codigo embebido en C)
                        c = getchar () ;
                        putchar (c) ;
                    } while (c != '\n') ;
                } else {		// ==> comentario, ignorar la linea
                    while (c != '\n') {
                        c = getchar () ;
                    }
                }
            }
        } else if (c == '\\') c = getchar () ;
		
        if (c == '\n')
            n_line++ ;

    } while (c == ' ' || c == '\n' || c == 10 || c == 13 || c == '\t') ;

    if (c == '\"') {
        i = 0 ;
        do {
            c = getchar () ;
            temp_str [i++] = c ;
        } while (c != '\"' && i < 255) ;
        if (i == 256) {
            printf ("AVISO: string con mas de 255 caracteres en linea %d\n", n_line) ;
        }		 	// habria que leer hasta el siguiente " , pero, y si falta?
        temp_str [--i] = '\0' ;
        yylval.code = gen_code (temp_str) ;
        return (STRING) ;
    }

    if (c == '.' || (c >= '0' && c <= '9')) {
        ungetc (c, stdin) ;
        scanf ("%d", &yylval.value) ;
//         printf ("\nDEV: NUMBER %d\n", yylval.value) ;        // PARA DEPURAR
        return NUMBER ;
    }

    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        i = 0 ;
        while (((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') || c == '_') && i < 255) {
            temp_str [i++] = tolower (c) ;
            c = getchar () ;
        }
        temp_str [i] = '\0' ;
        ungetc (c, stdin) ;

        yylval.code = gen_code (temp_str) ;
        symbol = search_keyword (yylval.code) ;
        if (symbol == NULL) {    // no es palabra reservada -> identificador antes vrariabre
//               printf ("\nDEV: IDENTIF %s\n", yylval.code) ;    // PARA DEPURAR
            return (IDENTIF) ;
        } else {
//               printf ("\nDEV: OTRO %s\n", yylval.code) ;       // PARA DEPURAR
            return (symbol->token) ;
        }
    }

    if (strchr (ops_expandibles, c) != NULL) { // busca c en ops_expandibles
        cc = getchar () ;
        sprintf (temp_str, "%c%c", (char) c, (char) cc) ;
        symbol = search_keyword (temp_str) ;
        if (symbol == NULL) {
            ungetc (cc, stdin) ;
            yylval.code = NULL ;
            return (c) ;
        } else {
            yylval.code = gen_code (temp_str) ; // aunque no se use
            return (symbol->token) ;
        }
    }

//    printf ("\nDEV: LITERAL %d #%c#\n", (int) c, c) ;      // PARA DEPURAR
    if (c == EOF || c == 255 || c == 26) {
//         printf ("tEOF ") ;                                // PARA DEPURAR
        return (0) ;
    }

    return c ;
}


int main ()
{
    yyparse () ;
}

    

