//
//  main.cpp
//  OpenCLTest
//
//  Created by Galiev Aynur on 07.05.16.
//  Copyright © 2016 FlatStack. All rights reserved.
//

#include <iostream>
#include <OpenCL/opencl.h>
#include "FSSAlgorithm.hpp"

int main(int argc, const char * argv[]) {
 
    int top[]    = { 1,  2, 3, 10, 10, 1,  12, 4, 4,  5,  11, 4, 7,  6,  5, 6  };
    int bottom[] = { 10, 8, 8, 8,  2,  11, 8,  3, 12, 13, 9,  9, 13, 14, 7, 14 };
    
    FSSAlgorithm algorithm = FSSAlgorithm(top, bottom, 16, 10, 10, 2, 50, 0.6, 0.8, 0.1);
    algorithm.start();
    
//    int error;
//    cl_device_id device_id;
//    cl_uint maxComputeUnits;
//    error = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
//    if (error != CL_SUCCESS) {
//        return EXIT_FAILURE;
//    }
//    
//    error = clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(maxComputeUnits), &maxComputeUnits, NULL);
//    
//    cl_platform_id platform_id;
//    cl_uint ret_num_platforms;
//    error = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
//    
//    cl_uint ret_num_devices;
//    /* получить доступные устройства */
//    error = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
//    
//    /* создать контекст */
//    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &error);
//    
//    /* создаем команду */
//    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &error);
//    
//    cl_program program = NULL;
//    cl_kernel kernel = NULL;
//    
//    FILE *fp;
//    const char fileName[] = "kernel.cl";
//    size_t source_size;
//    char *source_str;
//    
//    try {
//        fp = fopen(fileName, "r");
//        if (!fp) {
//            fprintf(stderr, "Failed to load kernel.\n");
//            exit(1);
//        }
//        source_str = (char *)malloc(1000);
//        source_size = fread(source_str, 1, 1000, fp);
//        fclose(fp);
//    }
//    catch (int a) {
//        printf("%d", a);
//    }
//    
//    /* создать бинарник из кода программы */
//    program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &error);
//    
//    /* скомпилировать программу */
//    error = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
//    
//    /* создать кернел */
//    kernel = clCreateKernel(program, "test", &error);
//
//    cl_mem memobj = NULL;
//    int memLenth = 10;
//    cl_int* mem = (cl_int *)malloc(sizeof(cl_int) * memLenth);
//    
//    /* создать буфер */
//    memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, memLenth * sizeof(cl_int), NULL, &error);
//    
//    /* записать данные в буфер */
//    error = clEnqueueWriteBuffer(command_queue, memobj, CL_TRUE, 0, memLenth * sizeof(cl_int), mem, 0, NULL, NULL);
//    
//    /* устанавливаем параметр */
//    error = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);
//    
//    size_t global_work_size[1] = { 10 };
//    size_t local_work_size[1] = { 10 };
//    
//    /* выполнить кернел */
//    error = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);
//    
//    /* считать данные из буфера */
//    error = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0, memLenth * sizeof(float), mem, 0, NULL, NULL);
//    
//    for(int i=0; i<memLenth; i++)
//    {
//        printf("%d\n", mem[i]);
//    }
    
    
    return 0;
}
