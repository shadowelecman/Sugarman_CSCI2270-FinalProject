using namespace std;
#include "MovieTree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void printMenu(){ //prints a menu, nothing fancy
	cout<<"======Main Menu======"<<endl;
	cout<<"1. Find a movie"<<endl;
	cout<<"2. Rent a movie"<<endl; 
	cout<<"3. Print the inventory"<<endl;
	cout<<"4. Return a movie"<<endl;
	cout<<"5. Count the movies"<<endl;
	cout<<"6. Quit"<<endl; 
}

void printManagerMenu(){ //this is the manager's menu, which allows you to add or remove a movie from the list
	cout<<"======Main Menu======"<<endl;
	cout<<"1. Find a movie"<<endl;
	cout<<"2. Add a movie"<<endl; 
	cout<<"3. Print the inventory"<<endl;
	cout<<"4. Delete a movie"<<endl;
	cout<<"5. Count the movies"<<endl;
	cout<<"6. Print users"<<endl;
	cout<<"7. Quit"<<endl;
}

int main(int arc, char* argv[]){
	MovieTree* bwah=new MovieTree;
	string stream;
	string com;
	string user; //keeps track of who's logged in
	bool good=false;//helps determine if username is legitimate
	bool pgood=false;//helps determine if password is legitimate
	bool manager=false;//helps determine if the user is a manager or not, and by extension which menu to pull up
	bool done=false;//lets the menu work, as long as it's false it'll keep showing up
	int command=0;
	ifstream mt;
	mt.open(argv[1]);
	if(mt.good()){
		while(getline(mt, stream)){
			stringstream ack(stream);//special thanks to Kristin on the forums for linking to some helpful stringstream notes
			string temp;
			getline(ack,temp,',');
			string title=temp;
			getline(ack,temp,',');
			string years=temp;
			getline(ack,temp);
			string quantity=temp;
			int year=stoi(years);
			int quant=stoi(quantity);
			bwah->addMovieNode(title,year,quant);
		}
	}
	mt.close();
	mt.open(argv[2]);
	if(mt.good()){
		while(getline(mt, stream)){ //this stringstream helps add users to the database
			stringstream ack(stream);
			string temp;
			getline(ack,temp,',');
			string user=temp;
			getline(ack,temp,',');
			string password=temp;
			getline(ack,temp,',');
			bool manager;
			if(temp=="true"){
				manager=true;
			}else{
				manager=false;
			}
			getline(ack,temp);
			string reviews=temp;
			bwah->addUser(user,password,manager,reviews);
		}
	}
	mt.close();
	while(!pgood){
		cout<<"Please log in"<<endl;
		cout<<"username:"<<endl;
		getline(cin,com);
		good=bwah->userLogin(com);
		if(good){
			user=com;
			cout<<"password:"<<endl;
			getline(cin,com);
			cout<<com<<endl;
			manager=bwah->isManager(user);
			pgood=bwah->passwordLogin(user,com);
		}
	}
	if(!manager){
		while(!done){
			printMenu();
			cin>>command;
			if(command==6){
				delete bwah;
				done=true;
				cout<<"Goodbye!"<<endl;
			} 
			else if(command==1){
				cout<<"Enter title:"<<endl;
				cin.ignore();
				getline(cin,com);
				bwah->findMovie(com);
			}
			else if(command==2){
				cout<<"Enter title:"<<endl;
				cin.ignore();
				getline(cin,com);
				bwah->rentMovie(com,argv[1]);
			}
			else if(command==3){
				bwah->printMovieInventory();
			}
			else if(command==4){
				cout<<"Enter title:"<<endl;
				cin.ignore();
				getline(cin,com);
				bwah->returnMovie(com,user,argv[2],argv[1]);
			}
			else if(command==5){
				cout<<"Tree contains: "<<bwah->countMovieNodes()<<" movies."<<endl; 
			}
		}
	}else{
		while(!done){
			printManagerMenu();
			cin>>command;
			if(command==7){
				delete bwah;
				done=true;
				cout<<"Goodbye!"<<endl;
			} 
			else if(command==1){
				cout<<"Enter title:"<<endl;
				cin.ignore();
				getline(cin,com);
				bwah->findMovie(com);
			}
			else if(command==2){
				cout<<"Enter title:"<<endl;
				cin.ignore();
				getline(cin,com);
				string title=com;
				cout<<"Enter year:"<<endl;
				getline(cin,com);
				int year=stoi(com);
				cout<<"Enter quantity:"<<endl;
				getline(cin,com);
				int quant=stoi(com);
				bwah->addMovieNode(title,year,quant);
				bwah->addMovieFile(title,year,quant,argv[1],argv[2],user);
			}
			else if(command==3){
				bwah->printMovieInventory();
			}
			else if(command==4){
				cout<<"Enter title:"<<endl;
				cin.ignore();
				getline(cin,com);
				bwah->deleteMovieNode(com,argv[1],argv[2]);
			}
			else if(command==5){
				cout<<"Tree contains: "<<bwah->countMovieNodes()<<" movies."<<endl; 
			}
			else if(command==6){
				bwah->printUsers();
			}
		}
	}
	return 0;
}
