#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX_STR         64
#define MAX_RUNS        50
#define BOREDOM_MAX     100
#define C_TRUE          1
#define C_FALSE         0
#define HUNTER_WAIT     5000
#define GHOST_WAIT      600
#define NUM_HUNTERS     4
#define FEAR_MAX        10
#define LOGGING         C_TRUE


typedef enum EvidenceType EvidenceType;
typedef enum GhostClass GhostClass;

enum EvidenceType { EMF, TEMPERATURE, FINGERPRINTS, SOUND, EV_COUNT, EV_UNKNOWN };
enum GhostClass { POLTERGEIST, BANSHEE, BULLIES, PHANTOM, GHOST_COUNT, GH_UNKNOWN };
enum LoggerDetails { LOG_FEAR, LOG_BORED, LOG_EVIDENCE, LOG_SUFFICIENT, LOG_INSUFFICIENT, LOG_UNKNOWN };

typedef struct Room 	RoomType;
typedef struct house	HouseType;
typedef struct ghost	GhostType;
typedef struct hunter	HunterType;
typedef struct evlist	EvidenceList;
typedef struct NodeEv	NodeTypeEv;
typedef struct roomlist	RoomList;
typedef struct NodeRoom	NodeTypeRoom;

typedef struct evlist{
    NodeTypeEv* head;
    NodeTypeEv* tail;
    sem_t 	babyLock;
}EvidenceList;

typedef struct NodeEv{
    EvidenceType data;
    struct NodeEv* next;
}NodeTypeEv;

typedef struct roomlist{
    NodeTypeRoom* head;
    NodeTypeRoom* tail;
    int size;
}RoomList;

typedef struct NodeRoom{
    RoomType* data;
    struct NodeRoom* next;
}NodeTypeRoom;

typedef struct Room{
    char    name[MAX_STR];
    GhostType*  ghost;
    RoomList rooms;
    HunterType* hunters[NUM_HUNTERS];
    EvidenceList evlist;
    sem_t 	babyLock;
}RoomType;

typedef struct house{
    RoomList    rooms;
    HunterType* hunters[NUM_HUNTERS];
    EvidenceList evlist;
}HouseType;

typedef struct ghost{
    GhostClass GhostClass;
    RoomType*   curRoom;
    int     boredomTimer;
}GhostType;

typedef struct hunter{
    int     fearTimer;
    int     boredomTimer;
    EvidenceList* evlist; //if hunter changes it then all hunters get the change
    EvidenceType equip;
    RoomType* curRoom;
    char    name[MAX_STR];
}HunterType;


// Helper Utilies
int randInt(int,int);        // Pseudo-random number generator function
float randFloat(float, float);  // Pseudo-random float generator function
enum GhostClass randomGhost();  // Return a randomly selected a ghost type
void ghostToString(enum GhostClass, char*); // Convert a ghost type to a string, stored in output paremeter
void evidenceToString(enum EvidenceType, char*); // Convert an evidence type to a string, stored in output parameter

// Logging Utilities
void l_hunterInit(char* name, enum EvidenceType equipment);
void l_hunterMove(char* name, char* room);
void l_hunterReview(char* name, enum LoggerDetails reviewResult);
void l_hunterCollect(char* name, enum EvidenceType evidence, char* room);
void l_hunterExit(char* name, enum LoggerDetails reason);
void l_ghostInit(enum GhostClass type, char* room);
void l_ghostMove(char* room);
void l_ghostEvidence(enum EvidenceType evidence, char* room);
void l_ghostExit(enum LoggerDetails reason);

void initGhost(RoomType*, GhostType**);
void* startGhost(void*);
void GhostToString(GhostClass, char*);

void addEvidence(EvidenceList*, EvidenceType);
void removeEvidence(EvidenceList*, EvidenceType);
EvidenceType pickEvidence(GhostType*); //We may be able to just copy data into evNodes
void initEvidenceList(EvidenceList*);
EvidenceType numberToEquipment(int);
void cleanupEvidenceList(EvidenceList);
void EvidenceToString(EvidenceType, char*);

void initRoomList(RoomList*);
RoomType* createRoom(char*);
void addRoom(RoomList*, RoomType*);
void connectRooms(RoomType*, RoomType*);
void cleanupRoomListData(RoomList* list);
void cleanupRoomList(RoomList* list);

void initHunter(HunterType**, RoomType*, EvidenceList*,EvidenceType, char*);
void* startHunter(void*);
void cleanupHunter(HunterType*);
GhostClass determineGhost(EvidenceList*);

void initHouse(HouseType*);
void cleanHouse(HouseType);
void populateRooms(HouseType*);
