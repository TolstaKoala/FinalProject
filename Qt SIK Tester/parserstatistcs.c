#include "parserstatistcs.h"
#include <stdio.h>
#include <stdlib.h>


void parse_statistics(portStatistics_t* portStatistics, char *buff){
    unsigned int count_str = 0, buff_count = 0, string_count = 0;
    char string[300];

    for(count_str = 0; count_str < 31; count_str++){
        while(*(buff + buff_count) != 0xd){
            string[string_count] = *(buff + buff_count);
            buff_count++;
            string_count++;
        }
        string[string_count] = *(buff + buff_count);
        buff_count++;
        string_count++;
        switch(count_str){
            case 0:
            case 1:
            case 2:
            case 3:
                break;
            /* receiveTotal & transmitTotal */
            case 4:
                string2TwoWords(&string[0],string_count,&portStatistics->receiveTotal.packets, &portStatistics->transmitTotal.packets);
                break;
            case 5:
                string2TwoWords(&string[0],string_count,&portStatistics->receiveTotal.octets, &portStatistics->transmitTotal.octets);
                break;
            case 6:
                string2TwoWords(&string[0],string_count,&portStatistics->receiveTotal.highPriorityPackets, &portStatistics->transmitTotal.highPriorityPackets);
                break;
            case 7:
                string2TwoWords(&string[0],string_count,&portStatistics->receiveTotal.lowPriorityPackets, &portStatistics->transmitTotal.lowPriorityPackets);
                break;
            case 8:
                string2TwoWords(&string[0],string_count,&portStatistics->receiveTotal.broadcast, &portStatistics->transmitTotal.broadcast);
                break;
            case 9:
                string2TwoWords(&string[0],string_count,&portStatistics->receiveTotal.multicast, &portStatistics->transmitTotal.multicast);
                break;
            case 10:
                string2TwoWords(&string[0],string_count,&portStatistics->receiveTotal.broadAndMulticast, &portStatistics->transmitTotal.broadAndMulticast);
                break;
            case 11:
                string2TwoWords(&string[0],string_count,&portStatistics->receiveTotal.errorPackets, &portStatistics->transmitTotal.errorPackets);
                break;
            /* receiveSizeCounters & transmitSizeCounters */
            case 12:
            case 13:
            case 14:
                break;
            case 15:
                string2TwoWords(&string[0],string_count,&portStatistics->receiveSizeCounters._64bytes, &portStatistics->transmitSizeCounters._64bytes);
                break;
            case 16:
                string2TwoWords(&string[0],string_count,&portStatistics->receiveSizeCounters._65_127bytes, &portStatistics->transmitSizeCounters._65_127bytes);
                break;
            case 17:
                string2TwoWords(&string[0],string_count,&portStatistics->receiveSizeCounters._128_255bytes, &portStatistics->transmitSizeCounters._128_255bytes);
                break;
            case 18:
                string2TwoWords(&string[0],string_count,&portStatistics->receiveSizeCounters._256_511bytes, &portStatistics->transmitSizeCounters._256_511bytes);
                break;
            case 19:
                string2TwoWords(&string[0],string_count,&portStatistics->receiveSizeCounters._512_1023bytes, &portStatistics->transmitSizeCounters._512_1023bytes);
                break;
            case 20:
                string2TwoWords(&string[0],string_count,&portStatistics->receiveSizeCounters._1024bytes, &portStatistics->transmitSizeCounters._1024bytes);
                break;
            /* receiveErrorCounters & transmitErrorCounters */
            case 21:
            case 22:
            case 23:
                break;
            case 24:
                string2TwoWords(&string[0],string_count,&portStatistics->receiveErrorCounters.crcAligment, &portStatistics->transmitErrorCounters.collisions);
                break;
            case 25:
                string2TwoWords(&string[0],string_count,&portStatistics->receiveErrorCounters.undersize, &portStatistics->transmitErrorCounters.drops);
                break;
            case 26:
                string2TwoWords(&string[0],string_count,&portStatistics->receiveErrorCounters.oversize, &portStatistics->transmitErrorCounters.overflow);
                break;
            case 27:
                string2OneWord(&string[0],string_count,&portStatistics->receiveErrorCounters.fragments);
                break;
            case 28:
                string2OneWord(&string[0],string_count,&portStatistics->receiveErrorCounters.jabbers);
                break;
            case 29:
                string2OneWord(&string[0],string_count,&portStatistics->receiveErrorCounters.drops);
                break;
            case 30:
                break;
        }
        string_count = 0;
    }
}

void string2TwoWords(char *string,unsigned int string_count,unsigned int *word1, unsigned int *word2){
    unsigned int result = 0;
    unsigned int i = 0;
    unsigned int num_count = 0;
    unsigned int word_num1 = 0;
    unsigned int word_num2 = 0;
    while(i < string_count){
        if((*(string + i) >= '0') && (*(string + i) <= '9')){
            result = (result * 10) + (*(string + i) - '0');
            num_count++;
        }else if (num_count != 0){
            num_count = 0;
            if( (i > (string_count/3)) && (i < (2*string_count/3))){
                word_num1 = 1;
                *word1 = result;
                result = 0;
            }else if(i > (2*string_count/3)){
                word_num2 = 1;
                *word2 = result;
                result = 0;
            }
        }
        i++;
    }
    if(word_num1 == 0 && word_num2 == 0){
        *word1 = 0;
        *word2 = 0;
    }
    if(word_num1 == 0)*word1 = 0;
    if(word_num2 == 0)*word2 = 0;
}

void string2OneWord(char *string,unsigned int string_count,unsigned int *word1){
    unsigned int result = 0;
    unsigned int i = 0;
    unsigned int num_count = 0;
    unsigned int word_num1 = 0;
    while(i < string_count){
        if((*(string + i) >= '0') && (*(string + i) <= '9')){
            result = (result * 10) + (*(string + i) - '0');
            num_count++;
        }else if (num_count != 0){
            word_num1 = 1;
            *word1 = result;
        }
        i++;
    }
    if(word_num1 == 0)*word1 = 0;
}
