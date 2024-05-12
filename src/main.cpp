#include "../headers/htmlparser.hpp"
#include <iostream>


int main() {
        
    Parser::Url url = Parser::Url("https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html");
    url.connect();
    std::list<std::string> html = url.get_html();
    Parser::HTML_content h_con = Parser::HTML_content(html);
    std::string con = h_con.get_html_content("body > div > div > div > div > 4a");

    std::cout << con << '\n';


    return 0;
}
