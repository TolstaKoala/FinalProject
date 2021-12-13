#ifndef PARSERSTATISTCS_H
#define PARSERSTATISTCS_H

#ifdef __cplusplus
extern "C"
{
#endif


typedef struct{
    unsigned int            packets;
    unsigned int            octets;
    unsigned int            highPriorityPackets;
    unsigned int            lowPriorityPackets;
    unsigned int            broadcast;
    unsigned int            multicast;
    unsigned int            broadAndMulticast;
    unsigned int            errorPackets;
}total_t;

typedef struct{
    unsigned int            _64bytes;
    unsigned int            _65_127bytes;
    unsigned int            _128_255bytes;
    unsigned int            _256_511bytes;
    unsigned int            _512_1023bytes;
    unsigned int            _1024bytes;
}sizeCounters_t;

typedef struct{
    unsigned int            crcAligment;
    unsigned int            undersize;
    unsigned int            oversize;
    unsigned int            fragments;
    unsigned int            jabbers;
    unsigned int            drops;
}receiveErrorCounters_t;

typedef struct{
    unsigned int            collisions;
    unsigned int            drops;
    unsigned int            overflow;
}transmitErrorCounters_t;

typedef struct{
    total_t                 receiveTotal;
    total_t                 transmitTotal;
    sizeCounters_t          receiveSizeCounters;
    sizeCounters_t          transmitSizeCounters;
    receiveErrorCounters_t  receiveErrorCounters;
    transmitErrorCounters_t transmitErrorCounters;
}portStatistics_t;


void parse_statistics(portStatistics_t* portStatistics, char *buff);
void string2OneWord(char *string,unsigned int string_count,unsigned int *word1);
void string2TwoWords(char *string,unsigned int string_count,unsigned int *word1, unsigned int *word2);

#ifdef __cplusplus
}
#endif

#endif // PARSERSTATISTCS_H
