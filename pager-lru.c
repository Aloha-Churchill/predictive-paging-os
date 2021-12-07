/*
 * File: pager-lru.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains an lru pageit
 *      implmentation.
 */
#include <stdio.h> 
#include <stdlib.h>
#include <limits.h>
#include "simulator.h"


void pageit(Pentry q[MAXPROCESSES]) { 
    
    /* This file contains the stub for an LRU pager */
    /* You may need to add/remove/modify any part of this file */

    /* Static vars */
    static int initialized = 0;
    static int tick = 1; // artificial time
    static int timestamps[MAXPROCESSES][MAXPROCPAGES];
    //static int num_frames = 0;
    // int lru = INT MAX

    /* Local vars */
    int proctmp;
    int pagetmp;

    /* initialize static vars on first run */
    if(!initialized){
	for(proctmp=0; proctmp < MAXPROCESSES; proctmp++){
	    for(pagetmp=0; pagetmp < MAXPROCPAGES; pagetmp++){
		timestamps[proctmp][pagetmp] = 0; 
	    }
	}
	initialized = 1;
    }
    
    /* TODO: Implement LRU Paging */

    /* Local vars */
    int proc;
    //int pc;
    int page;
    int oldpage; 
    int lru = INT_MAX;
    int lru_page = INT_MAX;

   for(proc=0; proc<MAXPROCESSES; proc++){
       if(q[proc].active){

            page = q[proc].pc/PAGESIZE;
            //timestamps[proc][page] = 0;
            timestamps[proc][page] = tick;

            if(!q[proc].pages[page]){
                if(!pagein(proc,page)){
                    for(oldpage=0; oldpage<q[proc].npages; oldpage++){
                        if(q[proc].pages[oldpage] == 1){
                            if(timestamps[proc][oldpage] < lru){
                                lru = timestamps[proc][oldpage];
                                lru_page = oldpage;
                            }
                        }
                        
                    }
                    if(pageout(proc, lru_page)){
                        break;
                    }
                }
            }
        }
   }


    /* advance time for next pageit iteration */
    tick++;

} 
