#include <stdio.h>
#include "webcrawler.h"


#define BUFFER_MAX 500			 //maximum word size
#define DESCRIPTION_MAX 500

/*
* CS251 Lab 3 - Summer 2014
* @author Taylor T. Johnson
*
* webcrawler.cpp
*
* This program takes in root URLs as command line args,
* fetches the HTML files of the roots, finds links found
* in said files, and creates an array as well as a linked list of 
* these URLs leading to valid HTML or text files. While parsing the HTML
* files, a descripition of each page is extracted from the first 500 characters
* in the content. Words encountered in the content are mapped to a hash table
* containing a linked list of all the URLs that lead to pages in which the
* word can be found.
*/

char word_buffer[BUFFER_MAX];	//global char array to for fetching content
int buffer_index = 0;	//next empty index in parse_Buffer
int description_index = 0;	//next empty index in the description
int description_word_count = 0;	//number of words in current URL's description

/*
* WebCrawler - constructor for WebCrawler Object
* 
* Parameters:
*	maxUrls: maximum number of URLs to store in _urlArray
*	nurlRoots: number of root URLS
*	urlRoots: points to string array containing root URLs
*/
WebCrawler::WebCrawler(int maxUrls, int nurlRoots, const char ** urlRoots){
	_maxUrls = maxUrls;
	_urlArray = new URLRecord[_maxUrls];
	_headURL = 0;
	_tailURL = 0;
	_urlToURLRecord = new HashTableTemplate<int>();
	_wordToURLRecordList = new HashTableTemplate<URLRecordList*>();

	int i = 0;
	int n = 0;

	for(i = 0; i < nurlRoots; i++){
		if(fetchHTML(urlRoots[i], &n)){
			//add urlRoots to urlArray
			_urlArray[_tailURL]._url = strdup(urlRoots[i]);
			//add urlRecord to url list	
			_urlToURLRecord->insertItem(_urlArray[_tailURL]._url, _tailURL);
			++_tailURL;	
		}
	}

	//allocate space for descriptions in each URLRecord
	for(i = 0; i < _maxUrls; i++){
		_urlArray[i]._description = (char *)calloc(sizeof(char), DESCRIPTION_MAX + 1);
	}
}


/*
* isWord
* This function returns true if the string contains
* only alphabetic characters and apostrophes.
*
* Parameters:
*	word: string containing word
*	len: length of word
*/
bool isWord(char * word, int len){
	int i = 0;
	for(i = 0; i < len; i++){
		char c = word[i];
		if( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
			c == '\''){
			continue;
		}else{
			return false;
		}
	}
	return true;
}

/*
* addWordToWordTable
* Checks if the word has already been added to the _wordToURLRecord
* hash table. If it is not found, it is added to the table with a linked
* list containing the index of the URL in the URLArray as it's data.
* If the word was previously added to the table, it traverses the word's
* linked list of URLRecordLists to ensure it was not previously encountered
* at the _headURL (curred HTML file being parsed). If it was not previously 
* encouted at _headURL, it is added to the URLRecord list for the current word
*/
void
WebCrawler::addWordToWordTable(const char * word){
	//determine if word exists in word table
	URLRecordList* url_entry;
	URLRecordList* prev_entry;
	if( _wordToURLRecordList->find(word, &url_entry) == false){
		//word is not in table; add to table
		//make new URLRecordList node with current data
		url_entry = new URLRecordList;
		url_entry->_urlRecordIndex = _headURL;
		url_entry->_next = NULL;
		//insert word into table
		_wordToURLRecordList->insertItem(word, url_entry);
		return;
	}else{
		//word was found in table
		//ensure word was not previously encoutered in current HTML file
		//printf("WORD ALREADY IN TABLE!!!\n");
		prev_entry = url_entry;
		int i = 0;
		while(url_entry != NULL){
			if(url_entry->_urlRecordIndex == _headURL){
			//word was previously found and added to list at current URL
			return;
			//printf("ALREADY FOUND HERE!!!!\n");
			}
			if(i > 0){
				prev_entry = url_entry;
			}
			i++;
			url_entry = url_entry->_next;
		}
		//word previously added to table, but not at this URL
		//add current url to URLRecordList
		url_entry = new URLRecordList;	//COULD POTENTIALLY STILL BE NULL????
		url_entry->_urlRecordIndex = _headURL;
		url_entry->_next = NULL;
		prev_entry->_next = url_entry;
		return;
	}
}

/*
*	onAnchorFound
*	This function is called by parse() when a url inside tags is found in the HTML doc
*	This function checks if URL leads to a valid HTML or text file
*	If it does, the url is added to the _urlArray and url list in the webcrawler obj.
* If url does not lead to a valid HTML or text file, the function simply returns
*/	
void
WebCrawler::onAnchorFound(char* url){
	//ensure url is an HTML file
	if(_tailURL == _maxUrls || url == NULL){
	//list is full or there was a problem with the url
	return;
	}

	int n = 0;
	char * html;


	//Fix relative urls
	if(url[0] != 'h' && url[1] != 't' && url[2] != 't' && url[3] != 'p'){
		//Encountered relative URL; append to head URL
		if(url[0] != '/'){
			return;
		}
		int host_len = strlen(_urlArray[_headURL]._url);	//find length of host URL
		int relative_len = strlen(url);						//find length of relative url
		char* full_url = (char*)malloc(sizeof(char) * (host_len + relative_len +1));
		strcpy(full_url, _urlArray[_headURL]._url);
		strcat(full_url, url);
		url = full_url;
		//url now points to complete (non-relative) link encountered by parse()
	}

	//ensure url does not already exist in _urlArray by using URL hash table
	int index = 0;
	if(_urlToURLRecord->find(url, &index) == false){
		//url is a new url
		if((html = fetchHTML(url, &n)) == NULL){
			//url does not lead to a valid HTML or Text file
			return;
		}
		_urlArray[_tailURL]._url = strdup(url);	//add url to _urlArray
		_urlToURLRecord->insertItem(_urlArray[_tailURL]._url, _tailURL);	//add url to url list
		_tailURL++;
	}
}

