#ifndef _HTMLPARSER_H_
#define _HTMLPARSER_H_

typedef struct {
    char *html;
    unsigned int size;
} html_content;

html_content *Parser_init(const char *url);
char *Parser_pars(const char *expression, html_content *page);

void Paresr_cleanup(html_content *page);

#endif //_HTMLPARSER_H_
