/* 
 * file: bridge.h
 * ==============
 *
 * The bridge.h file defines the interface for a set of functions that 
 * simulates the operation of the bridge according to the spanning tree
 * algorithm   
 * 
 */

#ifndef _bridge_h
#define _bridge_h

#define MAXNUMBEROFPORTSINABRIDGE 1024 /* Max number of ports in a bridge */
#define MAXNUMBEROFFDBENTRIES 1024     /* Max number of FDB entries */

#define ALLBRIDGES   -100   /* multicast/broadcast addresses are negative */
#define BROADCAST -1        /* will be used as destination address in */
                            /* broadcast frames */
#define BRIDGEPROTOCOL 66   /* to be used for DSAP, SSAP values in BPDUs */
#define IP           1      /* will be used as DSAP, SSAP in data packets
			       sent by stations */

/* Number of fields in the data part of a MAC frame */
#define FIELDS 250

/*
 * Type: PortStateType
 * ===================
 *
 * This enumeration distinguishes the possible states a port can have
 *
 */

typedef enum {Forwarding, Blocking} PortStateType;

/* 
 * Type: MACFrame
 * ==============
 *
 * Format of the frame in which all BPDUs and data messages will be
 * encapsulated. The data array should be used as specified by the
 * function GetConfigBPDUatPort()
 *
 */
typedef struct
{
  int dstAddress;		/* MAC address of destination */
  int srcAddress;		/* MAC addess of source */
  int length;			/* length of frame in bytes,
				   for BPDUs should be set to 35 */
  int DSAP;
  int SSAP;			/* protocol number, set DSAP and SSAP
				   to the BRIDGEPROTOCOL value in
				   BPDUs */
  int data[FIELDS];		/* This array contains the data in the
				   frame. You are free to organize the
				   BPDU data in this array the way you
				   want, except as noted for the
				   GetConfigBPDUatPort() function
				   below */
} MACFrame;

typedef struct bpdu
{
  int root;
  int cost;
  int transmittingbridgeid;
  int transmittingportid;
  int rcvedid;
  /* data */
} bpdu;
/* 
 * Type MACQueueElement
 * ====================
 *
 * Format of an element in the receive queue of the bridge. Refer to
 * the explaination of the Execute subroutine for an explaination of
 * the receive queue.
 *
 */
typedef struct MACqueue_element
{
  MACFrame *frame;                  /* pointer to the frame */ 
  struct MACqueue_element *next;    /* pointer to the next
				       MACQueueElement */
  int MACaddress;       
              /* identifies port where frame is
				       received */
                                    /* MACaddress refers to the
				       globalPortID */
} MACQueueElement;

/* 
 * Type Bridge
 * ===========
 *
 * Format of a bridge structure. This format is rudimentary. You will
 * need to add a bunch of other stuff to it.
 *
 */
 typedef struct portinfo
{
  int port;
  int mac;
  PortStateType state;

  /* data */
} portinfo;



 typedef struct entry
{
  int stationnumber;
  int portno;
  /* data */
} entry;

typedef struct
{
  int bridgeID;  /* bridge ID */
  int numPorts;  /* number or ports in the bridge */
  int rp;
  bpdu best;
  portinfo pinfo[1000];
  entry fwdtable[1000];
  int noofentries;
  /* Add more elements here as needed.... */
} Bridge;


/* 
 * Function: AllocateBridge 
 * ======================== 
 * 
 * This function is called by the simulator to allocate the memory for
 * your bridge structure. This has already been implemented for
 * you. You must not make any changes or calls to this function.
 *
 */
void *AllocateBridge();

/* 
 * Function: InitializeBridge
 * ==========================
 *
 * This function initializes the bridge and sets its parameters.
 * *bridge is a pointer to the bridge structure to be modified.  The
 * only other argument requiring explaination is int
 * *MACaddresses. This is an integer array containing the MAC
 * addresses of the bridge ports. It has been allocated enough memory
 * for as many elements as the number of ports of the bridge. So use
 * it accordingly and do not cross these bounds. Element 0 is the MAC
 * address of port having port ID 0 of the bridge. Element 1 is the
 * MAC address of port with port ID 1 of the bridge and so on.
 *
 * You should IGNORE the three parameters ignore1, ignore2 and
 * ignore3. These are not used in this assignment. They are needed for
 * compatibility with the simulator interface.
 *
 */
void InitializeBridge(Bridge *bridge, int bridgeID, int numPorts, int
		      *MACaddresses, int ignore1, int ignore2, int ignore3);

