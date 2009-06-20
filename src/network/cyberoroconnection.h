#include <QtNetwork>
#include "networkconnection.h"
#include "messages.h"
#include "newline_pipe.h"
#include <vector>

class BoardDispatch;
class GameDialogDispatch;
class TalkDispatch;
class QuickConnection;

class CyberOroConnection : public NetworkConnection
{
	public:
		CyberOroConnection(const QString & user, const QString & pass);
		~CyberOroConnection();
		virtual void sendText(QString text);
		virtual void sendText(const char * text);
		virtual void sendDisconnect(void);
		virtual void sendMsg(unsigned int game_id, QString text);
		virtual void sendMsg(PlayerListing & player, QString text);
		virtual void sendToggle(const QString & param, bool val);
		virtual void sendObserve(const GameListing & game);
		virtual void sendObserveOutside(const GameListing & game);
		virtual void sendStatsRequest(const PlayerListing &) {};
		virtual void sendPlayersRequest(void) {};
		virtual void sendGamesRequest(void) {};
		virtual void sendMatchInvite(const PlayerListing & opponent);
		virtual void adjournGame(const GameListing & game);
		virtual void sendMove(unsigned int game_id, class MoveRecord * move);
		virtual void sendRequestCount(unsigned int game_id);
		virtual void sendRequestMatchMode(unsigned int game_id);
		virtual void sendAcceptRequestMatchMode(unsigned int game_id);
		virtual void sendDeclineRequestMatchMode(unsigned int game_id);
		virtual void sendTimeLoss(unsigned int game_id);
		virtual void sendMatchRequest(class MatchRequest * mr);
		virtual void sendRematchRequest(void);
		virtual void sendRematchAccept(void);
		virtual void declineMatchOffer(const PlayerListing & opponent);
		virtual void cancelMatchOffer(const PlayerListing & opponent);
		virtual void acceptMatchOffer(const PlayerListing & opponent, class MatchRequest * mr);
		virtual void sendAdjournRequest(void);
		virtual void sendAdjourn(void);
		virtual void sendRefuseAdjourn(void);
		virtual void handlePendingData(newline_pipe <unsigned char> * p);
		virtual bool isReady(void);
		virtual void onReady(void);
		virtual void changeServer(void);
		virtual void createRoom(void);
		virtual void sendCreateRoom(class RoomCreate * room);
		virtual void sendJoinRoom(const RoomListing & room, const char * password = 0);
		
		virtual char * sendRequestAccountInfo(int * size, void * p);
		virtual void handleAccountInfoMsg(int size, char * msg);
		
		virtual void addFriend(PlayerListing & player);
		virtual void removeFriend(PlayerListing & player);
		virtual char * sendAddFriend(int * size, void * p);
		virtual void recvFriendResponse(int size, char * msg);
		virtual char * sendRemoveFriend(int * size, void * p);
		//virtual void addFan(PlayerListing & player);
		//virtual void removeFan(PlayerListing & player);
		virtual void addBlock(PlayerListing & player);
		virtual void removeBlock(PlayerListing & player);
		virtual char * sendAddBlock(int * size, void * p);
		virtual char * sendRemoveBlock(int * size, void * p);
		
		BoardDispatch * getBoardFromAttrib(QString black_player, unsigned int black_captures, float black_komi, QString white_player, unsigned int white_captures, float white_komi);
		BoardDispatch * getBoardFromOurOpponent(QString opponent);
		virtual const PlayerListing & getOurListing(void);
		virtual unsigned short getRoomNumber(void) { return room_were_in; };
		
		virtual void closeBoardDispatch(unsigned int game_id);
		
		virtual int gd_verifyBoardSize(int v);
		virtual QTime gd_checkMainTime(TimeSystem s, const QTime & t);
		virtual QTime gd_checkPeriodTime(TimeSystem s, const QTime & t);
		virtual unsigned int gd_checkPeriods(TimeSystem s, unsigned int p);
		
