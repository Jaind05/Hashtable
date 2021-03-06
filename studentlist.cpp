
// Dhruv Jain
// 12/2/2020
//Student List
//Pointers and Structs


#include <iostream>
#include <cstring>
#include <fstream>
#include <string.h>



using namespace std;


struct student { //student Struct with First name, Last name, ID, and Gpa 

  char name [100];
  char last_name [50];
  int id;
  float gpa;
  student * nextstudent;

};

int Hashfunction(student*, int); //function to figure out where to place each student

int main(){

  srand (time(NULL));
  cout << "How many student would you like?" << endl; //asks for how many random students the user would like
  int numberofrand = 0;
  cin >> numberofrand;
  //next group of lines reads in from file, and puts the lists of firstnames and lastnames into arrays of char arrays
  streampos size;
  char * memblock;
  char * pch;
  char randfirstname[100][30]; //array of random firstnames
  ifstream file ("example.txt", ios::in|ios::binary|ios::ate);
  if (file.is_open())
    {
      size = file.tellg();
      memblock = new char [size];
      file.seekg (0, ios::beg);
      file.read (memblock, size);
      file.close();
    }
  pch = strtok (memblock," ,.-");
  int i = 0;
  while (pch != NULL)
    {
      strcpy(randfirstname[i], pch);
      pch = strtok (NULL, " ,.-");
      i++;
    }
  delete[] memblock;


  streampos size1;
  char * memblock1;
  char * pch1;
  char randlastname[100][30]; //array of random lastnames
  ifstream file1 ("example2.txt", ios::in|ios::binary|ios::ate);
  if (file1.is_open())
    {
      size1 = file1.tellg();
      memblock1 = new char [size1];
      file1.seekg (0, ios::beg);
      file1.read (memblock1, size1);
      file1.close();
    }
  pch1 = strtok (memblock1," ,.-");
  int x = 0;
  while (pch1 != NULL)
    {
      strcpy(randlastname[x], pch1);
      pch1 = strtok (NULL, " ,.-");
      x++;
    }

  

  bool stop = false; //boolean to keep track if the user is still using the application
  int buckets = numberofrand/3; //number of initial buckets is number of random students divided by 3
  if(numberofrand%3 != 0){
    buckets++;
  }
  //int i = 0; //keeps track of how many students are made
  char input[30]; //char to track what the user wants to do
  student* sa;
  sa = new student[100]; //array of head students
  student *temp;
  student *temp2;

  for (i = 0; i < 100; i++){ //initializes all ids to -1
    sa[i].id = -1;
  }
  cout << "buckets " << buckets << endl;

  
  for (i = 0; i < numberofrand; i++){
    //cout << i << endl;
    
    if(i < buckets){ //cereating and randomizing head students 
      sa[i].id = i;
      strcpy(sa[i].name, randfirstname[rand() % 10]);
      strcpy(sa[i].last_name, randlastname[rand()% 25]);
      sa[i].gpa = (rand()/(double)RAND_MAX)*4;
    }
    else if(i >= buckets && i < buckets*2){ //creating and randomizing 2nd students
      //cout << "second" << endl;
      temp = new student;
      temp->id = i;
      strcpy(temp->name, randfirstname[rand() % 10]);
      strcpy(temp->last_name, randlastname[rand()% 25]);
      temp->gpa = (rand()/(double)RAND_MAX)*4;
      int tempbucket = Hashfunction(temp, buckets);

      sa[tempbucket].nextstudent = temp;
        
    }
    else if(i >= buckets*2){ //creating and randomizing 3rd students
      //cout << "third" << i << endl;
      temp2 = new student;
      temp2->id = i;
      strcpy(temp2->name, randfirstname[rand() % 10]);
      strcpy(temp2->last_name, randlastname[rand()% 25]);
      temp2->gpa = (rand()/(double)RAND_MAX)*4;
      int tempbucket = Hashfunction(temp2, buckets);
      (sa[tempbucket].nextstudent)->nextstudent = temp2;

    }

  }

  student *tempnew;
  student *curr;
  while (stop == false){ // while loop for application
    cout << "Please enter ADD, PRINT, or DELETE. If you would like to close the application enter QUIT" << endl; 
    cin >> input;// Reads user input
    if(strcmp(input,"ADD")==0){ //if user enters add
      cout << "Please enter the first name of the student" << endl;
      tempnew = new student;
      bool needreset = false;
      cin >> tempnew->name; //reads name
      cout << "Please enter the Last name of the student" << endl;
      cin >> tempnew->last_name; //reads last name
      cout << "Please enter the ID of the student" << endl;
      cin >> tempnew->id; //reads id
      cout << "Please enter the Gpa of the student" << endl;
      cin >> tempnew->gpa; //reads gpa
      tempnew->nextstudent = NULL;

      int placement = Hashfunction(tempnew, buckets); //runs hash function on new student to figure out where to place them


      //THREE CONDITIONS to decide where to place depending on how many students we have in each hash
      if(sa[placement].id == -1){
	memcpy(&sa[placement], tempnew, sizeof(student));
      }
      else if(sa[placement].nextstudent == NULL){
	sa[placement].nextstudent = tempnew;
      }
      else if ((sa[placement].nextstudent)->nextstudent == NULL){
	(sa[placement].nextstudent)->nextstudent = tempnew;
      }
      else {
	((sa[placement].nextstudent)->nextstudent)->nextstudent = tempnew;
	buckets = buckets*2;
	needreset = true;
      }
      
      if(needreset == true){ //if we need to reshuffle the hash table
	//cout << buckets <<endl;
	cout << "Need to re Hash" << endl;
	student* tempsa;
	tempsa = new student[1000];
        for (i = 0; i < 1000; i++){
	  tempsa[i].id = -1;
	}
	
	//copy over to a temp Array all the elements from the hash table
	for(int x = 0;x<buckets/2;x++){
	  if(sa[x].id != -1){ //If the Student that the printer is on is not null
	    memcpy(&tempsa[sa[x].id], &sa[x], sizeof(student));
	    //tempsa[sa[x].id].nextstudent = NULL;
	    cout << x << endl;
	    if(sa[x].nextstudent != NULL){
	      memcpy(&tempsa[(sa[x].nextstudent)->id], sa[x].nextstudent, sizeof(student));
	      
	      //tempsa[(sa[x].nextstudent)->id].nextstudent = NULL;
	      if((sa[x].nextstudent)->nextstudent != NULL){
		memcpy(&tempsa[((sa[x].nextstudent)->nextstudent)->id], (sa[x].nextstudent)->nextstudent, sizeof(student));
		if((sa[x].nextstudent)->nextstudent->nextstudent != NULL){
		  //cout << "do we come here?" << endl;
		  memcpy(&tempsa[((sa[x].nextstudent)->nextstudent->nextstudent)->id], (sa[x].nextstudent)->nextstudent->nextstudent, sizeof(student));

		  tempsa[((sa[x].nextstudent)->nextstudent->nextstudent)->id].nextstudent = NULL;
		}

		tempsa[((sa[x].nextstudent)->nextstudent)->id].nextstudent = NULL;
	      }
	      tempsa[(sa[x].nextstudent)->id].nextstudent = NULL;
	    }
	    tempsa[sa[x].id].nextstudent = NULL;

	  }
	}
	//delete original hashtables value
	for(int x = 0;x<buckets/2;x++){
          if(sa[x].id != -1){ //If the Student that the printer is on is not null
            if(sa[x].nextstudent != NULL){
              if((sa[x].nextstudent)->nextstudent != NULL){
		if(((sa[x].nextstudent)->nextstudent)->nextstudent != NULL){
		  delete (((sa[x].nextstudent)->nextstudent)->nextstudent);
		}
		delete ((sa[x].nextstudent)->nextstudent);
              }
	      delete (sa[x].nextstudent);
            }
	    sa[x].id = -1;
	    sa[x].nextstudent = NULL;
          }
        }

	for (int x = 0; x<1000; x++){ //replaces all of the students in thier new hash locations
	  //cout << x << endl;
	  if(tempsa[x].id != -1){
	    
	    int placement = Hashfunction(&tempsa[x], buckets);
	    
	    if(sa[placement].id == -1){
	      memcpy(&sa[placement], &tempsa[x], sizeof(student));
	    }
	    else if(sa[placement].nextstudent == NULL){

              curr = new student;
              memcpy(curr, &tempsa[x], sizeof(student));

	      curr->nextstudent = NULL;
	      sa[placement].nextstudent = curr;
	    }
	    else if ((sa[placement].nextstudent)->nextstudent == NULL){
	      
              curr = new student;
              memcpy(curr, &tempsa[x], sizeof(student));
	      curr->nextstudent = NULL;
	      (sa[placement].nextstudent)->nextstudent = curr;
	    }
	    
	  } 
	}
      }
      
      //i++; //increase amount of students created
    }
    else if(strcmp(input,"DELETE")==0){
	  cout << "Please enter student id of student you would like to delete" << endl;
	  int tempid = 0;
	  cin >> tempid;


      int curr_b = tempid % buckets;
      
      if(sa[curr_b].id == tempid){
	cout << "found the student to delete, its the head" << endl;
	if(sa[curr_b].nextstudent!= NULL){
	  memcpy(&sa[curr_b], sa[curr_b].nextstudent, sizeof(student));
	}
	else{
	  sa[curr_b].id = -1;
	}
	//if there is something hanging off of head, store it in a temp ptr,
	//clean up the head
	//copy temp ptr memory to head
	//release temp ptr memory
      }
      else if(sa[curr_b].nextstudent->id == tempid){
        cout << "found the student to delete, its one after head" << endl;
	if((sa[curr_b].nextstudent)->nextstudent!=NULL){
	  ((sa[curr_b].nextstudent)->nextstudent)->nextstudent = NULL;
	  student *tempdel;
	  tempdel = (sa[curr_b].nextstudent)->nextstudent;
	  delete (sa[curr_b].nextstudent);
	  sa[curr_b].nextstudent = tempdel;
	}
	else {
	  delete (sa[curr_b].nextstudent);
	  sa[curr_b].nextstudent == NULL;
	}
	
	//check if there something after, if it is store a temp ptr to it and then delete thsi element
	//then delete the element.
	// then point the head to this temp ptr.
      }
      else if ((sa[curr_b].nextstudent)->nextstudent->id == tempid){
        cout << "found the student to delete, its two after head" << endl;
	delete ((sa[curr_b].nextstudent)->nextstudent);
        (sa[curr_b].nextstudent)->nextstudent = NULL;	
      }
      else {
        cout << "student not found" << endl;
      }


    }
    else if(strcmp(input,"PRINT")==0){
      for(int x = 0;x<buckets;x++){
	if(sa[x].id != -1){ //If the Student that the printer is on is not null
	  cout << sa[x].name << " " << sa[x].last_name << ", " << sa[x].id << ", "; //print First name, last name, and ID
	  cout.setf(ios::showpoint); //show floating zeros
	  cout.precision(3); //sets presicion to 3
	  cout << sa[x].gpa << endl; //prints gpa
	  cout.precision(100000000); // sets precision to a big number again
	  cout.unsetf(ios::showpoint);
	  if(sa[x].nextstudent != NULL){ //if there is a second student
	    cout <<"   "<< (sa[x].nextstudent)->name << " " << (sa[x].nextstudent)->last_name << ", " << (sa[x].nextstudent)->id << ", "; //print First name, last name, and ID
	    cout.setf(ios::showpoint); //show floating zeros
	    cout.precision(3); //sets presicion to 3
	    cout << (sa[x].nextstudent)->gpa << endl; //prints gpa
	    cout.precision(100000000); // sets precision to a big number again
	    cout.unsetf(ios::showpoint);
	    if((sa[x].nextstudent)->nextstudent != NULL){ //if there is a third student
	      cout << "      " << ((sa[x].nextstudent)->nextstudent)->name << " " << ((sa[x].nextstudent)->nextstudent)->last_name << ", " << ((sa[x].nextstudent)->nextstudent)->id << ", "; //print First name, last name, and ID
	      cout.setf(ios::showpoint); //show floating zeros
	      cout.precision(3); //sets presicion to 3
	      cout << ((sa[x].nextstudent)->nextstudent)->gpa << endl; //prints gpa
	      cout.precision(100000000); // sets precision to a big number again
	      cout.unsetf(ios::showpoint);
	    }
	  }
	}
      }
    }
    else if(strcmp(input,"QUIT")==0){
      stop = true;
      
    }
    else{
      cout << "That was not a valid command" << endl; 
    }
  }
}

int Hashfunction(student * s, int numbuckets){ //hash function that divides the student by numbuckets and gives placement
  int hashvalue = s->id % numbuckets;
  return hashvalue;
}
