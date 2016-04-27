using namespace std;
#include "MovieTree.h"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

MovieTree::MovieTree(){
	root=new MovieNode;
	root=NULL;
}

MovieTree::~MovieTree(){
	DeleteAll(root);
}

void MovieTree::printMovieInventory(){ //this public command leads to the private command which prints the movie inventory. Exciting, I know
	printMovieInventory(root);
}

int MovieTree::countMovieNodes(){ //counts the movie nodes. Exactly what you'd expect
	int count=0;
	countMovieNodes(root, count);
	return count;
}
        
void MovieTree::deleteMovieNode(string title,string movieFile,string userFile){ //this function deletes a movie node from the tree, then goes through both the movie file and the user file to delete the offending film from those files as well
	MovieNode *temp=search(title);
	if(temp==NULL){
		cout<<"Movie not found."<<endl;
	}
	else{
		if(temp!=root){ //the part of the code deleting the node from the tree is pretty much the same as Assignment 7, won't lie
			if(temp->rightChild==NULL&&temp->leftChild==NULL){
				if(temp->parent->rightChild==temp){
					temp->parent->rightChild=NULL;
					delete temp;
				}
				else if(temp->parent->leftChild==temp){
					temp->parent->leftChild=NULL;
					delete temp;
				}
			}
			else if(temp->rightChild==NULL){
				if(temp->parent->rightChild==temp){
					temp->leftChild->parent=temp->parent;
					temp->parent->rightChild=temp->leftChild;
					delete temp;
				}
				else if(temp->parent->leftChild==temp){
					temp->leftChild->parent=temp->parent;
					temp->parent->leftChild=temp->leftChild;
					delete temp;
				}
			}
			else if(temp->leftChild==NULL){
				if(temp->parent->rightChild==temp){
					temp->rightChild->parent=temp->parent;
					temp->parent->rightChild=temp->rightChild;
					delete temp;
				}
				else if(temp->parent->leftChild==temp){
					temp->rightChild->parent=temp->parent;
					temp->parent->leftChild=temp->rightChild;
					delete temp;
				}
			}
			else if(temp->leftChild!=NULL&&temp->rightChild!=NULL){
				MovieNode *min=treeMinimum(temp);
				min->parent=temp->parent;
				if(min->rightChild!=NULL){
					min->rightChild->parent=min->parent;
					min->parent->leftChild=min->rightChild;
				}
				if(temp->parent->rightChild==temp){
					temp->parent->rightChild=min;
				}
				else if(temp->parent->leftChild==temp){
					temp->parent->leftChild=min;
				}
				min->leftChild=temp->leftChild;
				if(temp->rightChild!=min){
					min->rightChild=temp->rightChild;
				}
				if(min->leftChild!=NULL){
					min->leftChild->parent=min;
				}
				if(min->rightChild!=NULL){
					min->rightChild->parent=min;
				}
				delete temp;
			}
		}
		else{
			MovieNode *min=treeMinimum(temp);
			if(min->rightChild!=NULL){
				min->rightChild->parent=min->parent;
				min->parent->leftChild=min->rightChild;
			}
			else if(min->rightChild==NULL){
				min->parent->leftChild=NULL;
			}
			min->parent=NULL;
			min->leftChild=temp->leftChild;
			min->rightChild=temp->rightChild;
			if(min->leftChild!=NULL){
				min->leftChild->parent=min;
			}
			if(min->rightChild!=NULL){
				min->rightChild->parent=min;
			}
			delete temp;
			root=min;
		} //this is where it gets fun
		vector<string> movies;
		vector<string> users;
		string whoa;
		int moviePlace=0;
		int count=0;
		ifstream thatfile; //we open the movie file and save all the stuff that's there so we can avoid overwriting the whole file
		thatfile.open(movieFile);
		if(thatfile.good()){
			while(getline(thatfile,whoa)){
				movies.push_back(whoa);
			}
		}
		thatfile.close();
		thatfile.open(movieFile);
		if(thatfile.good()){
			while(getline(thatfile,whoa)){
				stringstream ack(whoa);
				string yep;
				string line;
				getline(ack,yep,',');
				if(yep==title){
					moviePlace=count;
					movies[moviePlace]=yep;
				}
				count++;
			}
		}
		thatfile.close();
		ofstream myfile; //thanks to http://www.cplusplus.com/doc/tutorial/files/ for the refresher on writing things to files
		myfile.open(movieFile);
		if(myfile.good()){
			for(int i=0;i<movies.size();i++){
				if(i!=moviePlace){
					myfile<<movies[i]<<endl;
				}
			}
		}
		myfile.close();
		thatfile.open(userFile);
		if(thatfile.good()){
			while(getline(thatfile,whoa)){ //this is a similar idea to the movies, except we're updating the user doc as well
				users.push_back(whoa);
			}
		}
		thatfile.close();
		count=0;
		thatfile.open(userFile);
		if(thatfile.good()){
			while(getline(thatfile,whoa)){
				stringstream ack(whoa);
				string bwah;
				string line;
				getline(ack,bwah,',');
				line=bwah+",";
				getline(ack,bwah,',');
				line=line+bwah+",";
				getline(ack,bwah,',');
				line=line+bwah+",";
				for(int i=0;i<moviePlace+1;i++){
					getline(ack,bwah,',');
					if(i!=moviePlace){
						line=line+bwah;
						if(i+2!=titles.size()){ //since we're deleting things and if we get to this point the last one is probably what's being deleted, you can end the commas a bit early
							line=line+",";
						}
					}
					if(i==moviePlace){
						break;
					}
				}
				if(moviePlace+1!=titles.size()){
					getline(ack,bwah);
					line=line+bwah;
				}
				users[count]=line;
				count++;
			}
		}
		thatfile.close();
		myfile.open(userFile);
		if(myfile.good()){
			for(int i=0;i<users.size();i++){
				myfile<<users[i]<<endl;
			}
		}
		myfile.close();
		for(int i=0;i<userList.size();i++){
			userList[i]->reviews[moviePlace].pop_back();
		}
		titles[moviePlace].pop_back();
	}
}

