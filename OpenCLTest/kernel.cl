
__kernel void test(__global int* message)
{
    
    __local int array[10];
    int gid = get_global_id(0);
    int lid = get_local_id(0);
    int group_id = get_group_id(0);
    printf("Global - %d, Local - %d, Group - %d\n", gid, lid, group_id);
    int some;
    for (int k=0; k<100000000-10000000*gid; k++) {
        some = k*k;
    }
    printf("Thread %d finished\n", gid);
    message[0] = gid;
    printf("Thread %d started with %d\n", gid, message[0]);
}