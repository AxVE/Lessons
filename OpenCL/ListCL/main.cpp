#include <iostream>
#include <fstream>

#define __CL_ENABLE_EXCEPTIONS //Enable CL's exceptions

#include <CL/cl.hpp> //version 1.2
#include <vector>
#include <string>

using namespace std;

int main(){
	try{
		//Display csv header
		cout << "platform_id\tplatform_name\tdevice_id\tdevice_name\tdevice_vendor\tdevice_profile\tdevice_version\tdriver_version\tdevice_opencl_c_version" << endl;
		
		//get all platforms (drivers)
		vector<cl::Platform> all_platforms;
		cl::Platform::get(&all_platforms);
		if(all_platforms.size() == 0){
			cerr << "No platforms found. Check OpenCL installation !" << endl;
			return 1;
		}
		
		for(size_t p=0; p < all_platforms.size(); p++){
			string line = to_string(p)+"\t"+all_platforms[p].getInfo<CL_PLATFORM_VERSION>();
			line.pop_back();

			//Devices
			vector<cl::Device> all_devices;
			all_platforms[p].getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
			for(size_t d=0; d < all_devices.size(); d++){
				string device_line = line;
				device_line += "\t" + to_string(d);
				device_line+= "\t" + all_devices[d].getInfo<CL_DEVICE_NAME>();
				device_line.pop_back();
				device_line+= "\t" + all_devices[d].getInfo<CL_DEVICE_VENDOR>();
				device_line.pop_back();
				device_line+= "\t" + all_devices[d].getInfo<CL_DEVICE_PROFILE>();
				device_line.pop_back();
				device_line+= "\t" + all_devices[d].getInfo<CL_DEVICE_VERSION>();
				device_line.pop_back();
				device_line+= "\t" + all_devices[d].getInfo<CL_DRIVER_VERSION>();
				device_line.pop_back();
				device_line+= "\t" + all_devices[d].getInfo<CL_DEVICE_OPENCL_C_VERSION>();
				device_line.pop_back();
				cout << device_line << endl;
			}
		}
		
	}

	//Get OpenCL errors
	catch(cl::Error e){cerr << endl << e.what() << " : " << e.err() << endl;}

	return 0;
}
