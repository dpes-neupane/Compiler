#include<iostream>
#include<string>
#include<unordered_map>
#include<vector>
#include<iterator>
#include<algorithm>

using namespace std;
unordered_map<string, vector<string> > grule;

unordered_map<string, vector<string> > first;
unordered_map<string, vector<string> > follow;

string heads[100];
int nrules=0;


void traverseVector(string head){
    for( auto iter = grule[head].begin(); iter!=grule[head].end(); ++iter){
        cout<< *iter<<"\n";
    }
}

void traverseFirst(string head){
    for( auto iter = first[head].begin(); iter!=first[head].end(); ++iter){
        cout<< *iter<<" ";
    }
}
void traverseFollow(string head){
    for( auto iter = follow[head].begin(); iter!=follow[head].end(); ++iter){
        
        cout<< *iter<<" ";
    }
}


void parseProduction(string production){
    string body="";
    string head="";
    int j=0;
    while(production[j] != ' '){
        head+=production[j];
        j++;
    }
    j+=3;
    for(; j<production.length(); j++){
        if(production[j] =='|')break;
        body+=production[j];
    }
    grule[head].push_back(body);
    // cout<<body;
    body="";
    j+=1;
    while(j<production.length()){
        body+=production[j];
        j++;
    }
    if(body!="")
    grule[head].push_back(body);
    heads[nrules++]=head;
    // cout<<body;

}

void calcFirst(string head){
    string f="";
    
    
    for(int i=0; i<grule[head].size(); i++){
        f = "";
        int j=1;
        string body = "";
        body = grule[head][i];
        // cout<<body<<"\n";
        while(body[j] != ' ' && j < body.length()){ // we found the first terminal or non-terminal
            f+=body[j];
            // cout<<f;
            j++;
        }
        //now we have to find out if it is a non-terminal or not 
        // cout<<f;
        if(f >= "A" && f <= "Z"){
            //then we just go to that non-terminal's body and generate its FIRST first
            first[head] = first[f];
            int flag = 0;
            // check if the first of f has epsilon 
            for(int a=0; a<first[head].size(); a++){
                
                if(first[head][a] == "e") flag=1;

            }
            if(flag){
                j++;
                while(body[j] != ' ' && j < body.length()){ // we found the next terminal or non-terminal
                f+=body[j];
                // cout<<f;
                j++;
                }
            }


        }
        else if( f == "e"){
            first[head].push_back("e");
                
            // cout<<"    epsilon  ";
        }
        else { // then it is a terminal -- just push it
        if(find(first[head].begin(), first[head].end(), f) == first[head].end())
            first[head].push_back(f);
            // cout<<"  "<<f<<"at  ";
        }
    }
    // cout<<"\n";

    
}
//Return index if substring is present in the string
int check_substring(string str1, string str2)
{   
   int i=-1;
        // counter for substring
    if(str2.length() == 1){
        // cout<<str1;
        for(int j=0; j<str1.length(); j++){
            if( str1[j] == str2[0] && str1[j+1] == ' '){
                i=j;
                break;
            }
            
            
        }
        return i;
    }

    else{
        
        int c = 0; 
        for( i=0;i<str1.length();i++)
        {
            if(c==str2.length())
            {  break; }
            if(str2[c]==str1[i])
            {
                c++;
            }
            else
                {
                    // if the character next to ith character is duplicate
                    if(c > 0)
                    {
                        i -= c;
                    }
                    c = 0;
                }
        }
        //checking if the substring is present or not
        if(c < str2.length())
        {
            return -1;
        }
        else{
        return i-c;
        }
    }
    
}


//find next element in the body of teh production 

