
#include <string.h>
#include "search-engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>


#define MAX_WORD 550
#define MAX_URLS 1000
#define BILLION  10E9

/*
 * @ author Taylor T. Johnson
 * CS251 - Summer 2014
 */


bool ranked = false;

void
SearchEngine::build_url_array(){
  _urlArray = new URLRecord[MAX_URLS];
  FILE* urls;
  urls = fopen("url.txt", "r");

  fseek(urls, 0, SEEK_END);
  int urlFSize = ftell(urls);
  rewind(urls);

  // allocate memory to contain whole file
  char* file_buffer = (char*)malloc(sizeof(char)*urlFSize);

  // copy url file into buffer
  size_t result = fread(file_buffer, 1, urlFSize, urls);

  char buffer[MAX_WORD];
  int f_index = 0;    // index in file buffer
  int b_index = 0;    // index in word buffer
  int url_index = 0;  // current url in url array


  while(file_buffer[f_index] != '\0'){
    // skip to begining of url
    while(file_buffer[f_index] != 'h'){
      if( file_buffer[f_index] == '\0'){
        //reached end of file
        free(file_buffer);
        fclose(urls);
        return;
      }
      f_index++;
    }
    // fill buffer with url
    b_index = 0;
    while(file_buffer[f_index] != '\n'){
      buffer[b_index] = file_buffer[f_index];
      b_index++;
      f_index++;
    }
    buffer[b_index++] = '\0';   // null terminate buffer to prepare for strdup
    // copy url in buffer to current urlRecord's in _urlArray
    _urlArray[url_index]._url = strdup(buffer);

    f_index++; //increment past newline after url
    b_index = 0;   //reset buffer index

    // fill buffer with URL description from url.txt
    while(file_buffer[f_index] != '\n'){
      buffer[b_index] = file_buffer[f_index];
      b_index++;
      f_index++;
    }
    buffer[b_index++] = '\0';   // null terminate buffer to prepare for strdup
    // copy description in buffer to current urlRecord's in _urlArray
    _urlArray[url_index]._description = strdup(buffer);

    //  current url and url's description are now in _urlArray
    url_index++; 
  }

}


/*
* writeURLFIle
* This function writes all URLs with their respective
* descriptions to a text file whose name is specified
* by the parameter "urlFileName"
*
* Parameters:
* urlFileName: string to name output file
*/
void
SearchEngine::writeURLFile(const char* urlFileName){
  int i = 0;
  FILE* fp;
  fp = fopen(urlFileName, "w");
  for(i = 0; i < MAX_URLS; i++){
    fprintf(fp, "%d %s\n",i, _urlArray[i]._url);
    fprintf(fp, "%s\n\n", _urlArray[i]._description);
  }
  fclose(fp);
}

bool
isAlpha(char c){
  if( (c >= 'a' && c <= 'z') || (c >= 'A' && c<= 'Z')){
    return true;
  } else {
    return false;
  }
}

bool
isDigit(char c){
  if( c >= '0' && c <= '9'){
    return true;
  }else{
    return false;
  }
}
void
SearchEngine::build_dictionary(){
  //ArrayDictionary* array = new ArrayDictionary;
  FILE* words;
  words = fopen("word.txt", "r");

  //obtain file sizes
  fseek(words, 0, SEEK_END);
  int wordFSize = ftell(words);
  rewind(words);

  // allocate memory to contain whole file 
  char* file_buffer = (char*)malloc(sizeof(char)*wordFSize);

  // copy url file into buffer
  size_t result = fread(file_buffer, 1, wordFSize, words);

  char buffer[MAX_WORD];
  char digit_buffer[5];
  int b_index = 0;
  int f_index = 0;
  int d_index = 0;

  while(file_buffer[f_index] != '\0'){

    URLRecordList* record = NULL;
    // skip to beginning of next word
    while( !isAlpha(file_buffer[f_index]) ){
      if(file_buffer[f_index] == '\0'){
        // reached end of file
        free(file_buffer);
        fclose(words);
        return;
      }
      f_index++;
    }
    // currently at beginning of new word; copy word into buffer
    b_index = 0;
    while( file_buffer[f_index] != ' '){
       if( file_buffer[f_index] == '\0'){
        //reached end of file
        free(file_buffer);
        fclose(words);
        return;
      }
      buffer[b_index] = file_buffer[f_index];
      b_index++;
      f_index++;
    }
    // word now in buffer; terminate with null terminator for copying
    buffer[b_index] = '\0';

    d_index = 0;
    while( file_buffer[f_index] != '\n'){
      //create linked list of url indexes following word
      if( file_buffer[f_index] == '\0'){
        //reached end of file
        free(file_buffer);
        fclose(words);
        return;
      }else if( file_buffer[f_index] == ' '){
        if(d_index != 0){
          // reached end of current url index 
          digit_buffer[d_index] = '\0';
          d_index = 0;
          // digit buffer now contains a null terminated string containing a url index
          if(record == NULL){
            // encountered first url index listed after word
            record = new URLRecordList;     //create URLRecord
            record->_urlIndex = atoi(digit_buffer);
            record->_next = NULL;
            assert(record != NULL);
          } else {
            // encountered another url index in list following word
            // encountered another url index after word 
            URLRecordList* current = record;    // set current to head
            // navigate linked list until end is found
            while( current->_next != NULL){
              current = current->_next;
            }
            assert(current != NULL);
            URLRecordList* n = new URLRecordList;
    
            n->_urlIndex = atoi(digit_buffer);
            n->_next = NULL;
            current->_next = n;
          }
        }
        f_index++;
      }else{
        digit_buffer[d_index] = file_buffer[f_index];
        d_index++;
        f_index++;
      }
    }
      // add url in buffer and new record to array dictionary
      _wordToURLList->addRecord(buffer, record);
  }
}


