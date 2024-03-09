#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>



typedef enum {
    TOKEN_INVALID,
    TOKEN_STRING,
    TOKEN_NUMBER,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_NULL,
    TOKEN_COLON,
    TOKEN_COMMA,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET
} TokenType;


typedef struct {
    TokenType type;
    char* value;
} Token;


Token* init_token(TokenType type, char* value) {
    Token* token = malloc(sizeof(Token));
    token->type = type;
    token->value = value;
    return token;
}

void free_token(Token* token) {
    free(token->value);
    free(token);
}



Token* get_next_token(char* json) {
    static char* pos = NULL;
    if (json != NULL)
        pos = json;

    while (*pos != '\0') {
        if (isspace(*pos)) {
            pos++;
            continue;
        }

        if (*pos == '"') {
            char* start = ++pos;
            while (*pos != '"') {
                if (*pos == '\\')
                    pos++;
                pos++;
            }
            char* value = strndup(start, pos - start);
            pos++;
            return init_token(TOKEN_STRING, value);
        }

        if (isdigit(*pos) || *pos == '-') {
            char* start = pos;
            while (isdigit(*pos) || *pos == '-' || *pos == '.' || *pos == 'e' || *pos == 'E') {
                pos++;
            }
            char* value = strndup(start, pos - start);
            return init_token(TOKEN_NUMBER, value);
        }

        if (strncmp(pos, "true", 4) == 0) {
            pos += 4;
            return init_token(TOKEN_TRUE, strdup("true"));
        }
        if (strncmp(pos, "false", 5) == 0) {
            pos += 5;
            return init_token(TOKEN_FALSE, strdup("false"));
        }
        if (strncmp(pos, "null", 4) == 0) {
            pos += 4;
            return init_token(TOKEN_NULL, strdup("null"));
        }

        switch (*pos) {
            case ':': pos++; return init_token(TOKEN_COLON, strdup(":"));
            case ',': pos++; return init_token(TOKEN_COMMA, strdup(","));
            case '{': pos++; return init_token(TOKEN_LEFT_BRACE, strdup("{"));
            case '}': pos++; return init_token(TOKEN_RIGHT_BRACE, strdup("}"));
            case '[': pos++; return init_token(TOKEN_LEFT_BRACKET, strdup("["));
            case ']': pos++; return init_token(TOKEN_RIGHT_BRACKET, strdup("]"));
            default:
                fprintf(stderr, "Erreur: Caractère inattendu: '%c'\n", *pos);
                return init_token(TOKEN_INVALID, NULL);
        }
    }

    return NULL;
}