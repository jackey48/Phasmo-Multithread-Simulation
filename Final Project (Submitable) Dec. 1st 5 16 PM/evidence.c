#include "defs.h"

// EvidenceType pickEvidence(GhostType* ghost){
//     int num = randInt(1, EV_COUNT - 1);
//     if (ghost->GhostClass == POLTERGEIST){
//         if (num == 1){
//             return EMF;
//         }
//         else if (num == 2){
//             return TEMPERATURE;
//         }
//         else{
//             return FINGERPRINTS;
//         }
//     }
//     else if (ghost->GhostClass == BANSHEE){
//         if (num == 1){
//             return EMF;
//         }
//         else if (num == 2){
//             return TEMPERATURE;
//         }
//         else{
//             return SOUND;  
//         }
//     }
//     else if (ghost->GhostClass == BULLIES){
//         if (num == 1){
//             return EMF;
//         }
//         else if (num == 2){
//             return FINGERPRINTS;
//         }
//         else{
//             return SOUND;
//         }
//     }
//     else{
//         if (num == 1){
//             return TEMPERATURE;
//         }
//         else if (num == 2){
//             return FINGERPRINTS;
//         }
//         else{
//             return SOUND;
//         }
//     }   
// }

EvidenceType pickEvidence(GhostType* ghost){
    int num = randInt(1, EV_COUNT);
    switch (ghost->GhostClass){
    case POLTERGEIST: 
        switch (num){
        case 1:
            return EMF;
            break;
        
        case 2:
            return TEMPERATURE;
            break;

        case 3:
            return FINGERPRINTS;
            break;
        default:
            return EV_UNKNOWN;
            break;
        }
        break;

    case BANSHEE:
        switch (num){
        case 1:
            return EMF;
            break;
        
        case 2:
            return TEMPERATURE;
            break;

        case 3:
            return SOUND;
            break;
        default:
            return EV_UNKNOWN;
            break;
        }
        break;
    
    case BULLIES:
        switch (num){
        case 1:
            return EMF;
            break;
        
        case 2:
            return FINGERPRINTS;
            break;

        case 3:
            return SOUND;
            break;
        default:
            return EV_UNKNOWN;
            break;
        }
        break;

    case PHANTOM:
        switch (num){
        case 1:
            return TEMPERATURE;
            break;
        
        case 2:
            return FINGERPRINTS;
            break;

        case 3:
            return SOUND;
            break;
        default:
            return EV_UNKNOWN;
            break;
        }
        break;

    default:
    	printf("ERROR: Evidence not Found");
        break;
    }

}

void initEvidenceList(EvidenceList* ev){
    NodeTypeEv* node = malloc(sizeof(NodeTypeEv));
    ev->head = node;
    node->next = ev->tail;
    ev->tail = NULL;
    sem_init(&ev->babyLock, 0 ,1);

}

void addEvidence(EvidenceList* ev, EvidenceType evidence){//add to next node of ev
    NodeTypeEv* currNode = ev->head; 
    while (currNode->next != NULL){
        currNode = currNode->next;
        if(currNode->data == evidence){
			return;
		}
    }
    NodeTypeEv* newNode = malloc(sizeof(NodeTypeEv));
    newNode->data = evidence;
    newNode->next = NULL;
    currNode->next = newNode;
    ev->tail = newNode;     
}

void removeEvidence(EvidenceList* ev, EvidenceType evidence){
    NodeTypeEv* curNode = ev->head;
    NodeTypeEv* prevNode = NULL;

    while (curNode->next != NULL){
        prevNode = curNode;
        curNode = curNode->next;
        if(curNode->data == evidence){
        	prevNode->next = curNode->next;
            free(curNode);
            return;
        }

    }

    ev->tail = prevNode;
}

EvidenceType numberToEquipment(int n){
    switch (n){
    case 0:
        return EMF;
        break;
    case 1:
        return TEMPERATURE;
        break;
    case 2:
        return FINGERPRINTS;
        break;
    case 3:
        return SOUND;
        break;
    default:
    return EV_UNKNOWN;
        break;
    }
}

void EvidenceToString(EvidenceType evidence, char* name){
	switch (evidence){
    case EMF:
        strcpy(name, "EMF");
        break;
    case TEMPERATURE:
        strcpy(name, "TEMPERATURE");
        break;
    case FINGERPRINTS:
        strcpy(name, "FINGERPRINTS");
        break;
    case SOUND:
        strcpy(name, "SOUND");
        break;
    default:
    	printf("Unable to find evidence\n");
        break;
    }
}

void cleanupEvidenceList(EvidenceList evlist){
	NodeTypeEv* prev = evlist.head;
	NodeTypeEv* cur = evlist.head->next;
    if (cur == NULL){
        free(prev);
        return;
    }
    
    
	while(cur != NULL){//what if no evidence was placed in the room
		free(prev);
		prev = cur;
		cur = cur->next;
	}

    free(prev);
    
}
