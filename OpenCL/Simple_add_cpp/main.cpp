#include <iostream>
#include <fstream>

#define __CL_ENABLE_EXCEPTIONS //Enable CL's exceptions

#include <CL/cl.hpp> //version 1.2
#include <vector>
#include <string>

using namespace std;

int main(){
	try{
		//get all platforms (drivers)
		vector<cl::Platform> all_platforms;
		cl::Platform::get(&all_platforms);
		cout << "[OpenCL] platforms found: " << all_platforms.size() << endl;
		if(all_platforms.size() == 0){
			cerr << "\tNo platforms found. Check OpenCL installation !" << endl;
			return 1;
		}
		
		//Get the platform to use (default: first)
		cl::Platform platform = all_platforms[0];
		cout << "[OpenCL] Using platform: " << platform.getInfo<CL_PLATFORM_NAME>() << endl;

		//Get all devices
		vector<cl::Device> all_devices;
		platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
		cout << "[OpenCL] devices found: " << all_devices.size() << endl;
		if(all_devices.size() == 0){
			cerr << "\tNo devices found. Check OpenCL installation !" << endl;
			return 1;
		}

		//Get the device to use (default: first)
		cl::Device device = all_devices[0];
		cout << "[OpenCL] Using device: " << device.getInfo<CL_DEVICE_NAME>() << endl;

		//Create context and sources;
		cout << "[OpenCL] Create context and sources" << endl;
		cl::Context context({device});
		cl::Program::Sources sources;

		//Kernel calculates for each element C=A+B
		cout << "[OpenCL] Get kernel code 'simple_add'" << endl;
		//string kernel_code="void kernel simple_add(global const int* A, global const int* B, global int* C){C[get_global_id(0)] = A[get_global_id(0)]+B[get_global_id(0)];}";
		//sources.push_back({kernel_code.c_str(), kernel_code.length()});
		ifstream cl_file("simple_add.cl");
		string cl_string(istreambuf_iterator<char>(cl_file), (istreambuf_iterator<char>()));
		sources.push_back({cl_string.c_str(), cl_string.length()+1});


		//Build the kernel source
		cout << "[OpenCL] Build kernel source" << endl;
		cl::Program program(context, sources);
		if(program.build({device}) != CL_SUCCESS){
			cerr << "\tError building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << endl;
			return 1;
		}
		

		//Create buffers on the device
		cout << "[OpenCL] Create buffers" << endl;
		cl::Buffer buffer_A(context, CL_MEM_READ_WRITE, sizeof(int)*10);
		cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, sizeof(int)*10);
		cl::Buffer buffer_C(context, CL_MEM_READ_WRITE, sizeof(int)*10);

		int A[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
		int B[] = {0, 1, 2, 0, 1, 2, 0, 1, 2, 0};

		//Create queue of jobs
		cout << "[OpenCL] Prepare command queue" << endl;
		cl::CommandQueue queue(context, device);
		//Write arrays A and B to the device
		queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof(int)*10, A);
		queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, sizeof(int)*10, B);
		
		//Run
		cout << "[OpenCL] Run kernel code 'simple_add'" << endl;
		//cl::KernelFunctor run_simple_add(cl::Kernel(program, "simple_add"), queue, cl::NullRange, cl::NDRange(10), cl::NullRange); //NDRange is the number of threads, here it's only 10 because our data set have a size of 10.
		cl::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer> run_simple_add(cl::Kernel(program, "simple_add"));
		cl::EnqueueArgs eargs(queue, cl::NullRange, cl::NDRange(10), cl::NullRange);
		run_simple_add(eargs, buffer_A, buffer_B, buffer_C).wait(); //Run it (and wait the end)

		//Get results
		int C[10];
		queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, sizeof(int)*10,C);
		cout << "A: "; for(int i=0; i < 10; i++){cout << A[i] << "\t";} cout << endl;
		cout << "B: "; for(int i=0; i < 10; i++){cout << B[i] << "\t";} cout << endl;
		cout << "C: "; for(int i=0; i < 10; i++){cout << C[i] << "\t";} cout << endl;
		
		//Run2
		cout << "[OpenCL] Run2 kernel code 'simple_add'" << endl;
 		//Copy C into A
		for(int i=0; i<10;i++){A[i]=C[i];}
		queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof(int)*10, A);
		//Run it and wait the end
		run_simple_add(eargs, buffer_A, buffer_B, buffer_C).wait();

		//Get results
		queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, sizeof(int)*10,C);
		cout << "A: "; for(int i=0; i < 10; i++){cout << A[i] << "\t";} cout << endl;
		cout << "B: "; for(int i=0; i < 10; i++){cout << B[i] << "\t";} cout << endl;
		cout << "C: "; for(int i=0; i < 10; i++){cout << C[i] << "\t";} cout << endl;
	}

	//Get OpenCL errors
	catch(cl::Error e){cerr << endl << e.what() << " : " << e.err() << endl;}

	return 0;
}