SearchEngine::SearchEngine( int port, DictionaryType dictionaryType):
  MiniHTTPD(port)
{
  build_url_array();
  switch(dictionaryType){
    case 0:
      // Array
      _wordToURLList = new ArrayDictionary;
      build_dictionary();
      break;
    case 1:
      // Hash
      _wordToURLList = new HashDictionary;
      build_dictionary();
      break;
    case 2:
      // AVL
      _wordToURLList = new AVLDictionary;
      build_dictionary();
      break;
    case 3:
      // BinarySearchDictionary
      _wordToURLList = new BinarySearchDictionary;
      build_dictionary(); 
      break;
  }
  /*
  // Create dictionary of the indicated type
  if(strcmp(DictionaryType, "array") == 0){
    // Dictionary is of type array
    ;
  }
*/

  // Populate dictionary and sort it if necessary
}

void
SearchEngine::dispatch( FILE * fout, const char * documentRequested)
{
  struct timespec start, end;
  if(clock_gettime( CLOCK_REALTIME, &start) == -1){
    printf("GETTIME ERROR!\n");
  }
  
  //start = gethrtime();
  if (strcmp(documentRequested, "/")==0) {
    // Send initial form
    fprintf(fout, "<TITLE>CS251 Search</TITLE>\r\n");
    fprintf(fout, "<CENTER><H1><em>Ask Cheeves</em></H1>\n");
    fprintf(fout, "<H2>\n");
    fprintf(fout, "<FORM ACTION=\"search\">\n");
    fprintf(fout, "Search:\n");
    fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
    fprintf(fout, "</H2>\n");
    fprintf(fout, "</FORM></CENTER>\n");
    return;
  }

  // TODO: The words to search in "documentRequested" are in the form
  // /search?word=a+b+c
  //
  // You need to separate the words before search
  // Search the words in the dictionary and find the URLs that
  // are common for al the words. Then print the URLs and descriptions
  // in HTML. Make the output look nicer.

  char buffer[MAX_WORD];
  char buffer_index =0;
  int num_words;
  int i = 13;     //increment i to index of start of first search parameter in documentRequested
  while(documentRequested[i] != '\0'){
    //count # of '+';
    if(documentRequested[i] == '+'){
      ++num_words;
    }
    i++;
  }
  ++num_words; 
  
  //allocate memory to hold search parameters
  i = 13;         //reset i to index of start of first search param in documentRequested
  
  //create array of char pointers to store searched words
  char** searched_words = (char**)malloc(sizeof(char*)*num_words);
  int current_word = 0;
  //put all searched parameters in searched_words
  while(documentRequested[i] != '\0'){
    if(documentRequested[i] == '+'){
      //reached end of current word; add word buffer to searched words
      buffer[buffer_index] = '\0';
      searched_words[current_word] = strdup(buffer);
      current_word++;
      buffer_index = 0;
      i++;
    }else{
      //add character to buffer
      buffer[buffer_index] = documentRequested[i];
      buffer_index++;
      i++;
    }
  }
  //add final word in buffer to searched words
  buffer[buffer_index] = '\0';
  searched_words[current_word] = strdup(buffer);

  // Here the URLs printed are hardwired
  
  const int nurls=2;

  int search_query_length = strlen(documentRequested) - 10;
  char * words = (char*)malloc(sizeof(char)*search_query_length);
  words[0] = '\0';
  for(i = 0; i < num_words -1; i++){
    strcat(words, searched_words[i]);
    strcat(words, " ");
  }
  strcat(words, searched_words[i]);

  // search and add results to file

  URLRecordList* data;
  
  ResultList* results = NULL;
  ResultList* current = results;


  //create master list of results
  bool found_match = false;
  for(i = 0; i < num_words; i++){
    data = (URLRecordList*)_wordToURLList->findRecord(searched_words[i]);
    while(data != NULL){
      if(results == NULL){
        //begin master list
        results = new ResultList;
        results->url_index = data->_urlIndex;
        results->count = 1;
        results->next = NULL;
      } else {
        //check list of current results for match
        current = results;
        while(current != NULL){
          if(data->_urlIndex == current->url_index){
            // current page also contained searched word
            current->count++;
            found_match = true;
            break;
          }
          current = current->next;
        }
        if(!found_match){
          //no match was found; add to end of list
          current = results;
          while(current->next != NULL){
            current = current->next;
          }
          ResultList* r = new ResultList;
          r->url_index = data->_urlIndex;
          r->count = 1;
          r->next = NULL;
          current->next = r;
          found_match = false;
        }
      }
      data = data->_next;
    }
    
  }

  if(clock_gettime( CLOCK_REALTIME, &end) == -1){
    printf("GETTIME ERROR!\n");
  }

  double accum = (double)( end.tv_sec - start.tv_sec ) + (double)(end.tv_nsec - start.tv_nsec ) / (double)BILLION;


  fprintf( stderr, "Search for words: \"%s\"\n", words);

  fprintf( fout, "<TITLE>Search Results</TITLE>\r\n");
  fprintf( fout, "<H1> <Center><em>Ask Cheeves</em></H1>\n");
  fprintf( fout, "<H2> Search Results for \"%s\"</center></H2>\n", words );

 current = results;
  if(ranked == false){
    while(current != NULL){
      if(current->count == num_words){
        int index = current->url_index;
        fprintf( fout, "<h3>%d. <a href=\"%s\">%s</a><h3>\n", index, _urlArray[index]._url, _urlArray[index]._url);
        fprintf( fout, "<blockquote>%s<p></blockquote>\n", _urlArray[index]._description );
      }
      current = current->next;
    }
  } else {
    // searched results print out in ranked order

    for(i = num_words; i > 0; i--){
      current = results;
      while(current != NULL){
        if(current->count == i){
          int index = current->url_index;
          fprintf( fout, "<h3>%d. <a href=\"%s\">%s</a><h3>\n", index, _urlArray[index]._url, _urlArray[index]._url);
          fprintf( fout, "<blockquote>%s<p></blockquote>\n", _urlArray[index]._description );
        }
        current = current->next;
      }
    }
  }

  
  fprintf( fout, "<h3>\nYour search took %f seconds!<h3>\n", accum);   

  char* p = (char*)(documentRequested +13);
  int req_len = strlen(p);
  char* lmgtfy = (char*)malloc(sizeof(char)* (22 + req_len));
  char bs[] = "http://lmgtfy.com/?q=";
  for(i = 0; i<22; i++){
    lmgtfy[i] = bs[i];
  }
  strcat(lmgtfy, p);

  fprintf( fout, "<h2>\n<a href=\"%s\">Didn't find what you were looking for?</a><h2>\n",  lmgtfy);   

  // Add search form at the end
  fprintf(fout, "<HR><H2>\n");
  fprintf(fout, "<FORM ACTION=\"search\">\n");
  fprintf(fout, "Search:\n");
  fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
  fprintf(fout, "</H2>\n");
  fprintf(fout, "</FORM>\n");

  fprintf(stderr, "documentRequested = %s\n", documentRequested);
}

