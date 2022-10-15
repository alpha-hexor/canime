#include <iostream>
#include <curl/curl.h>

using namespace std;
size_t size = 0;

//function to write shit 
size_t write_to_string(void *ptr, size_t size, size_t count, void *stream) {
  ((string*)stream)->append((char*)ptr, 0, size*count);
  return size*count;
}


//function for get request
string get(string link){
  CURL *curl;
  CURLcode res;
  string response;
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if(curl) 
  {
    //init
    curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
 
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    
  
    res = curl_easy_perform(curl);
    
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();

  return response;
}

//send a head req to get redirect url
string head(string link){
  CURL *curl;
  CURLcode res;
  string response;
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if(curl) 
  {
    //init
    curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
 
    curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    
  
    res = curl_easy_perform(curl);
    
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();

  return response;
}


//function to encode payload
string encode(string value){
    CURL *curl = curl_easy_init();
    string output = curl_easy_escape(curl,value.c_str(),value.size());
    curl_easy_cleanup(curl);
    return output;
}
