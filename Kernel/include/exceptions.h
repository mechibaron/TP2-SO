#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

void exceptionDispatcher(uint64_t exception, uint64_t *stackFrame);
void backUpRipRsp(uint64_t *ip, uint64_t *rsp);

#endif /* __EXCEPTIONS_H__ */