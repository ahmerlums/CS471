/* 
 * File: bridge.c 
 * ==============
 *
 * This is a rough skeleton of bridge.c which only lists the subroutines
 * you HAVE to implement as the simulator makes use of these
 * subrouines. Complete this file by filling in these subroutines and
 * writing your own supplemental subroutines that you might need
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "bridge.h"

void *AllocateBridge()
{ 
	/* Do not make any changes to this subroutine */

	return ((void*)malloc(sizeof(Bridge)));
}

void InitializeBridge (Bridge *bridge, int bridgeID, int numPorts, int
		    *MACaddresses, int ignore1, int ignore2, int ignore3)
{
	bridge->bridgeID=bridgeID;
	bridge->numPorts=numPorts;
	bridge->best.root = bridgeID;
	bridge->best.transmittingbridgeid = bridgeID;
	bridge->best.cost = 0;
	bridge->best.rcvedid=-32;
	bridge->noofentries=0;
	bridge->rp = -1;
	int i = 0;

	while (i<numPorts)
	{
		bridge->pinfo[i].port = i;
		bridge->pinfo[i].mac = MACaddresses[i];
		bridge->pinfo[i].state = Forwarding;
		i++;
	}

	/* add your code here */
}


int Execute(Bridge *bridge, MACQueueElement *receiveQueue, unsigned long step)
{ 
	MACQueueElement *f= receiveQueue;
	int i=0;
	bpdu arr[bridge->numPorts];
	while (i<bridge->numPorts)
	{
		arr[i].root = 999999; 
		arr[i].cost = 999999;
		arr[i].transmittingbridgeid = 999999;
		arr[i].transmittingportid = 999999;
		i++;
	}
	while (f!=NULL)
	{	
		MACFrame* frame = f->frame;
		i=0;
		int blockedport = 0;
		while (i<bridge->numPorts)
		{
			if (bridge->pinfo[i].state==Blocking&&bridge->pinfo[i].mac==f->MACaddress)
			{
					blockedport = 1; 
					break;
				}

			i++;
		}
		if (blockedport==0)
		{
		if (frame->DSAP == BRIDGEPROTOCOL)
		{
			i = 0;
		while (i<bridge->numPorts)
		{
			if(f->MACaddress==bridge->pinfo[i].mac)
			{

			if (frame->data[0]<arr[i].root)
			{
				arr[i].root = frame->data[0]; 
				arr[i].cost = frame->data[1];
				arr[i].transmittingbridgeid = frame->data[2];
				arr[i].transmittingportid = frame->data[3];
				arr[i].rcvedid = f->MACaddress;
				

			}
			else if (frame->data[0]==arr[i].root)
			{
				if (frame->data[1]<arr[i].cost)
				{
				arr[i].root = frame->data[0]; 
				arr[i].cost = frame->data[1];
				arr[i].transmittingbridgeid = frame->data[2];
				arr[i].transmittingportid = frame->data[3];
				arr[i].rcvedid = f->MACaddress;

				}

				else if (frame->data[1]==arr[i].cost)
				{
					if (frame->data[2]<arr[i].transmittingbridgeid)
					{
						arr[i].root = frame->data[0]; 
						arr[i].cost = frame->data[1];
						arr[i].transmittingbridgeid = frame->data[2];
						arr[i].transmittingportid = frame->data[3];
						arr[i].rcvedid = f->MACaddress;
					}

					else if (frame->data[2]<arr[i].transmittingbridgeid)
					{
						if (frame->data[3]<arr[i].transmittingportid)
						{
							arr[i].root = frame->data[0]; 
							arr[i].cost = frame->data[1];
							arr[i].transmittingbridgeid = frame->data[2];
							arr[i].transmittingportid = frame->data[3];
							arr[i].rcvedid = f->MACaddress;
						}



					}



				}



			}



			
		}
		i++;
	}
}

else {

	i = 0;
	int exists = 0;
	int exists1 =0;
	while (i<bridge->noofentries)
	{
		if(bridge->fwdtable[i].stationnumber==frame->srcAddress)
		{
			exists = 1;	
		}
		if (bridge->fwdtable[i].stationnumber==frame->dstAddress)
		{
			MACFrame tosend;
			tosend.dstAddress = frame->dstAddress;
			tosend.srcAddress = frame->srcAddress;
			tosend.length = 35;
			tosend.DSAP = IP;
			tosend.SSAP = IP;	
			tosend.data[0]=frame->data[0];
			tosend.data[1]=frame->data[1];
			tosend.data[2]=frame->data[2];
			tosend.data[3]=frame->data[3];
			int r = 0;
			while (r<bridge->numPorts)
			{
				if (bridge->fwdtable[i].portno==bridge->pinfo[r].port&&bridge->pinfo[r].mac!=f->MACaddress)
				{
					SendMACFrame(bridge->pinfo[r].mac, tosend);
					break;
				}
				r++;
			}
			

			exists1=1;

		}

		i++;
	}
	if (exists==0)
	{
		bridge->fwdtable[bridge->noofentries].stationnumber = frame->srcAddress;

		i = 0;
		while(i<bridge->numPorts){
			if (bridge->pinfo[i].mac==f->MACaddress)
				bridge->fwdtable[bridge->noofentries].portno = bridge->pinfo[i].port;
			i++;
		}
			bridge->noofentries++;

	}
	if (exists1==0)
	{
		i=0;
		while (i<bridge->numPorts||frame->dstAddress==BROADCAST)
		{
			if (bridge->pinfo[i].state!=Blocking&&bridge->pinfo[i].mac!=f->MACaddress)
			{
			MACFrame tosend;
			tosend.dstAddress = frame->dstAddress;
			tosend.srcAddress = frame->srcAddress;
			tosend.length = 35;
			tosend.DSAP = IP;
			tosend.SSAP = IP;
			tosend.data[0]=frame->data[0];
			tosend.data[1]=frame->data[1];
			tosend.data[2]=frame->data[2];
			tosend.data[3]=frame->data[3];
			SendMACFrame(bridge->pinfo[i].mac, tosend);
		}
				i++;

			}
	}


}

}
	f=f->next;
}

i = 0;
bpdu best=bridge->best;
while (i<bridge->numPorts)
{
			if (arr[i].root<best.root)
			{
				best =arr[i];
				

			}
			else if (arr[i].root==best.root)
			{
				if (arr[i].cost<best.cost)
				{
					best = arr[i];

				}

				else if (arr[i].cost==best.cost)
				{
					if (arr[i].transmittingbridgeid<best.transmittingbridgeid)
					{
						best = arr[i];

					}

					else if (arr[i].transmittingbridgeid==best.transmittingbridgeid)
					{
						if (arr[i].transmittingportid<best.transmittingportid)
						{
							best = arr[i];

						}
					


					}
					
					


				}
				


			}
			
			i++;


}



if (bridge->bridgeID!=best.root)
{
	best.cost++;
	bridge->best=best;
	bridge->rp = best.rcvedid;
}
i = 0;

while (i<bridge->numPorts)
{
	if (bridge->rp!=bridge->pinfo[i].mac)
	{
			if (arr[i].root<best.root)
			{
				bridge->pinfo[i].state = Blocking;

			}
			else if (arr[i].root==best.root)
			{
				if (arr[i].cost<=best.cost)
				{
					bridge->pinfo[i].state = Blocking;

				}

				else if (arr[i].cost==best.cost)
				{
					if (arr[i].transmittingbridgeid<best.transmittingbridgeid)
					{
						bridge->pinfo[i].state= Blocking;

					}

					else if (arr[i].transmittingbridgeid==best.transmittingbridgeid)
					{
						if (arr[i].transmittingportid<best.transmittingportid)
						{
							bridge->pinfo[i].state = Blocking;

						}
					


					}
					
					


				}
				


			}
			}
			i++;


}


i=0;
			while (i<bridge->numPorts)
			{
				 if (bridge->pinfo[i].state!=Blocking&&bridge->pinfo[i].mac!=bridge->rp)
				{
					MACFrame tosend;
					tosend.dstAddress = ALLBRIDGES;
					tosend.srcAddress = bridge->pinfo[i].port;
					tosend.length = 35;
					tosend.DSAP = BRIDGEPROTOCOL;
					tosend.SSAP = BRIDGEPROTOCOL;
					//if (bridge->bridgeID==0)
					//	tosend.data[0] = 0;

					tosend.data[0] = best.root;
					tosend.data[1] = best.cost;
					tosend.data[2] = bridge->bridgeID;
					tosend.data[3] = bridge->pinfo[i].port;			
					SendMACFrame(bridge->pinfo[i].mac, tosend);
				}
				i++;
			}


	return 0;


}

