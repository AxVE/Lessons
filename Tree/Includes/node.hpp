#ifndef NODE_HPP
#define NODE_HPP

#include <cstddef> //size_t
#include <vector>

class Node{
	public:
		Node();
		Node(Node& father);
		Node(Node* father);
		~Node();
		void set_father(Node* father);
		void add_son(Node* son);
		void create_son();
		size_t get_nb_sons() const {return m_sons.size();}

	private:
		Node* m_father = nullptr;
		std::vector<Node*> m_sons;
};


#endif
