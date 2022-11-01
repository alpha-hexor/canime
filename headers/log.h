#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string>

using namespace std;


//create a linked list
typedef struct anime{
    string anime_name;
    string ep_to_watch;
    string last_ep;
    struct anime *next;
}ANIME;

//start pointer
ANIME *start = NULL;



//function to append data
void append(string name,string ep,string last){
    ANIME *curr,*temp;

    //memory allocation
    temp = (ANIME *)malloc(sizeof(ANIME));

    //if fails
    if(temp == NULL){
        printf("[*]Memory allocation failed");
        exit(1);
    }
    else{
        temp->anime_name = name;
        temp->ep_to_watch = ep;
        temp->last_ep = last;
        temp->next = NULL;

        //for first time
        if(start == NULL)
            start = temp;
        else{
            curr = start;
            while(curr->next != NULL)
                curr = curr->next;
            curr->next = temp;
        }
    }
}

//function to search and overwrite data
int search(string name,string ep,string last){
    //cout << "[*] I got called";
    ANIME *curr = start;
    if(start == NULL)
        return 0;
    else{
        int flag=0;
        while(curr != NULL){
            //if matches then update the episode number with the latest one
            if(curr->anime_name == name){
                curr->ep_to_watch = ep;

                //for ongoing anime
                curr->last_ep = last;
                flag = 1;
            }
            curr = curr -> next;
        }
        return flag;
    }

    
}

//function to prepare the linked list. Split the data
// when "_" is detected 
//example:- death-note_1_37 -----> death-note,1,37
void split(string line){
    string words[3];
    int j=0;

    string temp = "";

    for(int i=0;i<(int)line.size();i++){
        if(line[i] != '_')
            temp += line[i];
        else{
            //cout << j << endl;
            words[j] = temp;
            j++;
            temp = ""; // reinitialize the string
        }
    }
    //for last word
    words[2] = temp;
 
    
    if(search(words[0],words[1],words[2]) == 0)
        append(string(words[0]),string(words[1]),string(words[2]));
}

//funciton to display the animes
void display(){
    ANIME *curr = start;
    int count = 1;
    cout << "[*]Anime to Continue....." << endl;
    while(curr != NULL){
        cout << "["<<count<<"] " << curr->anime_name<< endl;
        count ++;
        curr = curr->next;
    }
}

void write(string name,string ep_Watch,string end_ep){
    //function to write data in file


    fstream log;

    log.open("canime.log",ios::app);
    log << name << "_" << ep_Watch << "_" << end_ep << "\n";
    log.close();
}

auto parse(){
    //function to parse log file and return 
    //anime name episode+1 and last episode

    fstream log;
    string line;
    int i=1;

    log.open("canime.log",ios::in);

    //poin to the begining of the file
    log.seekg(0,ios::beg);

    while(log){
        getline(log,line);
        //cout << line;

        //get rid of the last line
        if((int)line.size() != 0){
            split(line);
        }
        
    }
    display();
    int choice;

    cout << "[*]Enter index: ";
    cin >> choice;
    choice -= 1;


    //structure for return data
    struct anime_stuff{
        string name;
        string ep_to_watch;
        string last_ep;
    };


    ANIME *curr = start;
    if(curr == NULL)
        exit(1);
    else{

        while(choice --)
            curr = curr->next;
            
        if(curr == NULL){
            cout << "[*]Wrong index!!!! Try again";
            exit(1);
        }

        return anime_stuff{curr->anime_name,to_string(stoi(curr->ep_to_watch) + 1),curr->last_ep};


    }
}

