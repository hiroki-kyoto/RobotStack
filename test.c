#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <string.h>

#define NAME_LENGTH 20

typedef struct _Robot
{
	char name[NAME_LENGTH];
	int energy;
	float data;
}Robot;

/** methods for Robot Class **/
void RobotCreate(Robot **robot)
{
	*robot = (Robot*)malloc(sizeof(Robot));
	// initialize the arguments
	// unnamed robot
	char *name = "NOT SET";
	memset((char*)(*robot)->name, 0, sizeof((*robot)->name));
	memcpy((char*)(*robot)->name, (char*)name, strlen(name)*sizeof(char));
	// dead robots
	(*robot)->energy = 0;
	// carry no data
	(*robot)->data = 0.0;
}

void RobotDestroy(Robot **robot)
{
	// name has to be non-const
	free(*robot);
	*robot = NULL;
}

void RobotDump(Robot *robot)
{
	printf("Name:	%s\n", robot->name);
	printf("Energy:	%d\n", robot->energy);
	printf("Data:	%f\n", robot->data);
}

void RobotSetName(Robot *robot, char name[NAME_LENGTH])
{
	memcpy((char*)robot->name, (char*)name, strlen(name)*sizeof(char));
}

typedef struct _Node
{
	Robot* data;
	struct _Node* prev;
}Node;

void NodeCreate(Node **node)
{
	*node = (Node*)malloc(sizeof(Node)*1);
	(*node)->data = NULL;
	(*node)->prev = NULL;
}

void NodeDestroy(Node **node)
{
	RobotDestroy(&((*node)->data));
	free(*node);
	*node = NULL;
}

void NodeDump(Node* node)
{
	printf("Node Data:\n");
	RobotDump(node->data);
	printf("Node Prev:\t%d\n", (int)node->prev);
}

typedef struct _Stack
{
	Node *head;
	Node *tail;
	int count;
}Stack;

void StackCreate(Stack **stack)
{
	*stack = (Stack*)malloc(sizeof(Stack));
	// the first empty node, different from tail, it's not just a pointer.
	Node *node;
	NodeCreate(&node);
	(*stack)->head = node;
	(*stack)->head->data = NULL;
	(*stack)->head->prev = NULL;

	// the stack is empty, so the head meets the tail.
	(*stack)->tail = (*stack)->head;

	(*stack)->count = 0;
}

void StackDestroy(Stack **stack)
{
	// unitl stack empty
	while((*stack)->head!=(*stack)->tail)
	{
		Node *prev = (*stack)->tail->prev;
		// destroy stack node, with given custom method
		NodeDestroy(&((*stack)->tail));
		(*stack)->tail = prev;
	}
	// clear orginal nodes
	NodeDestroy(&((*stack)->head));
	// free the stack shell
	free(*stack);
	*stack = NULL;
}

void StackPush(Stack *stack, Robot *robot)
{
	Node* node;
	NodeCreate(&node);
	node->data = robot;
	node->prev = stack->tail;

	stack->tail = node;
	stack->count ++;
}

void StackPop(Stack *stack, Robot **robot)
{
	if(stack->count){
		Node *node = stack->tail;
		stack->tail = stack->tail->prev;
		// clear up node and return node data
		*robot = node->data;
		// one thing to remember, the robot object 
		// should be released by the outside environment
		free(node);
		stack->count --;
	}
	else {
		// the stack is empty print out error information
		printf("error in StackPop(), stack is empty\n");
	}
}

void StackDump(Stack *stack)
{
	printf("Stack Information:\n");
	while(stack->count)
	{
		Robot *robot;
		StackPop(stack, &robot);
		RobotDump(robot);
		RobotDestroy(&robot);
	}
}

int main(int argc, const char** argv)
{
	// ROBOT TEST
	printf("ROBOT TEST:\n");
	Robot *A, *B;
	RobotCreate(&A);
	RobotCreate(&B);
	char nameA[NAME_LENGTH] = "ROBOT A";
	char nameB[NAME_LENGTH] = "ROBOT B";
	RobotSetName(A, nameA);
	RobotSetName(B, nameB);
	
	// full energy is 100
	A->energy = 38;
	B->energy = 76;
	// the secret of each robot
	A->data = 27.33;
	B->data = -67.868;
	
	RobotDump(A);
	RobotDump(B);

	// NODE TEST
	printf("\nNODE TEST:\n");
	Node *node;
	NodeCreate(&node);
	node->data = A;
	node->prev = NULL;

	NodeDump(node);

	// STACK TEST
	printf("\nSTACK TEST:\n");
	Stack *stack;
	StackCreate(&stack);
	StackPush(stack, A);
	StackPush(stack, B);

	StackDump(stack);
	// clear memory
	StackDestroy(&stack);

	return 0;
}
