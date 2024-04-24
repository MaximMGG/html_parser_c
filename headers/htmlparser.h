#ifndef _HTMLPARSER_H_
#define _HTMLPARSER_H_

typedef struct {
    char *html;
    char *parser_content;
    unsigned int size;
} html_content;

typedef enum {
    PARSER_OK,
    PARSER_BAD_URL,
    PARSER_PARS_ERROR,
    PARSER_WRITEFUNC_ERROR
}PARSER_CODE;

PARSER_CODE Parser_init(const char *url, html_content *content);
PARSER_CODE Parser_pars(const char *expression, html_content *page);

void Paresr_cleanup(html_content *page);

#endif //_HTMLPARSER_H_
