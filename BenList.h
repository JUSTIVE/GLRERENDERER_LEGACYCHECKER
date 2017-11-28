#pragma once
#include<iostream>
using namespace std;
template <typename T>
struct Node{
public:
	Node<T>*Link;
	T data;
};

template <typename T>
class BenList{
private:
	unsigned int size;
	Node<T>* lastNode;
	void destroy(Node<T> node){
		if (node == NULL)
			return;
		destroy(node->Link);
		free(node);
		return;
	};
public:
	Node<T>* List;

	BenList(){
		this->size = 0;
		this->List = NULL;
		this->lastNode = NULL;
	};
	
	int getSize(){ return this->size; }
	Node<T>* createNode(T data){
		Node<T>* newNode = (Node<T>*)malloc(sizeof(Node<T>));
		newNode->data = data;
		newNode->Link = NULL;
		return newNode;
	};
	void append(T data){
		Node<T>* newNode = this->createNode(data);
		if (newNode == NULL)
			cout << "new Node Create error!!" << endl;
		if (List == NULL){
			List = newNode;
			lastNode = newNode;
			size++;
			return;
		}

		lastNode->Link = newNode;
		lastNode = newNode;
		size++;
		return;
	};
	bool isThereThisGuy(T target){
		if (this == NULL)
			return false;
		if (this->List == NULL)
			return false;
		Node<T>* currentNode = this->List;
		while (currentNode->Link != NULL){
			if (currentNode->data == target)
				return true;
			currentNode = currentNode->Link;
		}
		return false;
	};
	int find(T target){
		if (List == NULL)
			return -1;
		int position = -1;
		Node<T>* currentNode = this->List;
		while (currentNode->Link != NULL){
			position++;
			if (currentNode->data == target)
				return position;
			currentNode = currentNode->Link;
		}
		return position;
	};
	void cleanUp(){
		Node<T>* targetNode;
		Node<T>* currentNode = this->List;
		while (currentNode != NULL){
			targetNode = currentNode;
			currentNode = currentNode->Link;
			free(targetNode);
		}
		size = 0;
	}
	~BenList(){
		
	}
};


