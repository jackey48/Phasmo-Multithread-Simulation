#include "defs.h"

int main()
{
    // Initialize the random number generator
    srand(time(NULL));
    
    // Create the house: You may change this, but it's here for demonstration purposes
    // Note: This code will not compile until you have implemented the house functions and structures
 
    HouseType* house = calloc(1, sizeof(HouseType));
    initHouse(house);
    populateRooms(house);
 
    NodeTypeRoom* cur = house->rooms.head->next;
    
    int num = randInt(1, house->rooms.size);
    for (int i = 0; i < num; i++){
        cur = cur->next;
    }
    GhostType* ghosty;

    initGhost(cur->data, &ghosty);

    HunterType* hunters[NUM_HUNTERS];

    char* name = malloc(sizeof(char) * MAX_STR);

    for (int i = 0; i < NUM_HUNTERS; i++){//make num_hunter later
        printf("Please enter Hunter (%i)'s name: ", i + 1);
        scanf("%s", name);
        hunters[i] = malloc(sizeof(HunterType));
        initHunter(&hunters[i], house->rooms.head->next->data, &house->evlist, numberToEquipment(i), name);
        house->rooms.head->next->data->hunters[i] = hunters[i];
        house->hunters[i] = hunters[i];
    }
    
    pthread_t ghost;    
    pthread_create(&ghost, NULL, startGhost, &ghosty);

    pthread_t hunter0;
    pthread_t hunter1;
    pthread_t hunter2;
    pthread_t hunter3;

    pthread_create(&hunter0, NULL, startHunter, &hunters[0]);
    pthread_create(&hunter1, NULL, startHunter, &hunters[1]);
    pthread_create(&hunter2, NULL, startHunter, &hunters[2]);
    pthread_create(&hunter3, NULL, startHunter, &hunters[3]);

    pthread_join(hunter0, NULL);
    pthread_join(hunter1, NULL);
    pthread_join(hunter2, NULL);
    pthread_join(hunter3, NULL);
    pthread_join(ghost, NULL);

	printf("====================================\n");
    printf("All done! Let's tally the results...\n");
    printf("====================================\n");
    int scaredCount = 0;
    int boredCount = 0;
    int evidenceCount = 0;
    
    for(int i = 0; i < NUM_HUNTERS; i++){
    	if(hunters[i]->fearTimer >= FEAR_MAX){
    		scaredCount++;
    		printf("Hunter %s got scared and left\n", hunters[i]->name);
    	}
    	if(hunters[i]->boredomTimer >= BOREDOM_MAX){
    		boredCount++;
    		printf("Hunter %s got bored and left\n", hunters[i]->name);
    	}
    }
    printf("------------------------------------\n");
    if((boredCount + scaredCount) < 4){
    	printf("It seems the ghost has been discovered!\n");
    	printf("The hunters have won the game!\n");
    }
    else {
    	printf("It seems the ghost has not been discovered.\n");
    	printf("The Ghost has won the game!\n");
    }
    
    NodeTypeEv* curEv = house->evlist.head;
    if(house->evlist.head->next != NULL){
    	printf("The hunters collected the following evidence:\n");
    	while (curEv->next != NULL){
        	curEv = curEv->next;
        	evidenceCount++;
        	EvidenceToString(curEv->data, name);
        	printf("%s\n", name);
    	}
    }
    if(evidenceCount == 3){
    	GhostToString(determineGhost(hunters[0]->evlist), name);
    	printf("Using the evidence they found, they correctly determined that the ghost is %s\n", name);
    }
         
    free(name);
    free(ghosty);
    cleanHouse(*house);
    for (int i = 0; i < NUM_HUNTERS; i++){
        free(hunters[i]);
    }
    free(house);

    return 0;

}

