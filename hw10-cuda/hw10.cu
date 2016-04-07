#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <iostream>
#include <cuda.h>
#include <cuda_runtime.h>
#include <helper_functions.h>
#include <windows.h>
#include <iomanip>

using namespace std;

/*
 *  Return elapsed wall time since last call (seconds)
 */
static double t0=0;
double Elapsed(void)
{
   //  Windows version of wall time
   LARGE_INTEGER tv,freq;
   QueryPerformanceCounter((LARGE_INTEGER*)&tv);
   QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
   double t = tv.QuadPart/(double)freq.QuadPart;
   double s = t-t0;
   t0 = t;
   return s;
}

/*
 *  Initialize matrix with random values
 */
void RandomInit(float x[],const unsigned int n)
{
    for (unsigned int i=0;i<n*n;i++)
        x[i] = rand() / (float)RAND_MAX;
}

/*
 *  Initialize fastest GPU device
 */
int InitGPU(int verbose)
{
    //  Get number of CUDA devices
    int num;
    if (cudaGetDeviceCount(&num))
    {
        cerr << "Cannot get number of CUDA devices" << endl;
        exit(1);
    }
    if (num<1)
    {
        cerr << "No CUDA devices found\n" << endl;
        exit(1);
    }

    //  Get fastest device
    cudaDeviceProp prop;
    int   MaxDevice = -1;
    int   MaxGflops = -1;
    for (int dev=0;dev<num;dev++)
    {
        if (cudaGetDeviceProperties(&prop,dev))
        {
           cerr << "Error getting device " << dev << " properties" << endl;
           exit(1);
        }
        int Gflops = prop.multiProcessorCount * prop.clockRate;
        if (verbose)
        {
            cout << "CUDA Device " << dev << ": " << prop.name << " Gflops " << (1e-6*Gflops) << " Processors " << prop.multiProcessorCount << " Threads/Block " << prop.maxThreadsPerBlock << endl;
        }
        if(Gflops > MaxGflops)
        {
            MaxGflops = Gflops;
            MaxDevice = dev;
        }
    }

    //  Print and set device
    if (cudaGetDeviceProperties(&prop,MaxDevice))
    {
        cerr << "Error getting device " << MaxDevice << " properties" << endl;
    }
    cout << "Fastest CUDA Device " << MaxDevice << ": " << prop.name << endl;
    cudaSetDevice(MaxDevice);

    //  Return max thread count
    return prop.maxThreadsPerBlock;
}

/*
 * C = A * B -- host
 */
void AxBh(float C[], const float A[], const float B[], unsigned int n)
{
   for (unsigned int i=0;i<n;i++)
      for (unsigned int j=0;j<n;j++)
      {
         double sum=0;
         for (unsigned int k=0;k<n;k++)
            sum += (double)A[i*n+k] * (double)B[k*n+j];
         C[i*n+j] = (float)sum;
      }
}

/*
 * Compute one element of A * B
 */
__global__ void AxB(float C[],const float A[],const float B[],const unsigned int n)
{
   unsigned int j = blockIdx.x*blockDim.x+threadIdx.x;
   unsigned int i = blockIdx.y*blockDim.y+threadIdx.y;
   float sum =0;
   for (int k=0;k<n;k++)
      sum += A[i*n+k] * B[k*n+j];
   C[i*n+j] = sum;
}

/*
 * C = A * B -- device
 */
void AxBd(float Ch[],const float Ah[],const float Bh[],const unsigned int Bw,const unsigned int Bn)
{
    //  Calculate matrix dimensions
    int n = Bw*Bn;
    int N = n*n*sizeof(float);

    // Allocate device memory
    float* Ad;
    float* Bd;
    float* Cd;
    if (cudaMalloc((void**)&Ad,N))
    {
        cerr << "Cannot allocate device memory Ad" << endl;
        exit(1);
    }
    if (cudaMalloc((void**)&Bd,N))
    {
        cerr << "Cannot allocate device memory Bd" << endl;
        exit(1);
    }
    if (cudaMalloc((void**)&Cd,N))
    {
        cerr << "Cannot allocate device memory Cd" << endl;
        exit(1);
    }
    // Copy A and B from host to device
    if (cudaMemcpy(Ad,Ah,N,cudaMemcpyHostToDevice))
    {
        cerr << "Cannot copy A from host to device" << endl;
        exit(1);
    }
    if (cudaMemcpy(Bd,Bh,N,cudaMemcpyHostToDevice))
    {
        cerr << "Cannot copy B from host to device" << endl;
        exit(1);
    }

    // Set size of block to Bw x Bw, and Bn x Bn blocks
    dim3 threads(Bw,Bw);
    dim3 grid(Bn,Bn);
    // Execute the kernel
    AxB<<<grid,threads>>>(Cd,Ad,Bd,n);
    if (cudaGetLastError())
    {
        cerr << "AxB failed" << endl;
        exit(1);
    }

    // Copy C from device to host
    if (cudaMemcpy(Ch,Cd,N,cudaMemcpyDeviceToHost))
    {
        cerr << "Cannot copy C from device to host" << endl;
        exit(1);
    }

    //  Free device memory
    cudaFree(Ad);
    cudaFree(Bd);
    cudaFree(Cd);
}

int main(int argc, char** argv)
{
    cout << "Josh Killinger: HW10" << endl;

	int maxThreadsPerBlock = InitGPU(1);

    //  Total width is block times number of blocks
    int Bw = 32;
    int Bn = 32;
    int n = Bw*Bn;
    int N = n*n*sizeof(float);
    cout << "Bw=" << Bw << " Bn=" << Bn << " n=" << n << endl;

    // Allocate host matrices A/B/C/R
    float* Ah = (float*)malloc(N);
    float* Bh = (float*)malloc(N);
    float* Ch = (float*)malloc(N);
    float* Rh = (float*)malloc(N);
    if (!Ah || !Bh || !Ch || !Rh)
    {
        cerr << "Cannot allocate host memory" << endl;
        exit(1);
    }

    // Initialize A & B
    srand(9999);
    RandomInit(Ah,n);
    RandomInit(Bh,n);

    //  Compute R = AB on host
    Elapsed();
    AxBh(Rh,Ah,Bh,n);
    double Th = Elapsed();

    //  Compute C = AB on device
    Elapsed();
    AxBd(Ch,Ah,Bh,Bw,Bn);
    double Td = Elapsed();

    //  Compute difference between R and C
    double r2=0;
    for (int i=0;i<n*n;i++)
        r2 += fabs(Ch[i]-Rh[i]);
    r2 /= n*n;

    //  Free host memory
    free(Ah);
    free(Bh);
    free(Ch);
    free(Rh);


    //  Print results
    cout << setw(7) << setprecision(4);
    cout << "Host   Time = " << Th << " s" << endl;
    cout << "Device Time = " << Td << " s" << endl;
    cout << "Speedup = " << Th/Td << endl;
    cout << "Difference = " << r2 << endl;

    return 0;
}