#include "packetQueue.h"

PACKET_QUEUE_NODE *initPacketQueueNode(PACKET *targetPacket) {
    if(targetPacket == NULL)
    {
        return NULL;
    }

    PACKET_QUEUE_NODE* newNode = calloc(1,sizeof(PACKET_QUEUE_NODE));
    if(newNode == NULL)
    {
        return NULL;
    }

    newNode->packet = makePacketCopy(targetPacket);
    newNode->nextNode = NULL;
    newNode->prevNode = NULL;

    return newNode;
}

void freePacketQueueNode(PACKET_QUEUE_NODE *freedNode) {
    if(freedNode == NULL)
    {
        return;
    }

    if(freedNode->packet != NULL)
    {
        free(freedNode->packet);
    }

    if(freedNode->nextNode != NULL)
    {
        freedNode->nextNode->prevNode = freedNode->prevNode;
    }

    if(freedNode->prevNode != NULL)
    {
        freedNode->prevNode->nextNode = freedNode->nextNode;
    }

    free(freedNode);

    return;
}

PACKET_QUEUE *initPacketQueue() {
    PACKET_QUEUE* newQueue = calloc(1, sizeof(PACKET_QUEUE));
    if(newQueue == NULL)
    {
        return NULL;
    }

    newQueue->length = 0;
    newQueue->head = NULL;
    newQueue->tail = NULL;

    return newQueue;
}

void freePacketQueue(PACKET_QUEUE *freedQueue) {
    if(freedQueue == NULL)
    {
        return;
    }

    if(freedQueue->length == 0)
    {
        free(freedQueue);
        return;
    }

    PACKET_QUEUE_NODE* headNode = freedQueue->head;

    while(headNode->prevNode != NULL)
    {
        freePacketQueueNode(headNode->prevNode);
    }

    freePacketQueueNode(headNode);

    free(freedQueue);

    return;
}

int enqueuePacket(PACKET_QUEUE *targetQueue, PACKET *targetPacket) {
    if(targetQueue == NULL || targetPacket == NULL)
    {
        return PACKETQUEUE_NULL_PTR;
    }

    if(targetQueue->length == PACKETQUEUE_MAX_QUEUE_LENGTH)
    {
        return PACKETQUEUE_QUEUE_FULL;
    }

    PACKET_QUEUE_NODE* newNode = initPacketQueueNode(targetPacket);

    if(targetQueue->length == 0)
    {
        targetQueue->head = newNode;
        targetQueue->tail = newNode;
    } else
    {
        targetQueue->tail->prevNode = newNode;
        newNode->nextNode = targetQueue->tail;
        targetQueue->tail = newNode;
    }

    targetQueue->length++;

    return PACKETQUEUE_SUCCESS;
}

PACKET *dequeuePacket(PACKET_QUEUE* targetQueue) {
    if(targetQueue == NULL || targetQueue->length == 0)
    {
        return NULL;
    }

    PACKET_QUEUE_NODE* takenNode = targetQueue->head;

    if(targetQueue->length == 1)
    {
        targetQueue->head == NULL;
        targetQueue->tail == NULL;
    } else
    {
        targetQueue->head = targetQueue->head->prevNode;
        targetQueue->head->nextNode->prevNode = NULL;
        targetQueue->head->nextNode = NULL;
    }

    targetQueue->length--;

    PACKET* newPacket = makePacketCopy(takenNode->packet);
    takenNode->nextNode = NULL;
    takenNode->prevNode = NULL;

    freePacketQueueNode(takenNode);

    return newPacket;
}
