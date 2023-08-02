#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Definir a estrutura para representar um Token
typedef struct {
    char type;      // Tipo do token ('+', '-', '*', '/', '(', ')', '=', 'd' (número), 's' (string), 'v' (variável), 'k' (keyword)
    char *lexeme;   // O lexema (o valor textual do token)
} Token;

// Definir os tipos de token
#define T_PLUS     '+'
#define T_MINUS    '-'
#define T_MULTIPLY '*'
#define T_DIVIDE   '/'
#define T_LPAREN   '('
#define T_RPAREN   ')'
#define T_EQUAL    '='
#define T_NUMBER   'd'
#define T_STRING   's'
#define T_VARIABLE 'v'
#define T_KEYWORD  'k'

// Função para obter o próximo token do código fonte
Token nextToken(char **source) {
    Token token;
    token.lexeme = NULL;

    while (**source && **source == ' ') {
        (*source)++;
    }

    char currentChar = **source;

    if (currentChar == '\0') {
        token.type = '\0';
        return token;
    }

    if (currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/' ||
        currentChar == '(' || currentChar == ')' || currentChar == '=') {
        token.type = currentChar;
        (*source)++;
    } else if (currentChar >= '0' && currentChar <= '9') {
        token.type = T_NUMBER;
        int lexemeLength = 0;
        char *lexeme = (char *)malloc(100 * sizeof(char));
        while (**source && **source >= '0' && **source <= '9') {
            lexeme[lexemeLength++] = **source;
            (*source)++;
        }
        lexeme[lexemeLength] = '\0';
        token.lexeme = lexeme;
    } else if (currentChar == '"') {
        token.type = T_STRING;
        (*source)++;
        int lexemeLength = 0;
        char *lexeme = (char *)malloc(100 * sizeof(char));
        while (**source && **source != '"') {
            lexeme[lexemeLength++] = **source;
            (*source)++;
        }
        lexeme[lexemeLength] = '\0';
        (*source)++;
        token.lexeme = lexeme;
    } else if (currentChar >= 'a' && currentChar <= 'z') {
        token.type = T_VARIABLE;
        int lexemeLength = 0;
        char *lexeme = (char *)malloc(100 * sizeof(char));
        while (**source && ((**source >= 'a' && **source <= 'z') || (**source >= '0' && **source <= '9'))) {
            lexeme[lexemeLength++] = **source;
            (*source)++;
        }
        lexeme[lexemeLength] = '\0';
        token.lexeme = lexeme;
    } else {
        token.type = T_KEYWORD;
        int lexemeLength = 0;
        char *lexeme = (char *)malloc(100 * sizeof(char));
        while (**source && **source != ' ') {
            lexeme[lexemeLength++] = **source;
            (*source)++;
        }
        lexeme[lexemeLength] = '\0';
        token.lexeme = lexeme;
    }

    return token;
}

// Função para liberar a memória alocada para o token
void freeToken(Token *token) {
    if (token->lexeme) {
        free(token->lexeme);
    }
}

// Função para executar o código SimpleLang
void executeSimpleLang(char *source) {
    Token token;

    while ((token = nextToken(&source)).type != '\0') {
        if (token.type == T_EQUAL) {
            Token variable = nextToken(&source);
            if (variable.type != T_VARIABLE) {
                printf("Error: Invalid variable name.\n");
                break;
            }

            Token value = nextToken(&source);
            if (value.type == T_NUMBER) {
                printf("Assigning %s = %s\n", variable.lexeme, value.lexeme);
            } else if (value.type == T_STRING) {
                printf("Assigning %s = \"%s\"\n", variable.lexeme, value.lexeme);
            } else {
                printf("Error: Invalid value type.\n");
                break;
            }
        }
    }
}

// Exemplo de código em SimpleLang
int main() {
    char source[] = "x = 10 y = \"Hello, SimpleLang!\" z = 5";
    executeSimpleLang(source);

    return 0;
}