unordered_map<string, vector<string>> isequalto;
void calcFollow(string head){

    for(int i=0; i<nrules; i++){
        for(int j=0; j<grule[heads[i]].size(); j++){
            string body=grule[heads[i]][j];
            int index = check_substring(body, head);
            if(index > -1 ){
                int flag =0;
                int flag2 = 1; // for checking the value of the last element of the body, to see if its terminal or not
                //find next symbol and calculate the first of that 
                // cout<<index;
                string next = "";
                while(body[index] != ' ' && index < body.length()){//the grammmar is E -> E' T then we have to push through the E and ' into 
                    index++;
                }
                if(index < body.length()) 
                {
                    index++;
                    while(body[index] != ' ' && index < body.length()){//find the next non-terminal if it exists
                        next+=body[index];
                        index++;
                    }
                   
                    // cout<<next<<"\n";
                    
                    
                    if(next[0] >= 'A' && next[0] <='Z'){ //if the found next is non-terminal
                        for(int b=0; b< first[next].size(); b++){ //take the first and put it on the current head
                                                                 //this rule is the first rule for FOLLOW    
                            if(first[next][b] != "e") {  //"e" is the epsilon
                                // cout<<first[next][b];
                                int count=0;
                                for(int k = 0; k< follow[head].size(); k++){
                                    if (follow[head][k] != first[next][b]) count++;
                                }
                                if(count == follow[head].size())
                                follow[head].push_back(first[next][b]);
                                

                            }
                            else{
                                flag = 1;
                            }
                        // }cout<<"\n";
                        }
                    }
                    else{ // if the next is a terminal just push it
                        follow[head].push_back(next);
                        flag2 = 0;
                    }
                    
                    
                    
                    
                    
                }
                if(flag || (body.length() <= index && flag2 ) ){//if there is epsilon or nothing after the current non-terminal then just push it 
                    if(heads[i] != head)
                    isequalto[head].push_back(heads[i]);
                }
                
                
            }

        }
    }

}

void transferFollow(string head, string headpast){
    // cout<<" \n"<<head<<" ";
    if(isequalto.find(head) == isequalto.end()){
        for(int j=0; j < follow[ head].size(); j++){
            if(find(follow[headpast].begin(), follow[headpast].end(), follow[ head] [j]) == follow[headpast].end())
            follow[headpast].push_back(follow[ head] [j]);
        }
        return;
    }
    for(int i=0; i < isequalto[head].size(); i++){
        // if(head == "F")
        // cout<<isequalto[head][i]<<" ";
        for(int j=0; j < follow[ isequalto[head] [i] ].size(); j++){
            if(find(follow[headpast].begin(), follow[headpast].end(), follow[ isequalto[head] [i] ] [j]) == follow[headpast].end())
            follow[headpast].push_back(follow[ isequalto[head] [i] ] [j]);
        }
        transferFollow(isequalto[head][i], head);
    }  
    // }cout<<"\n";
    return;
}



int main(){
    string production;
    int n;
    cout<<"Enter the number of rules you want to enter";
    cin>>n;
    cout<<"Now enter the production rules:\n";
    cin>>ws;
    for(int i=0; i<n; i++){
        getline(cin, production);
        
        
        parseProduction(production);
        // traverseVector(heads[i]);
        
    }
    cout<<"....\n";
    for(int i=n-1; i>=0; i--){
        
        calcFirst(heads[i]);
       
        
        // traverseFollow(heads[i]);
    }
    for(int i=n-1; i>=0; i--){
        calcFollow(heads[i]);
        // traverseFollow(heads[i]);
    }
    
        
    follow[heads[0]].push_back("$");
         
        
    
    for(int i=n-1; i>=0; i--){
        // for(int j=0; j < isequalto[heads[i]].size(); j++){
        //     cout<<isequalto[heads[i]][j]<<" ";
        // }cout<<"\n";
        transferFollow(heads[i], heads[i]);
    }
    for(int i=n-1; i>=0; i--){

        cout<<"The first of "<<heads[i]<<" is----> ";
        traverseFirst(heads[i]);

        cout<<"\nThe follow of "<<heads[i]<<" is----> ";
        
        traverseFollow(heads[i]);

        cout<<"\n";         
        
    }
}