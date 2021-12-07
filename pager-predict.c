#include <stdio.h> 
#include <stdlib.h>
#include "simulator.h"

void pageit(Pentry q[MAXPROCESSES]) { 

    /* initialize static vars on first run */
    static int initialized = 0;
    static int pc_previous[MAXPROCESSES];
    static int program_type[MAXPROCESSES];

    
    /* initialize static vars on first run */
    if(!initialized){
        for(int proctmp=0; proctmp < MAXPROCESSES; proctmp++){
            pc_previous[proctmp] = 0;
            program_type[proctmp] = 0;
        }
        initialized = 1;
    }
    
    /* Local vars */
    int pages_to_swap[MAXPROCESSES][MAXPROCPAGES] = {0};
    int num_frames_allocated = 0; 

    /* determine program type and location of executionn for each process, based
     * on this information, decide which pages to swap in or out
     */
    for(int proc=0; proc<MAXPROCESSES; proc++){
        if(q[proc].active){
            int pc = q[proc].pc;
            int page = pc/PAGESIZE;

            int pc_jump = abs(pc_previous[proc] - pc);

            if(pc_jump > 1){
                //setting program types

                switch(pc_jump) {
                    //going back to initial for loop in program 1
                    case 1533:
                        program_type[proc] = 1;
                        break;

                    //program branches to the else
                    case 902:
                        program_type[proc] = 1;
                        break;

                    //program 1 finishes executing 900 instructions in the if
                    case 132:
                        program_type[proc] = 1;
                        break;
                    
                    //program 2 loop back to beginning
                    case 1129:
                        program_type[proc] = 2;
                        break;

                    //go  back to inner for loop
                    case 516:
                        program_type[proc] = 3;
                        break;

                    //go back to beginning of execution
                    
                    case 1683:
                        program_type[proc] = 3;
                        break;
                        
                    case 503:
                        program_type[proc] = 5;
                        break;
                    

                    //exit conditions detect exit conditions and then set back to program_type = default
                    //exit from prog 1 and page out all frames it used
                    case 1534:
                        program_type[proc] = 0;
                        break;

                    //exit from prog 2
                    case 1130:
                        program_type[proc] = 0;
                        break;

                    //exit from prog 3
                    case 1684:
                        program_type[proc] = 0;
                        break;

                    //exit from prog 5
                    case 504:
                        program_type[proc] = 0;
                        break;
                }
            }

            switch(program_type[proc]){
                case 1:
                    switch(page){
                        case 1:
                            pages_to_swap[proc][page] = 1;
                            pages_to_swap[proc][5] = 1;
                            pages_to_swap[proc][2] = 2;
                            break;

                        case 5:
                            pages_to_swap[proc][page] = 1;
                            pages_to_swap[proc][0] = 1;
                            break;
                        
                        default:
                            pages_to_swap[proc][page] = 1;
                            pages_to_swap[proc][(page+1)%8] = 1;
                            break;
                    }
                    break;

                case 2:
                    pages_to_swap[proc][page] = 1;
                    pages_to_swap[proc][(page+1)%5] = 1;
                    break;

                case 3:
                    switch(page){
                        case 6:
                            pages_to_swap[proc][page] = 1;
                            pages_to_swap[proc][4] = 1;
                            pages_to_swap[proc][0] = 2;
                            break;

                        default:
                            pages_to_swap[proc][page] = 1;
                            pages_to_swap[proc][(page+1)%7] = 1;
                            break;
                    }
                    break;

                case 5:
                    pages_to_swap[proc][0] = 1;
                    pages_to_swap[proc][1] = 1;
                    break;

                default:
                    pages_to_swap[proc][page] = 1;
                    pages_to_swap[proc][(page+1)%8] = 1;
                    break;
            }

            pc_previous[proc] = pc;
            
        }
   }

   for(int i=0; i<MAXPROCESSES; i++){
       for(int j=0; j<MAXPROCPAGES; j++){
           if(pages_to_swap[i][j] == 0 && q[i].pages[j]){
               pageout(i,j);
           }
       }
   }

    for(int i=0; i<MAXPROCESSES; i++){
       for(int j=0; j<MAXPROCPAGES; j++){
           if(pages_to_swap[i][j] == 1 && !q[i].pages[j]){
               pagein(i,j);
           }
       }
   }



   for(int i=0; i<MAXPROCESSES; i++){
       for(int j=0; j<MAXPROCPAGES; j++){
           if(q[i].pages[j] || pages_to_swap[i][j]){
               num_frames_allocated ++;
           }
       }
   }

    for(int i=0; i<MAXPROCESSES; i++){
        for(int j=0; j< MAXPROCPAGES; j++){
            if(num_frames_allocated < 42){ //by experimentaiton this worked best
                if(pages_to_swap[i][j] == 2 && !q[i].pages[j]){
                    pagein(i,j);
                    num_frames_allocated ++;
                }
            }
            else{
                goto label;
            }
        }
    }

    label:
        ;

        



} 