void
printUsage()
{
  const char * usage =
    "Usage: search-engine port (array | hash | avl | bsearch) -r\n"
    "  It starts a search engine at this port using the\n"
    "  data structure indicated. Port has to be larger than 1024.\n"
    "  Without the -r flag, only results containing all searched words\n"
    "  Will be displayed. The -r flag specifies ranked result.\n"
    "  Search results will be displayed in ranked order.\n";

  fprintf(stderr, "%s", usage);
}

int main(int argc, char ** argv)
{
  if (argc < 3) {
    printUsage();
    return 1;
  }

  // Get port
  int port;
  sscanf( argv[1], "%d", &port);

  if(argc == 4){
    if(strcmp(argv[3], "-r") == 0){
      ranked = true;
    }
  }

  // Get DictionaryType
  const char * dictType = argv[2];
  DictionaryType dictionaryType;
  if (!strcmp(dictType, "array")) {
    dictionaryType = ArrayDictionaryType;
  }
  else if (!strcmp(dictType, "hash")) {
    dictionaryType = HashDictionaryType;
  }
  else if (!strcmp(dictType, "avl")) {
    dictionaryType = AVLDictionaryType;
  }
  else if (!strcmp(dictType, "bsearch")) {
    dictionaryType = BinarySearchDictionaryType;
  }
  else {
    printUsage();
    return 0;
  }
  
  SearchEngine httpd(port, dictionaryType);
  
  httpd.run();

  return 0;
}
