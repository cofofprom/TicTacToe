#ifndef TICTACTOE_PACKETQUEUE_H
#define TICTACTOE_PACKETQUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include "packet.h"

#define PACKETQUEUE_MAX_QUEUE_LENGTH 256

#define PACKETQUEUE_SUCCESS 0
#define PACKETQUEUE_QUEUE_FULL -1
#define PACKETQUEUE_NULL_PTR -2

typedef struct packet_queue_node_struct
{
    PACKET* packet;
    struct packet_queue_node_struct* nextNode;
    struct packet_queue_node_struct* prevNode;
}PACKET_QUEUE_NODE;

typedef struct packet_queue_struct
{
    PACKET_QUEUE_NODE* head;
    PACKET_QUEUE_NODE* tail;
    int length;
}PACKET_QUEUE;

///@brief Initializes new packet queue node from packet
///@param targetPacket Packet to use in this node
///@retval NULL On error
///@retval PACKET_QUEUE_NODE* On success
PACKET_QUEUE_NODE*  initPacketQueueNode(PACKET* targetPacket);

///@brief Frees packet queue node
///@details Frees targeted node, and updates pointers in previous and next nodes, if they are available
///@param freedNode node to be freed
void freePacketQueueNode(PACKET_QUEUE_NODE* freedNode);

///@brief Initializes new packet queue
///@retval NULL On error
///@retval PACKET_QUEUE* on success
PACKET_QUEUE* initPacketQueue();

///@brief Frees packet queue and its nodes
///@param freedQueue Queue to be freed
void freePacketQueue(PACKET_QUEUE* freedQueue);

///@brief Adds packet to targeted queue
///@param targetQueue Queue to add packet to
///@param targetPacket Packet to add to queue
///@retval PACKETQUEUE_SUCCESS On success
///@retval PACKETQUEUE_QUEUE_FULL If target queue is full
///@retval PACKETQUEUE_NULL_PTR NULL pointer pointer encountered
int enqueuePacket(PACKET_QUEUE* targetQueue, PACKET* targetPacket);

///@brief Removes packet from targeted queue
///@param targetQueue Queue to add packet to
///@param targetPacket Packet to be added to queue
///@retval NULL If target queue is empty or on error
///@retval PACKET* On success
PACKET* dequeuePacket(PACKET_QUEUE* targetQueue);

#endif //TICTACTOE_PACKETQUEUE_H
