/*
	Example of tree
	contact: drahull (Axel VERDIER)
	email: axel.l.verdier@free.fr
	 
	TODO: 'protected' methods and attributs to heritance
*/

#include <iostream>
#include "node.hpp"

using namespace std;


int main(){
	cout << "Initialize a basic tree" << endl;
	Node tree_root;

	cout << endl << "Create 1 son" << endl;
	tree_root.create_son();

	cout << endl << "Create 1 son 'n' in main and add it" << endl;
	Node* n = new Node;
	tree_root.add_son(n);
	
	cout << "\tNumber of sons of the root: " << tree_root.get_nb_sons() << endl;
	
	cout << endl << "Create 2 sons for the node 'n'" << endl;
	n -> add_son(new Node); //With creating a Node
	n -> create_son(); //Calling the specific method
	
	cout << "\tNumber of sons of 'n': " << n -> get_nb_sons() << endl;

	cout << "\tNumber of sons of the root: " << tree_root.get_nb_sons() << endl;

	cout << endl << "Removing the node 'n'. The sons of 'n' are now sons of the father of 'n'." << endl;
	delete n;

	cout << "\tNumber of sons of the root: " << tree_root.get_nb_sons()  << endl;


	return 0;
}