void MovieTree::addMovieNode(string title, int releaseYear, int quantity){ //this adds a movie node to the BST, but NOT to the movie file
	MovieNode *temp=root;
	titles.push_back(title);
	if(temp==NULL){ //this creates a root
		temp=new MovieNode(title,releaseYear,quantity); //thanks to https://en.wikipedia.org/wiki/Binary_search_tree for how to properly initialize a root
		root=temp;
	}
	else{
		MovieNode *par=temp;//par will keep track of what the parent needs to be
		while(temp!=NULL){ //this loop helps find where the new node can be placed without overwriting something
			if(title.compare(temp->title)<0){
				par=temp;
				temp=temp->leftChild;
			}
			else if(title.compare(temp->title)>0){
				par=temp;
				temp=temp->rightChild;
			}
		}
		MovieNode *newmn= new MovieNode(title,releaseYear,quantity);
		newmn->parent=par;
		if(title.compare(par->title)<0){
			par->leftChild=newmn;
		}
		else if(title.compare(par->title)>0){
			par->rightChild=newmn;
		}
	}
}
void MovieTree::findMovie(string title){ //searches the tree for a movie. Boring but practical, basically the same as in Assignment 7
	MovieNode *temp=search(title);
	if(temp!=NULL){
		cout<<"Movie Info:"<<endl;
		cout<<"==========="<<endl;
		cout<<"Title:"<<temp->title<<endl;
		cout<<"Year:"<<temp->year<<endl;
		cout<<"Quantity:"<<temp->quantity<<endl;
	}
	else{
		cout<<"Movie not found."<<endl;
	}
}

