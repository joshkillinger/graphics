#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <iostream>
#include <cuda.h>
#include <cuda_runtime.h>
//#include <helper_functions.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include <iomanip>

using namespace std;

/*
 *  Return elapsed wall time since last call (seconds)
 */
static double t0=0;
double Elapsed(void)
{
#ifdef _WIN32
   //  Windows version of wall time
   LARGE_INTEGER tv,freq;
   QueryPerformanceCounter((LARGE_INTEGER*)&tv);
   QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
   double t = tv.QuadPart/(double)freq.QuadPart;
#else
   //  Unix/Linux/OSX version of wall time
   struct timeval tv;
   gettimeofday(&tv,NULL);
   double t = tv.tv_sec+1e-6*tv.tv_usec;
#endif
   double s = t-t0;
   t0 = t;
   return s;
}

/*
 *  Initialize matrix with random values
 */
void RandomInit(float x[],const unsigned int n)
{
    for (unsigned int i=0;i<n;i++)
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

void MatMul(int maxThreadsPerBlock)
{
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
    RandomInit(Ah,n*n);
    RandomInit(Bh,n*n);

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
    cout << "Host   Time = " << Th << " s" << endl;
    cout << "Device Time = " << Td << " s" << endl;
    cout << "Speedup = " << Th/Td << endl;
    cout << "Difference = " << r2 << endl;
}

/*
 * c = a dot B -- host
 */
void Doth(float c[], const float a[], const float B[], unsigned int n)
{
    for (unsigned int i = 0; i < n; i++)
    {
        //double sum = 0;
        //for (unsigned int j = 0; j < n; j++)
        //{
        //    sum += (double)a[j] * (double)B[(i * n) + j];
        //}
        //c[i] = (float)sum;
        c[i] = i;
    }
}

/*
 * Compute a dot B[i]
 */
__global__ void Dot(float c[], const float a[], const float B[], const unsigned int n)
{
   unsigned int i = blockIdx.x*blockDim.x+threadIdx.x;
   //float sum =0;
   //for (int j = 0; j < n; j++)
   //   sum += a[j] * B[(i*n)+j];
   //c[i] = sum;
   c[i] = i;
}

void Dotd(float ch[],const float ah[],const float Bh[],const unsigned int n)
{
    //  Calculate matrix dimensions
    int N = n*n*sizeof(float);

    // Allocate device memory
    float* ad;
    float* Bd;
    float* cd;
    if (cudaMalloc((void**)&ad, n * sizeof(float)))
    {
        cerr << "Cannot allocate device memory Ad" << endl;
        exit(1);
    }
    if (cudaMalloc((void**)&Bd, N))
    {
        cerr << "Cannot allocate device memory Bd" << endl;
        exit(1);
    }
    if (cudaMalloc((void**)&cd, n * sizeof(float)))
    {
        cerr << "Cannot allocate device memory Cd" << endl;
        exit(1);
    }
    // Copy A and B from host to device
    if (cudaMemcpy(ad, ah, n * sizeof(float), cudaMemcpyHostToDevice))
    {
        cerr << "Cannot copy A from host to device" << endl;
        exit(1);
    }
    if (cudaMemcpy(Bd, Bh, N, cudaMemcpyHostToDevice))
    {
        cerr << "Cannot copy B from host to device" << endl;
        exit(1);
    }

    // Set size of block to 32, and 32 blocks
    dim3 threads(32);
    dim3 grid(32);
    // Execute the kernel
    AxB<<<grid,threads>>>(cd, ad, Bd, n);
    if (cudaGetLastError())
    {
        cerr << "Dot failed" << endl;
        exit(1);
    }

    // Copy C from device to host
    if (cudaMemcpy(ch, cd, n * sizeof(float), cudaMemcpyDeviceToHost))
    {
        cerr << "Cannot copy C from device to host" << endl;
        exit(1);
    }

    //  Free device memory
    cudaFree(ad);
    cudaFree(Bd);
    cudaFree(cd);
}

void MegaDot(int maxThreadsPerBlock)
{
    int n = 1024;
    int N = n * n * sizeof(float);

    cout << n << " vectors of " << n << " length = " << N << " bytes" << endl;

    // Allocate host matrices A/B/C/R
    float* ah = (float*)malloc(n * sizeof(float));
    float* Bh = (float*)malloc(N);
    float* ch = (float*)malloc(n * sizeof(float));
    float* rh = (float*)malloc(n * sizeof(float));
    if (!ah || !Bh || !ch || !rh)
    {
        cerr << "Cannot allocate host memory" << endl;
        exit(1);
    }

    // Initialize a & B
    srand(9999);
    RandomInit(ah,n);
    RandomInit(Bh,n*n);

    //  Compute r = a dot B on host
    Elapsed();
    Doth(rh,ah,Bh,n);
    double Th = Elapsed();

    //  Compute c = a dot B on device
    Elapsed();
    Dotd(ch,ah,Bh,n);
    double Td = Elapsed();

    //  Compute difference between R and C
    double r2=0;
    for (int i = 0; i < n; i++)
        r2 += fabs(ch[i]-rh[i]);
    r2 /= n;

    cout << "Host results: ";
/*    for (int i = 0; i < n; i++)
    {
        cout << rh[i] << ", ";
    }
*/
    cout << rh[0] << ", " << rh[1] << ", ..., " << rh[n-2] << ", " << rh[n-1];

    cout << endl << "Device results: ";
/*    for (int i = 0; i < n; i++)
    {
        cout << ch[i] << ", ";
    }
*/
    cout << ch[0] << ", " << ch[1] << ", ..., " << ch[n-2] << ", " << ch[n-1];
    cout << endl;

    //  Free host memory
    free(ah);
    free(Bh);
    free(ch);
    free(rh);

    //  Print results
    cout << "Host   Time = " << Th << " s" << endl;
    cout << "Device Time = " << Td << " s" << endl;
    cout << "Speedup = " << Th/Td << endl;
    cout << "Difference = " << r2 << endl;
}

int main(int argc, char** argv)
{
    cout << "Josh Killinger: HW10" << endl;
    cout << setw(7) << setprecision(4);

	int maxThreadsPerBlock = InitGPU(1);

    //MatMul(maxThreadsPerBlock);

    MegaDot(maxThreadsPerBlock);

    char c;
    cin >> c;

    return 0;
}