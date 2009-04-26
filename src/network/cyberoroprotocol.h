#define ORO_CODETABLE		0x00fa
#define ORO_ROOMLIST		0xc661
#define ORO_PLAYERLIST		0xd061
#define ORO_GAMELIST		0xda61
#define ORO_BROADCASTLIST	0xf861

#define ORO_PLAYERROOMJOIN	0x0e56
#define ORO_CREATEROOM		0xca5d

#define ORO_MOVE		0xdcaf
#define ORO_BROADCASTMOVE	0x42f4
#define ORO_UNDO		0xe6af
#define ORO_DECLINEUNDO		0xf0af
#define ORO_ACCEPTUNDO		0xebaf
#define ORO_MOVELIST		0xd7af
#define ORO_BROADCASTMOVELIST	0x4cf4

#define ORO_ENTERSCORING	0xc9b3
#define ORO_REMOVESTONES	0xe2b3
#define ORO_STONESDONE		0xf1b3
#define ORO_RESIGN		0xb0b3
#define ORO_TIMELOSS		0xf6b3
#define ORO_ADJOURNREQUEST	0xb5b3
#define ORO_ADJOURNDECLINE	0xbfb3
#define ORO_ADJOURN		0xbab3

#define ORO_SETPHRASECHAT	0x55c3
#define ORO_LOBBYCHAT		0x0a5a
#define ORO_ROOMCHAT		0xec59
#define ORO_PERSONALCHAT	0xe259
#define ORO_SERVERANNOUNCE	0xf659

#define ORO_MATCHOFFER		0xc8af

/* We need a subclass of a packet structure
 * with a send method that does necessary
 * encoding and a different subclass for
 * each message type so we can just fill
 * it out and make the code clean. 
 * doublecheck sizeof() stuff */