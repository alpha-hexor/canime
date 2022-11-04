#include <iostream>
#include <stdlib.h>
#include <string>
#include <regex>
#include <bits/stdc++.h>
#include "headers/base64.h"
#include "headers/httpx.h"
#include "headers/log.h"

//color shit
#define PROMPT "\x1B[4;32m"
#define INFO "\033[1;95m"
#define ERROR "\x1B[1;31m"
#define SHOW "\x1B[4;34m"
 

using namespace std;

//global shit
string main_url = "https://gogoanime.dk/";
string animix_api_link = "https://animixplay.to/api/";

//function to get regex first match
string re(string buffer,string pattern){

    //convert pattern into regex
    regex x = regex(pattern);
    smatch m;
    regex_search(buffer,m,x);

    return string(m[1]);
}

//function to get streaming link
string get_streaming_link(string buffer){ 
    int j = 0,choice;
    string links[10],qualities[10];

    //convert patter into regex
    regex x("#EXT-X-STREAM-INF:.*??\n+(.+)");
    auto words_begin = std::sregex_iterator(buffer.begin(), buffer.end(),x);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {

        smatch match =*i;
        string match_str = match.str(); 
        string link = re(match_str,"(https://.*)");
        string quality = re(match_str,"RESOLUTION=[0-9]+x([0-9]+)");
        links[j] = link;
        qualities[j] = quality;
        j++;

    }
    cout << "\n\n" ;
    cout << SHOW <<"[*]Available Qualities" << endl;
    
    for(int z=0;z<j;z++){
        cout << INFO << "[" << z+1 << "]: " << qualities[z] << endl;
    }
    cout << "\n\n";
    cout << PROMPT << "[*]Enter option: ";
    cin >> choice;

    return links[choice-1];
    
}

//function to search anime
string search(string anime_name){
    int j = 0,choice;
    string names[100];
    string name = encode(anime_name);
    string result = get(main_url+"search.html?keyword="+name);
    
    //regex
    regex x = regex("<p class=\"name\">\n\t+<a href=\"/category/([^\"]*)\".*");
    
    auto words_begin = std::sregex_iterator(result.begin(), result.end(),x);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {

        smatch match =*i;
        string match_str = match.str(); 
        //cout << match_str;
    
        string n = re(match_str,"\"/category/(.*?)\" title");
        names[j] = n;
        j++;

    }
    cout << "\n\n";
    cout << SHOW << "[*]Available Animes" << endl;
    
    for(int z=0;z<j;z++){
        cout << INFO << "[" << z+1 << "]: " << names[z] << endl;
    }
    cout << "\n\n";
    cout << PROMPT <<"[*]Enter option: ";
    cin >> choice;
    return names[choice-1];
}

//function to search episode
string search_ep(string name){
    string result=get(main_url+"category/"+name);
    //cout << result;
    string end_ep = re(result,"<a href=\"#\" class=\"active\" ep_start = '[0-9]' ep_end = '([^']+)'>.*</a>");
    return end_ep;
}
//function to fix_id
string create_id(string enc_id){

    int s = (int)enc_id.size();
    string temp;
    for(int i=0;i<s-2;i++)
        temp+= enc_id[i];

    string new_id = string(temp+char(enc_id[s-2] +1));

    return new_id;

}
//function to stream episode
void stream_episode(string name,string ep_to_watch,string last_ep){
    string op;
    string x = get(main_url+name+"-episode-"+ep_to_watch);
    string iframe_url = re(x,"data-video=\"([^\"]*)\" >.*");
    string video_id = re(iframe_url,"id=(.*)&");
    string enc_id = base64encode(video_id);
    
    string final_link = animix_api_link+"cW9" + create_id(enc_id)  + "MVFhzM0dyVTh3ZTlP" + base64encode(enc_id);

    string res = head(final_link);
    string redirect_url = re(res,"location: (.*)");
    string base64part = re(redirect_url,"#(aHR0[^#]+)");
    

    string streaming_url = string(base64decode(base64part));
    string q = get(streaming_url);
    //cout << q;
    
    string command = "mpv '" + get_streaming_link(q) + "' --force-media-title="+name+"-episode-"+ep_to_watch+" > /dev/null 2>&1 &";
    system(command.c_str());

    //for next episode
    if((stoi(ep_to_watch) + 1) <= stoi(last_ep)){
        cout << "\n";
        cout << SHOW << "[*]Want to start next episode[y/n]: ";
        cin >> op;

        if (op == "n")
            exit(0);
        int ep_next = stoi(ep_to_watch) + 1;
        write(name,to_string(ep_next),last_ep);
        stream_episode(name,to_string(ep_next),last_ep);
    }
    else
        exit(0);
}

//main function
int main(int argc,char* argv[]){
    //if no command line argument passed start in normal mode
    if(argc == 1){

        string anime;
        cout << PROMPT << "[*]Enter query: ";
        getline(cin,anime);
        string anime_to_watch = search(anime);
        string ep_end = search_ep(anime_to_watch);

        string ep_to_watch;
        cout << "\n";
        cout << PROMPT << "[*]Total Episode(1-"+ep_end+"): ";
        cin >> ep_to_watch;

        if(stoi(ep_to_watch) >= 1 && stoi(ep_to_watch) <= stoi(ep_end)){
            write(anime_to_watch,ep_to_watch,ep_end);
            stream_episode(anime_to_watch,ep_to_watch,ep_end);
        }
        else{
            cout << ERROR << "[*]Invalid Episode";
            exit(0);
        }
    }

    if(argc >=2){
        if(string(argv[1]) == "-c"){
            auto [anime_name, ep_to_watch,last_ep] = parse();
            //cout << value1 << ", " << value2 << ", " << value3 << endl;

            stream_episode(anime_name,ep_to_watch,last_ep);
        }
        else if(string(argv[1]) == "-h"){
            //print the help message

            cout << "\n";
            cout << "[*]Usage:- " << endl;
            cout << PROMPT << "./anime    -----------> Normal usage" << endl;
            cout << PROMPT << "./anime -c -----------> Load from logfile" << endl;
            cout << PROMPT << "./anime -h -----------> To watch help message" << endl;

        }
    }

    return 0;
}