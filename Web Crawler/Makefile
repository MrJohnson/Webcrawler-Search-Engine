
all: gethttp webcrawler

gethttp: gethttp.cpp openhttp.cpp SimpleHTMLParser.cpp
	g++ -o gethttp -g gethttp.cpp openhttp.cpp SimpleHTMLParser.cpp

webcrawler: webcrawler.cpp openhttp.cpp SimpleHTMLParser.cpp
	g++ -o webcrawler -g webcrawler.cpp openhttp.cpp SimpleHTMLParser.cpp


run:
	clear
	./webcrawler -u 1000 http://www.purdue.edu

clean:
	clear
	rm -f url.txt word.txt *.o gethttp webcrawler  core

