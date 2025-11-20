
/*

	author: Myron Lafty
	date: Oct 28, 2025

*/

#include<iostream>

using namespace std;

struct Node {

	int dat;
	Node* next;
};

Node* insert_head(Node* root, int dat) {

	Node* insert = new Node();
	insert->dat = dat;
	insert->next = nullptr;
	
	Node* temp = root;

	while (temp->next != nullptr) {
		
		temp = temp->next;
	}
	
	insert->next = temp;

	return insert;
}

void insert(Node* root, int dat, int loc) {
	
	// pls don't pass in less than 1

	if (loc < 1) {

		return;
	}

	Node* insert = new Node();
	insert->dat = dat;
	insert->next = nullptr;

	Node* temp = root;
	int idx = 1;

	while (temp->next != nullptr && idx < loc) {
		
		temp = temp->next;
		idx++;
	}	

	temp->next = insert;
}

void remove(Node* node, int loc) {
	
	
}