int *GetFDBEntry (Bridge *bridge, int MACaddress)
{ 
	int i = 0;
	int arr[bridge->numPorts];
	while (i<bridge->numPorts)
	{
		arr[i]=0;
		i++;
	}
	i=0;
	while (i<bridge->noofentries)
	{
		if (bridge->fwdtable[i].stationnumber==MACaddress)
		{
			int r = 0;
			while (r<bridge->numPorts)
			{
				if (bridge->fwdtable[i].portno==bridge->pinfo[r].port)
					arr[r]=1;
				r++;
			}
		
		}

		i++;
	}
	return arr;
}

int GetRootPort (Bridge *bridge)
{
	return bridge->rp;
}

PortStateType GetPortState (Bridge *bridge, int MACaddress)
{
	int i=0;
	int num = bridge->numPorts;
	while (i<num)
	{
		if (bridge->pinfo[i].mac==MACaddress)
		{
			return bridge->pinfo[i].state;
		
		}
		i++;
	}

}

MACFrame *GetConfigBPDUatPort (Bridge *bridge, int MACaddress)
{	
	MACFrame tosend ;
	tosend.dstAddress = ALLBRIDGES;
	tosend.srcAddress = MACaddress;
	tosend.DSAP = BRIDGEPROTOCOL;
	tosend.SSAP = BRIDGEPROTOCOL;
	tosend.length = 35;
	tosend.data[0] = bridge->best.root;
	tosend.data[1] = bridge->best.cost;
	tosend.data[2] = bridge->bridgeID;
	int i=0;
	while (i<bridge->numPorts)
	{
		if(bridge->pinfo[i].mac==MACaddress)
			tosend.data[3] =bridge->pinfo[i].port;
		i++;
	}


	return &tosend;	
}

void BridgeCleanup(void)
{	
 	// What to clean here. No input arguments!
	/* add your code here */
}
