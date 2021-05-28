#ifndef __BUF_H
#define __BUF_H

#define BSIZE 512

#include "sleeplock.h"

// xv6中磁盘块数据结构,块大小512字节
struct buf {
  int valid;
  int disk;		// does disk "own" buf? 
  uint dev;
  uint sectorno;	// sector number 扇区号
  struct sleeplock lock;
  uint refcnt;
  struct buf *prev;
  struct buf *next;
  uchar data[BSIZE];
};

void            binit(void);
struct buf*     bread(uint, uint);
void            brelse(struct buf*);
void            bwrite(struct buf*);

#endif
