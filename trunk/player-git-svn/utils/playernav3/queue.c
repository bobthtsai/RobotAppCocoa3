#include <stdio.h>
#include <stdlib.h>
#include "playernav.h"

void queue_create() {
	front = rear = (Node*) malloc(sizeof(Node));
	front->next = rear->next = NULL;
}

void queue_showfront(){
	if(front->next == NULL)
		printf("\n佇列為空！");
	else
		printf("\n頂端值：%d", front->next->data);
}

void queue_add(pose_t data) {
	Node *newnode;

	newnode = (Node*) malloc(sizeof(Node));

	if(front->next == NULL)
		front->next = newnode;

	newnode->data = data;
	newnode->next = NULL;
	rear->next = newnode;
	rear = newnode;
}

void queue_delete() {
	Node* tmpnode;

	if(front->next == NULL) {
		printf("\n佇列已空！");
		return;
	}

	tmpnode = front->next;
	front->next = tmpnode->next;
	free(tmpnode);
}

void queue_showqueue() {
	Node* tmpnode;

	tmpnode = front->next;

	printf("\nContent:");
	while(tmpnode != NULL) {
		printf("%d ", tmpnode->data);
		tmpnode = tmpnode->next;
	}
}