/*
* onContentFound
* This function is called by parse() when a character outside of tags in an HTML
* file is encountered. Parameter "c" is added to a global word_buffer until a space
* character is encoutered. When this occurs, the word in the buffer is added to the
* description in the current URLRecord while there is still room. This function then 
* calls the helper function addWordToTable with a dynamically allocated copy of the 
* word in the word_buffer to add the word to the hash table and/or to add the current
* URL to said word's URLRecordList in the table, if applicable.
*/
void
WebCrawler::onContentFound(char c){

	//add character to parse_buffer and description if applicable


	if(c != ' ' && c != '\t' && c != '\n' && c != '.' && c != ','
		&& c != '!' && c != ':' && c != ';' && c != '\"'){
		//"c" is simply another character in the current word
		//add "c" to the parse_buffer at the current buffer_index
		if(buffer_index < BUFFER_MAX -1){
			word_buffer[buffer_index++] = c;
		}
	}else{
		//reached end of current word
		word_buffer[buffer_index++] = '\0';
		//printf("%s\n",word_buffer); 
		int word_len = strlen(word_buffer);	//find length of current word
		//ensure word is a valid word
		if(!isWord(word_buffer, word_len)){
			buffer_index = 0;
			return;	//not a valid word
		}

		if( description_index + word_len < DESCRIPTION_MAX && description_word_count < 100 ){
			//there is still room in current URL's description to add word
			//add word to current URL's description
			strcat(_urlArray[_headURL]._description, word_buffer);	//add current word to URL's description
			description_word_count++;	//set description_index to index of null terminator
			description_index = description_index + word_len;
			if(description_word_count < 100 && description_index < DESCRIPTION_MAX ){
				//There are less than 100 words in current URL's description and still space to add a space character between words
				strcat(_urlArray[_headURL]._description, " ");	//add space in description after last word
				description_index++;
			}	
		}
		//insert word and URLRecord into _wordToURLRecord hash table
		addWordToWordTable(strdup(word_buffer));
		buffer_index = 0;
	}


}

/*
* writeURLFIle
* This function writes all URLs with their respective
* descriptions to a text file whose name is specified
* by the parameter "urlFileName"
*
*	Parameters:
*	urlFileName: string to name output file
*/
void
WebCrawler::writeURLFile(const char* urlFileName){
	int i = 0;
	FILE* fp;
	fp = fopen(urlFileName, "w");
	for(i = 0; i < _tailURL; i++){
		fprintf(fp, "%d %s\n",i, _urlArray[i]._url);
		fprintf(fp, "%s\n\n", _urlArray[i]._description);
	}
	fclose(fp);
}

/*
* writeWordFile
* This function traverses the word hash table printing to file 
* each word in the table followed by a list of the indexes of the 
* URLs in the _urlArray at which the word was encountered.
*
* Parameters:
* wordFileName: string to name output file
*/
void
WebCrawler::writeWordFile(const char *wordFileName){
	FILE* fp = fopen(wordFileName, "w");
	const char * word;
	URLRecordList* list;

	HashTableTemplateIterator<URLRecordList*> iterator(_wordToURLRecordList);

	while(iterator.next(word, list)){	
		fprintf(fp, "%s ",word);
		while(list){
			fprintf(fp, "%d ", list->_urlRecordIndex);
			list = list->_next;
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}


/*
* crawl
* This function iterates through the URL array fetching
* the HTML by calling fetchHTML() and calls parse() on each
* fetched HTML string. parse() adds urls and words encountered
* to their respective data structures. This function continues
* to add and parse URLs until all URLs in the list have been parsed
*/
void
WebCrawler::crawl(){
char * html = NULL;
int size = 0;

	while(_headURL < _tailURL){
		buffer_index = 0;	
		description_index = 0;	
		description_word_count = 0;	
		
		printf("URL INDEX: %d\n", _headURL);
		assert(_urlArray[_headURL]._url != NULL);

		//fetch _headURL's HTML
		html = fetchHTML(_urlArray[_headURL]._url, &size);
		char * htmlCopy = strdup(html);
		parse(htmlCopy, size);	//parse _headURL
		free(htmlCopy);
		_headURL++;	//increment _headURL
	}
}

int main(int argc, const char ** argv){

	int maxUrls = 1000;	//default # of URLS
	int nurlRoots = 0;	
	int i = 1;	//first argument in argv containing a URL

	if(argc >= 4 && strcmp(argv[1],"-u") == 0){
		//number of max URLs specified
		maxUrls = atoi(argv[2]);	//update max # of URLs
		i = 3;
	}

	nurlRoots = argc - i;
	if(nurlRoots > maxUrls){
		nurlRoots = maxUrls;
	}

	WebCrawler wc(maxUrls, nurlRoots, argv + i);	//initialize web crawler

	wc.crawl();	

	wc.writeURLFile("url.txt");	//write url list with descriptions to file
	wc.writeWordFile("word.txt");	//write words with list of url indexes where found 

	return 0;
}