/* 
 * Function: Execute
 * =================
 *
 * The simulator program implements a round robin mechanism and calls
 * the execute routine of each of the bridges.
 *
 * Bridge *bridge: The bridge to be executed.
 *
 * MACQueueElement *receiveQueue: Queue containing the Configuration
 * BPDUs and dataframes received by the bridge since it last
 * executed. The MACQueueElements need to be processed during the
 * Execute funtion, along with any other activity to be performed
 * (i.e. sending of BPDUs, forwarding data frames). The frames in the
 * MACQueueElement linked list are in chronological order according to
 * when they were received.
 *
 * unsigned long step: this is NOT USED for this assignment. In
 * practice, bridges keep timers for different purposes and this field
 * provides a time step increment each time the Excecute function is
 * run. It indicates the amount of time elapsed since last
 * execution. Since this assignment does not include dynamic changes
 * in the topology that require timers to be kept, you should IGNORE
 * this field.
 *
 * Return from this subroutine when you are done processing all the
 * frames in the queue and performing any other activity that was
 * required. Return 0 upon successful execution 1 otherwise. If the
 * bridge needs to send frames as part of its execution, use the
 * following function call implemented in the simulator: void
 * SendMACFrame(int MACaddress, MACFrame frame); The function
 * SendMACFrame takes on the MAC address of the sending port, and the
 * frame to be sent. The MAC address is a simulation-wide unique
 * identifier for each port which is specified by the simulation
 * configuration file
 * 
 * You do not have to worry about freeing the memory for the receive
 * queue. The simulator performs this task.
 *
 */
int Execute(Bridge *bridge, MACQueueElement *receiveQueue, unsigned long step);

/* 
 * Function: GetFDBEntry
 * =====================
 *
 * This function is called by the simulator program to output the
 * filtering databases of each bridge in the topology after it has
 * simulated the transmission of some data frames.
 *
 * Bridge *bridge: The bridge structure whose filtering database entry
 * is to be returned.  dstAddress: The destination address in the
 * filtering database entry asked for. This could be a unicast address
 * or a multicast group address.
 *
 * The function should return a pointer to the first element in an
 * integer array containing as many elements as the number of ports in
 * the bridge. Each entry should be 0 or 1. If traffic destined to the
 * address dstAddress will be forwarded on a port, the corresponding
 * entry should be a 1, otherwise it should be a 0. Thus, the array
 * returned is sort of a port map.
 *
 * If there is no entry in your filtering database for the destination
 * entry specified, return an array of all zeros.
 *
 */
int *GetFDBEntry (Bridge *bridge, int MACaddress);

/* 
 * Function: GetPortState
 * ======================
 *
 * This function is called by the simulator program to display the
 * states of the bridges at any time during the simulation.
 *
 * Bridge *bridge: The bridge whose port state is requested.
 * MACaddress: The MAC address of the port whose state is requested.
 *
 * Return a value of type PortStateType defined above ({Forwarding OR
 * Blocking})
 *
 */
PortStateType GetPortState (Bridge *bridge, int MACaddress);

/* 
 * Function: GetRootPort
 * =====================
 *
 * This function is called by the simulator program for the purpose of
 * displaying port states of bridges. The function should return the
 * MAC address of the root port of the bridge. If the bridge is the
 * root bridge of the topology and there is no root port, return
 * -1. All other ports of the bridge (other than this port) for which
 * the port state was reported as Forwarding by a call to GetPortState
 * will be assumed to be designated by the simulator program as they
 * should be.
 *
 */
int GetRootPort (Bridge *bridge);

/* 
 * Function: GetConfigBPDUatPort
 * =============================
 *
 * This function is called by the simulator program for the purpose of
 * displaying the states of the bridges at any time during the
 * simluation. In particular, it requests the BPDU that the bridge
 * under consideration would send out at the port specified. The
 * simulator will always call this function for a port that is in a
 * designated state, so do not be bothered about being asked to give a
 * BPDU at a root or a blocked port.
 *
 * Bridge *bridge: The bridge whose BPDU is being requested int
 * MACaddress: MAC address of the port whose BPDU is being requested
 * The simulator only uses the first four data fields of the frame for
 * display. Make sure that these first four fields contain the
 * following information:
 * data[0]: root bridge ID; 
 * data[1]: cost to root; 
 * data[2]: transmitting bridge ID;
 * data[3]: transmitting port ID;
 *
 */
MACFrame *GetConfigBPDUatPort (Bridge *bridge, int MACaddress);

/* 
 * Function: BridgeCleanup
 * =======================
 *
 * At the end of simulations, the simulator calls this function.  You
 * don't need to worry much about cleannig up until this function is
 * called. This is your final chance to get rid of those heap memory
 * blocks that you allocated. The simulator will call this function
 * after it prints the FDB entries and bridge States .
 *
 */
void BridgeCleanup(void);

#endif

/* 
 * ADDITIONAL NOTES: In all the above functions, you can be assured
 * that the MAC address passed by the simulator program is one which
 * lies within the bridge being called upon.
 *
 */
