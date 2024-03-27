#include <stdio.h>
#include <stdlib.h>
#include <iostream>

typedef struct Node {
    int patientId;
    struct Node* next;
} Node;

typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

void initQueue(Queue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
}

int isEmpty(Queue* queue) {
    return queue->front == NULL;
}

void enqueue(Queue* queue, int patientId) {
    Node* newNode_ptr = (Node*)malloc(sizeof(Node));
    newNode_ptr->patientId = patientId;
    newNode_ptr->next = NULL;
    if (isEmpty(queue)) {
        queue->front = newNode_ptr;
        queue->rear = newNode_ptr;
    }
    else {
        queue->rear->next = newNode_ptr;
        queue->rear = newNode_ptr;
    }
}

int dequeue(Queue* queue) {

    Node* nodeToDelete = queue->front;
    int patientId = nodeToDelete->patientId;
    queue->front = nodeToDelete->next;//ֱ��ָ����һ��
    free(nodeToDelete);
    if (queue->front == NULL) {
        queue->rear = NULL;
        //��queue->front == NULLΪ��ʱ
        //����ǰ����Ѿ�ָ������һ��λ�ã�
        //����ȷ����ʱ�Ѿ����������������У�����˵��
        //��βҲҪ����ΪNULL
        //��Ϊ��������ɾ�����еĲ��������˵��ɾ����
        //��ǰԪ�غ���һ��Ϊ�գ��Ǿ�˵����ǰ����Ϊ��
        //��ȻҪ����ͷβ�����ã�
    }
    return patientId;
}

void displayQueue(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty.\n");
        return;
    }
    Node* currentNode = queue->front;
    while (currentNode != NULL) {
        printf("%d ", currentNode->patientId);
        currentNode = currentNode->next;
    }
    printf("\n");

    //return  û��Ҫд������ִ�н�������Ҳ��
}

int main() {
    Queue queue;
    initQueue(&queue);

    int choice, patientId;
    do {
        printf("1. �Ŷ�\n");
        printf("2. ����\n");
        printf("3. �鿴�Ŷ�\n");
        printf("0. �˳�\n");
        printf("����������ѡ��: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("�����벡����: ");
            scanf("%d", &patientId);
            enqueue(&queue, patientId);
            break;
        case 2:
            if (!isEmpty(&queue)) {
                printf("���ﲡ����: %d\n", dequeue(&queue));
            }
            else {
                printf("����Ϊ�գ�û�в����ڵȴ����\n");
            }
            break;
        case 3:
            printf("��ǰ�ŶӲ��˵Ĳ�����: ");
            displayQueue(&queue);
            break;
        case 0:
            printf("�������˳���\n");
            break;
        default:
            printf("��Чѡ�����������롣\n");
            break;
        }

        printf("\n");
    } while (choice != 0);

    return 0;
}

