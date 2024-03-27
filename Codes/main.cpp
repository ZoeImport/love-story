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
    queue->front = nodeToDelete->next;//直接指向下一个
    free(nodeToDelete);
    if (queue->front == NULL) {
        queue->rear = NULL;
        //当queue->front == NULL为空时
        //基于前面的已经指向了下一个位置，
        //可以确定此时已经遍历完了整个队列，所以说，
        //队尾也要更新为NULL
        //因为我们这是删除队列的操作，如果说，删除完
        //当前元素后下一个为空，那就说明当前队列为空
        //当然要将队头尾都重置！
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

    //return  没必要写，等它执行结束函数也行
}

int main() {
    Queue queue;
    initQueue(&queue);

    int choice, patientId;
    do {
        printf("1. 排队\n");
        printf("2. 就诊\n");
        printf("3. 查看排队\n");
        printf("0. 退出\n");
        printf("请输入您的选择: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("请输入病历号: ");
            scanf("%d", &patientId);
            enqueue(&queue, patientId);
            break;
        case 2:
            if (!isEmpty(&queue)) {
                printf("就诊病历号: %d\n", dequeue(&queue));
            }
            else {
                printf("队列为空，没有病人在等待就诊。\n");
            }
            break;
        case 3:
            printf("当前排队病人的病历号: ");
            displayQueue(&queue);
            break;
        case 0:
            printf("程序已退出。\n");
            break;
        default:
            printf("无效选择，请重新输入。\n");
            break;
        }

        printf("\n");
    } while (choice != 0);

    return 0;
}