void MovieTree::rentMovie(string title, string movieFile){ //the same as Assignment 7, except it updates the movie document to reflect the change in quantity
	MovieNode *temp=search(title);
	if(temp==NULL){
		cout<<"Movie not found."<<endl;
	}
	else{
		if(temp->quantity==0){
			cout<<"Movie unavailable"<<endl;
			return;
		}
		temp->quantity=temp->quantity-1;
		cout<<"Movie has been rented."<<endl;
		cout<<"Movie Info:"<<endl;
		cout<<"==========="<<endl;
		cout<<"Title:"<<temp->title<<endl;
		cout<<"Year:"<<temp->year<<endl;
		cout<<"Quantity:"<<temp->quantity<<endl; 
		
		int moviePlace;//helps the program match up the review to the movie
		for(int i=0;i<titles.size();i++){
			if(temp->title==titles[i]){
				moviePlace=i;
				cout<<moviePlace<<endl;
				break;
			}
		}
		
		quantAdjust(temp->quantity,movieFile,title,moviePlace);
	}
}
void MovieTree::quantAdjust(int quantity,string movieFile,string movieTitle,int moviePlace){ //changes the quantity of films in the film file
	vector<string> movies;
	string temp;
	ifstream thatfile; //we open the movie file so we can avoid overwriting the whole file
	thatfile.open(movieFile);
	if(thatfile.good()){
		while(getline(thatfile,temp)){
			movies.push_back(temp);
		}
	}
	thatfile.close();
	thatfile.open(movieFile);
	if(thatfile.good()){
		while(getline(thatfile,temp)){
			stringstream ack(temp);
			string yep;
			string line;
			getline(ack,yep,',');
			if(yep==movieTitle){
				line=yep;
				getline(ack,yep,',');
				line=line+","+yep+",";
				movies[moviePlace]=line;
			}
		}
	}
	ofstream myfile; //thanks to http://www.cplusplus.com/doc/tutorial/files/ for the refresher on writing things to files
	myfile.open(movieFile);
	if(myfile.good()){
		for(int i=0;i<movies.size();i++){
			myfile<<movies[i];
			if(i==moviePlace){
				myfile<<quantity;
			}
			myfile<<endl;
		}
	}
	myfile.close();
}

