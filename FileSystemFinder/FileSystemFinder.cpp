#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include<iostream>
#include<ctype.h>
#include<string.h>
#include <stdlib.h>
#include<errno.h>
#include<unistd.h>

using namespace std;
int main(int argc, char* argv[]) {
    FILE *fd,*clusEntry,*fp;
	int result;
	unsigned char buf[512];
	int ntfs[8];
	int fat32[8];
	bool ntfs_chk=false;
	bool fat32_chk=false;
	int ntfs_sample[8]={78,84,70,83,32,32,32,32};
	int fat32_sample[8]={70,65,84,51,50,32,32,32};
	string logicalVolume, lv;
	char c;

	if ( argc != 2 ) /* argc should be 2 for correct execution */
	    {
	        /* We print argv[0] assuming it is the program name */
	        printf( "usage: %s filename\n FileSystemFinder is exiting...\n", argv[0] );
	        cin>>c;
	        return 0;

	    }

///	cout << "Enter logical volume to be scanned" << endl;
//	cin >> lv;
	lv=argv[1];
	logicalVolume = "\\\\.\\";
	logicalVolume = logicalVolume + lv + ":";
	//cout << "entered volume" << logicalVolume;
	//fp = fopen("\\\\.\\F:", "rb");
	fp = fopen64(logicalVolume.c_str(), "rb");
	if (!fp) {
		printf("\nfp unsuccessful");
		return -2;
	}
	result = fread(buf, 512, 1, fp);
	// Bytes per sector calculation

	for (int i=3,j=0;i<11,j<8;i++,j++)
	{
		ntfs[j]=buf[i];
	}
	for (int i=82,j=0;i<90,j<8;i++,j++)
	{
		fat32[j]=buf[i];
	}
	for (int i=0;i<8;i++)
	{
		if(ntfs[i]==ntfs_sample[i])
		{
			ntfs_chk=true;
		}
		else
		{
			ntfs_chk=false;
		}
	}
	if(ntfs_chk==true)
	{
		cout << "\nThis Media has NTFS Filesystem";
		return 0;
	}
	for (int i=0;i<8;i++)
	{
		if(fat32[i]==fat32_sample[i])
		{
			//cout << fat32[i] << " " << fat32_sample[i]<< "\n";
			fat32_chk=true;
		}
		else
		{
			fat32_chk=false;
		}
	}
	if(fat32_chk==true)
	{
		cout << "\nThis Media has FAT32 Filesystem";
		return 1;
	}
    cin>>c;
	return 2;

}





