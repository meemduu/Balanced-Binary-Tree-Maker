#include <iostream>
#include <variant>
#include <vector>
#include <memory>
#include <cmath>
#include "BinaryNode.h"
#include "BinaryNodeTree.h"
#include <iomanip>

using namespace std;

	//------------------------------------------------------------
	// Display method.
	//------------------------------------------------------------
void display(int& i) {
	cout << "Visited node: " << i << endl;
}

	//------------------------------------------------------------
	// Method for building the binary tree.
	//------------------------------------------------------------
template <class ItemType>
shared_ptr<BinaryNode<ItemType>> buildBalancedTree(const vector<ItemType>& data, shared_ptr<BinaryNode<ItemType>> rootPtr) {
	for (const auto& value : data) {
		auto newNodePtr = make_shared<BinaryNode<ItemType>>(value);
		rootPtr = buildBalancedTreeHelper(rootPtr, newNodePtr);
	}
	return rootPtr;
}

	//------------------------------------------------------------
	// buildBalancedTree helper method. This method builds the tree similar to the logic balancedAdd function of BinaryNodeTree Class.
	//------------------------------------------------------------
template <class ItemType>
shared_ptr<BinaryNode<ItemType>> buildBalancedTreeHelper(shared_ptr<BinaryNode<ItemType>> subTreePtr, shared_ptr<BinaryNode<ItemType>> newNodePtr) {
	BinaryNodeTree<ItemType> tree;
	if (subTreePtr == nullptr)
		return newNodePtr;
	else {
		auto leftPtr = subTreePtr->getLeftChildPtr();
		auto rightPtr = subTreePtr->getRightChildPtr();

		if (tree.getHeightHelper(leftPtr) > tree.getHeightHelper(rightPtr)) {
			rightPtr = buildBalancedTreeHelper(rightPtr, newNodePtr);
			subTreePtr->setRightChildPtr(rightPtr );
		} else {
			leftPtr = buildBalancedTreeHelper(leftPtr, newNodePtr);
			subTreePtr->setLeftChildPtr(leftPtr);
		}

		return subTreePtr;
	}
}



	//------------------------------------------------------------
	// Method for checking if the tree is a perfect tree.
	// This will never return true because of the way i coded the buildBalancedTree method. But it's too late to switch now :(
	//------------------------------------------------------------
template<class ItemType>
bool isPerfect(BinaryNodeTree<ItemType>& tree) {
    int numberofheight = tree.getHeight();
    int numberofnode = tree.getNumberOfNodes();
    return ((pow(2, numberofheight) - 1) == numberofnode);
}



	//------------------------------------------------------------
	// Helper method of the isFull method.
	//------------------------------------------------------------
template <class ItemType>
bool isFullHelper(shared_ptr<BinaryNode<ItemType>> root) {
    if (!root)
        return true;
    
    if (!root->getLeftChildPtr() && !root->getRightChildPtr())
        return true;
    
    if (root->getLeftChildPtr() && root->getRightChildPtr())
        return isFullHelper(root->getLeftChildPtr()) && isFullHelper(root->getRightChildPtr());
    
    return false;
}

	//------------------------------------------------------------
	// Method for checking if the tree is a full tree.
	// Again, this will never return true aswell because of the same reason mentioned in the isPerfect method comment.
	//------------------------------------------------------------
template <class ItemType>
bool isFull(BinaryNodeTree<ItemType>& tree) {
    return isFullHelper(tree.rootPtr);
}



	//------------------------------------------------------------
	// Helper method of the isComplete method.
	//------------------------------------------------------------
template <class ItemType>
bool isCompleteHelper(shared_ptr<BinaryNode<ItemType>> root, int index, int totalNodes) {
    if (!root) return true;
    
    if (index >= totalNodes) return false;
    
    return isCompleteHelper(root->getLeftChildPtr(), 2 * index + 1, totalNodes) &&
           isCompleteHelper(root->getRightChildPtr(), 2 * index + 2, totalNodes);
}

	//------------------------------------------------------------
	// Method for checking if the tree is a complete tree.
	//------------------------------------------------------------
template <class ItemType>
bool isComplete(BinaryNodeTree<ItemType>& tree) {
    int totalNodes = tree.getNumberOfNodes();
    return isCompleteHelper(tree.rootPtr, 0, totalNodes);
}


	//------------------------------------------------------------
	// Helper ethod of fancyDisplay method.
	// Had to do it sideways because the normal way is a little too hard i want to keep my sanity in check.
	//------------------------------------------------------------
template <class ItemType>
void fancyDisplayHelper(shared_ptr<BinaryNode<ItemType>> root, int indent) {
    if (!root)
        return;
    
    if (root->getRightChildPtr() != nullptr) {
        fancyDisplayHelper(root->getRightChildPtr(), indent + 4);
        cout << setw(indent) << " " << " /\n";
    }
    
    cout << setw(indent) << " " << root->getItem() << "\n";
    
    if (root->getLeftChildPtr() != nullptr) {
        cout << setw(indent) << " " << " \\\n";
        fancyDisplayHelper(root->getLeftChildPtr(), indent + 4);
    }
}

	//------------------------------------------------------------
	// Method for displaying the tree with / and \.
	//------------------------------------------------------------
