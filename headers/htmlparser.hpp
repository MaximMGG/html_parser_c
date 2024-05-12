#ifndef HTML_PARSER
#define HTML_PARSER

#include <string>
#include <stdlib.h>
#include <list>
#include <curl/curl.h>
#include <string.h>




namespace Parser {

    class Tag {
        private:
            std::string tag;
            std::string type;
            std::string type_content;
            int count = 0;
        public:
            Tag(const std::string tag, const std::string type, const std::string type_content) 
                : tag(tag), type(type), type_content(type_content) {} 
            Tag(std::string expression) {
                char buf[512]{0};
                bool has_type = false;

                if (expression[0] >= '0' && expression[0] <= '9') {
                    count = expression[0] - 48;
                }

                for(int i = count == 0 ? 0 : 1, j = 0; i < expression.size(); i++) {
                    switch(expression[i]) {
                        case '(': {
                                      this->tag = buf;
                                      memset(buf, 0, 512);
                                      j = 0;
                                      has_type = true;
                                  } break;
                        case '[': {
                                      this->type = buf;
                                      memset(buf, 0, 512);
                                      j = 0;
                                  } break;
                        case ']': {
                                      this->type_content = buf;
                                      memset(buf, 0, 512);
                                      j = 0;
                                  } break;
                        default: {
                            buf[j++] = expression[i];

                        }
                    }
                }
                if (!has_type) {
                    this->tag = buf;
                }
            }

            ~Tag(){};
            int get_count() {
                return this->count;
            }

            std::string get_full_tag() {
                std::string ftag{};
                ftag = "<";
                ftag += tag;
                if (type != "") {
                    ftag += " " + type;
                }
                if (type_content != "") {
                    ftag += "=\"" + type_content + "\"";
                }
                return ftag;
            }

            std::string get_close_tag() {
                std::string close_tag{};
                close_tag = "</";
                close_tag += tag + ">";
                return close_tag;
            }
    };


    //body > div > div > div > div > 4a!

    class HTML_content {
        private:
            std::list<std::string> html;
            std::list<Tag> tags;
            std::string expression;
            bool in_tag = false;


            void set_tags() {
                char buf[512]{0};
                for(int i = 0, j = 0; i < expression.size(); i++) {
                    if (expression[i] == ' ') {
                        tags.push_back(Tag(buf));
                        memset(buf, 0, 512);
                        j = 0;
                    } else if (expression[i] == '>') {
                        i++;
                        continue;
                    } 
                    else {
                        buf[j++] = expression[i];
                    }
                }
                int buf_l = strlen(buf);
                if (buf[buf_l - 1] == '!') {
                    in_tag = true;
                    buf[buf_l - 1] = '\0';
                    tags.push_back(Tag(buf));
                } else {
                    tags.push_back(Tag(buf));
                }
            }


        public:

            HTML_content(std::list<std::string> html, std::string expression = "") 
                : html(html), expression(expression){
                    if (expression != "") {
                        set_tags();
                    }
                };
            ~HTML_content(){};

            std::string get_html_content(std::string expression) {
                this->expression = expression;
                set_tags();

                int same = 0;
                bool find = false;

                std::string content = "";

                std::list<std::string> buf;

                for(std::list<Tag>::iterator i = tags.begin(); i != tags.end(); i++) {
                    std::string t_open = i->get_full_tag();
                    std::string t_close = i->get_close_tag();
                    int tag_count = i->get_count();

                    for(std::list<std::string>::iterator it = html.begin(); it != html.end(); it++) {
                        if (it->find(t_open) && !find) {
                            if (tag_count > 0) {
                                tag_count--;
                                continue;
                            }
                            find = true;
                            buf.push_back(*it);
                        } else if (it->find(t_open) && find) {
                            same++;
                            buf.push_back(*it);
                        } 
                        if (it->find(t_close)) {
                            if (same > 0) {
                                same--;
                                buf.push_back(*it);
                            } else {
                                buf.push_back(*it);
                                html = buf;
                                break;
                            }
                        }
                    }
                }

                for(std::list<std::string>::iterator it = html.begin(); it != html.end(); it++) {
                    content += *it;
                }

                return content;
            }
    };


    class Url {
        private:
            std::list<std::string> html;
            std::string url;
            CURL *curl;
            CURLcode res;

            struct mem {
                char *res;
                size_t size;

            };

        public:
            Url(std::string url) : url(url) {}
            ~Url(){}


            static size_t mset(void *data, size_t size, size_t smemb, void *res) {
                size_t real_size = size * smemb;
                struct mem *m = (struct mem *) res;

                char *p = (char *)realloc(m->res, m->size + real_size + 1);
                if (!p) {
                    return 0;
                }

                m->res = p;
                memcpy(&(m->res[m->size]), data, real_size);
                m->size += real_size;
                m->res[real_size] = 0;

                return real_size;
            }



            void connect() {
                curl = curl_easy_init();

                struct mem m {0};
                m.res = (char *) malloc(1);

                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&m);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, mset);

                res = curl_easy_perform(curl);

                int len = strlen(m.res);
                char buf[512]{0};

                for(int i = 0, j = 0; i < len; i++) {
                    if (m.res[i] == '\n') {
                        html.push_back(buf);
                        memset(buf, 0, 512);
                        j = 0;
                    } else {
                        buf[j++] = m.res[i];
                    }
                } 

                free(m.res);
            }

            std::list<std::string> get_html() {
                return html;
            }
    };
}


#endif //HTML_PARSER
