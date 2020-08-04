#include "audio.h"
#include "math.h"
#include "mcp3008.h"
#include "constants.h"
#include "refvals.h"
#include "gpio.h"
#include "gpioextra.h"
#include "gpioevent.h"
#include "printf.h"
#include "constants.h"
#include "malloc.h"

static int get_edit_distance(int *vals1, int len1, int *vals2, int len2) {

  int memo[len1 + 1][len2 + 1];

  for(int i = 0; i <= len1; i++) {
    for(int j = 0; j <= len2; j++) {
      if(i == 0 && j ==0) {
        memo[i][j] = 0; 
      }else if(i == 0) {
        memo[i][j] = 100000; 
      } else if(j == 0) {
        memo[i][j] = 100000; 
      } else if( abs(vals1[i - 1] - vals2[j -1]) <= SIMILARITY_TOLERANCE) { 
        memo[i][j] = memo[i-1][j-1];
      } else {
        memo[i][j] = min3(memo[i-1][j], memo[i-1][j-1], memo[i][j-1]) + abs(vals1[i-1] - vals2[j-1]) * abs(vals1[i-1] - vals2[j-1]) / 100;
      }
    }
  }
  return memo[len1][len2]; 
}

static int chunk_distance(int *vals1, int len1, int *vals2, int len2) { 

    int ave1[NUM_CHUNKS]; 

    int chunkLen1 = len1 / NUM_CHUNKS; 
   
    for(int i = 0; i < NUM_CHUNKS; i++) {
      ave1[i] = 0; 
      for(int j = 0; j < chunkLen1; j++) {
        ave1[i] += vals1[j + i*chunkLen1]; 
      }
      ave1[i] /= chunkLen1; 
    }

    return get_edit_distance(ave1, NUM_CHUNKS, vals2, NUM_CHUNKS); 
}


static int get_ave_distance(int *input_audio, int audio_len, int lex_idx) { 
    int tot = 0;
    int cnt = 0; 
    for( int i = 0; i < MAX_AUDIO_FILE_CNT; i++) {
        int edit_dist = chunk_distance(input_audio, audio_len, refVals[lex_idx][i], ref_val_lengths[lex_idx][i]); 
        tot += edit_dist; 
        cnt ++; 
    }
    return tot / cnt; 
}


int get_audio() {
  int vals[8];
  int tot_length = 0; 
  int minDist = 999999; 
  int min_idx = -1; 

  while (tot_length < 8) {
     int read = mcp3008_read(AUDIO_READ_CHANNEL);
       vals[tot_length++] = read;
  }
  // printf("Comparing values\n");
  // if (1==0) {
  for(int i = 0; i < LEXICON_SIZE; i++) {
    int ave_dist = get_ave_distance(vals, tot_length, i);
    // printf("Average dist %s: %d\n", lexicon[i], ave_dist);
    if(ave_dist < minDist) {
      minDist = ave_dist; 
      min_idx = i; 
    }
  }
// }

  return min_idx;
}