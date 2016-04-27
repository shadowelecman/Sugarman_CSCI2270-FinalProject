#ifndef MOVIETREE_H
#define MOVIETREE_H
#include <iostream>
#include <vector>

struct MovieNode{
    std::string title;
    int year;
    int quantity;
    MovieNode *parent;
    MovieNode *leftChild;
    MovieNode *rightChild;

    MovieNode(){
	title="";
	year=0;
	quantity=0;
	parent=NULL;
	leftChild=NULL;
	rightChild=NULL;
    }

    MovieNode(std::string in_title, int in_year, int in_quantity)
    {
        title = in_title;
        year = in_year;
        quantity = in_quantity;
        parent = NULL;
        leftChild = NULL;
        rightChild = NULL;
    }

};

struct User{
	std::string name;
	std::string password;
	bool manager;
	std::vector<std::string> reviews;
	
	User(){
		name="";
		password="";
		manager=false;
		reviews=std::vector<std::string>(); //thanks to http://www.cplusplus.com/reference/vector/vector/operator=/ for how to initialize an empty vector
	}
	
	User(std::string in_name,std::string in_pw,bool man,std::vector<std::string> review){
		name=in_name;
		password=in_pw;
		manager=man;
		reviews=review;
	}
};

class MovieTree
{

    public:
        MovieTree();
        ~MovieTree();
        void printMovieInventory();
        int countMovieNodes();
        void deleteMovieNode(std::string title,std::string movieFile,std::string userFile);
        void addMovieNode(std::string title, int releaseYear, int quantity);
        void findMovie(std::string title);
        void rentMovie(std::string title,std::string movieFile);
        void addUser(std::string user, std::string password, bool manager, std::string reviews);
        bool userLogin(std::string user);
        bool passwordLogin(std::string user, std::string password);
        bool isManager(std::string user);
        void printUsers();
        void addMovieFile(std::string title,int year,int quantity,std::string movieFile,std::string userFile,std::string user);
        void returnMovie(std::string title,std::string user,std::string userFile,std::string movieFile);

    protected:

    private:
        void DeleteAll(MovieNode * node); //use this for the post-order traversal deletion of the tree
        void printMovieInventory(MovieNode * node);
        void reviewMovie(std::string user,std::string userFile,int titleNumber);
        void countMovieNodes(MovieNode *node, int &c);
        void quantAdjust(int quantity,std::string movieFile,std::string movieTitle,int moviePlace);
        MovieNode* search(std::string title);
        MovieNode* treeMinimum(MovieNode *node);
        MovieNode *root;
        User* userSearch(std::string user);
        std::vector<User*> userList;
        std::vector<std::string> titles;
};

#endif // MOVIETREE_H
