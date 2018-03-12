static inline int mysleep​(​​){ 
    return​ syscall​(​335​​); 
}

static​ inline int mywakeup(​​)
{
    return​ syscall​(336);
}
