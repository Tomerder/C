#include <stdio.h>
#include <stdlib.h>

#define NUM_OF_ITEMS 10

typedef struct Node Node;

struct Node{
	int m_key;
	Node* m_next;
};


/*--------------------------------------------------------------------*/

Node* buildList(int* arrOfInts);
void addNode(Node* _head, int _toAdd);

void PrintList(Node* _head);

/*--------------------------------------------------------------------*/

Node* revListIter(Node* _head);

Node* revListRec(Node* _head);
Node* revListRecImp(Node* _node , Node* _nodeNext);

Node* revListRec2(Node* _head);

/*--------------------------------------------------------------------*/

int main(int argc,char** argv)
{
	int arrOfInts[NUM_OF_ITEMS] = {1,2,3,4,5,6,7,8,9,10};
	Node* root;

	root = buildList(arrOfInts);
	PrintList(root);

	root = revListIter(root);	
	PrintList(root);

	root = revListRec(root);
	PrintList(root);
	
	root = revListRec2(root);
	PrintList(root);

	return 0;
}


/*--------------------------------------------------------------------*/

Node* revListRec(Node* _head)
{
	Node* nodeNext = _head->m_next;

	_head->m_next = NULL;

	return revListRecImp(_head, nodeNext);
}


Node* revListRecImp(Node* _node , Node* _nodeNext)
{
	Node* nodeNextNext;

	if(_nodeNext == NULL){
		return _node;
	}

	nodeNextNext = _nodeNext->m_next;

	_nodeNext->m_next = _node;
	return revListRecImp(_nodeNext, nodeNextNext); 

}

/*--------------------------------------------------------------------*/

Node* revListIter(Node* _head)
{
	Node* node = _head;
	Node* nodeNext = _head->m_next;
	Node* nodeNextNext;

	node->m_next = NULL;

	while(nodeNext){
		nodeNextNext = nodeNext->m_next;

		nodeNext->m_next = node;

		node = nodeNext;
		nodeNext = nodeNextNext;		
	}

	return node;
}

/*--------------------------------------------------------------------*/

Node* revListRec2(Node* _node)
{
	Node* newHead;
	
	if(_node->m_next == NULL){
		return _node;   /*return old tail as new head*/
	}else{
		newHead = revListRec2(_node->m_next);  /*goto end*/
		_node->m_next->m_next = _node;  /*reverse link*/
	}

	_node->m_next = NULL;   /*set old head (last node that we deal with) as new tail*/

	return newHead;
}

/*--------------------------------------------------------------------*/

Node* buildList(int* _arrOfInts)
{
	int i;	
	Node* head = malloc(sizeof(Node)); 
	
	head->m_key = _arrOfInts[0];
	head->m_next = NULL;  
	
	for(i=1; i<NUM_OF_ITEMS; i++){
		addNode(head, _arrOfInts[i]);
	}

	return head;
}

void addNode(Node* _head, int _toAdd)
{
	Node* newNode = malloc(sizeof(Node));
	Node* node = _head;

	newNode->m_key = _toAdd;
	newNode->m_next = NULL;

	while(node->m_next){
		node = node->m_next;
	}

	node->m_next = newNode;
}

/*--------------------------------------------------------------------*/
void PrintList(Node* _head){
	Node* curNode = _head;
	
	printf("list:\n");

	while(curNode != NULL){
		printf("%d ", curNode->m_key);
		curNode = curNode->m_next;
	}	
	printf("\n");

}

/*--------------------------------------------------------------------*/
