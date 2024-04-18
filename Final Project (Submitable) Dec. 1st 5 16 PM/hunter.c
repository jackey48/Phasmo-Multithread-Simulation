#include "defs.h"

void initHunter(HunterType** hunter, RoomType* van, EvidenceList* evList, EvidenceType ev, char* name){
    (*hunter)->boredomTimer = 0;
    (*hunter)->fearTimer = 0;
    strcpy((*hunter)->name, name);

    (*hunter)->curRoom = van;
    (*hunter)->evlist = evList;
    (*hunter)->equip = ev;
    l_hunterInit(name,ev);
}


void *startHunter(void* hunterly){ //Give them a player number somehow
    int choice = 0, num = 0;			//Variables used to store random integers
    NodeTypeRoom* cur;					//Temporary Room Node structure for iterating room lists
    NodeTypeEv* curEv;					//Temporary Evidence Node structure for iterating evidence lists
    HunterType** hunter = hunterly;		//New home for thread argument
    RoomType* tempRoom;					//Temporory room address for locksorting

    while ((*hunter)->boredomTimer < BOREDOM_MAX || (*hunter)->fearTimer < FEAR_MAX){
        int count = 1;

        //checks if ghost is in room and does correct operations
        if((*hunter)->curRoom->ghost != NULL){
            (*hunter)->boredomTimer = 0;
            (*hunter)->fearTimer++;
            
            if((*hunter)->fearTimer == FEAR_MAX){
                
                l_hunterExit((*hunter)->name, LOG_FEAR);
                break;
            }

        }   
        else{
            (*hunter)->boredomTimer++;
            if((*hunter)->boredomTimer == BOREDOM_MAX){
                l_hunterExit((*hunter)->name, LOG_BORED);
                break;
            }
        }

        choice = randInt(1, 5);
        if (choice == 1){//move to a random room
        	tempRoom = (*hunter)->curRoom;
        	
            sem_wait(&(*hunter)->curRoom->babyLock);				//Should be unaffected by other waits
            
            cur = (*hunter)->curRoom->rooms.head->next;
            num = randInt(0, (*hunter)->curRoom->rooms.size);
            for (int i = 0; i < num; i++){
                cur = cur->next;
            }

            sem_post(&(*hunter)->curRoom->babyLock);				//Prevents read data races
            
            if(&cur->data > &(*hunter)->curRoom){				//Lock sort
            	sem_wait(&cur->data->babyLock);
            	sem_wait(&(*hunter)->curRoom->babyLock);
            }
            else {
            	sem_wait(&(*hunter)->curRoom->babyLock);
            	sem_wait(&cur->data->babyLock);
            }
            
            for (int i = 0; i < NUM_HUNTERS; i++){
                if ((*hunter)->curRoom->hunters[i] == (*hunter)){
                    (*hunter)->curRoom->hunters[i] = NULL;
                }
                
            }
            
            (*hunter)->curRoom = cur->data;

            for (int i = 0; i < NUM_HUNTERS; i++){
                if (cur->data->hunters[i] == NULL){
                    cur->data->hunters[i] = *hunter;
                }
            }
            
            sem_post(&tempRoom->babyLock);
            sem_post(&(*hunter)->curRoom->babyLock);

            l_hunterMove((*hunter)->name, cur->data->name);
        

        }
        else if (choice == 2){//collect evidence (remove evidence that hunter can collect)
            //needs to find if it's EvidenceType exists in rooms evidence list
            //remove it and free() the data || Placed into remove Evidence now
            //add it the EvidenceList in hunter || Dereference and add it to the hunters list should be ok

            sem_wait(&(*hunter)->curRoom->evlist.babyLock);
            

            curEv = (*hunter)->curRoom->evlist.head;
            while(curEv->next != NULL){
                curEv = curEv->next;
                if(curEv->data == (*hunter)->equip){
                    addEvidence((*hunter)->evlist, (*hunter)->equip);
                    removeEvidence(&(*hunter)->curRoom->evlist, (*hunter)->equip);
                    l_hunterCollect((*hunter)->name, (*hunter)->equip, (*hunter)->curRoom->name);

                    break;
                }
            }
        
            sem_post(&(*hunter)->curRoom->evlist.babyLock);

        }
        else if (choice == 3){//review if all evidence is collected only for 3/4 ghost
            sem_wait(&(*hunter)->evlist->babyLock);
            curEv = (*hunter)->evlist->head;
            while (curEv->next != NULL){
                curEv = curEv->next;
                count++;
            }
            sem_post(&(*hunter)->evlist->babyLock);

            //printf("%i", count);
            if (count == EV_COUNT){
                l_hunterReview((*hunter)->name, LOG_SUFFICIENT);
                l_hunterExit((*hunter)->name, LOG_EVIDENCE);
                break;
            }
            l_hunterReview((*hunter)->name, LOG_INSUFFICIENT);

        }
        else if (choice == 4){//remove once hunter is done
       		//printf("Pass\n");         
        }
        
        usleep(HUNTER_WAIT);
    }

    for (int i = 0; i < NUM_HUNTERS; i++){
        if ((*hunter)->curRoom->hunters[i] == (*hunter)){
            (*hunter)->curRoom->hunters[i] = NULL;
        }
        
    }

    return(0);
}

GhostClass determineGhost(EvidenceList* list){
	NodeTypeEv* curEv = list->head;
	int total = 0;
    while (curEv->next != NULL){
        	curEv = curEv->next;
        	total += curEv->data;
    }
    switch(total){
    	case 3:
    		return POLTERGEIST;
    		break;
    	case 4:
    		return BANSHEE;
    		break;
    	case 5:
    		return BULLIES;
    		break;
    	case 6:
    		return PHANTOM;
    		break;
    	default:
    		return GH_UNKNOWN;
    }
}
