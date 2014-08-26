#include <stdio.h>
#include <windows.h>
#include <fcntl.h>
#include <unistd.h>
#include<iostream>
#include<ctype.h>
#include<string.h>
#include <stdlib.h>
#include <fstream>
//#include<MyGlobal>

#define MAX 256
using namespace std;
int main(void) {
	int dr_type=99;
	char dr_avail[MAX];
	char *temp=dr_avail;
	string glob_drives;
	ofstream drives_info("drives_info.txt");


	/* 1st we fill the buffer */
	cout<<"-------------------------------------------------------------"<<endl;
	cout<<"              Looking for harddisk partitions                "<<endl;
	cout<<"-------------------------------------------------------------"<<endl;
	GetLogicalDriveStrings(MAX,dr_avail);
	while(*temp!=NULL) { // Split the buffer by null
		dr_type=GetDriveType(temp);
		switch(dr_type) {
			case 0: // Unknown
			printf("%s : Unknown Drive type\n",temp);
			break;
			case 1: // Invalid
			printf("%s : Drive is invalid\n",temp);
			break;
			case 2: // Removable Drive
			printf("%s : Removable Drive\n",temp);
			break;
			case 3: // Fixed
			printf("%s : Hard Disk (Fixed)\n",temp);
			drives_info<<temp[0]<<endl;

			break;
			case 4: // Remote
			printf("%s : Remote (Network) Drive\n",temp);
			break;
			case 5: // CDROM
			printf("%s : CD-Rom/DVD-Rom\n",temp);
			break;
			case 6: // RamDrive
			printf("%s : Ram Drive\n",temp);
			break;
		}
		//printf("%s",temp);
		temp += lstrlen(temp) +1; // incriment the buffer
	}
//	cout<<res<<endl;

	return 0;
}






