#ifndef _GDT_H
#define _GDT_H

void initializeGDT(void);
void setTssRsp(void *rsp);

#endif
