#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <curl/curl.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>


using namespace std;
using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

#ifndef TOKEN
#pragma warning("Cannot work without an API token")
#endif

#define xstr(s) str(s)
#define str(s) #s


size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
  // cout << "received: " << ptr << endl;
  // Read json.
  ptree pt2;
  std::istringstream is (ptr);
  read_json (is, pt2);
  BOOST_FOREACH(ptree::value_type &v, pt2.get_child("oncalls")) {
    // The data function is used to access the data stored in a node.
    if (v.second.get<int>("escalation_level") == 1) {
      cout << v.second.get<std::string>("user.summary") << endl;
    }
  }
  return nmemb;
}


int main(int argc, char** argv) {
  CURL *curl;
  CURLcode res;
 
  curl = curl_easy_init();
  if(curl) {
    struct curl_slist *chunk = NULL;
 
    /* Add a custom header */ 
    chunk = curl_slist_append(chunk, "Accept: application/vnd.pagerduty+json;version=2");
 
    /* Modify a header curl otherwise adds differently */
    std::ostringstream ss;
    const char* apitoken = "Authorization: Token token=" xstr(TOKEN);
    chunk = curl_slist_append(chunk, apitoken);
 
    /* set our custom set of headers */ 
    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
 
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.pagerduty.com/oncalls?time_zone=UTC");
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
 
    /* free the custom headers */ 
    curl_slist_free_all(chunk);
  }

  return 0;
}