		virtual unsigned int rankToScore(QString rank);
		virtual unsigned long getGameDialogFlags(void);	
		virtual bool playerTrackingByID(void) { return true; };
		virtual bool supportsMultipleUndo(void) { return true; };
		virtual bool supportsRequestMatchMode(void) { return true; };
		virtual bool supportsRequestAdjourn(void) { return true; };
		virtual bool supportsRequestCount(void) { return true; };
		virtual bool supportsObserveOutside(void) { return true; };
		virtual bool supportsServerChange(void) { return true; };
		virtual bool supportsRematch(void) { return true; };
		virtual bool supportsFriendList(void) { return true; };
		virtual bool supportsBlockList(void) { return true; };
		virtual unsigned long getPlayerListColumns(void) { return PL_NOMATCHPREFS; };
		virtual bool supportsCreateRoom(void) { return true; };
		virtual unsigned long getRoomStructureFlags(void) { return (RS_NOROOMLIST | RS_ONEROOMATATIME | RS_ONEGAMEPERROOM); };
		virtual void timerEvent(QTimerEvent * event);
	private:
		void handleServerList(unsigned char * msg);
		int reconnectToServer(void);
		void sendPersonalChat(const PlayerListing & player, const char * text);
		void sendRoomChat(const char * text);
		friend class SetPhrasePalette;
		void requestAccountInfo(void);
		void sendLogin(void);
		void sendSetChatMsg(unsigned short phrase_id);
		void sendObserveAfterJoining(const GameListing & game);
		void sendFinishObserving(const GameListing & game);
		void sendLeave(const GameListing &);
		void sendGameUpdate(unsigned short game_code);
		void sendKeepAlive(const GameListing & game);
		void sendRequestKeepAlive(const GameListing & game);
		enum FriendsBlocksMsgType { fbm_addFriend, fbm_removeFriend, fbm_addBlock, fbm_removeBlock };
		char * sendFriendsBlocksMsg(int * size, void * p, enum FriendsBlocksMsgType f);
		void sendMatchInvite(const PlayerListing & player, bool accepting);
		void sendDeclineMatchInvite(const PlayerListing & player);
		void sendDeclineMatchOffer(const PlayerListing & player);
		void sendMatchOfferPending(const PlayerListing & player);
		void sendMatchOfferCancel(const PlayerListing & player);
		void sendMatchOffer(const MatchRequest & mr, bool counteroffer = false);
		void sendInvitationSettings(bool invite);
		void sendUndo(unsigned int game_code, const MoveRecord * move);
		void sendDeclineUndo(unsigned int game_code, const MoveRecord * move);
		void sendAcceptUndo(unsigned int game_code, const MoveRecord * move);
		void sendRemoveStones(unsigned int game_code, const MoveRecord * move);
		void sendEnterScoring(unsigned int game_code);
		void sendDoneScoring(unsigned int game_id, unsigned short opp_id);
		void sendResign(unsigned int game_code);
		void sendMatchResult(unsigned short game_code);
		void sendNigiri(unsigned short game_code, bool odd);
		void sendDisconnectMsg(void);
		void sendChallengeResponse(void);
		void encode(unsigned char * h, unsigned int cycle_size);
		void handlePassword(QString msg);
		void handleMessage(QString msg);
		void handleMessage(unsigned char * msg, unsigned int size);
		void handleCodeTable(unsigned char * msg, unsigned int size);
		void handleConnected(unsigned char * msg, unsigned int size);
		void handlePlayerList(unsigned char * msg, unsigned int size);
		QString rating_pointsToRank(unsigned int rp);
		QString getCountryFromCode(unsigned char code);
		void handleRoomList(unsigned char * msg, unsigned int size);
		void handleBroadcastGamesList(unsigned char * msg, unsigned int size);
		int getPhase(unsigned char byte);
		QString getRoomTag(unsigned char byte);
		void handleGamesList(unsigned char * msg, unsigned int size);
		void handlePlayerConnect(unsigned char * msg, unsigned int size);
		void handlePlayerRoomJoin(unsigned char * msg, unsigned int size);
		void removeObserverFromGameListing(const PlayerListing * p);
		void handlePlayerDisconnect2(unsigned char * msg, unsigned int size);
		void handleServerAnnouncement(unsigned char * msg, unsigned int size);
		void handleServerRoomChat(unsigned char * msg, unsigned int size);
		void handlePersonalChat(unsigned char * msg, unsigned int size);
		void handleRoomChat(unsigned char * msg, unsigned int size);
		void handleSetPhraseChatMsg(unsigned char * msg, unsigned int size);
		void handleNewGame(unsigned char * msg, unsigned int size);
		void handleGameEnded(unsigned char * msg, unsigned int size);
		void handleNewRoom(unsigned char * msg, unsigned int size);
		void handleGameMsg(unsigned char * msg, unsigned int size);
		void handleBettingMatchStart(unsigned char * msg, unsigned int size);
		void handleBettingMatchResult(unsigned char * msg, unsigned int size);
		void handleRequestCount(unsigned char * msg, unsigned int size);
		void handleRequestMatchMode(unsigned char * msg, unsigned int size);
		void handleTimeLoss(unsigned char * msg, unsigned int size);
		void handleMove(unsigned char * msg, unsigned int size);
		void handleUndo(unsigned char * msg, unsigned int size);
		void handleDeclineUndo(unsigned char * msg, unsigned int size);
		void handleAcceptUndo(unsigned char * msg, unsigned int size);
		void handleMoveList(unsigned char * msg, unsigned int size);
		void handleMoveList2(unsigned char * msg, unsigned int size);
		void handleObserverList(unsigned char * msg, unsigned int size);
		void handleMatchOpened(unsigned char * msg, unsigned int size);
		void handleResumeMatch(unsigned char * msg, unsigned int size);
		void handleObserveAfterJoining(unsigned char * msg, unsigned int size);
		void handleInvitationSettings(unsigned char * msg, unsigned int size);
		QString getStatusFromCode(unsigned char code, QString rank);
		int compareRanks(QString rankA, QString rankB);
		void handleCreateRoom(unsigned char * msg, unsigned int size);
		void handleAdjournRequest(unsigned char * msg, unsigned int size);
		void handleAdjournDecline(unsigned char * msg, unsigned int size);
		void handleAdjourn(unsigned char * msg, unsigned int size);
		void handleMatchInvite(unsigned char * msg, unsigned int size);
		void handleMatchOffer(unsigned char * msg, unsigned int size);
		void handleRematchRequest(unsigned char * msg, unsigned int size);
		void handleRematchAccept(unsigned char * msg, unsigned int size);
		void handleMatchDecline(unsigned char * msg, unsigned int size);
		void handleAcceptMatchInvite(unsigned char * msg, unsigned int size);
		void handleDeclineMatchInvite(unsigned char * msg, unsigned int size);
		void handleMatchRoomOpen(unsigned char * msg, unsigned int size);
		void handleResign(unsigned char * msg, unsigned int size);
		void handleEnterScoring(unsigned char * msg, unsigned int size);
		void handleRemoveStones(unsigned char * msg, unsigned int size);
		void handleStonesDone(unsigned char * msg, unsigned int size);
		void handleScore(unsigned char * msg, unsigned int size);
		void handleGamePhaseUpdate(unsigned char * msg, unsigned int size);
		void handleMsg3(unsigned char * msg, unsigned int size);
		void handleFriends(unsigned char * msg, unsigned int size);
		void handleMsg2(unsigned char * msg, unsigned int size);
		void handleNigiri(unsigned char * msg, unsigned int size);
		class GameData * getGameData(unsigned int game_id);
		int time_to_seconds(const QString & time);
		void killActiveMatchTimers(void);
		void startMatchTimers(bool ourTurn);
		void setRoomNumber(unsigned short number);
		void setAttachedGame(PlayerListing * const player, unsigned short game_id);
		
