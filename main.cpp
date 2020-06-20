/*
*Name: Everardo Gutierrez
*Assignment: Project 1
*Overview: Program is to read in a spam file containing spam email addresses and be saved to a vector.
*User can then display the spam email list or cross check with the saved instances for if entered value is spam or not. 
*Finally, the user is able to filter out all non-spam emails of a loaded email list file to a designted output file user has entered.
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "ourvector.h"
using namespace std;

//Load Function: Function is to load the file user has entered and push its contents to the vector passed by refrence. 
//Parameters: Vector Passed by Refrence, Input File Stream Object
//Return: No Value Returned
void loadFunc(ourvector<string>& spamVector, ifstream& inFile){
    string fileName, fileLine;
    
    cin >> fileName;
    inFile.open(fileName);
    if(!inFile.is_open()){
        cout << "**Error, unable to open '" << fileName << "'" << endl;
        return;
    }
    while(!inFile.eof()){
        inFile >> fileLine;
        if(!inFile.fail()){
            spamVector.push_back(fileLine);
        }
    }
    inFile.close();
    cout << "Loading '" << fileName << "'" << endl;
    cout << "# of spam entries: " << spamVector.size() << endl;
}

//Display Function: Function is to loop through the vector passed by refrence and display its contents
//Parameters: Vector Passed by Refrence
//Return: No Value Returned
void displayFunc(ourvector<string>& spamVector){
    if(spamVector.size() == 0)
        return;
    for(int i = 0; i < spamVector.size(); ++i){
        cout << spamVector[i] << endl;
    }
}

//emailSplit Function: Function is to split an email string into substrings saved to emailuser and emaildomain
//Parameters: Email String user entered, Pass by Refrence to string variable that will hold user portion of email, 
//             Pass by Refrence to string variable that will hold dpmain portion of email
//Return: No Value Returned
void emailSplit(string email, string& emailUser, string& emailDomain){
    unsigned int i;
    for(i = 0; i < email.size(); ++i){
        if(email[i] == '@'){
            emailUser = email.substr(0, i);
            emailDomain = email.substr(i+1);
            break;
        }
    }
    
}

//spamSplit Function: Function is to split a spam email string into substrings saved to spamuser and spamdomain
//Parameters: Email spam String user entered, Pass by Refrence to string variable that will hold user portion of spam email, 
//             Pass by Refrence to string variable that will hold dpmain portion of spam email
//Return: No Value Returned
void spamSplit(string email, string& emailUser, string& emailDomain){
    unsigned int i;
    for(i = 0; i < email.size(); ++i){
        if(email[i] == ':'){
            emailDomain = email.substr(0, i);
            emailUser = email.substr(i+1);
            break;
        }
    }
}

//binarySearch Function: Function is to perform binary search on vector passed by refrence
//Parameters: Spam email string to locate in vector, Pass by Refrence to vector holding spam emails
//Return: True if Email is found in vector else false the email does not exist in the vector
bool binarySearch(string email, int left, int right, ourvector<string>& spamVector){
    string emailUser, emailDomain;
    string spamUser, spamDomain;
    emailSplit(email, emailUser, emailDomain);
    
    while(left <= right){
        int middle = left + (right - left) / 2;
        
        spamSplit(spamVector[middle], spamUser, spamDomain);
        if(spamDomain == emailDomain){
            if(spamUser == "*" || spamUser == emailUser){
                return true;
            }else if(spamUser < emailUser){
                if(binarySearch(email, left, middle - 1, spamVector))
                    return true;
                else 
                    return binarySearch(email, middle+1, right, spamVector);
            }else{
                return binarySearch(email, left, middle - 1, spamVector);
            }
        }else if(spamDomain > emailDomain){
            return binarySearch(email, left, middle - 1, spamVector);
        }else{
            return binarySearch(email, middle+1, right, spamVector);
        }
        
    }
    return false;
}

//Check Function: Function is to check if a particular spam email exists in the vector 
//Parameters: Pass by Refrence vector containing spam list
//Return: No Value Returned
void checkFunc(ourvector<string>& spamVector){
    string email;
    cin >> email;
    if(binarySearch(email, 0, spamVector.size() - 1, spamVector) )
        cout << email << " is spam" << endl;
    else 
        cout << email << " is not spam" << endl;
}

//Filter Function: Function is to read in an emails file containing emails to be cross checked to vector containing spam emails.
//                    Non-spam emails are to be saved to the file user has entered. 
//Parameters: Pass by Refrence vector containing spam list
//Return: No Value Returned
void filterFunc(ourvector<string>& spamVector){
    string fileName, outFileName;
    int msgID, numEmails = 0, totalEmails = 0;
    string email;
    string subject;
    ofstream outFile;
    ifstream inFile;
    
    cin >> fileName;
    cin >> outFileName;
    
    inFile.open(fileName);
    if(!inFile.is_open()){
        cout << "**Error, unable to open '" << fileName << "'" << endl;
        return;
    }
    outFile.open(outFileName);
    if(!outFile.is_open()){
         cout << "**Error, unable to open '" << outFileName << "'" << endl;
        return;
    }
    while(!inFile.eof()){
        inFile >> msgID;

        inFile >> email;

        getline(inFile, subject);
        if(inFile.fail()){
            continue;
        }
        totalEmails++;
        if(binarySearch(email, 0, spamVector.size() - 1, spamVector)){ //Email read is spam
            continue;
        }else{ //Email read is not spam, save to output file
            numEmails++;
            outFile << msgID << " " << email << " " << subject << endl;
        }
    }
    inFile.close();
    outFile.close();
    cout << "# emails processed: " << totalEmails << endl;
    cout << "# non-spam emails: " << numEmails << endl;
}

int main(){
    ourvector<string> spamVector;
    string command;
    ifstream inFile;
    
    cout <<"** Welcome to spam filtering app **" << endl << endl;
    do{
        cout << "Enter command or # to exit> ";
        cin >> command;
        if(command == "#"){
            continue;
        }else if(command == "load"){
            spamVector.clear();
            loadFunc(spamVector, inFile);
        }else if(command == "display"){
            displayFunc(spamVector);
        }else if(command == "check"){
            checkFunc(spamVector);
        }else if(command == "filter"){
            filterFunc(spamVector);
        }else{
            cout <<"**invalid command" << endl;
        }
        cout << endl;
    }while(command != "#");
    return 0;
}