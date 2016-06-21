#include "node.hpp"

#include <vector>
#include <algorithm> //find

using namespace std;

//Constructor basic
Node::Node(){}

//Constructor with father given by reference (normal)
Node::Node(Node& father){
	set_father(&father);
}

//Constructor with father given by pointer
Node::Node(Node* father){
	set_father(father);
}

//Destructor
Node::~Node(){
	//Set the current father as the father of all the sons
	const size_t s = m_sons.size();
	for(size_t i = 0; i < s; i++){
		m_sons[i]->set_father(m_father);
	}
	//remove this node from its father's sons list
	set_father(nullptr);
}


void Node::set_father(Node* father){
	//Old father ? Remove this node as son of the old father
	if(m_father != nullptr){
		vector<Node*>::iterator this_son = find(m_father -> m_sons.begin(), m_father->m_sons.end(), this);
		*this_son = nullptr;//Set the pointer to null (
		m_father -> m_sons.erase(this_son);//remove the pointer from the vector
	}
	//New father ? Add this node as son
	if(father != nullptr){father -> add_son(this);}
	else{m_father = nullptr;}
}

//Add a son and set the current Node as the father
void Node::add_son(Node* son){
	m_sons.push_back(son);
	son -> m_father = this;
}

//Create a son and set the current Node as father
void Node::create_son(){
	Node* son = new Node;
	this -> add_son(son);
}
