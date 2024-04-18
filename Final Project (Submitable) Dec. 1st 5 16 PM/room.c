#include "defs.h"
#include <string.h>

RoomType* createRoom(char* name){
    RoomType* room = calloc(1, sizeof(RoomType));
    strcpy(room->name, name);
    room->ghost = NULL;
    
    sem_init(&room->babyLock, 0 ,1);
    
    initEvidenceList(&room->evlist);
    initRoomList(&room->rooms);
    return room;
}

void initRoomList(RoomList *list){
    NodeTypeRoom* node = malloc(sizeof(NodeTypeRoom));
    list->head = node;
    list->head->next = list->tail;
    list->tail = NULL;
}

void connectRooms(RoomType* room1, RoomType* room2){
    addRoom(&room1->rooms, room2);//assume '&' before room stops complaining then
    addRoom(&room2->rooms, room1);
}

void addRoom(RoomList* roomList, RoomType* room){//add to next node of roomList
    NodeTypeRoom* newNode = malloc(sizeof(NodeTypeRoom));
    newNode->data = room;
    newNode->next = NULL;
    NodeTypeRoom* currNode = roomList->head; 
    while (currNode->next != NULL){
        currNode = currNode->next;
    }
    currNode->next = newNode;
    roomList->tail = newNode;
    roomList->size++;
    return;
}

void cleanupRoomListData(RoomList* list){
	NodeTypeRoom* cur = list->head->next;

	while(cur->next != NULL){

		cleanupEvidenceList(cur->data->evlist);
		cleanupRoomList(&cur->data->rooms);
		free(cur->data);
		cur = cur->next;
	}
	cleanupEvidenceList(cur->data->evlist);
    cleanupRoomList(&cur->data->rooms);
    free(cur->data);
}


void cleanupRoomList(RoomList* list){
    NodeTypeRoom* prev = list->head;
    NodeTypeRoom* cur = list->head->next;
    
	while(cur != NULL){
		free(prev);
		prev = cur;
		cur = cur->next;
	}
	free(prev);
}
