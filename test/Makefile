MAIN = htmlp
HTMLP_SRC = $(wildcard ../src/*.c) html_parser_test.c
HTMLP_OBJ = $(patsubst %.c, %.o, $(HTMLP_SRC))

$(MAIN): $(HTMLP_OBJ)
	gcc -o $@ $^ -g

%.o: %.c
	gcc -o $@ -c $< -g

clear:
	rm $(MAIN) $(HTMLP_OBJ)
