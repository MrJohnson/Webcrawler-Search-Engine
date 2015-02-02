
#include "minihttpd.h"
#include "dictionary.h"
#include "array-dictionary.h"
#include "hash-dictionary.h"
#include "bsearch-dictionary.h"
#include "avl-dictionary.h"
// Types of dictionaries used
enum DictionaryType {
  ArrayDictionaryType,
  HashDictionaryType,
  AVLDictionaryType,
  BinarySearchDictionaryType
};

struct ResultList{
  int url_index;
  int count;
  ResultList* next;
};

// Stores a URL and the first 100 words
struct URLRecord {
  char * _url;          // URL of the document.
  char * _description;  // First 100 words of the document without tags
};

// Used to implement a list of URL indexes of the url array.
// There is one list of URLs for each word 
struct URLRecordList {
  int _urlIndex;     // Index into the URL Array
  URLRecordList * _next;   // Pointer to the next URL Record in the list
};

// Inherits from MiniHTTPD
class SearchEngine : public MiniHTTPD {
  URLRecord * _urlArray;   // Array of URLs 
  Dictionary * _wordToURLList;
  
  // Add any other member variables you need
 public:
  // Constructor for SearchEngine
  SearchEngine( int port, DictionaryType dictionaryType);

  // Called when a request arrives
  void dispatch( FILE * out, const char * requestLine );

  // builds an array of URLRecords from url.txt
  void build_url_array();

  // copies url.txt from created _urlArray
  void writeURLFile(const char* urlFileName);

  void build_dictionary();


};