void MovieTree::returnMovie(string title, string user,string userFile,string movieFile){ //returns a movie, asks the user to review the movie if he's never seen it before, and offers a new movie
	MovieNode *temp=search(title);
	if(temp==NULL){
		cout<<"Movie not found."<<endl;
	}
	else{
		temp->quantity=temp->quantity+1;
		cout<<"Movie has been returned."<<endl;
		cout<<"Movie Info:"<<endl;
		cout<<"==========="<<endl;
		cout<<"Title:"<<temp->title<<endl;
		cout<<"Year:"<<temp->year<<endl;
		cout<<"Quantity:"<<temp->quantity<<endl; 
		
		int moviePlace;//helps the program match up the review to the movie
		for(int i=0;i<titles.size();i++){
			if(temp->title==titles[i]){
				moviePlace=i;
				break;
			}
		}
		
		quantAdjust(temp->quantity,movieFile,title,moviePlace);
		
		User *tempr=userSearch(user); //this will allow the user to review a movie if they haven't reviewed it before
		
		if(tempr->reviews[moviePlace]=="0"){
			cout<<"Would you like to review this movie?"<<endl;
			string com;
			getline(cin,com);
			if(com=="yes"){
				reviewMovie(user,userFile,moviePlace);
			}
		}
		for(int i=0;i<userList.size();i++){
			if(userList[i]->name==user){
				if(userList[i]->reviews[moviePlace]=="L"){
					cout<<"If you liked that movie, try this one!"<<endl;
				}else if(userList[i]->reviews[moviePlace]=="X"){
					cout<<"Sorry you didn't like that movie, try this one instead!"<<endl;
				} else if(userList[i]->reviews[moviePlace]=="0"){
					cout<<"Try this movie next!"<<endl;
				}
				bool found=false;
				for(int j=0;j<userList.size();j++){ //the program only suggests one movie, and it prefers to suggest films that the user has never seen before
					if(j!=i){
						if(userList[j]->reviews[moviePlace]==userList[i]->reviews[moviePlace]){
							for(int k=0;k<userList[j]->reviews.size();k++){
								if(userList[j]->reviews[k]=="L"&&userList[i]->reviews[k]=="0"){
									cout<<titles[k]<<endl;
									found=true;
									return;
								}
							}
						}
					}
				}
				if(!found){ //if it can't find a movie that the user has never seen, it defaults to a movie that it knows the user likes
					for(int j=0;j<userList.size();j++){
						if(j!=i){
							if(userList[j]->reviews[moviePlace]==userList[i]->reviews[moviePlace]){
								for(int k=0;k<userList[j]->reviews.size();k++){
									if(userList[j]->reviews[k]=="L"&&userList[i]->reviews[k]!="X"){
										cout<<titles[k]<<endl;
										return;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void MovieTree::reviewMovie(string user,string userFile,int titleNumber){
	//this function uses the user file and the order in which the movies were added to update the user file to accomodate the user's new review 
	vector<string> users;
	ifstream thatfile;
	string temp;
	thatfile.open(userFile);
	if(thatfile.good()){
		while(getline(thatfile,temp)){
			users.push_back(temp);
		}
	}
	thatfile.close();
	thatfile.open(userFile);
	if(thatfile.good()){
		string line; //this string is the final line that we want to use in the file
		string stream;
		int count=0;
		while(getline(thatfile,stream)){
			stringstream ack(stream);
			string bwah;
			getline(ack,bwah,',');
			if(bwah==user){
				line=bwah+",";
				getline(ack,bwah,',');
				line=line+bwah+",";
				getline(ack,bwah,',');
				line=line+bwah+",";
				for(int i=0;i<titleNumber+1;i++){
					getline(ack,bwah,',');
					if(i==titleNumber){
						cout<<"Type the letter L if you liked this movie, and the letter X if you disliked it"<<endl;
						string com;
						getline(cin,com);
						if(com=="L"||com=="X"){
							bwah=com;
							for(int k=0;k<userList.size();k++){
								if(userList[k]->name==user){
									userList[k]->reviews[titleNumber]=com;
									break;
								}
							}
						}
					}
					line=line+bwah;
					if(i+1!=titles.size()){
						line=line+",";
					}
				}
				if(titleNumber+1!=titles.size()){
					getline(ack,bwah);
					line=line+bwah;
				}
				users[count]=line;
			}
			count++;
		}
	}
	thatfile.close();
	ofstream myfile;
	myfile.open(userFile);
	if(myfile.good()){
		for(int i=0;i<users.size();i++){
			myfile<<users[i]<<endl;
		}
	}
	myfile.close();
}

User* MovieTree::userSearch(string user){ //searches for a user in the user list
	for(int i=0;i<userList.size();i++){
		if(user==userList[i]->name){
			return userList[i];
		}
	}
	return NULL;
}

void MovieTree::printMovieInventory(MovieNode * node){ //prints the tree in alphabetical order
	if(node->leftChild!=NULL){
		printMovieInventory(node->leftChild);
	}
	cout<<"Movie: "<<node->title<<" "<<node->quantity<<endl;
	if(node->rightChild!=NULL){
		printMovieInventory(node->rightChild);
	}
}
        
MovieNode* MovieTree::search(string title){ //searches the tree for a specfic movie
	MovieNode *temp=root;
	while(temp!=NULL){
		if(title.compare(temp->title)<0){
			temp=temp->leftChild;
		}
		else if(title.compare(temp->title)>0){
			temp=temp->rightChild;
		}
		else if(title==temp->title){
			return temp;
		}
	}
	return NULL;
}

void MovieTree::DeleteAll(MovieNode * node){ //deletes everything when you're done
	if(node->leftChild!=NULL){
		DeleteAll(node->leftChild);
	}
	if(node->rightChild!=NULL){
		DeleteAll(node->rightChild);
	}
	delete node;
}

void MovieTree::countMovieNodes(MovieNode *node, int &c){ //thanks to the forums for the tip about the &, this function just counts the nodes and tells you how many there are
	if(node->leftChild!=NULL){
		countMovieNodes(node->leftChild,c);
	}
	c++;
	if(node->rightChild!=NULL){
		countMovieNodes(node->rightChild,c);
	}
}

MovieNode* MovieTree::treeMinimum(MovieNode *node){
	MovieNode *temp=node;
	if(temp->rightChild!=NULL){
		temp=temp->rightChild;
	}
	while(temp->leftChild!=NULL){
		temp=temp->leftChild;
	}
	return temp;
}

void MovieTree::addUser(std::string user, std::string password, bool manager, std::string reviews){ //adds a new user to the database
	vector<string> rev; //this will help us split up that string that has all the reviews on it
	stringstream s(reviews);
	string temp;
	while(getline(s,temp,',')){
		rev.push_back(temp);
	}
	User *newuser=new User(user,password,manager,rev);
	userList.push_back(newuser);
}

bool MovieTree::userLogin(string user){ //confirms that there is actually a user under the name inputted
	for(int i=0;i<userList.size();i++){
		if(user==userList[i]->name){
			return true;
		}
	}
	return false;
}

bool MovieTree::passwordLogin(string user, string password){ //makes sure the inputted password matches the user's password
	for(int i=0;i<userList.size();i++){
		if(user==userList[i]->name){
			if(password==userList[i]->password){
				return true;
			}
		}
	}
	return false;
}

bool MovieTree::isManager(string user){ //checks to see if the user is a manager or not
	for(int i=0;i<userList.size();i++){
		if(user==userList[i]->name){
			if(userList[i]->manager==true){
				return true;
			}
		}
	}
	return false;
}

void MovieTree::printUsers(){ //prints out all the users for the manager in case someone forgets their password
	for(int i=0;i<userList.size();i++){
		cout<<"username: "<<userList[i]->name<<endl;
		cout<<"password: "<<userList[i]->password<<endl;
	}
}

void MovieTree::addMovieFile(string title,int year,int quantity,string movieFile,string userFile,string user){ //adds a movie to the movie file and lets the manager put a review on it to help people find a similar movie they may like
	vector<string> movies;
	vector<string> users;
	string temp;
	ifstream thatfile; //we open the movie file so we can avoid overwriting the whole file
	thatfile.open(movieFile);
	if(thatfile.good()){
		while(getline(thatfile,temp)){
			movies.push_back(temp);
		}
	}
	thatfile.close();
	ofstream myfile; //thanks to http://www.cplusplus.com/doc/tutorial/files/ for the refresher on writing things to files
	myfile.open(movieFile);
	if(myfile.good()){
		for(int i=0;i<movies.size();i++){
			myfile<<movies[i]<<endl;
		}
		myfile<<title<<","<<year<<","<<quantity<<endl;
	}
	myfile.close();
	thatfile.open(userFile);
	if(thatfile.good()){
		while(getline(thatfile,temp)){ //this is a similar idea to the movies, except we're updating the user doc as well
			users.push_back(temp);
		}
	}
	thatfile.close();
	myfile.open(userFile);
	if(myfile.good()){
		for(int i=0;i<users.size();i++){
			myfile<<users[i]<<",0"<<endl;
		}
	}
	myfile.close();
	for(int i=0;i<userList.size();i++){
		userList[i]->reviews.push_back("0");
	}
	reviewMovie(user,userFile,(titles.size()-1)); //reviewing the movie helps with the "recommending movies" function
}
