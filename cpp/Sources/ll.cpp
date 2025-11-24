
/*

	author: Myron Lafty
	date: Oct 28, 2025

*/

Node* insert(Node* root, Vector dat, int loc) {
	
	if (root == nullptr) {

		root = new Node();
		root->next = nullptr;
		root->dat = dat;

		return root;
	}	
	
	if (loc == 0) {

		// insert head

		Node* in = new Node();
        	in->dat = dat;
        	in->next = root;

        	return in;
	}
	else if (loc < 0) {
		
		// do nothing

		return root;
	}

	// insert body or tail
	// idx starts at 1 because the iterating node "prev" is the node previous to the
	// node we want to insert
	// prev will start at root
	// if desired loc is beyond size, insert gets appended to tail

	int idx = 1;
	Node* in = new Node();
	Node* prev = root;
	in->dat = dat;

	while (prev->next != nullptr && idx < loc) {
		
		prev = prev->next;
		idx++;
	}	
	
	in->next = prev->next;
	prev->next = in;

	return root;
}

Node* remove(Node* root, int loc) {
	
	if (root == nullptr) {
		
		return nullptr;
	}
	
	if (loc == 0) {
		
		Node* newRoot = root->next;

		delete root;
		return newRoot;
	}

	Node* prev = root;
	Node* cur = root->next;
	int idx = 1;
	
	while (cur != nullptr && idx < loc) {
		prev = cur;
		cur = cur->next;
		idx++;
	}

	if (cur == nullptr) {
	
		return root;
	}

	prev->next = cur->next;

	delete cur;
	return root;
}

