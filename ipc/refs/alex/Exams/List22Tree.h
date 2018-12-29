/**************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-05    
    Last modified date:		2013-09-05
    Description: Exam problem: turn sorted linked list to tree and vice-varsa,
    				general functions.
***************************************************************************/

#ifndef __LIST22TREE_H__
#define __LIST22TREE_H__

typedef struct TreeNode_t TreeNode;
typedef struct ListNode_t ListNode;

struct TreeNode_t
{
	int			m_key;
	TreeNode*	m_left;
	TreeNode*	m_right;
};


struct ListNode_t
{
	int			m_key;
	ListNode*	m_prev;
	ListNode*	m_next;
};



enum Sides {LEFT = -1, RIGHT = 1};
enum TraverseMode {PRE, IN, POST};

TreeNode*	TreeNodeCreate	(int _key);
void		TreeDestroy		(TreeNode* _root);
TreeNode*	TreeInsert		(TreeNode* _root, int _key);
int			TreeFind		(TreeNode **_root, int _key);
void		TreePrint		(TreeNode* _root, int _mode);


ListNode*	ListNodeCreate	(int _key);
void		ListDestroy		(ListNode* _head);
ListNode*	ListInsert		(ListNode* _head, int _key);
int			ListFind		(ListNode **_head, int _key);
void		ListPrint		(ListNode* _head);


#endif /* __LIST22TREE_H__ */

