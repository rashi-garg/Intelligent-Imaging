#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include<errno.h>
#include<unistd.h>
//#include<cstring>
#include<ctype.h>
#include<string.h>
#include <hashlibpp.h>
#include<iostream>
#include<iomanip>
#include<conio.h>

using namespace std;
static inline void loadbar( long x,  long n, unsigned int w = 50)
{
    if ( (x != n) && (x % (n/100) != 0) ) return;

    float ratio  =  x/(float)n;
    int   c      =  ratio * w;

    cout << setw(3) << (int)(ratio*100) << "% [";
    for (int x=0; x<c; x++) cout << "=";
    for (int x=c; x<w; x++) cout << " ";
    cout << "]\r" << flush;
}
static inline void loadbar( float z, long x, long n, unsigned int w = 50)
{
    if ( (x != n) && (x % (n/100) != 0) ) return;

    float ratio  =  x/(float)n;
    int   c      =  ratio * w;

    cout <<setw(10)<<z<<"%zero"<<setw(3) << (int)(ratio*100) << "% [";
    for (int x=0; x<c; x++) cout << "=";
    for (int x=c; x<w; x++) cout << " ";
    cout << "]\r" << flush;
}


int main(int argc, char* argv[]) {
	int fd;
	int result;
	int bytes_sector[2];
	int reservd_sector[2];
	int sector_cluster;
	int numberof_FAT;
	int totalsector_32[4];
	int FAT_sector[4];
	char FAT_sec[8] = "";
	unsigned char buf[512];
	char bytes_sec[4] = "";
	int bytes_sec_int;
	char *bytes;
	char total_sec[8] = "";
	int total_sec_int;
	char reserved_sec[4] = "";
	int reserved_sec_int;
	std::string sample_hash;
	long clusternumber,firstSectorOfCluster,count_unalloc=0,count_alloc=0,count_free=0,count_zero=0;
	FILE *fp, *temp, *clustAdd,*hashSample, *zeroclusters,*output,*output2;
	struct clustEntry{
		long clustnum;
		long sectorOffset;
	};
	struct clustEntry cEntry,zEntry;
	string logicalVolume,lv;
	if ( argc != 2 ) /* argc should be 2 for correct execution */
			    {
			        /* We print argv[0] assuming it is the program name */
			        printf( "usage: %s filename\n FileSystemFinder is exiting...\n", argv[0] );
			        return 0;

			    }
	//cout<<"Enter logical volume to be scanned"<<endl;
	//cin>>lv;
	lv=argv[1];
	logicalVolume = "\\\\.\\";
	logicalVolume = logicalVolume+lv+":";
	//cout<<"entered volume"<<logicalVolume;
	//fp = fopen("\\\\.\\I:", "rb");
	fp = fopen64(logicalVolume.c_str(),"rb");
	if (!fp) {
		printf("\nfp unsuccessful");
		return -2;
	}

	hashwrapper *myWrapper = new sha512wrapper();

	//	result = read(fd, buf, 512);
	result = fread(buf, 512, 1, fp);
	int i = 0, j = 0;
	int m = 11, n = 14, k = 32, l = 36;
	for (i = 0; i < 3; i = i + 2) {
		char buffer[2] = "";
		char s[2] = "0";

		itoa(buf[m], buffer, 16);

		if (strlen(buffer) < 2) {
			strcat(s, buffer);

			bytes_sector[i] = s[0];
			bytes_sector[i + 1] = s[1];
			//printf("%c,%c \n", bytes_sector[i],bytes_sector[i+1]);
			//printf("Hello");
		} else {
			bytes_sector[i] = buffer[0];
			bytes_sector[i + 1] = buffer[1];
			//printf("%c,%c \n", bytes_sector[i],bytes_sector[i+1]);
			//printf("%s \n",bytes_sector[i]);
		}
		//strcat(bytes_sec, bytes_sector[i]);
		//printf("%s \n",bytes_sec);
		m = m + 1;

	}
	//printf("%s \n",bytes_sec);
	char bytes_sec_big[5] = "";
	bytes_sec_big[0] = bytes_sector[2];
	bytes_sec_big[1] = bytes_sector[3];
	bytes_sec_big[2] = bytes_sector[0];
	bytes_sec_big[3] = bytes_sector[1];
	bytes_sec_big[8] = '\0';

	//printf("%s \n",bytes_sec_big);

	bytes_sec_int = strtol(bytes_sec_big, &bytes, 16);

	for (i = 0; i < 3; i = i + 2) {

		char buffer[2] = "";
		char s[2] = "0";

		itoa(buf[n], buffer, 16);

		if (strlen(buffer) < 2) {
			strcat(s, buffer);
			reservd_sector[i] = s[0];
			reservd_sector[i + 1] = s[1];

			//printf("%c,%c \n", reservd_sector[i],reservd_sector[i+1]);
		} else {
			reservd_sector[i] = buffer[0];
			reservd_sector[i + 1] = buffer[1];
			//printf("%c,%c \n", reservd_sector[i],reservd_sector[i+1]);
			//printf("%s \n",reservd_sector[i]);
		}
		//strcat(reserved_sec, reservd_sector[i]);
		//printf("%s \n",reserved_sec);
		n = n + 1;
	}
	//printf("%s \n",bytes_sec);
	char reserved_sec_big[5] = "";
	reserved_sec_big[0] = reservd_sector[2];
	reserved_sec_big[1] = reservd_sector[3];
	reserved_sec_big[2] = reservd_sector[0];
	reserved_sec_big[3] = reservd_sector[1];
	reserved_sec_big[4] = '\0';

	//printf("%s \n",reserved_sec_big);
	char *reserved;
	reserved_sec_int = strtol(reserved_sec_big, &reserved, 16);
	sector_cluster = buf[13];
	numberof_FAT = buf[16];

	for (i = 0; i < 7; i = i + 2) {
		char buffer[2] = "";
		char s[2] = "0";

		itoa(buf[k], buffer, 16);

		if (strlen(buffer) < 2) {
			strcat(s, buffer);

			totalsector_32[i] = s[0];
			totalsector_32[i + 1] = s[1];
			//printf("%s \n",totalsector_32[i]);
		} else {
			totalsector_32[i] = buffer[0];
			totalsector_32[i + 1] = buffer[1];
			//printf("%s \n",totalsector_32[i]);
		}
		//strcat(total_sec, totalsector_32[i]);
		//printf("%s \n",total_sec);
		k = k + 1;

	}
	char total_sec_big[9] = "";
	//printf("%s \n",total_sec);
	total_sec_big[0] = totalsector_32[6];
	total_sec_big[1] = totalsector_32[7];
	total_sec_big[2] = totalsector_32[4];
	total_sec_big[3] = totalsector_32[5];
	total_sec_big[4] = totalsector_32[2];
	total_sec_big[5] = totalsector_32[3];
	total_sec_big[6] = totalsector_32[0];
	total_sec_big[7] = totalsector_32[1];
	total_sec_big[8] = '\0';
	//printf("%s \n",total_sec_big);

	//	sscanf(FAT_sec_big,"%d",&FAT_sec_int);
	//	printf("%d \n",FAT_sec_int);

	char *total;
	total_sec_int = strtol(total_sec_big, &total, 16);

	for (i = 0; i < 7; i = i + 2) {
		char buffer[2] = "";
		char s[2] = "0";

		itoa(buf[l], buffer, 16);
		//printf("%s \n",buffer);
		if (strlen(buffer) < 2) {
			strcat(s, buffer);
			//printf("%s \n",s);
			FAT_sector[i] = s[0];
			FAT_sector[i + 1] = s[1];
			//printf("%c,%c \n",FAT_sector[i],FAT_sector[i+1]);
		} else {
			FAT_sector[i] = buffer[0];
			FAT_sector[i + 1] = buffer[1];
			//printf("%c,%c \n",FAT_sector[i],FAT_sector[i+1]);
			//printf("%s \n",FAT_sector[i]);
		}
		//strcat(FAT_sec, FAT_sector[i]);

		l = l + 1;
	}
	//printf("%s \n",FAT_sector);
	char FAT_sec_big[9] = "";
	FAT_sec_big[0] = FAT_sector[6];
	FAT_sec_big[1] = FAT_sector[7];
	FAT_sec_big[2] = FAT_sector[4];
	FAT_sec_big[3] = FAT_sector[5];
	FAT_sec_big[4] = FAT_sector[2];
	FAT_sec_big[5] = FAT_sector[3];
	FAT_sec_big[6] = FAT_sector[0];
	FAT_sec_big[7] = FAT_sector[1];
	FAT_sec_big[8] = '\0';
	//printf("%s \n",FAT_sec_big);
	const char *test1 = FAT_sec_big;

	//	sscanf(FAT_sec_big,"%d",&FAT_sec_int);
	//	printf("%d \n",FAT_sec_int);

	char *pEnd = NULL;
	const unsigned int FAT_sec_int = strtoul(FAT_sec_big, &pEnd, 16);
	//printf("%d \n", test);
	//printf("\nerror:%s", strerror(errno));

	printf("\n%d bytes per sector \n", bytes_sec_int);
	printf("%d sector per cluster \n", sector_cluster);
	printf("%d number of FAT \n", numberof_FAT);
	printf("%d reserved sector \n", reserved_sec_int);
	printf("%d total sectors in FAT32 \n", total_sec_int);
	printf("%u Number of sectors in one FAT \n", FAT_sec_int);

	//Calculating first data sector
	const long firstDataSector = reserved_sec_int
			+ (numberof_FAT * FAT_sec_int);
//	printf("%d first data sector \n", firstDataSector);
	//Size of 1 cluster in bytes
	const long clustSize = sector_cluster * bytes_sec_int;

	//Going to FAT1
	unsigned long long currpos = 0; //Current position of pointer
	unsigned char clustContent[clustSize]; // Buffer to read one cluster
	const long sec_fat1 = reserved_sec_int * bytes_sec_int;
	const long fat2_offset = (reserved_sec_int + FAT_sec_int) * bytes_sec_int;
//	printf("\n fat2 offset %ld", fat2_offset);
	long fat1_offset = fseek(fp, sec_fat1, SEEK_SET);
//	printf("hello");


	hashSample = fopen("Samplecluster.txt","wb");
	if(!hashSample){
		printf("\nCannot open Samplecluster.txt");
		exit(1);
	}
	// Calculating sample hash for unallocated cluster
	for(i=0;i<clustSize;i++){
		clustContent[i]=0;
	}
	long bytes_written = fwrite(clustContent, clustSize, 1, hashSample);

	if (bytes_written != 1) {
		printf("\n bytes written %ld", bytes_written);
		fclose(hashSample);

		printf("\n Some error occurred while writing Sample cluster to file.Exiting");
		return -2;
	} else {
		//printf("\n bytes written %ld", bytes_writen);
		sample_hash = myWrapper->getHashFromFile("Samplecluster.txt");
//		cout << endl << "Sample hash"<< sample_hash;

		fclose(hashSample);
		//check

		}


	//************************************Sample hash calculation complete********************************************************************************************

	clustAdd = fopen("clusterAdress.txt","wb");
	if(!clustAdd){
		printf("\nCannot open file clusterAddress.txt");
		exit(1);
	}

//	cEntry = (struct clustEntry *)malloc(sizeof(struct clustEntry));
/*	if(!cEntry){
		printf("\nError alloating required space");
		exit(1);
	}*/
	//final=fopen("Zero_Cluster_FAT.txt","w+");

	if (fat1_offset != -1L) {
	//	printf("\nfseek successful");
		int sec_num = 0;
		printf("\nScanning FAT to find out allocated and unallocated clusters\n");
		while (1) {
			//read one sector at a time
			result = fread(buf, bytes_sec_int, 1, fp);
			//read individual cluster entries in 1 sector. 1 cluster = 4 bytes
		//	printf("\n--------------------------------------------%lu",
		//			fat2_offset);
			//		return 0;
			int cluster[4];
			for (i = 0; i < bytes_sec_int;) {
				clusternumber = ((i + (sec_num * bytes_sec_int)) / 4);
				//		printf("\nValue of cluster %d", clusternumber);
				cluster[0] = buf[i + 3];
				cluster[1] = buf[i + 2];
				cluster[2] = buf[i + 1];
				cluster[3] = buf[i];
				//		printf("\n--------------------------------------------%lu",fat2_offset);
				//		return -1;

				//printf("\ni:%d", i);
				if (((cluster[0] % 16) == 0) && (cluster[1] == 0)
						&& (cluster[2] == 0) && (cluster[3] == 0)) {
//					printf("\n cluster[0]: %ld",cluster[0]);
//					printf("\ncluster %d is unallocated", clusternumber);
					if(cluster[0]==0){
					count_unalloc++;
					}
					else{
						count_free++;
					}

					cEntry.clustnum = clusternumber;

					//Calculating first sector of this cluster
					if (clusternumber > 1) {
						 firstSectorOfCluster = ((clusternumber - 2)
								* sector_cluster) + firstDataSector;
	//					printf("\nfirstSectorOfCluster: %ld",
	//							firstSectorOfCluster);
						cEntry.sectorOffset = firstSectorOfCluster;
						fwrite(&cEntry,sizeof(struct clustEntry),1,clustAdd);



					}
				} else {
//					printf("\nOffset:%d",
//							i + (sec_num * bytes_sec_int) + sec_fat1);
//					printf("\ncluster %d is allocated", clusternumber);
					count_alloc++;



				}
				/*printf("\ncluster1:%u", cluster[0]);
				 printf("\ncluster2:%u", cluster[1]);
				 printf("\ncluster3:%u", cluster[2]);
				 printf("\ncluster4:%u", cluster[3]);*/
				//	printf("\nOffset:%d", i + (sec_num * bytes_sec_int)+sec_fat1);
				//	printf("\n.....................................%ld",fat2_offset);
				i = i + 4;

				//break;
			}
			sec_num++;
			currpos = ftello64(fp);
//			printf("\nCurrent position: %ld", currpos);
//			printf("\nfat2 offset: %ld", fat2_offset);

			loadbar(currpos,fat2_offset);
			//printf("\ncurpos %ld",currpos);
			//Stop when pointer reaches end of FAT1
			if (currpos == fat2_offset) {
				break;
			}
			//

			//break;
		}

		fclose(clustAdd);

	} else {
		printf("\n%s", strerror(errno));
	}
	long total_clus = count_alloc + count_unalloc+count_free;
	printf("\n Total clusters = %ld",total_clus);
	printf("\nPercentage of clusters that are unallocated and never used:%lf",((double)count_unalloc/(double)total_clus)*100 );
	printf("\nPrecentage of clusters that are unallocated but have been used at least once:%lf",((double)count_free/(double)total_clus)*100);
	printf("\nPercentage of clusters that are allocated:%lf",((double)count_alloc/(double)total_clus)*100);
	printf("\nScanning to find zero clusters...\n");

	long clustersToTest = count_unalloc + count_free;
	cout<<clustersToTest<<endl;
	clustAdd = fopen("clusterAdress.txt","rb");
	zeroclusters = fopen("zeroclusters.txt","wb");
	if(!clustAdd){
		printf("\nCannot open file clusterAddress.txt for reading");
		exit(1);
	}
	if(!zeroclusters){
			printf("\nCannot open file zeroclusters.txt for reading");
			exit(1);
		}
	string temp_file = "output_"+ lv +".txt";
		cout <<"temporary file"<<temp_file<<endl;
	long count =0;
	while (fread(&cEntry, sizeof(struct clustEntry), 1, clustAdd)) {
		count++;
		//	printf("\n cluster number: %ld, sectorOffset: %ld",cEntry.clustnum,cEntry.sectorOffset);
		firstSectorOfCluster = cEntry.sectorOffset;
		clusternumber = cEntry.clustnum;
		result = fseeko64(fp, firstSectorOfCluster * bytes_sec_int, SEEK_SET);
		if (result != -1) {
			result = fread(clustContent, bytes_sec_int, sector_cluster, fp);
//									printf("\n%d bytes read", result);
			string testStr(clustContent,
					clustContent
							+ sizeof clustContent / sizeof clustContent[0]);
			string hash1 = myWrapper->getHashFromString(testStr);
			if (hash1.compare(sample_hash) == 0) {
				count_zero++;
				fwrite(&cEntry, sizeof(struct clustEntry), 1, zeroclusters);
				//		cout <<endl<<count_zero;
			}
			/*

			 //Write cluster to  temporary file for creating hash
			 temp = fopen("cluster", "w");
			 if (!temp) {
			 printf("\Can not create temporary files");
			 return -2;
			 }

			 long bytes_writen = fwrite(clustContent, clustSize,
			 1, temp);

			 if (bytes_writen != 1) {
			 printf("\n bytes written %ld", bytes_writen);
			 fclose(temp);

			 printf(
			 "\n Some error occurred while writing cluster to file.Exiting");
			 return -2;
			 } else {
			 //printf("\n bytes written %ld", bytes_writen);
			 std::string hash1 = myWrapper->getHashFromFile(
			 "cluster");
			 //cout << endl << hash1;
			 if(hash1.compare(sample_hash)==0){
			 count_zero++;
			 fwrite(&cEntry,sizeof(struct clustEntry),1,zeroclusters);
			 //		cout <<endl<<count_zero;
			 }

			 fclose(temp);

			 }*/
		}
		//loadbar(count, clustersToTest);
		output2 = fopen(temp_file.c_str(), "w");
				if (!output2) {
					cout << endl << "Cannot write output2" << endl;
					cout << strerror(errno);
					getch();
					return 0;
				}
				fprintf(output2, "\n logical Volume: %s\n", logicalVolume.c_str());
				fprintf(output2, "FAT32 File System (64 bit)\n");
				fprintf(output2, "Total Clusters:%ld", total_clus);
				fprintf(output2,
						"\nPercentage of clusters that are unallocated and never used:%lf",
						((double) count_unalloc / (double) total_clus) * 100);
				fprintf(output2, "%d bytes per sector \n", bytes_sec_int);
				fprintf(output2, "%d sector per cluster \n", sector_cluster);
				fprintf(output2, "%d number of FAT \n", numberof_FAT);
				fprintf(output2, "%d reserved sector \n", reserved_sec_int);
				fprintf(output2, "%d total sectors in FAT32 \n", total_sec_int);
				fprintf(output2, "%u Number of sectors in one FAT \n", FAT_sec_int);

				//printf("\nPercentage of clusters that are unallocated but have been used at least once:%lf",((double)count_free/(double)total_clus)*100);
				fprintf(output2, "\nPercentage of unallocated clusters:%lf",
						((double) count_unalloc / (double) total_clus) * 100);
				fprintf(output2, "\nPercentage completion fo task:%lf\n",
								((double) count / (double) clustersToTest) * 100);
				fprintf(output2, "\n Percentage of zero clusters: %lf\n",
						((double) count_zero / (double) total_clus) * 100);
				fprintf(output2,
						"********************************************************\n");
				fclose(output2);


		loadbar(((float)count_zero/(float)total_clus)*100,count, clustersToTest);

		if (feof(clustAdd)) {
			break;
		}

	}

	printf("\nPercentage of clusters that are zero:%lf",((double)count_zero/(double)total_clus)*100);

	//just to stop the screen from closing abruptly
	//char x;
	//cin>>x;
	fclose(zeroclusters);

		//Writing results in file
		output = fopen("output.txt", "a+");
		if (!output) {
			cout << endl << "Cannot write output" << endl;
			getch();
			return 0;
		}
		fprintf(output,"\n logical Volume: %s\n",logicalVolume.c_str());
		fprintf(output,"FAT32 File System (64 bit)\n");
		fprintf(output,"Total Clusters:%ld",total_clus);
		fprintf(output,"\nPercentage of clusters that are unallocated and never used:%lf",
				((double) count_unalloc / (double) total_clus) * 100);
		fprintf(output,"%d bytes per sector \n", bytes_sec_int);
		fprintf(output,"%d sector per cluster \n", sector_cluster);
		fprintf(output,"%d number of FAT \n", numberof_FAT);
		fprintf(output,"%d reserved sector \n", reserved_sec_int);
		fprintf(output,"%d total sectors in FAT32 \n", total_sec_int);
		fprintf(output,"%u Number of sectors in one FAT \n", FAT_sec_int);

		//printf("\nPercentage of clusters that are unallocated but have been used at least once:%lf",((double)count_free/(double)total_clus)*100);
		fprintf(output,"\nPercentage of unallocated clusters:%lf",((double)count_unalloc/(double)total_clus)*100);
		fprintf(output,"\n Percentage of zero clusters: %lf\n",((double)count_zero/(double)total_clus)*100);
		fprintf(output,"********************************************************\n");
		fclose(output);
	getch();
	/*
	printf("\n%d bytes per sector \n", bytes_sec_int);
	printf("%d sector per cluster \n", sector_cluster);
	printf("%d number of FAT \n", numberof_FAT);
	printf("%d reserved sector \n", reserved_sec_int);
	printf("%d total sectors in FAT32 \n", total_sec_int);
	printf("%d Number of sectors in one FAT \n", FAT_sec_int);
	printf("%ld fat 1 offset \n", sec_fat1);
	printf("%ld fat2 offset \n", fat2_offset);
*/
	return 0;
//latest
}




