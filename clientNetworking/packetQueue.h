#ifndef TICTACTOE_PACKETQUEUE_H
#define TICTACTOE_PACKETQUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include "packet.h"

#define PACKETQUEUE_MAX_QUEUE_LENGTH 256

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
///@param freedNode
void freePacketQueueNode(PACKET_QUEUE_NODE* freedNode);

#endif //TICTACTOE_PACKETQUEUE_H
