//Fully associative cache simulation computer systems architecture coursework
//Name: Sanchayata Jain
//Date: 10/03/2020


/* this program simulates a fully associative cache and outputs the number of cache read hits, cache read misses, cache write hits
 cache write misses, reads from external memory and writes to external memory */


//library's used in the program
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//functions declaration
int tag_calc(int hex, int mode, int words);
int cache_size(int mode, int line_words);
void mode(int mode_num, char file_name[]);

int main(){
char fileB[] = "bubble_sort_trace_001.trc";     //storing file name in char array
char fileC[] = "cross_correlation_trace_001.trc";     //storing file name in char array
//calling mode function with the parameters(mode, file name)
mode(1,fileB); // using mode 1 and bubble sort trace file
mode(2,fileB);
mode(3,fileB);
mode(4,fileB);
mode(5,fileB);
mode(6,fileB);
mode(7,fileB);
mode(8,fileB);

mode(1,fileC); //using mode 1 and cross correlation trace file
mode(2,fileC);
mode(3,fileC);
mode(4,fileC);
mode(5,fileC);
mode(6,fileC);
mode(7,fileC);
mode(8,fileC);


}

void mode(int mode_num, char file_name[]){
//initialising variables to 0
int found = 0; //flag to see if tag bits calculated from memory address in trace file is already in the cache
int NRCH=0; //cache read hit
int NRCM=0; //cache read miss
int NWCH=0; //cache write hit
int NWCM=0; //cache write miss
int NRA=0; //read accesses from external memory
int NWA=0; //write accesses to external memory
int words_amount = pow(2,(mode_num-1)); // amount of words in each block/line in main memory/cache
char rw; //used to store R or W from trace file into rw
int hex; //used to store the hexadecimal memory address from trace file into hex.
int counter=0; //counter increments each time a line is filled in my cache array. allows array to loop from 0 to size until end of trace file
int size =cache_size(mode_num, words_amount); // function call to work out size of cache depending on which mode

//initialising variable arrays: cache, dirty_bit and valid_flag to 0
int *cache=(int*)calloc(size,sizeof(int));          // the array cache, is actually an array only holding the tag bits which are used to identify which data is stored in cache and which data isn't in the cache
int *dirty_bit=(int*)calloc(size,sizeof(int));     // dirty_bit used to check if data in cache line has been previously modified (written to)
int *valid_flag=(int*)calloc(size,sizeof(int));   //valid flag used to check if a line in the cache is filled or empty
FILE *trcfile; //Pointer to trace file
trcfile = fopen(file_name, "r"); //opening file using pointer trcfile
while(!feof(trcfile)){                     //stay in while loop until the end of the file is reached
    fscanf(trcfile, "%c %X ", &rw, &hex);   // reads a line at a time from start to end of file
    int tag = tag_calc(hex, mode_num, words_amount);        //calls tag function and stores tag bits in variable tag

  for(int i=0; i<size; i++){          //for loop ensures each line of cache is checked
   if(valid_flag[i]==1){              // ensures only valid flag for each line is checked first, to ensure tag bits are only compared with lines with data
    if(cache[i]==tag){                // can enter if statement if there tag bits calculated and tag bits stored in cache array match
        found=1;                      // tag bits found in cache and so found now is set to 1
        switch(rw){                     // use switch statement to see if memory address in trace file was read or write
        case 'R': NRCH++;               // if read then read cache hit increments by 1
        break;

        case 'W': NWCH++;               //if write then write cache hit increments by 1
        dirty_bit[i] = 1;               //memory address has been written to and so dirty bit must be 1
        break;
        }//end of switch statement
    }//end of  nested if statement
   } //end of if statement
  }//end of for statement

  if(found==0){                        //can enter if statement if tag bits calculated don't match any tag bits stored in cache array
    if(counter == size){                //if statement used to ensure FIFO occurs in array. when last element reached, counter resets to 0th element so
        counter=0;                       //then any new tag bits stored overwrite the oldest element in the array.
    }//end of if
    if(valid_flag[counter] ==1 && dirty_bit[counter]==1){        // NWA increases by number of words in a cache line, NWA only increases when line in cache is valid
        NWA += words_amount;                                    // and that line has been previously written to (so dirty bit is equal to 1)
    }
    valid_flag[counter] =0;
    cache[counter] = tag;                   //places tag bits into cache array
    valid_flag[counter] =1;                  //sets valid_flag to 1 as that line of the cache now has word(s) stored in it
    NRA +=  words_amount;                    // have to read from main memory as new memory address isn't stored in the cache yet and so NWA increments by words in a block

    switch(rw){                             // switch statement to see if address is being read to or written to
    case 'R': NRCM++;                       // read cache miss increments by 1
    dirty_bit[counter] = 0;                 // dirty bit for that cache line gets reset to 0
    break;

    case 'W': NWCM++;                       // write cache miss increments by 1
    dirty_bit[counter] = 1;                 // dirty bit for that cache line is set to 1 as that line has just been written to
    break;
    }            //end of switch statement
    counter++;                              // counter increments by 1 and so now next line of cache array, dirty_bit array and valid_flag array will be used

  }              //end of if statement
  found =0; //resetting flag in case it changed to 1;
}//end of while statment

fclose(trcfile);       // closes file which was opened
printf("%s, mode_num: %d, NRA: %d, NWA: %d, NRCH: %d,  NRCM: %d, NWCH: %d, NWCM: %d\n",file_name,mode_num, NRA, NWA, NRCH, NRCM, NWCH, NWCM); // prints all the variables previously declared above at the start of the mode function
}

//function used to return the number of lines in the cache which is stored in the variable size
//(an element of the cache array represents a line in the cache)
//parameters: mode is used in the switch statement
// line_words is the number of words in a block or cache line
int cache_size(int mode,int line_words){
int size;   // stores the number of lines in the cache, depending on which mode is being used
switch(mode){                   // switch statement is used to return different amount of cache lines for different modes
case 1: size=256/line_words;
break;

case 2: size=256/line_words;
break;

case 3: size=256/line_words;
break;

case 4: size=256/line_words;
break;

case 5: size=256/line_words;
break;

case 6: size=256/line_words;
break;

case 7: size=256/line_words;
break;

case 8: size=256/line_words;
break;
}
return size;                    //function returns size
}



int tag_calc(int hex, int mode, int words){       //function used to calculate what tag bits are for memory address read from trace files
int offset = log2(words);              // works out offset for each memory address
int TAG;
// switches between each mode to then be able to work out tag bits for each memory address in each mode
// tag bits are worked out by right bitshifting the memory address by its offset
// tag bits for a memory address are stored in the variable TAG
switch(mode){
case 1:
        TAG = hex >> offset;
    break;

case 2:
        TAG = hex >> offset;
    break;

case 3:
     TAG = hex >> offset;
    break;

case 4:
     TAG = hex >> offset;
    break;

case 5:
         TAG = hex >> offset;
    break;

case 6:
         TAG = hex >> offset;
    break;

case 7:
         TAG = hex >> offset;
    break;

case 8:
         TAG = hex >> offset;
    break;
}
return TAG;   //function returns TAG (tag bits of a memory address) to function mode
}