template <class ItemType>
void fancyDisplay(BinaryNodeTree<ItemType>& tree) {
    fancyDisplayHelper(tree.rootPtr, 0);
}



	/////////////////////////////////////////////////////////////
	// 														   //
	//						MAIN METHOD 					   //
	//					                         			   //
	/////////////////////////////////////////////////////////////
int main() {

	// Enforcing the tree height rule: "The tree must have at least 3 levels."
	int count;
	cout << "En az 4 veri giriniz." << endl;
	cout << "Kac adet veri gireceksiniz? ";
	cin >> count;

	if(count < 4) {
		cout << "Lutfen en az 4 veri giriniz!" << endl;
		return 1;
	}

	// Define a vector and put the initializing values of the tree in it.
	vector<int> data;
	cout << "Verileri aralarinda bosluk olacak sekilde giriniz: ";
	for(int i = 0; i < count; i++) {
		int value;
		cin >> value;
		data.push_back(value);
	}

	// Create BinaryNodeTree object.
	auto tree = make_unique<BinaryNodeTree<int>>();

	// Set the rootPtr as the return of buildBalancedTree method (which is the first node we crate, and the middle element of the vector).
	tree->rootPtr = buildBalancedTree<int>(data, tree->rootPtr);

	// invoke inorderTraverse to check the result.
	cout << "\nTree Inorder sonucu: " << endl;
	tree->inorderTraverse(display);
	
	
	
	// CLI Loop: Insertion, Deletion and Tree Type.
	
	// Defining a vector to hold the new values to be added from the user to the tree
	vector<int> insertionOrder;
	int choice;
	do {
		cout << "\ninsertion icin 1'e basiniz \ndeletion icin 2'ye basiniz \nTree tipi icin 3'e basiniz \nDisplay icin 4'e basiniz \ncikis icin 9'a basiniz: ";
		cin >> choice;
		
		// When Insertion is chosen:  
		if (choice == 1) {
			int newInteger;
			// ask for the new value from the user.
			cout << "Yeni degeri giriniz: ";
			cin >> newInteger;
			// add the new value to the data vector and to the insertionOrder vector.
			insertionOrder.push_back(newInteger);
			data.push_back(newInteger);
			// invoke the add method of the BinaryNodeTree with the new integer.
			tree->add(newInteger);
			cout << "Yeni deger eklendi. Yeni inorder sonucu:  " << endl;
			tree->inorderTraverse(display);
			cout << endl;
		}
		// When Deletion is chosen: 
		else if (choice == 2) {
			// check if the insertionOrder vector is empty.
			// if yes, that means the user is trying to delete elements without adding first which violates the 3 heigth rule.
			// the rule is the reason i implemented the insertionOrder system.
			if (!insertionOrder.empty()) {
				int lastInserted = insertionOrder.back();
				// delete the last added value from both data vector and the insertionOrder vector.
				insertionOrder.pop_back();
				data.pop_back();
				cout << "Son eklenen deger (" << lastInserted << ") siliniyor..." << endl;
				// rebuild the tree by using the data vector.
				tree->clear();
				tree->rootPtr = buildBalancedTree<int>(data, tree->rootPtr);
				// check if the tree is empty. If not, then traverse the tree to check if the deletion is successfull 
				if (tree->isEmpty()) {
				    cout << "Agac bos!" << endl;
				} else {
				    tree->inorderTraverse(display);
				    cout << endl;
				}
			} 
			// if the insertionOrder is empty, ask user to add values insted
			else {
				cout << "Yeni degerler ekleyiniz" << endl;
			}
		} 
		// When Type Check is chosen:
		else if (choice == 3){
				cout << "\nTree Tipi: " << endl;
				
				// invoke the isPerfect method and print the result accordingly
				bool perf = isPerfect(*tree);
				if (perf)
					cout << "Tree Perfect! " << endl;
				else
					cout << "Tree Perfect degil!" << endl;
				
				// invoke the isPerfect method and print the result accordingly
				bool full = isFull(*tree);
				if (full)
					cout << "Tree Full! " << endl;
				else
					cout << "Tree Full degil!" << endl;	
				
				// invoke the isPerfect method and print the result accordingly
				bool complete = isComplete(*tree);
				if (complete)
					cout << "Tree Complete! " << endl;
				else
					cout << "Tree Complete degil!" << endl;	
			}
		// When Fancy Display is chosen:
		else if (choice == 4){
			// invoke the fancyDisplay method.
			fancyDisplay(*tree);
		}
		
	} while (choice != 9);

	return 0;
}