		/* We should really just have the player listing for ourself FIXME */
		unsigned short our_player_id;
		unsigned char our_special_byte;
		QTextCodec * textCodec;
		QTextCodec * serverCodec;
		class SetPhrasePalette * setphrasepalette;
		/* FIXME */
		std::vector <class ServerItem *> serverList;
		int current_server_index;
		unsigned char * challenge_response;
		unsigned char * codetable;
		unsigned int codetable_IV, codetable_IV2;
		
		QuickConnection * metaserverQC;
		
		/* Since 0a7d comes before 1a81 and one has the number for human
		 * consumption and the other the game_code necessary for joinging,
		 * etc., we'll just combine them both at 1a81, checking names */
		std::vector <GameListing *> rooms_without_owners;
		std::map <unsigned short, unsigned short> game_code_to_number;
		unsigned short playerlist_skipnumber;
		unsigned short playerlist_received;
		unsigned short playerlist_roomnumber;
		unsigned short playerlist_observernumber;
		unsigned short roomlist_observers;
		std::vector <PlayerListing *> playerlist_inorder;
		
		unsigned short room_were_in;
		unsigned short connecting_to_game_number;	//awkward FIXME
		unsigned short playing_game_number;		//awkward
		bool we_send_nigiri;
		
		/* We can only play one game at a time so:...*/
		unsigned short our_game_being_played;	//redundant with playing_game_number??
		int matchKeepAliveTimerID, matchRequestKeepAliveTimerID;
		//unsigned short opp_requests_undo_move_number;
		unsigned short done_response;
		std::vector <MoveRecord> deadStonesList;
		bool receivedOppDone;
		
	private slots:
		virtual void OnConnected();
};
